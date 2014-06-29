
#include <lua_stratcom.h>

#include <lua.hpp>

#include <stratcom.h>

#define L_REG_ENTRY(e) { #e, e }

luaL_Reg const lua_stratcom[] = {
    L_REG_ENTRY(init),
    L_REG_ENTRY(shutdown),
    L_REG_ENTRY(open_device),
    L_REG_ENTRY(get_led_for_button),
    L_REG_ENTRY(create_input_events_from_states),
    {nullptr, nullptr}
};

luaL_Reg const lua_stratcom_device_member_functions[] = {
    L_REG_ENTRY(set_button_led_state),
    L_REG_ENTRY(get_button_led_state),
    L_REG_ENTRY(set_button_led_state_without_flushing),
    L_REG_ENTRY(flush_button_led_state),
    L_REG_ENTRY(led_state_has_unflushed_changes),
    L_REG_ENTRY(get_led_blink_interval),
    L_REG_ENTRY(set_led_blink_interval),
    L_REG_ENTRY(read_button_led_state),
    L_REG_ENTRY(read_led_blink_intervals),
    L_REG_ENTRY(read_input),
    L_REG_ENTRY(read_input_with_timeout),
    L_REG_ENTRY(read_input_non_blocking),
    L_REG_ENTRY(get_input_state),
    L_REG_ENTRY(is_button_pressed),
    L_REG_ENTRY(get_axis_value),
    L_REG_ENTRY(get_slider_state),
    {nullptr, nullptr}
};

#undef L_REG_ENTRY

namespace {
    char const* const TYPE_MT_KEY_DEVICE = "stratcom.device";
    char const* const TYPE_MT_KEY_INPUT_STATE = "stratcom.input_state";

    extern "C" int close_device(lua_State* L)
    {
        stratcom_device* device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
        stratcom_close_device(device);
        return 0;
    }
}

int luaopen_lua_stratcom(lua_State* L)
{
    // create metatable for stratcom.device userdata type
    if(luaL_newmetatable(L, TYPE_MT_KEY_DEVICE) != 1) {
        lua_pushstring(L, "Unable to register stratcom.device type with Lua.");
        lua_error(L);
    }

    // create the __index table containing the member functions for device
    lua_newtable(L);
    luaL_setfuncs(L, lua_stratcom_device_member_functions, 0);

    // setup metamethods
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, close_device);
    lua_setfield(L, -2, "__gc");

    lua_pop(L, 1);

    // create metatable for stratcom.input_state userdata type
    if(luaL_newmetatable(L, TYPE_MT_KEY_INPUT_STATE) != 1) {
        lua_pushstring(L, "Unable to register stratcom.input_state type with Lua.");
        lua_error(L);
    }
    lua_pop(L, 1);

    // create library table
    luaL_newlib(L, lua_stratcom);
#define ADD_CONSTANT(c) \
    lua_pushstring(L, #c); \
    lua_pushnumber(L, STRATCOM_##c); \
    lua_settable(L, -3)

    ADD_CONSTANT(LEDBUTTON_1);
    ADD_CONSTANT(LEDBUTTON_2);
    ADD_CONSTANT(LEDBUTTON_3);
    ADD_CONSTANT(LEDBUTTON_4);
    ADD_CONSTANT(LEDBUTTON_5);
    ADD_CONSTANT(LEDBUTTON_6);
    ADD_CONSTANT(LEDBUTTON_REC);
    ADD_CONSTANT(LEDBUTTON_ALL);
    ADD_CONSTANT(LEDBUTTON_NONE);

    ADD_CONSTANT(BUTTON_1);
    ADD_CONSTANT(BUTTON_2);
    ADD_CONSTANT(BUTTON_3);
    ADD_CONSTANT(BUTTON_4);
    ADD_CONSTANT(BUTTON_5);
    ADD_CONSTANT(BUTTON_6);
    ADD_CONSTANT(BUTTON_SHIFT1);
    ADD_CONSTANT(BUTTON_SHIFT2);
    ADD_CONSTANT(BUTTON_SHIFT3);
    ADD_CONSTANT(BUTTON_REC);
    ADD_CONSTANT(BUTTON_PLUS);
    ADD_CONSTANT(BUTTON_MINUS);

    ADD_CONSTANT(AXIS_X);
    ADD_CONSTANT(AXIS_Y);
    ADD_CONSTANT(AXIS_Z);

    ADD_CONSTANT(LED_ON);
    ADD_CONSTANT(LED_OFF);
    ADD_CONSTANT(LED_BLINK);

    ADD_CONSTANT(SLIDER_UNKNOWN);
    ADD_CONSTANT(SLIDER_1);
    ADD_CONSTANT(SLIDER_2);
    ADD_CONSTANT(SLIDER_3);

    ADD_CONSTANT(RET_SUCCESS);
    ADD_CONSTANT(RET_ERROR);
    ADD_CONSTANT(RET_NO_DATA);
#undef ADD_CONSTANT
    return 1;
}

int init(lua_State* L)
{
    stratcom_init();
    return 0;
}

int shutdown(lua_State* L)
{
    stratcom_shutdown();
    return 0;
}

int get_led_for_button(lua_State* L)
{
    auto button = static_cast<stratcom_button>(luaL_checkint(L, 1));
    auto ret = stratcom_get_led_for_button(button);
    lua_pushinteger(L, ret);
    return 1;
}

int create_input_events_from_states(lua_State* L)
{
    auto old_state = static_cast<stratcom_input_state*>(luaL_checkudata(L, 1, TYPE_MT_KEY_INPUT_STATE));
    auto new_state = static_cast<stratcom_input_state*>(luaL_checkudata(L, 2, TYPE_MT_KEY_INPUT_STATE));
    auto events = stratcom_create_input_events_from_states(old_state, new_state);
    lua_newtable(L);
    int count = 1;
    for(auto event_it = events; event_it != nullptr; event_it = event_it->next, ++count) {
        // push index into return table
        lua_pushinteger(L, count);

        // create table for single event
        lua_newtable(L);
        switch(event_it->type) {
        case STRATCOM_INPUT_EVENT_BUTTON: lua_pushstring(L, "button"); break;
        case STRATCOM_INPUT_EVENT_AXIS:   lua_pushstring(L, "axis"); break;
        case STRATCOM_INPUT_EVENT_SLIDER: lua_pushstring(L, "slider"); break;
        default:                          lua_pushstring(L, "unknown"); break;
        }
        lua_setfield(L, -2, "type");
        switch(event_it->type) {
        case STRATCOM_INPUT_EVENT_BUTTON:
            lua_pushinteger(L, event_it->desc.button.button);
            lua_setfield(L, -2, "button");
            lua_pushinteger(L, event_it->desc.button.status);
            lua_setfield(L, -2, "status");
            break;
        case STRATCOM_INPUT_EVENT_AXIS:
            lua_pushinteger(L, event_it->desc.axis.axis);
            lua_setfield(L, -2, "axis");
            lua_pushinteger(L, event_it->desc.axis.status);
            lua_setfield(L, -2, "status");
            break;
        case STRATCOM_INPUT_EVENT_SLIDER:
            lua_pushinteger(L, event_it->desc.slider.status);
            lua_setfield(L, -2, "status");
            break;
        }

        // set field in return table
        lua_settable(L, -3);
    }
    stratcom_free_input_events(events);
    return 1;
}

int open_device(lua_State* L)
{
    stratcom_device* device = stratcom_open_device();
    if(device) {
        void* userdata_memory = lua_newuserdata(L, sizeof(void*));
        luaL_setmetatable(L, TYPE_MT_KEY_DEVICE);
        *static_cast<stratcom_device**>(userdata_memory) = device;
        return 1;
    }
    return 0;
}

int get_button_led_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto led = static_cast<stratcom_button_led>(luaL_checkint(L, 2));
    auto state = stratcom_get_button_led_state(device, led);
    lua_pushinteger(L, state);
    return 1;
}

