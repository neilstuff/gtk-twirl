#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

/**
 * EVENT - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#define SELECT_TOOL 0
#define PLACE_TOOL 1
#define TRANSITION_TOOL 1

typedef struct _EVENT {

    void (*release) (struct _EVENT * event);

    union {
        struct {
            
            int tool;

        } button_event;

        struct {
            
            int x;
            int y;

        } mouse_down;

    } events;
        
} EVENT, * EVENT_P;

extern EVENT * event_create_tool_selected(int tool);

#endif // EVENT_H_INCLUDED