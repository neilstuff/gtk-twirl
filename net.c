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

#include "artifact.h"
#include "drawer.h"

#include "node.h"
#include "arc.h"

#include "event.h"
#include "controller.h"
#include "net.h"

#include "connector.h"



#define TO_CONTEXT(context) ((CONTEXT *)(context))

enum ACTION
{
    DRAW_NODE = 0,
    UNSELECT_ALL = 1,
    SELECT_NODE_BY_POINT = 2,
    NODE_AT_POINT = 3,
    GET_NEXT_NODE_ID = 4,
    DRAW_ARC = 5,
    EOF_ACTIONS
};

/**
 * @brief private structure used to call functions with a general parameter container
 *
 */
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
        struct
        {

            int id;

        } id_context;
    };

} CONTEXT, *CONTEXT_P;

/**
 * @brief create a Node or Place
 *
 */
gboolean net_node_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}

/**
 * @brief  iterator of nodes and arcs - the context determines the processor to apply to the artifact
 *
 */
void net_artifact_iterator(gpointer artifcat, gpointer context)
{

    switch (TO_CONTEXT(context)->action)
    {
    case SELECT_NODE_BY_POINT:
        if (TO_NODE(artifcat)->isNodeAtPoint(TO_NODE(artifcat), &TO_CONTEXT(context)->point_context.point))
        {
            TO_NODE(artifcat)->artifact.selected = TRUE;
        }
        else
        {
            TO_NODE(artifcat)->artifact.selected = FALSE;
        }
        break;
    case DRAW_ARC:
        TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                       &TO_ARC(artifcat)->painter);
        break;
    case DRAW_NODE:
        TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                       &TO_NODE(artifcat)->painter);
        break;
    case UNSELECT_ALL:
        TO_NODE(artifcat)->artifact.selected = FALSE;
        break;
    case GET_NEXT_NODE_ID:
        TO_CONTEXT(context)->id_context.id = TO_NODE(artifcat)->id >= TO_CONTEXT(context)->id_context.id
                                                 ? TO_NODE(artifcat)->id + 1
                                                 : TO_CONTEXT(context)->id_context.id;
        break;
    }
}

/**
 * @brief update the users tool selection
 *
 */
void net_tool_event_processor(NET *net, EVENT *event)
{

    net->tool = event->events.button_event.tool;
}

/**
 * @brief find a node given a point
 *
 */
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
 * @brief apply a context to all the transitions and places
 *
 */
void net_apply_context_all_nodes(NET *net, CONTEXT *context)
{

    g_ptr_array_foreach(net->places,
                        net_artifact_iterator, context);
    g_ptr_array_foreach(net->transitions,
                        net_artifact_iterator, context);
}

/**
 * @brief apply an action all the transitions and places
 *
 */
void net_apply_action_all_nodes(NET *net, enum ACTION action)
{
    CONTEXT context;

    context.action = action;

    net_apply_context_all_nodes(net, &context);
}

/**
 * @brief draw/paint the entire net to the drawing-canvas
 *
 */
void net_draw_event_processor(NET *net, EVENT *event)
{
    {
        CONTEXT context;

        context.action = DRAW_ARC;
        context.draw_context.drawer = create_drawer(event->events.draw_event.canvas);

        g_ptr_array_foreach(net->arcs,
                            net_artifact_iterator, &context);

        context.draw_context.drawer->release(context.draw_context.drawer);
    }

    {
        CONTEXT context;

        context.action = DRAW_NODE;
        context.draw_context.drawer = create_drawer(event->events.draw_event.canvas);

        g_ptr_array_foreach(net->places,
                            net_artifact_iterator, &context);
        g_ptr_array_foreach(net->transitions,
                            net_artifact_iterator, &context);

        context.draw_context.drawer->release(context.draw_context.drawer);
    }
}

/**
 * @brief select or create a node (based on the selected tool)
 *
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
    else
    {
        NODE *node = net_find_node_by_point(net, &point);

        if (node == NULL)
        {
            CONTEXT context;

            context.action = GET_NEXT_NODE_ID;
            context.id_context.id = 0;

            node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE);

            g_ptr_array_foreach((net->tool == PLACE_TOOL) ? net->places : net->transitions,
                                net_artifact_iterator, &context);
            node->id = context.id_context.id;
            node->setDefaultName(node);

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
            node->artifact.selected = TRUE;
        }

        net->controller->redraw(net->controller);
    }
}

/**
 * @brief start a drag (connecting nodes)
 *
 */
void net_start_drag_processor(NET *net, EVENT *event)
{
    POINT point;

    set_point(&point, event->events.start_drag_event.x, event->events.start_drag_event.y);

    NODE *node = net_find_node_by_point(net, &point);

    if (node != NULL)
    {
        create_connector(net->controller, net, node);
    }
}

/**
 * @brief connect this node to a node if one is present at this point
 *
 */
void net_connect_processor(NET *net, EVENT *event)
{
    NODE *target = net_find_node_by_point(net, event->events.connect_event.target);

    if (target != NULL && event->events.connect_event.source->type != target->type)
    {

        g_ptr_array_add(net->arcs, create_arc(event->events.connect_event.source, target));
    }
}

/**
 * @brief set the current tool - for the created net
 *
 */
void net_create_processor(NET *net, EVENT *event)
{

    net->tool = event->events.create_net.tool;
}

/**
 * @brief net event processor
 *
 */
void net_event_handler(EVENT *event, void *processor)
{

    if (TO_NET(processor)->processors[event->notification] != NULL)
    {
        TO_NET(processor)->processors[event->notification](TO_NET(processor), TO_EVENT(event));
    }
}

/**
 * @brief release/free the net object
 *
 */
void net_release(NET *net)
{
    g_free(net);
}

/**
 * @brief net constructor
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
    net->processors[UPDATE_DRAG] = NULL;
    net->processors[CONNECT_NODES] = net_connect_processor;
    net->processors[END_DRAG] = NULL;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;
}
