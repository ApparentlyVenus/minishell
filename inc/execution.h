#ifndef EXECUTION_H
#define EXECUTION_H

#include "minishell.h"
#include "parser.h"
#include "env.h"

// Enum for builtin command identification
typedef enum e_builtin_type {
    BUILTIN_NONE,
    BUILTIN_CD,
    BUILTIN_ECHO,
    BUILTIN_ENV,
    BUILTIN_EXIT,
    BUILTIN_EXPORT,
    BUILTIN_PWD,
    BUILTIN_UNSET
}   t_builtin_type;

// Execution context for a pipeline
typedef struct s_exec {
    int     cmd_count;      // Number of commands in the pipeline
    int     **pipes;        // Array of pipe file descriptors
    pid_t   *pids;          // Array of child process IDs
    int     last_exit_code; // Exit code of the last command
    t_env   *env_list;      // Pointer to environment variable list
}   t_exec;

// Function prototypes

// Main entry point for executing a parsed command line
void    execute_line(t_node *cmd_list, t_env *env_list);

// Execute a single command node (external or builtin)
int     execute_command(t_node *cmd_node, t_exec *ctx);

// Check if a command is a builtin and return its type
t_builtin_type   get_builtin_type(const char *cmd_name);

// Execute a builtin command
t_builtin_type   execute_builtin(t_node *cmd_node, t_exec *ctx);

// Setup input/output redirections for a command
int     setup_redirections(t_cmd *cmd);

// Setup pipes for a pipeline
int     setup_pipes(t_exec *ctx, int cmd_index);

// Close all pipes in the context
void    close_pipes(t_exec *ctx);

// Wait for all child processes and return the last exit code
int     wait_for_children(t_exec *ctx);

#endif // EXECUTION_H 