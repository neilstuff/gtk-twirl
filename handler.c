#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "handler.h"

/**
 * @brief Notify the handler of an event
 * 
 * @param event the event
 */
void handler_notify(EVENT * event)
{
}

/**
 * @brief Release the Handler and free any associated resources
 * 
 * @param handler the Handler to release
 */
void handler_release(HANDLER  * handler) 
{

    g_free(handler);

}

/**
 * Create and initialise the handler
 *
 * @return an initialised handler
 *
 */
HANDLER * handler_create()
{
    HANDLER *handler = g_malloc(sizeof(HANDLER));

    handler->release = handler_release;
    handler->notify = handler_notify;

    return handler;

}