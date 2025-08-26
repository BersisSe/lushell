#include "lua_integration.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <glob.h>
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
    return 1;  
}
static int l_pwd(lua_State *L){
    char buf[PATH_MAX];
    if (getcwd(buf,sizeof(buf)) != NULL)
    {
        lua_pushstring(L,buf);
        return 1;
    }
    return luaL_error(L, "Can't get the current directory");
}
static int l_mkdir(lua_State *L){
    const char *path = luaL_checkstring(L,1);
    if (mkdir(path, 0777) == 0)
    {
        lua_pushboolean(L,1);
        return 1;
    }
    lua_pushboolean(L,0);
    return 1;
}

static int l_rm(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    if (unlink(path) == 0) {
        lua_pushboolean(L, 1);  // Success
        return 1;
    }
    lua_pushnil(L);
    lua_pushfstring(L, "failed to remove '%s'", path);
    return 2;
}

static int l_glob(lua_State *L) {
    const char *pattern = luaL_checkstring(L, 1);

    glob_t results;
    int ret = glob(pattern, 0, NULL, &results);
    if (ret != 0) {
        lua_pushnil(L);
        lua_pushfstring(L, "no matches for pattern '%s'", pattern);
        return 2;
    }
    lua_newtable(L);
    for (size_t i = 0; i < results.gl_pathc; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushstring(L, results.gl_pathv[i]);
        lua_settable(L, -3);
    }

    globfree(&results);
    return 1;
}

static int l_getenv(lua_State *L){
    const char *env_param = luaL_checkstring(L,1);
    char *env = getenv(env_param);
    if(env != NULL){
        lua_pushstring(L,env);
        return 1;
    } else {
        lua_pushnil(L);
        return 1;
    }
}

static int l_setenv(lua_State *L){
    const char *env_name = luaL_checkstring(L,1);
    const char *env_val = luaL_checkstring(L,2);
    int res = setenv(env_name,env_val,1);
    if(res == 0){
        lua_pushboolean(L,1);
        return 1;
    } else {
        lua_pushboolean(L,0);
        return 1;
    }
}

void register_shell_functions(lua_State *L) {
    lua_register(L, "cd", l_cd);
    lua_register(L, "ls", l_ls);
    lua_register(L,"pwd",l_pwd);
    lua_register(L,"mkdir",l_mkdir);
    lua_register(L,"rm",l_rm);
    lua_register(L,"getenv",l_getenv);
    lua_register(L,"setenv",l_setenv);
    lua_register(L,"glob",l_glob);
}

void lua_init(void) {
    if (L != NULL) return; 
    L = luaL_newstate();
    luaL_openlibs(L);
    register_shell_functions(L);
}