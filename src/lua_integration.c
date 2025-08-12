#include "lua_integration.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
#include <dirent.h>
static lua_State *L = NULL;


void lua_run_code(const char *code) {
    if (L == NULL) {
        fprintf(stderr, "Lua not initialized.\n");
        return;
    }
    if (luaL_dostring(L, code) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void lua_run_file(const char *filename) {
    if (L == NULL) {
        fprintf(stderr, "Lua not initialized.\n");
        return;
    }
    if (luaL_dofile(L, filename) != LUA_OK) {
        fprintf(stderr, "Lua file error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void lua_close_state(void) {
    if (L) {
        lua_close(L);
        L = NULL;
    }
}

lua_State *lua_get_state(void) {
    return L;
}

static int l_cd(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    if (chdir(path) != 0) {
        lua_pushnil(L);
        lua_pushstring(L, "chdir failed");
        return 2;
    }
    lua_pushboolean(L, 1);
    return 1;
}

static int l_ls(lua_State *L) {
    const char *path = luaL_optstring(L, 1, ".");
    DIR *dir = opendir(path);
    if (!dir) {
        lua_pushnil(L);
        lua_pushfstring(L, "cannot open directory '%s'", path);
        return 2;
    }

    lua_newtable(L);  

    struct dirent *entry;
    int i = 1;
    while ((entry = readdir(dir)) != NULL) {
        lua_pushinteger(L, i++);
        lua_pushstring(L, entry->d_name);
        lua_settable(L, -3);
    }

    closedir(dir);
    return 1;  // table döndür
}

void register_shell_functions(lua_State *L) {
    lua_register(L, "cd", l_cd);
    lua_register(L, "ls", l_ls);
}

void lua_init(void) {
    if (L != NULL) return; // Zaten init edilmişse tekrar yapma
    L = luaL_newstate();
    luaL_openlibs(L);
    register_shell_functions(L);
}