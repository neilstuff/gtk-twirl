/**
 * @file event.c
 * @author Dr. Neil Brittliff (brittliff.org)
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

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"
#include "editor.h"
#include "drawer.h"
#include "node.h"

#include "reader.h"
#include "writer.h"
#include "event.h"


/**
 * @brief deallocate an event's storage
 *
 */
void event_release(EVENT *event)
{
    g_free(event);
}

/**
 * @brief event constructor - based on the notification type
 *
 */
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
            event->events.start_drag_event.x = va_arg(args, double);
            event->events.start_drag_event.y = va_arg(args, double);
            event->events.start_drag_event.mode = va_arg(args, int);
        }
        break;
        case UPDATE_DRAG:
        {
            event->events.update_drag_event.offset_x = va_arg(args, double);
            event->events.update_drag_event.offset_y = va_arg(args, double);
            event->events.update_drag_event.mode = va_arg(args, int);
        }
        break;
        case END_DRAG:
        {
            event->events.end_drag_event.offset_x = va_arg(args, double);
            event->events.end_drag_event.offset_y = va_arg(args, double);
            event->events.end_drag_event.mode = va_arg(args, int);
        }
        break;
        case CONNECT_NODES:
        {
            event->events.connect_event.source = va_arg(args, NODE *);
            event->events.connect_event.target = va_arg(args, POINT *);
        }
        break;
        case ACTIVATE_TOOLBAR:
        {
            event->events.activate_toolbar.activate = va_arg(args, int);
        }
        break;
        case SET_VIEW_SIZE:
        {
            copy_size(va_arg(args, SIZE*), &event->events.set_view_size.size);
        }
        break;
        case ACTIVATE_DELETE:
        {
            event->events.activate_toolbar.activate = va_arg(args, int);
        }
        break;
        case READ_NET:
        {
            event->events.read_net.reader = va_arg(args, READER*);
            event->events.read_net.filename = va_arg(args, char*);
        }
        break;
        case WRITE_NET:
        {
            event->events.write_net.writer = va_arg(args, WRITER*);
            event->events.write_net.filename = va_arg(args, char*);
  
        }
        break;
        case CLEAR_NET:
        break;

        
    }

    va_end(args);
    return event;
}
