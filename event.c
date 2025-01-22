/**
 * @file event.c
 * @author Neil.Brittliff@outlook.com
 * @brief An Event is designed to contain data to communicate between objects
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "event.h"

void event_release(EVENT *event) 
{ 
    g_free(event); 
}

EVENT *create_event(enum NOTIFICATION notification, ...)
{
    EVENT *event = g_malloc(sizeof(EVENT));
    va_list args;

    va_start(args, notification);

    event->notification = notification;
    event->release = event_release;

    switch (notification)
    {
        case CREATE_NET:
        {
            event->events.create_net.tool = va_arg(args, enum TOOL);
        }
        break;
        case TOOL_SELECTED:
        {
            event->events.button_event.tool = va_arg(args, enum TOOL);
        }
        break;
        case DRAW_REQUESTED:
        {
            event->events.draw_event.canvas = va_arg(args, cairo_t *);
            event->events.draw_event.width = va_arg(args, int);
            event->events.draw_event.height = va_arg(args, int);
        }
        break;
        case CREATE_NODE:
        {
            event->events.create_node.n_times = va_arg(args, int);
            event->events.create_node.x = va_arg(args, double);
            event->events.create_node.y = va_arg(args, double);
        }
        break;
        case START_DRAG:
        {
            event->events.drag_event.x = va_arg(args, double);
            event->events.drag_event.y = va_arg(args, double);           
        }
    }

    va_end(args);
    return event;
}
