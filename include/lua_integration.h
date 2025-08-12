#pragma once
#include <lua5.4/lua.h>

// Start the Lua VM
void lua_init(void);

// Run Lua Code
void lua_run_code(const char *code);

// Run Lua File
void lua_run_file(const char *filename);

// Close Lua VM
void lua_close_state(void);

// Get the global Lua State object
lua_State *lua_get_state(void);
