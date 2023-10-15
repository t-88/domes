#ifndef ENGINE_EVENTS_EVENT_HPP
#define ENGINE_EVENTS_EVENT_HPP

namespace Event {
    typedef enum EventType {
        MouseClickEvent,
        MouseScrollEvent,
        KeyPressEvent,
    } EventType;

    typedef enum EventTypeState {
        MouseDown,
        MouseUp,
    } EventTypeState;


    typedef struct Event {
        EventType type;
        EventTypeState type_state;

        int x , y;
        int button;

        int scroll_dir_x , scroll_dir_y;

        int key;
        char* text;
    } Event;
}

#endif // ENGINE_EVENTS_EVENT_HPP