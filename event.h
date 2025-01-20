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
#define TO_HANDLER(handler) ((HANDLER *)(handler))

enum NOTIFICATION
{
    CREATE_NET = 0,
    OPEN_NET,
    CLOSE_NET,
    TOOL_SELECTED,
    DRAW_REQUESTED,
    CREATE_NODE,
    START_DRAG,
    END_NOTIFICATION

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

            cairo_t *canvas;
            int width;
            int height;

        } draw_event;

        struct
        {

            int n_times;
            double x;
            double y;

        } create_node;
        struct
        {
            double x;
            double y;

        } drag_event;
        struct
        {

            enum TOOL tool;

        } create_net;

    } events;

} EVENT, *EVENT_P;

typedef struct _HANDLER
{

    void (*handler)(EVENT *event, void *processor);
    void *processor;

} HANDLER, HANDLER_P;

extern EVENT *create_event(enum NOTIFICATION notification, ...);

#endif // EVENT_H_INCLUDED