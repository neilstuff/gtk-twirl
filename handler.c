/**
 * @file handler.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  connector is a graphical way to connect a place with a transition or a transition to a place
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>


#include "artifact.h"
#include "drawer.h"
#include "editor.h"

#include "node.h"
#include "event.h"
#include "handler.h"

/**
 * @brief deallocate an handlers's storage
 *
 */
void handler_release(HANDLER *handler)
{
    g_free(handler);
}

/**
 * @brief create a handler
 *
 */
HANDLER * create_handler(HANDLER_T callback, void* processor)
{
    HANDLER * handler = g_malloc(sizeof(HANDLER));

    handler->handler = callback;
    handler->processor = processor;
    handler->release = handler_release;
     
    return handler;

}
