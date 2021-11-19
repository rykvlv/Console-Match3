#include "Controller.h"

extern "C" {
# include <Lua/lua.h>
# include <Lua/lauxlib.h>
# include <Lua/lualib.h>
}

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

extern "C" int sum(lua_State * L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = a + b;

    lua_pushinteger(L, result);

    return 1;
}

extern "C" int Run(lua_State * L) {
    RBW::Controller controller;
    controller.Run();

    lua_pushinteger(L, 0);

    return 1;
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "Run", Run);
    luaL_dofile(L, "script.lua");
    
    lua_close(L);
    return 0;
}