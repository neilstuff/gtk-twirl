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
#include "editor.h"
#include "drawer.h"

#include "node.h"
#include "arc.h"

#include "event.h"
#include "editor.h"
#include "controller.h"

#include "net.h"

#include "connector.h"

#define TO_CONTEXT(context) ((CONTEXT *)(context))

enum ACTION
{
    DRAW_NODE = 0,
    UNSELECT_ALL_NODES = 1,
    SELECT_NODE_BY_POINT = 2,
    NODE_AT_POINT = 3,
    GET_NEXT_NODE_ID = 4,
    DRAW_ARC = 5,
    POINT_IN_ARC = 6,
    UNSELECT_ALL_ARCS = 7,
    SELECT_ARC_BY_POINT,
    GET_VIEW_SIZE,
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
            int found;
            GPtrArray *nodes;
            GPtrArray *arcs;

        } point_context;
        struct
        {

            int id;

        } id_context;
        struct
        {

            SIZE size;

        } view_size;
    };

} CONTEXT, *CONTEXT_P;

/**
 * @brief find a node (place or transition)
 *
 */
gboolean net_node_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}

/**
 * @brief
 *
 */
gboolean net_arc_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}
/**
 * @brief  iterator of nodes and arcs - the context determines the processor to apply to the artifact
 *
 */
void net_artifact_iterator(gpointer artifact, gpointer context)
{

    switch (TO_CONTEXT(context)->action)
    {
    case SELECT_NODE_BY_POINT:
        if (TO_NODE(artifact)->isNodeAtPoint(TO_NODE(artifact), &TO_CONTEXT(context)->point_context.point))
        {
            TO_NODE(artifact)->artifact.selected = TRUE;
            g_ptr_array_add(TO_CONTEXT(context)->point_context.nodes, artifact);
            TO_CONTEXT(context)->point_context.found += 1;
        }
        else
        {
            TO_NODE(artifact)->artifact.selected = FALSE;
        }
        break;
    case SELECT_ARC_BY_POINT:
        if (TO_ARC(artifact)->isArcAtPoint(TO_ARC(artifact), &TO_CONTEXT(context)->point_context.point))
        {
            g_ptr_array_add(TO_CONTEXT(context)->point_context.arcs, artifact);
            TO_ARC(artifact)->artifact.selected = TRUE;
        }
        else
        {
            TO_ARC(artifact)->artifact.selected = FALSE;
        }
        break;
    case DRAW_ARC:
        TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                       &TO_ARC(artifact)->painter);
        break;
    case DRAW_NODE:
        TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                       &TO_NODE(artifact)->painter);
        break;
    case UNSELECT_ALL_NODES:
        TO_NODE(artifact)->artifact.selected = FALSE;
        break;
    case UNSELECT_ALL_ARCS:
        TO_ARC(artifact)->artifact.selected = FALSE;
        break;
    case GET_NEXT_NODE_ID:
        TO_CONTEXT(context)->id_context.id = TO_NODE(artifact)->id >= TO_CONTEXT(context)->id_context.id
                                                 ? TO_NODE(artifact)->id + 1
                                                 : TO_CONTEXT(context)->id_context.id;
        break;
    case GET_VIEW_SIZE:
    {
        double w = TO_NODE(artifact)->bounds.point.x + TO_NODE(artifact)->bounds.size.w;
        double h = TO_NODE(artifact)->bounds.point.y + TO_NODE(artifact)->bounds.size.h;

        if (TO_CONTEXT(context)->view_size.size.w < w)
        {
            TO_CONTEXT(context)->view_size.size.w = w;
        }
        if (TO_CONTEXT(context)->view_size.size.h < h)
        {
            TO_CONTEXT(context)->view_size.size.h = h;
        }
    }
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
 * @brief apply a context to all the arcs
 *
 */
void net_apply_context_all_arcs(NET *net, CONTEXT *context)
{
    g_ptr_array_foreach(net->arcs,
                        net_artifact_iterator, context);
}

/**
 * @brief apply an action on all the nodes (transitions and places)
 *
 */
void net_apply_action_all_nodes(NET *net, enum ACTION action)
{
    CONTEXT context;

    context.action = action;

    net_apply_context_all_nodes(net, &context);
}

/**
 * @brief apply an action on all the arcs
 *
 */
void net_apply_action_all_arcs(NET *net, enum ACTION action)
{
    CONTEXT context;

    context.action = action;

    net_apply_context_all_arcs(net, &context);
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

    net_apply_action_all_nodes(net, UNSELECT_ALL_NODES);
    net_apply_action_all_arcs(net, UNSELECT_ALL_ARCS);

    EVENT * clearEvent = create_event(CLEAR_EDITOR);

    net->controller->process(net->controller, clearEvent);

    clearEvent->release(clearEvent);

    if (net->tool == SELECT_TOOL)
    {
        CONTEXT context;

        context.action = SELECT_NODE_BY_POINT;
        context.point_context.found = 0;
        context.point_context.nodes = g_ptr_array_new();
        context.point_context.arcs = g_ptr_array_new();
        context.point_context.point.x = event->events.create_node.x;
        context.point_context.point.y = event->events.create_node.y;

        net_apply_context_all_nodes(net, &context);

        if (context.point_context.found == 1)
        {
            EDITOR *editor = net->controller->edit(net->controller);
            int iNode = 0;

            for (;iNode < context.point_context.nodes->len; iNode++) 
            {
                NODE * node = g_ptr_array_index(context.point_context.nodes, iNode);
                node->edit(node, editor);

            }          
        }
        else if (!context.point_context.found)
        {
            context.action = SELECT_ARC_BY_POINT;
            context.point_context.found = 0;
            net_apply_context_all_arcs(net, &context);
        }

        net->redraw(net);
    }
    else
    {
        NODE *node = net_find_node_by_point(net, &point);

        if (node == NULL)
        {
            CONTEXT context;

            context.action = GET_NEXT_NODE_ID;
            context.id_context.id = 0;

            node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE, net);

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

            EDITOR *editor = net->controller->edit(net->controller);

            node->edit(node, editor);

            g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);

            {
                CONTEXT context;

                context.action = GET_VIEW_SIZE;
                context.view_size.size.w = 0;
                context.view_size.size.h = 0;

                net_apply_context_all_nodes(net, &context);

                EVENT *resize = create_event(SET_VIEW_SIZE, &context.view_size.size);

                net->controller->process(net->controller, resize);

                resize->release(resize);
            }
            {
                EVENT *activate = create_event(ACTIVATE_TOOLBAR, TRUE);

                net->controller->process(net->controller, activate);

                activate->release(activate);
            }
        }
        else
        {
            EDITOR *editor = net->controller->edit(net->controller);

            node->edit(node, editor);

            node->artifact.selected = TRUE;
        }

        net->redraw(net);
    }
}

/**
 * @brief start a drag (connecting nodes)
 *
 */
void net_start_drag_processor(NET *net, EVENT *event)
{

    if (event->events.start_drag_event.mode != CONNECT)
    {
        return;
    }

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
 * @brief redraw the net
 *
 */

void net_redraw(NET *net)
{

    net->controller->redraw(net->controller);
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
    net->redraw = net_redraw;

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
