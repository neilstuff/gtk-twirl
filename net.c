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

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "event.h"
#include "handler.h"

#include "node.h"
#include "vertex.h"
#include "arc.h"

#include "editor.h"
#include "controller.h"

#include "net.h"

#include "connector.h"
#include "mover.h"
#include "selector.h"

#define TO_CONTEXT(context) ((CONTEXT *)(context))

enum ACTION
{
    DRAW_NODE = 0,
    UNSELECT_ALL_NODES,
    SELECT_NODE_BY_POINT,
    NODE_AT_POINT,
    GET_NEXT_NODE_ID,
    DRAW_ARC,
    POINT_IN_ARC,
    UNSELECT_ALL_ARCS,
    SELECT_ARC_BY_POINT,
    GET_NODE_VIEW_SIZE,
    GET_ARC_VIEW_SIZE,
    GET_ARCS_FOR_NODE,
    SELECT_NODE_BY_BOUNDS,
    GET_SELECTED_NODES,
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
        struct
        {
            NODE *node;
            GPtrArray *sources;
            GPtrArray *targets;
        } node_arcs;
        struct
        {
            GPtrArray *nodes;
            BOUNDS bounds;
        } node_selector;
        struct
        {
            NET * net;

            GPtrArray *nodes;
            GPtrArray *sources;
            GPtrArray *targets;
        } node_arc_selector;
    };

} CONTEXT, *CONTEXT_P;

GPtrArray *nodes = NULL;

/**
 * @brief find a node (place or transition)
 *
 */
gboolean net_node_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}

/**
 * @brieffind the arc by a given point
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
    case GET_NODE_VIEW_SIZE:
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
    case GET_ARC_VIEW_SIZE:
    {
        POINT point;

        TO_ARC(artifact)->getPathBounds(TO_ARC(artifact), &point);

        if (TO_CONTEXT(context)->view_size.size.w < point.x)
        {
            TO_CONTEXT(context)->view_size.size.w = point.x;
        }
        if (TO_CONTEXT(context)->view_size.size.h < point.y)
        {
            TO_CONTEXT(context)->view_size.size.h = point.y;
        }
    }
    break;
    case GET_ARCS_FOR_NODE:
    {
        if (TO_ARC(artifact)->source == TO_CONTEXT(context)->node_arcs.node)
        {
            g_ptr_array_add(TO_CONTEXT(context)->node_arcs.sources, TO_ARC(artifact));
        }

        if (TO_ARC(artifact)->target == TO_CONTEXT(context)->node_arcs.node)
        {
            g_ptr_array_add(TO_CONTEXT(context)->node_arcs.targets, TO_ARC(artifact));
        }
    }
    break;
    case SELECT_NODE_BY_BOUNDS:
    {
        if (point_in_bounds(&TO_NODE(artifact)->position, &TO_CONTEXT(context)->node_selector.bounds))
        {

            g_ptr_array_add(TO_CONTEXT(context)->node_selector.nodes, TO_NODE(artifact));
        }
        {
            TO_NODE(artifact)->artifact.selected = FALSE;
        }
    }
    break;
    case GET_SELECTED_NODES:
    {
        if (TO_NODE(artifact)->artifact.selected == TRUE)
        {
            g_ptr_array_add(TO_CONTEXT(context)->node_arc_selector.nodes, TO_NODE(artifact));

            {
                CONTEXT selector;
 
                selector.action = GET_ARCS_FOR_NODE;
                selector.node_arcs.node = TO_NODE(artifact);
                selector.node_arcs.sources = TO_CONTEXT(context)->node_arc_selector.sources;
                selector.node_arcs.targets = TO_CONTEXT(context)->node_arc_selector.targets;

                g_ptr_array_foreach(TO_CONTEXT(context)->node_arc_selector.net->arcs,
                            net_artifact_iterator, &selector);

            }
    
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
 * @brief find a vertex given a point
 *
 */
