#pragma once
#include <stddef.h>

// Initializes the shell environment
// Reads the history from the specified path and initializes Lua integration
void shell_init(const char *history_path);

// Starts the shell loop, reading user input and executing commands
void shell_run(void);

// Shuts down the shell environment
void shell_shutdown(const char *history_path);

// Tokenizes the input string into an array of arguments
// Returns the number of arguments parsed
size_t shell_tokenize(char *input, char **args, size_t max_args);
