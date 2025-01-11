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

#define TO_EVENT(event) ((EVENT *)(event))
#define NOTIFICATIONS 6

enum NOTIFICATION
{
    NEW_NET = 0,
    OPEN_NET = 1,
    CLOSE_NET = 2,
    TOOL_SELECTED = 3,
    DRAW_REQUESTED = 4,
    CREATE_NODE = 5,
    END_NOTIFICATION = 6
    
};

enum TOOL
{
    SELECT_TOOL,
    PLACE_TOOL,
    TRANSITION_TOOL
};

typedef struct _EVENT
{

    void (*release)(struct _EVENT *event);

    enum NOTIFICATION notification;

    union
    {
        struct
        {

            enum TOOL tool;

        } button_event;

        struct
        {

            cairo_t *cr;

        } draw_event;

        struct
        {

            int x;
            int y;

        } mouse_down;

    } events;

} EVENT, *EVENT_P;

extern EVENT *create_tool_selected_event(enum TOOL);
extern EVENT *create_draw_event(cairo_t *cr, int width, int height);

#endif // EVENT_H_INCLUDED