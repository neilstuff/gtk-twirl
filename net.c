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
#include "node.h"

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
 * @brief Notify the net a new tool has been selected
 *
 * @param net the net
 * @param event the tool selection event
 */
void net_tool_event_processor(NET *net, EVENT *event)
{

    net->tool = event->events.button_event.tool;

}

/**
 * @brief Notify the net must be redrawn
 *
 * @param net the net
 * @param event the tool selection event
 */
void net_draw_event_processor(NET *net, EVENT *event) {}

/**
 * @brief Notify the net a node is be created
 *
 * @param net the net
 * @param event the create node event
 */
void net_create_node_processor(NET *net, EVENT *event)
{
    int x = (int)event->events.create_node.x;
    int y = (int)event->events.create_node.y;

    int cx = x - (x % 32);
    int cy = y - (y % 32);

    cx = cx < 32 ? 32 : cx;
    cy = cy < 32 ? 32 : cy;

    printf("Creating Node: %d\n", net->tool);
    printf("Coordinates %d, %d, %d, %d\n", x, y, cx, cy);

    if (net->tool != SELECT_TOOL)
    {
        NODE *node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE);

        node->setPosition(node, cx, cy);

        g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);

    }

}

/**
 * @brief Reealse a Net and free any resources
 *
 * @param net the Net to release
 */
void net_release(NET *net) { g_free(net); }

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

    net->processors[DRAW_REQUESTED] = net_draw_event_processor;
    net->processors[TOOL_SELECTED] = net_tool_event_processor;
    net->processors[CREATE_NODE] = net_create_node_processor;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;

}
