#include "../../inc/execution.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

// Helper: Count the number of commands in a pipeline (left-associative tree)
static int count_commands(t_node *node) {
    if (!node) return 0;
    if (node->type == NODE_PIPE)
        return count_commands(node->left) + count_commands(node->right);
    if (node->type == NODE_CMD)
        return 1;
    return 0;
}

// Helper: Allocate pipes (n-1 pipes for n commands)
static int **allocate_pipes(int cmd_count) {
    if (cmd_count < 2) return NULL;
    int **pipes = malloc(sizeof(int *) * (cmd_count - 1));
    for (int i = 0; i < cmd_count - 1; i++) {
        pipes[i] = malloc(sizeof(int) * 2);
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            // Free previously allocated pipes
            for (int j = 0; j <= i; j++) free(pipes[j]);
            free(pipes);
            return NULL;
        }
    }
    return pipes;
}

// Helper: Free pipes
static void free_pipes(int **pipes, int cmd_count) {
    if (!pipes) return;
    for (int i = 0; i < cmd_count - 1; i++)
        free(pipes[i]);
    free(pipes);
}

// Helper: Get the nth command node in a left-associative pipeline
t_node *get_nth_command(t_node *node, int n) {
    if (!node) return NULL;
    if (node->type == NODE_PIPE) {
        int left_count = count_commands(node->left);
        if (n < left_count)
            return get_nth_command(node->left, n);
        else
            return get_nth_command(node->right, n - left_count);
    }
    if (node->type == NODE_CMD && n == 0)
        return node;
    return NULL;
}

// Main entry point for executing a parsed command line
void execute_line(t_node *cmd_list, t_env *env_list)
{
    if (!cmd_list) return;
    t_exec ctx;
    ctx.cmd_count = count_commands(cmd_list);
    ctx.pipes = allocate_pipes(ctx.cmd_count);
    ctx.pids = malloc(sizeof(pid_t) * ctx.cmd_count);
    ctx.last_exit_code = 0;
    ctx.env_list = env_list;

    for (int i = 0; i < ctx.cmd_count; i++) {
        t_node *cmd_node = get_nth_command(cmd_list, i);
        if (!cmd_node || !cmd_node->cmd) continue;
        ctx.pids[i] = fork();
        if (ctx.pids[i] == -1) {
            perror("fork");
            continue;
        }
        if (ctx.pids[i] == 0) { // Child
            setup_pipes(&ctx, i);
            setup_redirections(cmd_node->cmd);
            if (get_builtin_type(cmd_node->cmd->args[0]) != BUILTIN_NONE) {
                execute_builtin(cmd_node, &ctx);
                exit(0);
            } else {
                execute_command(cmd_node, &ctx);
                exit(1); // Should not reach here
            }
        }
    }
    close_pipes(&ctx);
    ctx.last_exit_code = wait_for_children(&ctx);
    free(ctx.pids);
    free_pipes(ctx.pipes, ctx.cmd_count);
}

// Execute a single command node (external or builtin)
int execute_command(t_node *cmd_node, t_exec *ctx)
{
    char *cmd = cmd_node->cmd->args[0];
    char *path = NULL;
    char **envp = to_envp(ctx->env_list);
    // If command contains '/', use as is
    if (strchr(cmd, '/')) {
        path = cmd;
    } else {
        // Search in PATH
        char *env_path = get_env_value(ctx->env_list, "PATH");
        if (!env_path) {
            fprintf(stderr, "%s: No such file or directory\n", cmd);
            exit(127);
        }
        char *paths = strdup(env_path);
        char *token = strtok(paths, ":");
        while (token) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
            if (access(full_path, X_OK) == 0) {
                path = strdup(full_path);
                break;
            }
            token = strtok(NULL, ":");
        }
        free(paths);
        if (!path) {
            fprintf(stderr, "%s: command not found\n", cmd);
            exit(127);
        }
    }
    execve(path, cmd_node->cmd->args, envp);
    perror("execve");
    exit(127);
}

// Check if a command is a builtin and return its type
t_builtin_type get_builtin_type(const char *cmd_name)
{
    if (!cmd_name) return BUILTIN_NONE;
    if (strcmp(cmd_name, "cd") == 0) return BUILTIN_CD;
    if (strcmp(cmd_name, "echo") == 0) return BUILTIN_ECHO;
    if (strcmp(cmd_name, "env") == 0) return BUILTIN_ENV;
    if (strcmp(cmd_name, "exit") == 0) return BUILTIN_EXIT;
    if (strcmp(cmd_name, "export") == 0) return BUILTIN_EXPORT;
    if (strcmp(cmd_name, "pwd") == 0) return BUILTIN_PWD;
    if (strcmp(cmd_name, "unset") == 0) return BUILTIN_UNSET;
    return BUILTIN_NONE;
}

// Execute a builtin command (stub, to be implemented per builtin)
t_builtin_type execute_builtin(t_node *cmd_node, t_exec *ctx)
{
    t_builtin_type type = get_builtin_type(cmd_node->cmd->args[0]);
    // Example: handle echo
    if (type == BUILTIN_ECHO) {
        for (int i = 1; cmd_node->cmd->args[i]; i++) {
            if (i > 1) write(1, " ", 1);
            write(1, cmd_node->cmd->args[i], strlen(cmd_node->cmd->args[i]));
        }
        write(1, "\n", 1);
        return type;
    }
    // TODO: Implement other builtins
    return type;
}

// Setup input/output redirections for a command
int setup_redirections(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    while (redir) {
        int fd;
        if (redir->type == REDIR_IN) {
            fd = open(redir->filename, O_RDONLY);
            if (fd == -1) {
                perror(redir->filename);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (redir->type == REDIR_OUT) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror(redir->filename);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redir->type == REDIR_OUT_APPEND) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror(redir->filename);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redir->type == HERE_DOC) {
            // TODO: Implement heredoc
        }
        redir = redir->next;
    }
    return 0;
}

// Setup pipes for a pipeline
int setup_pipes(t_exec *ctx, int cmd_index)
{
    // If not the first command, set stdin to previous pipe read end
    if (cmd_index > 0) {
        dup2(ctx->pipes[cmd_index - 1][0], STDIN_FILENO);
    }
    // If not the last command, set stdout to current pipe write end
    if (cmd_index < ctx->cmd_count - 1) {
        dup2(ctx->pipes[cmd_index][1], STDOUT_FILENO);
    }
    // Close all pipe fds in child
    for (int i = 0; i < ctx->cmd_count - 1; i++) {
        close(ctx->pipes[i][0]);
        close(ctx->pipes[i][1]);
    }
    return 0;
}

// Close all pipes in the context
void close_pipes(t_exec *ctx)
{
    if (!ctx->pipes) return;
    for (int i = 0; i < ctx->cmd_count - 1; i++) {
        close(ctx->pipes[i][0]);
        close(ctx->pipes[i][1]);
    }
}

// Wait for all child processes and return the last exit code
int wait_for_children(t_exec *ctx)
{
    int status = 0;
    int exit_code = 0;
    for (int i = 0; i < ctx->cmd_count; i++) {
        waitpid(ctx->pids[i], &status, 0);
        if (WIFEXITED(status))
            exit_code = WEXITSTATUS(status);
    }
    return exit_code;
} 