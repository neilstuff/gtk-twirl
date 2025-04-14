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
            NET *net;

            GPtrArray *nodes;
            GPtrArray *sources;
            GPtrArray *targets;
        } node_arc_selector;
    };

} CONTEXT, *CONTEXT_P;

GPtrArray *nodes = NULL;

void (*actions[EOF_ACTIONS])(gpointer artifact, gpointer context);

/**
 * @brief select the node at a given point
 *
 */
void net_select_node_by_point(gpointer artifact, gpointer context)
{
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
}

/**
 * @brief select the arc at a given point
 *
 */
void net_select_arc_by_point(gpointer artifact, gpointer context)
{

    if (TO_ARC(artifact)->isArcAtPoint(TO_ARC(artifact), &TO_CONTEXT(context)->point_context.point))
    {
        g_ptr_array_add(TO_CONTEXT(context)->point_context.arcs, artifact);
        TO_ARC(artifact)->artifact.selected = TRUE;
    }
    else
    {
        TO_ARC(artifact)->artifact.selected = FALSE;
    }
}

/**
 * @brief draw the node
 *
 */
void net_net_draw_node(gpointer artifact, gpointer context)
{

    TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                   &TO_NODE(artifact)->painter);
}

/**
 * @brief draw the arc
 *
 */
void net_draw_arc(gpointer artifact, gpointer context)
{

    TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                   &TO_ARC(artifact)->painter);
}

/**
 * @brief draw the node
 *
 */
void net_draw_node(gpointer artifact, gpointer context)
{

    TO_CONTEXT(context)->draw_context.drawer->draw(TO_DRAWER(TO_CONTEXT(context)->draw_context.drawer),
                                                   &TO_NODE(artifact)->painter);
}

/**
 * @brief unselect all nodes
 *
 */
void net_unselect_all_nodes(gpointer artifact, gpointer context)
{

    TO_NODE(artifact)->artifact.selected = FALSE;
}

/**
 * @brief unselect all arcs
 *
 */
void net_unselect_all_arcs(gpointer artifact, gpointer context)
{

    TO_ARC(artifact)->artifact.selected = FALSE;
}

/**
 * @brief get the next node identifier
 *
 */
void net_get_next_node_id(gpointer artifact, gpointer context)
{

    TO_CONTEXT(context)->id_context.id = TO_NODE(artifact)->id >= TO_CONTEXT(context)->id_context.id
                                             ? TO_NODE(artifact)->id + 1
                                             : TO_CONTEXT(context)->id_context.id;
}

/**
 * @brief recalculate the view size considering nodes
 *
 */
void net_get_node_view_size(gpointer artifact, gpointer context)
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

/**
 * @brief recalculate the view size considering arcs
 *
 */
void net_get_arc_view_size(gpointer artifact, gpointer context)
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

/**
 * @brief get the arcs for a node
 *
 */
void net_get_arcs_for_node(gpointer artifact, gpointer context)
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

/**
 * @brief select the node by bounding rectangle
 *
 */
void net_select_node_by_bounds(gpointer artifact, gpointer context)
{
    if (point_in_bounds(&TO_NODE(artifact)->position, &TO_CONTEXT(context)->node_selector.bounds))
    {

        g_ptr_array_add(TO_CONTEXT(context)->node_selector.nodes, TO_NODE(artifact));
    }
    {
        TO_NODE(artifact)->artifact.selected = FALSE;
    }
}

/**
 * @brief return all selected nodes in an array
 *
 */
void net_get_selected_nodes(gpointer artifact, gpointer context)
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
                                actions[GET_ARCS_FOR_NODE], &selector);
        }
    }
}

/**
 * @brief find a node (place or transition)
 *
 */
gboolean net_node_find_by_id(gconstpointer node, gconstpointer id)
{

    return TO_NODE(node)->id == *(int *)id;
}

/**
 * @brief find a node (place or transition)
 *
 */
gboolean net_node_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
}

/**
 * @brief find the arc by a given point
 *
 */
gboolean net_arc_find_by_point(gconstpointer node, gconstpointer point)
{

    return TO_NODE(node)->isNodeAtPoint(TO_NODE(node), TO_POINT(point));
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
                        actions[context->action], context);
    g_ptr_array_foreach(net->transitions,
                        actions[context->action], context);
}

/**
 * @brief apply a context to all the arcs
 *
 */
void net_apply_context_all_arcs(NET *net, CONTEXT *context)
{
    g_ptr_array_foreach(net->arcs,
                        actions[context->action], context);
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
                            actions[context.action], &context);

        context.draw_context.drawer->release(context.draw_context.drawer);
    }

    {
        CONTEXT context;

        context.action = DRAW_NODE;
        context.draw_context.drawer = create_drawer(event->events.draw_event.canvas);

        g_ptr_array_foreach(net->places,
                            actions[context.action], &context);
        g_ptr_array_foreach(net->transitions,
                            actions[context.action], &context);

        context.draw_context.drawer->release(context.draw_context.drawer);

    }
}

