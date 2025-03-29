/**
 * @file event.h
 * @author Neil.Brittliff@outlook.com
 * @brief prototype - an event is designed to contain data to communicate between objects
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include "geometry.h"

#define TO_EVENT(event) ((EVENT *)(event))

/**
 * @brief notification define the event type
 * 
 */
enum NOTIFICATION
{
    CREATE_NET = 0,
    OPEN_NET,
    CLOSE_NET,
    TOOL_SELECTED,
    DELETE_SELECTED,
    DRAW_REQUESTED,
    CREATE_NODE,
    START_DRAG,
    UPDATE_DRAG,
    END_DRAG,
    CONNECT_NODES,
    ACTIVATE_TOOLBAR,
    ACTIVATE_DELETE,
    SET_VIEW_SIZE,
    CLEAR_EDITOR,
    END_NOTIFICATION
};

/**
 * @brief user selected tool from the tool pane; can be either - 'select', 'place', and 'transition'
 * 
 */
enum TOOL
{
    SELECT_TOOL,
    PLACE_TOOL,
    TRANSITION_TOOL
};

/**
 * @brief an event structure is a union of multiple event types
 * 
 */
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

            int mode;

        } start_drag_event;
        
        struct
        {
            double offset_x;
            double offset_y;

            int mode;

        } update_drag_event;

        struct
        {
            double offset_x;
            double offset_y;

            int mode;

        } end_drag_event;

        struct
        {
            struct _NODE * source;
            POINT * target;

        } connect_event;

        struct
        {

            enum TOOL tool;

        } create_net;

        struct
        {

            int activate;

        } activate_toolbar;

        struct
        {

            SIZE size;

        } set_view_size;

    } events;

} EVENT, *EVENT_P;


extern EVENT *create_event(enum NOTIFICATION notification, ...);

#endif // EVENT_H_INCLUDED