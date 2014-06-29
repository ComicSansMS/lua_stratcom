# makefile for Lua 5.2 beta
project("Lua")

cmake_minimum_required(VERSION 2.8)

set(Lua_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(Lua_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src")

include_directories(${Lua_INCLUDE_DIR})

set(Lua_HEADERS
  "${Lua_INCLUDE_DIR}/lua.h"
  "${Lua_INCLUDE_DIR}/luaconf.h"
  "${Lua_INCLUDE_DIR}/lapi.h"
  "${Lua_INCLUDE_DIR}/lauxlib.h"
  "${Lua_INCLUDE_DIR}/lcode.h"
  "${Lua_INCLUDE_DIR}/lctype.h"
  "${Lua_INCLUDE_DIR}/ldebug.h"
  "${Lua_INCLUDE_DIR}/ldo.h"
  "${Lua_INCLUDE_DIR}/lfunc.h"
  "${Lua_INCLUDE_DIR}/lgc.h"
  "${Lua_INCLUDE_DIR}/llex.h"
  "${Lua_INCLUDE_DIR}/llimits.h"
  "${Lua_INCLUDE_DIR}/lmem.h"
  "${Lua_INCLUDE_DIR}/lobject.h"
  "${Lua_INCLUDE_DIR}/lopcodes.h"
  "${Lua_INCLUDE_DIR}/lparser.h"
  "${Lua_INCLUDE_DIR}/lstate.h"
  "${Lua_INCLUDE_DIR}/lstring.h"
  "${Lua_INCLUDE_DIR}/ltable.h"
  "${Lua_INCLUDE_DIR}/ltm.h"
  "${Lua_INCLUDE_DIR}/lualib.h"
  "${Lua_INCLUDE_DIR}/lundump.h"
  "${Lua_INCLUDE_DIR}/lvm.h"
  "${Lua_INCLUDE_DIR}/lzio.h"
)
source_group(include FILES ${Lua_HEADERS})

set(Lua_SOURCES_CORE
  "${Lua_SOURCE_DIR}/lapi.c"
  "${Lua_SOURCE_DIR}/lcode.c"
  "${Lua_SOURCE_DIR}/lctype.c"
  "${Lua_SOURCE_DIR}/ldebug.c"
  "${Lua_SOURCE_DIR}/ldo.c"
  "${Lua_SOURCE_DIR}/ldump.c"
  "${Lua_SOURCE_DIR}/lfunc.c"
  "${Lua_SOURCE_DIR}/lgc.c"
  "${Lua_SOURCE_DIR}/llex.c"
  "${Lua_SOURCE_DIR}/lmem.c"
  "${Lua_SOURCE_DIR}/lobject.c"
  "${Lua_SOURCE_DIR}/lopcodes.c"
  "${Lua_SOURCE_DIR}/lparser.c"
  "${Lua_SOURCE_DIR}/lstate.c"
  "${Lua_SOURCE_DIR}/lstring.c"
  "${Lua_SOURCE_DIR}/ltable.c"
  "${Lua_SOURCE_DIR}/ltm.c"
  "${Lua_SOURCE_DIR}/lundump.c"
  "${Lua_SOURCE_DIR}/lvm.c"
  "${Lua_SOURCE_DIR}/lzio.c"
)
source_group(core FILES ${Lua_SOURCES_CORE})

set(Lua_SOURCES_LIB
  "${Lua_SOURCE_DIR}/lauxlib.c"
  "${Lua_SOURCE_DIR}/lbaselib.c"
  "${Lua_SOURCE_DIR}/lbitlib.c"
  "${Lua_SOURCE_DIR}/lcorolib.c"
  "${Lua_SOURCE_DIR}/ldblib.c"
  "${Lua_SOURCE_DIR}/liolib.c"
  "${Lua_SOURCE_DIR}/lmathlib.c"
  "${Lua_SOURCE_DIR}/loslib.c"
  "${Lua_SOURCE_DIR}/lstrlib.c"
  "${Lua_SOURCE_DIR}/ltablib.c"
  "${Lua_SOURCE_DIR}/loadlib.c"
  "${Lua_SOURCE_DIR}/linit.c"
)
source_group(lib FILES ${Lua_SOURCES_LIB})

set(LIBRARY_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)

add_library(liblua SHARED ${Lua_SOURCES_CORE} ${Lua_SOURCES_LIB} ${Lua_HEADERS})
target_compile_definitions(liblua PRIVATE LUA_BUILD_AS_DLL)
add_executable(lua ${Lua_SOURCE_DIR}/lua.c)
target_link_libraries(lua liblua)

