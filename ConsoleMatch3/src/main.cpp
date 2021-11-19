#include "Controller.h"

extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

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