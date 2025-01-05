#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "event.h"

/**
 * @brief Release a Event and free any resources
 * 
 * @param event the event to release
 */
void event_release(EVENT  * event) 
{

    g_free(event);

}

/**
 * Create and initialise the Event
 *
 *  @param tool the tool selected by the user
 * 
 * @return an initialised event
 *
 */
EVENT * event_create_tool_selected(int tool)
{
    EVENT *event = g_malloc(sizeof(EVENT));

    event->release = event_release;

    event->events.button_event.tool = tool;

    return event;

}