#include "../../inc/execution.h"

/*
** EXECUTION MODULE - Handles command execution in minishell
** 
** This module manages:
** 1. Counting commands in a pipeline
** 2. Setting up pipes for inter-process communication
** 3. Forking processes and executing commands
** 4. Handling built-in commands vs external programs
** 5. Managing redirections (>, <, >>)
** 6. Waiting for child processes to complete
*/

/*
** count_commands - Recursively counts commands in the AST
** 
** For a pipeline like "ls | grep txt | wc -l", this returns 3
** The AST is left-associative: ((ls | grep) | wc)
*/
int	count_commands(t_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (count_commands(node->left) + count_commands(node->right));
	if (node->type == NODE_CMD)
		return (1);
	return (0);
}

/*
** allocate_pipes - Creates pipe pairs for inter-process communication
** 
** For n commands, we need (n-1) pipes
** Each pipe has 2 file descriptors: [0] for reading, [1] for writing
** Example: "cmd1 | cmd2 | cmd3" needs 2 pipes
*/
int	**allocate_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count < 2)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

/*
** free_pipes - Cleanup function for pipe arrays
*/
void	free_pipes(int **pipes, int cmd_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

/*
** get_nth_command - Retrieves the nth command node from pipeline
** 
** Navigates the left-associative AST to find a specific command
** Used to iterate through commands in execution order
*/
t_node	*get_nth_command(t_node *node, int n)
{
	int	left_count;

	if (!node)
		return (NULL);
	if (node->type == NODE_PIPE)
	{
		left_count = count_commands(node->left);
		if (n < left_count)
			return (get_nth_command(node->left, n));
		else
			return (get_nth_command(node->right, n - left_count));
	}
	if (node->type == NODE_CMD && n == 0)
		return (node);
	return (NULL);
}

/*
** setup_execution_context - Initializes execution environment
** 
** Sets up the context structure with:
** - Command count
** - Pipe array
** - Process ID array
** - Environment variables
*/
t_exec	*setup_execution_context(t_node *cmd_list, t_env *env_list)
{
	t_exec	*ctx;

	if (!cmd_list)
		return (NULL);
	ctx = malloc(sizeof(t_exec));
	if (!ctx)
		return (NULL);
	ctx->cmd_count = count_commands(cmd_list);
	ctx->pipes = allocate_pipes(ctx->cmd_count);
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pids)
	{
		free_pipes(ctx->pipes, ctx->cmd_count);
		free(ctx);
		return (NULL);
	}
	ctx->last_exit_code = 0;
	ctx->env_list = env_list;
	return (ctx);
}

/*
** execute_single_command - Handles execution of one command
** 
** This function:
** 1. Checks if command is built-in or external
** 2. Sets up pipes and redirections
** 3. Executes the command appropriately
** 4. Only called in child processes
*/
static void	execute_single_command(t_node *cmd_node, t_exec *ctx, int cmd_index)
{
	setup_pipes(ctx, cmd_index);
	setup_redirections(cmd_node->cmd);
	
	if (get_builtin_type(cmd_node->cmd->args[0]) != BUILTIN_NONE)
	{
		execute_builtin(cmd_node, ctx);
		exit(0);
	}
	else
	{
		execute_external_command(cmd_node, ctx);
		exit(127); // Should not reach here if execve succeeds
	}
}

/*
** execute_pipeline - Main execution function
** 
** Process flow:
** 1. Set up execution context
** 2. Fork child process for each command
** 3. In child: set up pipes/redirections and execute
** 4. In parent: close pipes and wait for children
*/
void	execute_pipeline(t_node *cmd_list, t_env *env_list)
{
	t_exec	*ctx;
	t_node	*cmd_node;
	int		i;

	if (!cmd_list)
		return ;
	ctx = setup_execution_context(cmd_list, env_list);
	if (!ctx)
		return ;
	i = 0;
	while (i < ctx->cmd_count)
	{
		cmd_node = get_nth_command(cmd_list, i);
		if (!cmd_node || !cmd_node->cmd)
		{
			i++;
			continue ;
		}
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
			perror("fork");
		else if (ctx->pids[i] == 0)
			execute_single_command(cmd_node, ctx, i);
		i++;
	}
	close_all_pipes(ctx);
	ctx->last_exit_code = wait_for_all_children(ctx);
	cleanup_execution_context(ctx);
}

/*
** execute_external_command - Executes external programs
** 
** Steps:
** 1. Check if command has '/' (absolute/relative path)
** 2. If not, search in PATH environment variable
** 3. Use execve to replace current process with command
*/
void	execute_external_command(t_node *cmd_node, t_exec *ctx)
{
	char	*cmd;
	char	*path;
	char	**envp;

	cmd = cmd_node->cmd->args[0];
	envp = convert_env_to_array(ctx->env_list);
	
	if (ft_strchr(cmd, '/'))
		path = cmd;
	else
		path = find_command_in_path(cmd, ctx->env_list);
	
	if (!path)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(path, cmd_node->cmd->args, envp);
	perror("execve");
	exit(127);
}

/*
** find_command_in_path - Searches for executable in PATH
** 
** Returns full path to executable or NULL if not found
*/
static char	*find_command_in_path(char *cmd, t_env *env_list)
{
	char	*env_path;
	char	**paths;
	char	*full_path;
	int		i;

	env_path = get_env_value(env_list, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = create_full_path(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_string_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_string_array(paths);
	return (NULL);
}

/*
** get_builtin_type - Identifies built-in commands
** 
** Returns the type of built-in command or BUILTIN_NONE
*/
t_builtin_type	get_builtin_type(const char *cmd_name)
{
	if (!cmd_name)
		return (BUILTIN_NONE);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BUILTIN_UNSET);
	return (BUILTIN_NONE);
}

/*
** setup_pipes - Configures pipe connections for a command
** 
** Pipeline flow: cmd1 | cmd2 | cmd3
** - cmd1: stdout -> pipe[0][1]
** - cmd2: stdin <- pipe[0][0], stdout -> pipe[1][1]  
** - cmd3: stdin <- pipe[1][0]
*/
void	setup_pipes(t_exec *ctx, int cmd_index)
{
	int	i;

	if (cmd_index > 0)
		dup2(ctx->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < ctx->cmd_count - 1) 
		dup2(ctx->pipes[cmd_index][1], STDOUT_FILENO);
	
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
}

/*
** setup_redirections - Handles file redirections
** 
** Supports: < (input), > (output), >> (append)
** TODO: Implement heredoc (<<)
*/
void	setup_redirections(t_cmd *cmd)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->filename);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redir->filename);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT_APPEND)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redir->filename);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}

/*
** close_all_pipes - Closes all pipe file descriptors in parent
*/
void	close_all_pipes(t_exec *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
}

/*
** wait_for_all_children - Waits for all child processes
** 
** Returns the exit code of the last command in the pipeline
** This mimics bash behavior where pipeline exit code = last command
*/
int	wait_for_all_children(t_exec *ctx)
{
	int	status;
	int	exit_code;
	int	i;

	status = 0;
	exit_code = 0;
	i = 0;
	while (i < ctx->cmd_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		i++;
	}
	return (exit_code);
}

/*
** cleanup_execution_context - Frees all allocated memory
*/
void	cleanup_execution_context(t_exec *ctx)
{
	if (!ctx)
		return ;
	if (ctx->pids)
		free(ctx->pids);
	free_pipes(ctx->pipes, ctx->cmd_count);
	free(ctx);
}