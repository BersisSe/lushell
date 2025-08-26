#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>

#include "shell.h"
#include "lua_integration.h"

// Terminal Color Macros
#define COLOR_RESET   "\033[0m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_RED    "\033[31m"

int main(int argc, char const *argv[])
{
    // if a file passed as a argument execute it using Lushell
     if (argc > 1) {
        lua_init();
        lua_run_file(argv[1]);
        // Waiting for input before closing the terminal window
        getchar();
        lua_close_state();
        return 0;
    }

    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Cannot find HOME environment variable.\n");
        return 1;
    }
    char lushell_folder[PATH_MAX];
    char history_path[PATH_MAX];
    snprintf(lushell_folder, sizeof(lushell_folder), "%s/.lushell", home);
    if (!opendir(lushell_folder)){
        printf(COLOR_RED".lushell Folder Not Found. Check Your Instalation"COLOR_RESET"\n");
    }
    strcpy(history_path,lushell_folder);
    strcat(history_path, "/history");
    
    shell_init(history_path);
    printf("Welcome to "COLOR_BLUE"lushell"COLOR_RESET"\n");
    printf("Type "COLOR_YELLOW"'exit' "COLOR_RESET"or press "COLOR_RED"Ctrl+D"COLOR_RESET" to exit.\n");

    shell_run();
    shell_shutdown(history_path);
}
