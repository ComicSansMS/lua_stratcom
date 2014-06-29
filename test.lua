local stratcom = require("lua_stratcom");

function print_event_table(events_t)
    for i, event in ipairs(events_t) do
        local event_str = "{ ";
        for k, v in pairs(event) do
            event_str = event_str .. k .. ": " .. v .. " ";
        end
        event_str = event_str .. "}";
        print("["..i.."] - ".. event_str);
    end
end

stratcom.init();
print("Acquiring device...");

local device = stratcom.open_device();
if(not device) then
    print("Could not acquire device.");
else
    print("Device acquired successfully.");
    local old_state = device:get_input_state();
    while(not device:is_button_pressed(stratcom.BUTTON_REC)) do
        device:read_input();
        local new_state = device:get_input_state();
        local input_events = stratcom.create_input_events_from_states(old_state, new_state);
        print_event_table(input_events);
        old_state = new_state;
    end
end

stratcom.shutdown();

