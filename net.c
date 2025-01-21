/**
 * @file net.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief A "Petri Net" representation
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
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
    SELECT_NODE_BY_POINT,
    NODE_AT_POINT,
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
        struct
        {

            POINT point;

        } point_context;
    };

} CONTEXT, *CONTEXT_P;

/**
 * @brief Returns true if the point is in the bounds of the tested node
 *
 * @param point the point to test
 * @param node the current node in the array
 * @return gboolean true if in the bounds, false otherwise
 */
gboolean net_node_find_by_point(gconstpointer node, gconstpointer point)
{
    printf("IM HERE 1 - %f:%f\n", TO_POINT(point)->x, TO_POINT(point)->y);
    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}

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
    case SELECT_NODE_BY_POINT:
        if (TO_NODE(node)->isNodeAtPoint(TO_NODE(node), &TO_CONTEXT(context)->point_context.point))
        {
            TO_NODE(node)->selected = TRUE;
        }
        else
        {
            TO_NODE(node)->selected = FALSE;
        }
        break;
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

NODE *net_find_node_by_point(NET *net, POINT *point)
{
    NODE *node = NULL;
    guint index;

    if (g_ptr_array_find_with_equal_func(net->places,
                                         point,
                                         net_node_find_by_point,
                                         &index))
    {

        return g_ptr_array_index(net->places, index);
    }

    if (g_ptr_array_find_with_equal_func(net->transitions,
                                         point,
                                         net_node_find_by_point,
                                         &index))
    {

        return g_ptr_array_index(net->transitions, index);
    }

    return NULL;
}

/**
 * @brief Apply a context to all nodes
 *
 * @param net that active net
 * @param context context to apply
 */
void net_apply_context_all_nodes(NET *net, CONTEXT *context)
{

    g_ptr_array_foreach(net->places,
                        net_node_iterator, context);
    g_ptr_array_foreach(net->transitions,
                        net_node_iterator, context);
}

/**
 * @brief Apply an action SELECT or UNSELECT on all nodes
 *
 * @param net that active net
 * @param action the action either SELECT OR UNSELECT
 */
void net_apply_action_all_nodes(NET *net, enum ACTION action)
{
    CONTEXT context;

    context.action = action;

    net_apply_context_all_nodes(net, &context);
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
void net_select_node_processor(NET *net, EVENT *event)
{
    POINT point;

    set_point(&point, event->events.create_node.x, event->events.create_node.y);

    net_apply_action_all_nodes(net, UNSELECT_ALL);

    if (net->tool == SELECT_TOOL)
    {
        CONTEXT context;

        context.action = SELECT_NODE_BY_POINT;
        context.point_context.point.x = event->events.create_node.x;
        context.point_context.point.y = event->events.create_node.y;

        net_apply_context_all_nodes(net, &context);

        net->controller->redraw(net->controller);
    }
    {
        NODE * node = net_find_node_by_point(net, &point);
        
        if (node == NULL)
        {
            CONTEXT context;
            
            node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE);

            int x = (int)event->events.create_node.x;
            int y = (int)event->events.create_node.y;

            int cx = x - (x % 30);
            int cy = y - (y % 30);

            cx = cx < 30 ? 30 : cx;
            cy = cy < 30 ? 30 : cy;

            printf("Coordinates %d, %d, %d, %d\n", x, y, cx, cy);

            context.action = DRAW_NODE;

            node->setPosition(node, cx, cy);

            g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);
        } 
        else 
        {
            node->selected = TRUE;
        }

        net->controller->redraw(net->controller);
    }
}

/**
 * @brief Notify the net to connect a node
 *
 * @param net the net
 * @param event the drag event
 */
void net_start_drag_processor(NET *net, EVENT *event)
{
    POINT point;
    printf("Points %f:%f\n", event->events.drag_event.x, event->events.drag_event.y);

    set_point(&point, event->events.drag_event.x, event->events.drag_event.y);

    net_find_node_by_point(net, &point);
}

/**
 * @brief Notify the net to create a node or select an existing node
 *
 * @param net the net
 * @param event the tool selection event
 */
void net_create_processor(NET *net, EVENT *event)
{

    net->tool = event->events.create_net.tool;
}

/**
 * @brief Event processor
 *
 * @param event the event to process
 * @param processor in this case the 'net' is the processor
 */
void net_event_handler(EVENT *event, void *processor)
{

    TO_NET(processor)->processors[event->notification](TO_NET(processor), TO_EVENT(event));
}

/**
 * @brief Reealse a Net and free any resources
 *
 * @param net the Net to release
 */
void net_release(NET *net)
{
    g_free(net);
}

/**
 * @brief Initialise the Net
 *
 * @return an initialised NET
 *
 */
NET *net_create(CONTROLLER *controller)
{
    NET *net = g_malloc(sizeof(NET));

    net->handler.handler = net_event_handler;
    net->handler.processor = net;

    net->controller = controller;
    net->processors[DRAW_REQUESTED] = net_draw_event_processor;
    net->processors[TOOL_SELECTED] = net_tool_event_processor;
    net->processors[CREATE_NODE] = net_select_node_processor;
    net->processors[CREATE_NET] = net_create_processor;
    net->processors[START_DRAG] = net_start_drag_processor;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;
}
