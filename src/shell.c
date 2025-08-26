#include "shell.h"
#include "lua_integration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 100
#define COLOR_RESET "\033[0m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"

static char cwd[PATH_MAX];

void shell_init(const char *history_path)
{
    read_history(history_path);
    lua_init();
}

void shell_run(void)
{
    char *input;
    char *args[MAX_ARGS];

    while (1)
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            strcpy(cwd, "?");
        }

        char prompt[PATH_MAX + 50];
        snprintf(prompt, sizeof(prompt), COLOR_BLUE "lushell" COLOR_RESET ":" COLOR_GREEN "%s" COLOR_RESET "> ", cwd);

        input = readline(prompt);
        if (!input)
            break; // Ctrl+D

        if (strlen(input) == 0)
        {
            free(input);
            continue;
        }

        add_history(input);

        // Exit Command
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        // Command Tokenizer
        size_t argc = shell_tokenize(input, args, MAX_ARGS);

        // cd Command
        if (strcmp(args[0], "cd") == 0)
        {
            if (argc < 2)
            {
                fprintf(stderr, "Usage: cd <dir>\n");
            }
            else if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
            free(input);
            continue;
        }

        if (strcmp(args[0], "lu") == 0)
        {
            if (argc < 2)
            {
                fprintf(stderr, "Usage: lu <script.lua>\n");
            }
            else
            {
                const char *script_path = args[1];
                lua_run_file(script_path);
            }
            free(input);
            continue;
        }

        // fork + exec
        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("exec");
            exit(1);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("fork");
        }

        free(input);
    }
}

void shell_shutdown(const char *history_path)
{
    write_history(history_path);
    lua_close_state();
}

size_t shell_tokenize(char *input, char **args, size_t max_args)
{
    size_t argc = 0;
    char *token = strtok(input, " ");
    while (token && argc < max_args - 1)
    {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;
    return argc;
}
