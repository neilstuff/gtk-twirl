/**
 * @brief Net - implementation
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#include <cairo.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "controller.h"
#include "event.h"
#include "net.h"

/**
 * @brief Invalidate a Drawing Area
 *
 * @param net the active Net
 * @param area the Area to invalidate
 *
 */
void net_invalidate_bounds(NET *net, GdkRectangle *area) {}

/**
 * @brief Invalidate the entire window
 *
 * @param net the active Net
 *
 */
void net_invalidate(NET *net) {}

/**
 * @brief Get the Current Cursor
 *
 * @param net the Net
 *
 * @return a constructed Cursor
 *
 */
GdkCursor *net_get_current_cursor(NET *net) {}

/**
 * @brief Create an empty Net
 *
 * @param net the Net
 *
 */
void create_empty_net(NET *net) {}

/**
 * @brief Play the NET
 *
 * @param net the NET
 *
 * @return 'TRUE' the play was successful, 'FALSE' otherwise
 *
 */
gint net_play(struct _NET *net) {}

/**
 * @brief Stop the NET
 *
 * @param net the NET
 *
 * @return 'TRUE' the play was successful, 'FALSE' otherwise
 *
 */
gint net_stop(struct _NET *net) { return TRUE; }

/**
 * @brief Notify the handler of an event
 *
 * @param net the Net
 * @param event the event
 */
void net_notify(NET *net, EVENT *event)
{

    switch (event->notification)
    {

    case TOOL_SELECTED:
        net->tool = event->events.button_event.tool;
        break;
    }

}

/**
 * @brief Reealse a Net and free any resources
 *
 * @param net the Net to release
 */
extern void net_release(NET *net) { g_free(net); }

/**
 * @brief Initialise the Net
 *
 * @return an initialised NET
 *
 */
NET *net_create(CONTROLLER *controller)
{
    NET *net = g_malloc(sizeof(NET));

    net->controller = controller;

    net->notify = net_notify;
    net->invalidateBounds = net_invalidate_bounds;
    net->invalidate = net_invalidate;
    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;
}
