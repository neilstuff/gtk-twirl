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
 * @brief Create and initialise the Event
 *
 * @param tool the tool selected by the user
 *
 * @return an initialised event
 *
 */
EVENT *create_tool_selected_event(enum TOOL tool)
{
    EVENT *event = g_malloc(sizeof(EVENT));

    event->release = event_release;

    event->notification = TOOL_SELECTED;
    event->events.button_event.tool = tool;

    return event;
}

/**
 * @brief Create a draw event
 *
 * @param cr the drawing context
 * @param width the width of the drawing Area
 * @param height the height of the drawing Area
 *
 * @return an initialised event
 */
EVENT *create_draw_event(cairo_t *cr, int width, int height)
{
    EVENT *event = g_malloc(sizeof(EVENT));

    event->release = event_release;

    event->notification = DRAW_REQUESTED;
    event->events.draw_event.cr = cr;

    return event;
}