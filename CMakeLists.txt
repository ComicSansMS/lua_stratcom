project(lua-stratcom)
cmake_minimum_required(VERSION 3.0)

set(LIBSTRATCOM_PREFIX_PATH "" CACHE PATH "Set this to the installation directory of the libstratcom binaries")

if(LIBSTRATCOM_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${LIBSTRATCOM_PREFIX_PATH})
endif()
find_package(libstratcom NO_MODULE REQUIRED)

add_subdirectory(lua)

add_library(lua_stratcom SHARED src/lua_stratcom.cpp include/lua_stratcom.h)
target_link_libraries(lua_stratcom stratcom liblua)
target_include_directories(lua_stratcom PRIVATE lua/src/)
target_include_directories(lua_stratcom PRIVATE include)

if(WIN32)
    get_property(dll TARGET stratcom PROPERTY IMPORTED_LOCATION_DEBUG)
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})
    get_property(dll TARGET stratcom PROPERTY IMPORTED_LOCATION_RELEASE)
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})
endif()
