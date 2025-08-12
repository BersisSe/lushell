#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include "shell.h"
#include "lua_integration.h"
#define COLOR_RESET   "\033[0m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_RED    "\033[31m"

int main(int argc, char const *argv[])
{
     if (argc > 1) {
        lua_init();
        // İlk argüman dosya ismi
        lua_run_file(argv[1]);
        getchar();
        // programı kapatabilir veya shell’e girebilir
        lua_close_state();
        return 0;
    }

    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Cannot find HOME environment variable.\n");
        return 1;
    }
    char history_path[PATH_MAX];
    snprintf(history_path, sizeof(history_path), "%s/.lushell_history", home);
    shell_init(history_path);
    printf("Welcome to "COLOR_BLUE"lushell"COLOR_RESET"\n");
    printf("Type "COLOR_YELLOW"'exit' "COLOR_RESET"or press "COLOR_RED"Ctrl+D"COLOR_RESET" to exit.\n");

    shell_run();
    shell_shutdown(history_path);
}