VERTEX *net_find_vertex_by_point(NET *net, POINT *point)
{
    for (int iArc = 0; iArc < net->arcs->len; iArc++)
    {
        ARC *arc = g_ptr_array_index(net->arcs, iArc);

        VERTEX *vertex = arc->getVertex(arc, point);

        if (vertex != NULL)
        {
            return vertex;
        }
    }

    return NULL;
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

void net_activate(NET *net, enum NOTIFICATION notification, int status)
{
    EVENT *activate = create_event(notification, status);

    net->controller->send(net->controller, activate);

    activate->release(activate);
}

/**
 * @brief resize the net
 *
 */
void net_resize(NET *net)
{
    CONTEXT context;

    context.action = GET_NODE_VIEW_SIZE;
    context.view_size.size.w = 0;
    context.view_size.size.h = 0;

    net_apply_context_all_nodes(net, &context);

    context.action = GET_ARC_VIEW_SIZE;

    g_ptr_array_foreach(net->arcs,
                        net_artifact_iterator, &context);

    EVENT *resize = create_event(SET_VIEW_SIZE, &context.view_size.size);

    net->controller->send(net->controller, resize);

    resize->release(resize);
}

/**
 * @brief select the nodes within the bounds and unselect the others
 *
 */
void net_select(NET *net, BOUNDS *bounds, GPtrArray *nodes)
{

    net_apply_action_all_arcs(net, UNSELECT_ALL_ARCS);

    {
        CONTEXT context;

        context.action = SELECT_NODE_BY_BOUNDS;
        context.node_selector.nodes = nodes;

        set_bounds(bounds, &context.node_selector.bounds);

        g_ptr_array_foreach(net->places,
                            net_artifact_iterator, &context);

        g_ptr_array_foreach(net->transitions,
                            net_artifact_iterator, &context);
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
            net->controller->message(net->controller, CLEAR_EDITOR);

            EDITOR *editor = net->controller->edit(net->controller);

            for (int iNode = 0; iNode < context.point_context.nodes->len; iNode++)
            {
                NODE *node = g_ptr_array_index(context.point_context.nodes, iNode);
                node->edit(node, editor);
            }
        }
        else if (!context.point_context.found)
        {
            net->controller->message(net->controller, CLEAR_EDITOR);

            context.action = SELECT_ARC_BY_POINT;
            context.point_context.found = 0;
            context.point_context.arcs = g_ptr_array_new();
            net_apply_context_all_arcs(net, &context);
            {
                EDITOR *editor = net->controller->edit(net->controller);
                int iArc = 0;

                for (; iArc < context.point_context.arcs->len; iArc++)
                {
                    ARC *arc = g_ptr_array_index(context.point_context.arcs, iArc);

                    arc->edit(arc, editor);
                }
            }

            if (net->controller->mode == CONNECT && context.point_context.arcs->len > 0)
            {
                int iArc = 0;

                for (; iArc < context.point_context.arcs->len; iArc++)
                {
                    ARC *arc = g_ptr_array_index(context.point_context.arcs, iArc);

                    arc->setVertex(arc, &point);
                }
            }
        }

        net_activate(net, ACTIVATE_DELETE, TRUE);

        net->redraw(net);
    }
    else
    {
        NODE *node = net_find_node_by_point(net, &point);

        if (node == NULL)
        {
            net->controller->message(net->controller, CLEAR_EDITOR);

            CONTEXT context;

            context.action = GET_NEXT_NODE_ID;
            context.id_context.id = 0;

            node = create_node(net->tool == PLACE_TOOL ? PLACE_NODE : TRANSITION_NODE, net);

            g_ptr_array_foreach((net->tool == PLACE_TOOL) ? net->places : net->transitions,
                                net_artifact_iterator, &context);
            node->id = context.id_context.id;
            node->setDefaultName(node);

            POINT point;

            set_point(&point, event->events.create_node.x, event->events.create_node.y);

            adjust_point(&point, 16);

            node->setPosition(node, point.x, point.y);

            context.action = DRAW_NODE;

            EDITOR *editor = net->controller->edit(net->controller);

            node->edit(node, editor);

            g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);

            net->resize(net);

            {
                EVENT *activate = create_event(ACTIVATE_TOOLBAR, TRUE);

                net->controller->send(net->controller, activate);

                activate->release(activate);
            }
        }
        else
        {
            net->controller->message(net->controller, CLEAR_EDITOR);

            EDITOR *editor = net->controller->edit(net->controller);

            node->edit(node, editor);

            node->artifact.selected = TRUE;
        }

        net_activate(net, ACTIVATE_DELETE, TRUE);

        net->redraw(net);
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

    if (event->events.start_drag_event.mode == CONNECT && node != NULL)
    {
        create_connector(net->controller, net, node);
    }
    else if (event->events.start_drag_event.mode == MOVE && node != NULL)
    {
        MOVER *mover = create_mover(MOVING_NODE, net->controller, &point, net);

        CONTEXT context;

        context.action = GET_ARCS_FOR_NODE;
        context.node_arcs.node = node;

        mover->addNode(mover, node);
        context.node_arcs.sources = mover->sources;
        context.node_arcs.targets = mover->targets;

        net_apply_context_all_arcs(net, &context);
    }
    else if (node == NULL)
    {
        VERTEX *vertex = net_find_vertex_by_point(net, &point);

        if (vertex == NULL)
        {
            create_selector(net->controller, &point, net);
        }
        else
        {

            MOVER *mover = create_mover(MOVING_VERTEX, net->controller, &point, net);

            mover->addVertex(mover, vertex);

            vertex->artifact.selected = TRUE;
        }
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
        net_apply_action_all_nodes(net, UNSELECT_ALL_NODES);
        net_apply_action_all_arcs(net, UNSELECT_ALL_ARCS);

        ARC *arc = create_arc(net, event->events.connect_event.source, target);

        arc->artifact.selected = TRUE;

        net->controller->message(net->controller, CLEAR_EDITOR);

        EDITOR *editor = net->controller->edit(net->controller);

        arc->edit(arc, editor);

        g_ptr_array_add(net->arcs, arc);

        net->controller->mode = FINALISE;

        net_activate(net, ACTIVATE_DELETE, TRUE);
        net->redraw(net);
    }
}

