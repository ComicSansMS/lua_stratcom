extern "C" {
    struct lua_State;
    __declspec(dllexport) int luaopen_lua_stratcom(lua_State* L);

    // library functions
    int init(lua_State* L);
    int shutdown(lua_State* L);
    int get_led_for_button(lua_State* L);
    int create_input_events_from_states(lua_State* L);
    int open_device(lua_State* L);
    //todo button range iterators

    // device member functions
    int get_button_led_state(lua_State* L);
    int set_button_led_state(lua_State* L);
    int set_button_led_state_without_flushing(lua_State* L);
    int flush_button_led_state(lua_State* L);
    int led_state_has_unflushed_changes(lua_State* L);
    int get_led_blink_interval(lua_State* L);
    int set_led_blink_interval(lua_State* L);
    int read_button_led_state(lua_State* L);
    int read_led_blink_intervals(lua_State* L);
    int read_input(lua_State* L);
    int read_input_with_timeout(lua_State* L);
    int read_input_non_blocking(lua_State* L);
    int get_input_state(lua_State* L);
    int is_button_pressed(lua_State* L);
    int get_axis_value(lua_State* L);
    int get_slider_state(lua_State* L);
}
