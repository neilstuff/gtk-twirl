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
#include "drawer.h"

#define TO_CONTEXT(context) ((CONTEXT *)(context))

enum ACTION
{
    DRAW_NODE,
    UNSELECT_ALL,
    EOF_ACTIONS
};

typedef struct _CONTEXT
{
    enum ACTION action;

    union
    {
        struct
        {

            DRAWER *drawer;

        } draw_context;

    };

} CONTEXT, *CONTEXT_P;

/**
 * @brief Process the action'x context - Draw all nodes, unselect nodes, etc
 *
 * @param node the current node
 * @param context the action's context
 */
void net_node_iterator(gpointer node, gpointer context)
{

    switch (TO_CONTEXT(context)->action)
    {
    case DRAW_NODE:
        TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer), TO_NODE(node));
        break;

    case UNSELECT_ALL:
        TO_NODE(node)->selected = FALSE;
        break;
    }

}

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
 * @param event the draw/redraw event
 */
void net_draw_event_processor(NET *net, EVENT *event)
{
    CONTEXT context;

    context.action = DRAW_NODE;
    context.draw_context.drawer = create_drawer(event->events.draw_event.canvas);

    g_ptr_array_foreach(net->places,
                        net_node_iterator, &context);
    g_ptr_array_foreach(net->transitions,
                        net_node_iterator, &context);

    context.draw_context.drawer->release(context.draw_context.drawer);
}

/**
 * @brief Notify the net a node is be created
 *
 * @param net the net
 * @param event the create node event
 */
void net_create_node_processor(NET *net, EVENT *event)
{
    CONTEXT context;

    context.action = UNSELECT_ALL;

    int x = (int)event->events.create_node.x;
    int y = (int)event->events.create_node.y;

    int cx = x - (x % 30);
    int cy = y - (y % 30);

    cx = cx < 30 ? 30 : cx;
    cy = cy < 30 ? 30 : cy;

    printf("Coordinates %d, %d, %d, %d\n", x, y, cx, cy);

    g_ptr_array_foreach(net->places,
                        net_node_iterator, &context);
    g_ptr_array_foreach(net->transitions,
                        net_node_iterator, &context);

    if (net->tool != SELECT_TOOL)
    {

        NODE *node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE);

        node->setPosition(node, cx, cy);

        g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);

        net->controller->redraw(net->controller);
    }
}


/**
 * @brief Notify the net has beencreate
 *
 * @param net the net
 * @param event the tool selection event
 */
void net_create_processor(NET *net, EVENT *event)
{

    net->tool = event->events.create_net.tool;
    
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
    net->processors[CREATE_NET] = net_create_processor;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;

}