/**
 * @brief delete all the selected nodes
 *
 */
void net_delete_selected(NET *net, EVENT *event)
{
    CONTEXT context;

    context.action = GET_SELECTED_NODES;
    context.node_arc_selector.net = net;
    context.node_arc_selector.nodes = g_ptr_array_new();
    context.node_arc_selector.sources = g_ptr_array_new();
    context.node_arc_selector.targets = g_ptr_array_new();

    net_apply_context_all_nodes(net, &context);

    for (int iNode = 0; iNode < context.node_arc_selector.nodes->len; iNode++)
    {
        NODE *node = g_ptr_array_index(context.node_arc_selector.nodes, iNode);
        
        g_ptr_array_remove (net->places, node);
        g_ptr_array_remove (net->transitions, node);
    }

    for (int iArc = 0; iArc < context.node_arc_selector.sources->len; iArc++)
    {
      
        g_ptr_array_remove (net->arcs, g_ptr_array_index(context.node_arc_selector.sources, iArc));
    }

    for (int iArc = 0; iArc < context.node_arc_selector.targets->len; iArc++)
    {
        
        g_ptr_array_remove (net->arcs, g_ptr_array_index(context.node_arc_selector.targets, iArc));
    }

    net->controller->message(net->controller, CLEAR_EDITOR);

    net_activate(net, ACTIVATE_DELETE, FALSE);
    net->resize(net);
    net->redraw(net);

}

/**
 * @brief create a net from a file
 *
 */
void net_read_net(NET *net, EVENT *event)
{

    printf("net_read_net: %s\n", event->events.read_net.filename);

}

/**
 * @brief save a net a formula
 *
 */
void net_write_net(NET *net, EVENT *event)
{

    printf("net_write_net: %s\n", event->events.write_net.filename);

    event->events.write_net.writer->write(event->events.write_net.writer, net);

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
    net->resize = net_resize;
    net->select = net_select;

    net->processors[DRAW_REQUESTED] = net_draw_event_processor;
    net->processors[TOOL_SELECTED] = net_tool_event_processor;
    net->processors[CREATE_NODE] = net_select_node_processor;
    net->processors[CREATE_NET] = net_create_processor;
    net->processors[START_DRAG] = net_start_drag_processor;
    net->processors[UPDATE_DRAG] = NULL;
    net->processors[CONNECT_NODES] = net_connect_processor;
    net->processors[DELETE_SELECTED] = net_delete_selected;
    net->processors[READ_NET] = net_read_net;
    net->processors[WRITE_NET] = net_write_net;
    net->processors[END_DRAG] = NULL;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    return net;
}