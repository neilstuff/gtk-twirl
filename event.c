#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "event.h"



/**
 * @brief Release a Event and free any resources
 *
 * @param event the event to release
 */
void event_release(EVENT *event) { g_free(event); }

/**
 * @brief Create a event object
 * 
 * @param notification the type of event
 * 
 * @return an initialised event
 */
EVENT *create_event(enum NOTIFICATION notification)
{
    EVENT *event = g_malloc(sizeof(EVENT));

    event->notification = notification;

    event->release = event_release;

    return event;

}

/**
 * @brief Create and initialise the Event
 *
 * @param tool the tool selected by the user
 *
 * @return an initialised event
 *
 */
EVENT *create_tool_selected_event(enum TOOL tool)
{
    EVENT *event = create_event(TOOL_SELECTED);
    
    event->events.button_event.tool = tool;

    return event;

}

/**
 * @brief Create a draw event
 *
 * @param cr the cairo drawing context 
 * @param width the width of the drawing Area
 * @param height the height of the drawing Area
 *
 * @return an initialised event
 */
EVENT *create_draw_event(cairo_t *cr, int width, int height)
{
    EVENT *event = create_event(DRAW_REQUESTED);

    event->events.draw_event.canvas = cr;

    return event;
}

/**
 * @brief Create a node (creation) event
 *
 * @param n_times the times the button was pressedt
 * @param x the 'x' location of the node
 * @param y the 'y' location of the node
 *
 * @return an initialised event
 */
EVENT *create_node_event(int n_times, double x, double y)
{
    EVENT *event = create_event(CREATE_NODE);

    event->events.create_node.n_times = n_times;
    event->events.create_node.x = x;
    event->events.create_node.y = y;

    return event;
}

/**
 * @brief Net is created

 * @return an initialised event
 */
EVENT *create_net_event(enum TOOL tool)
{
    EVENT *event = create_event(CREATE_NET);

    event->events.create_net.tool = tool;

    return event;

}