/**
 * @brief activate the net
 * 
 */
void net_reset(NET *net)
{

    for (int iNode = 0; net->places->len != 0;)
    {
        NODE *node = g_ptr_array_index(net->places, iNode);

        g_ptr_array_remove(net->places, node);

        node->release(node);

    }

    for (int iNode = 0; net->transitions->len != 0;)
    {
        NODE *node = g_ptr_array_index(net->transitions, iNode);

        g_ptr_array_remove(net->transitions, node);

        node->release(node);
    }

    for (int iArc = 0; net->arcs->len != 0;)
    {
        ARC *arc = g_ptr_array_index(net->arcs, iArc);

        g_ptr_array_remove(net->arcs, g_ptr_array_index(net->arcs, iArc));

        arc->release(arc);
    }

}

/**
 * @brief activate the net
 * 
 */
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
                        actions[context.action], &context);

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
                            actions[context.action], &context);

        g_ptr_array_foreach(net->transitions,
                            actions[context.action], &context);
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
                                actions[context.action], &context);
            node->id = context.id_context.id;
            node->setDefaultName(node);

            POINT point;

            set_point(&point, event->events.create_node.x, event->events.create_node.y);

            adjust_point(&point, 8);

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

        g_ptr_array_remove(net->places, node);
        g_ptr_array_remove(net->transitions, node);
    }

    for (int iArc = 0; iArc < context.node_arc_selector.sources->len; iArc++)
    {

        g_ptr_array_remove(net->arcs, g_ptr_array_index(context.node_arc_selector.sources, iArc));
    }

    for (int iArc = 0; iArc < context.node_arc_selector.targets->len; iArc++)
    {

        g_ptr_array_remove(net->arcs, g_ptr_array_index(context.node_arc_selector.targets, iArc));
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

    net_reset(net);

    event->events.read_net.reader->read(event->events.read_net.reader, net);

    net_apply_action_all_nodes(net, UNSELECT_ALL_NODES);
    net_apply_action_all_arcs(net, UNSELECT_ALL_ARCS);

    net->resize(net);
    net->redraw(net);

    EVENT *activate = create_event(ACTIVATE_TOOLBAR, TRUE);

    net->controller->send(net->controller, activate);

    activate->release(activate);
    
}

/**
 * @brief clear the net
 *
 */
void net_clear(NET *net, EVENT *event)
{

    net_reset(net);

    net->resize(net);
    net->redraw(net);

}

/**
 * @brief save a net a a file
 *
 */
void net_write_net(NET *net, EVENT *event)
{

    printf("net_write_net: %s\n", event->events.write_net.filename);

    event->events.write_net.writer->write(event->events.write_net.writer, net);
}

/**
 * @brief resize the net
 *
 */
NODE *net_find_node(NET *net, char *buffer)
{
    int type;
    int id;

    sscanf(buffer, "%d-%d", &type, &id);

    NODE *node = NULL;
    guint index;

    if (g_ptr_array_find_with_equal_func((type == PLACE_NODE) ? net->places : net->transitions,
                                         &id,
                                         net_node_find_by_id,
                                         &index))
    {

        return g_ptr_array_index((type == PLACE_NODE) ? net->places : net->transitions, index);
    }

    return NULL;
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
 * @brief add a node to the net
 *
 */
void net_add_node(NET *net, NODE *node)
{

    g_ptr_array_add(node->type == PLACE_NODE ? net->places : net->transitions, node);
}

/**
 * @brief add an arc to the net
 *
 */
void net_add_arc(NET *net, ARC *arc)
{

    g_ptr_array_add(net->arcs, arc);
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

    net->addNode = net_add_node;
    net->addArc = net_add_arc;

    net->findNode = net_find_node;

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
    net->processors[CLEAR_NET] = net_clear;

    net->release = net_release;

    net->places = g_ptr_array_new();
    net->transitions = g_ptr_array_new();
    net->arcs = g_ptr_array_new();

    actions[SELECT_NODE_BY_POINT] = net_select_node_by_point;
    actions[SELECT_ARC_BY_POINT] = net_select_arc_by_point;
    actions[DRAW_ARC] = net_draw_arc;
    actions[DRAW_NODE] = net_draw_node;
    actions[UNSELECT_ALL_NODES] = net_unselect_all_nodes;
    actions[UNSELECT_ALL_ARCS] = net_unselect_all_arcs;
    actions[GET_NEXT_NODE_ID] = net_get_next_node_id;
    actions[GET_NODE_VIEW_SIZE] = net_get_node_view_size;
    actions[GET_ARC_VIEW_SIZE] = net_get_arc_view_size;
    actions[GET_ARCS_FOR_NODE] = net_get_arcs_for_node;
    actions[SELECT_NODE_BY_BOUNDS] = net_select_node_by_bounds;
    actions[GET_SELECTED_NODES] = net_get_selected_nodes;
    

    return net;
}