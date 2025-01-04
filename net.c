#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>


#include "handler.h"
#include "controller.h"
#include "net.h"

/**
 * Net - implementation
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

/**
 * Invalidate a Drawing Area
 *
 * @param net the active Net
 * @param area the Area to invalidate
 *
 */
 void net_invalidate_bounds (NET * net, GdkRectangle * area)
 {
 }

/**
 * Invalidate the entire window
 *
 * @param net the active Net
 *
 */
 void net_invalidate (NET * net)
 {
 }

/**
 * Get the Current Cursor
 *
 * @param net the Net
 *
 * @return a constructed Cursor
 *
 */
GdkCursor * net_get_current_cursor (NET * net)
{
}

/**
 * Create an empty Net
 *
 * @param net the Net
 *
 */
void create_empty_net (NET * net)
{
}

/**
 * Play the NET
 *
 * @param net the NET
 *
 * @return 'TRUE' the play was successful, 'FALSE' otherwise
 *
 */
gint net_play(struct _NET * net)
{
}

/**
 * Stop the NET
 *
 * @param net the NET
 *
 * @return 'TRUE' the play was successful, 'FALSE' otherwise
 *
 */
gint net_stop(struct _NET * net)
{

    return TRUE;

}

/**
 * Initialise the Net
 *
 * @return an initialised NET
 *
 */
NET * create_net (CONTROLLER * controller)
{
    NET * net = g_malloc(sizeof(NET));

    net->invalidateBounds = net_invalidate_bounds;
    net->invalidate = net_invalidate;

    net->places = g_ptr_array_new ();
    net->transitions = g_ptr_array_new ();
    net->arcs = g_ptr_array_new ();

    return net;

}

/**
 * @brief Reealse a Net and free any resources
 * 
 * @param controller the Net to release
 */
extern void release_net(NET  * net) {
    
    g_free(net);

}