int set_button_led_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto led = static_cast<stratcom_button_led>(luaL_checkint(L, 2));
    auto state = static_cast<stratcom_led_state>(luaL_checkint(L, 3));
    auto ret = stratcom_set_button_led_state(device, led, state);
    lua_pushinteger(L, ret);
    return 1;
}

int set_button_led_state_without_flushing(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto led = static_cast<stratcom_button_led>(luaL_checkint(L, 2));
    auto state = static_cast<stratcom_led_state>(luaL_checkint(L, 3));
    stratcom_set_button_led_state_without_flushing(device, led, state);
    return 0;
}

int flush_button_led_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_flush_button_led_state(device);
    lua_pushinteger(L, ret);
    return 1;
}

int led_state_has_unflushed_changes(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_led_state_has_unflushed_changes(device);
    lua_pushboolean(L, ret);
    return 1;
}

int get_led_blink_interval(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    uint8_t on_time;
    uint8_t off_time;
    stratcom_get_led_blink_interval(device, &on_time, &off_time);
    lua_pushinteger(L, on_time);
    lua_pushinteger(L, off_time);
    return 2;
}

int set_led_blink_interval(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto on_time = static_cast<uint8_t>(luaL_checkint(L, 2));
    auto off_time = static_cast<uint8_t>(luaL_checkint(L, 3));
    auto ret = stratcom_set_led_blink_interval(device, on_time, off_time);
    lua_pushinteger(L, ret);
    return 1;
}

int read_button_led_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_read_button_led_state(device);
    lua_pushinteger(L, ret);
    return 1;
}

int read_led_blink_intervals(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_read_led_blink_intervals(device);
    lua_pushinteger(L, ret);
    return 1;
}

int read_input(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_read_input(device);
    lua_pushinteger(L, ret);
    return 1;
}

int read_input_with_timeout(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto timeout = luaL_checkint(L, 2);
    auto ret = stratcom_read_input_with_timeout(device, timeout);
    lua_pushinteger(L, ret);
    return 1;
}

int read_input_non_blocking(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_read_input_non_blocking(device);
    lua_pushinteger(L, ret);
    return 1;
}

int get_input_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = static_cast<stratcom_input_state*>(lua_newuserdata(L, sizeof(stratcom_input_state)));
    luaL_setmetatable(L, TYPE_MT_KEY_INPUT_STATE);
    *ret = stratcom_get_input_state(device);
    return 1;
}

int is_button_pressed(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto button = static_cast<stratcom_button>(luaL_checkint(L, 2));
    auto ret = stratcom_is_button_pressed(device, button);
    lua_pushboolean(L, ret);
    return 1;
}

int get_axis_value(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto axis = static_cast<stratcom_axis>(luaL_checkint(L, 2));
    auto ret = stratcom_get_axis_value(device, axis);
    lua_pushinteger(L, ret);
    return 1;
}

int get_slider_state(lua_State* L)
{
    auto device = *static_cast<stratcom_device**>(luaL_checkudata(L, 1, TYPE_MT_KEY_DEVICE));
    auto ret = stratcom_get_slider_state(device);
    lua_pushinteger(L, ret);
    return 1;
}
