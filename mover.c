/**
 * @file mover.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  mover is a graphical way to connect a place with a transition or a transition to a place
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <math.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "geometry.h"
#include "artifact.h"
#include "editor.h"
#include "drawer.h"

#include "node.h"
#include "event.h"
#include "handler.h"
#include "editor.h"
#include "controller.h"

#include "net.h"
#include "vertex.h"
#include "arc.h"
#include "mover.h"

/**
 * @brief  iterator through the nodes to adjust the first line's point
 *
 */
void mover_source_arc_iterator(gpointer artifact, gpointer node)
{

    VERTEX *vertex = g_ptr_array_index(TO_ARC(artifact)->vertices, 0);

    copy_point(&TO_NODE(node)->position, &vertex->point);
}

/**
 * @brief  iterator through the nodes to adjust the last line's point
 *
 */
void mover_target_arc_iterator(gpointer artifact, gpointer node)
{
    VERTEX *vertex = g_ptr_array_index(TO_ARC(artifact)->vertices, TO_ARC(artifact)->vertices->len - 1);

    copy_point(&TO_NODE(node)->position, &vertex->point);
}

/**
 * @brief event handler for a mover
 *
 */
void mover_event_handler(EVENT *event, void *processor)
{
    switch (event->notification)
    {
    case UPDATE_DRAG:
    {
        for (int iNode = 0; iNode < TO_MOVER(processor)->nodes->len; iNode++)
        {
            NODE *node = g_ptr_array_index(TO_MOVER(processor)->nodes, iNode);

            int x = TO_MOVER(processor)->offset.x + event->events.update_drag_event.offset_x;
            int y = TO_MOVER(processor)->offset.y + event->events.update_drag_event.offset_y;

            node->setPosition(node, x, y);

            g_ptr_array_foreach(TO_MOVER(processor)->sources, mover_source_arc_iterator, node);
            g_ptr_array_foreach(TO_MOVER(processor)->targets, mover_target_arc_iterator, node);
        }

        TO_MOVER(processor)->controller->redraw(TO_MOVER(processor)->controller);
    }
    break;

    case END_DRAG:
    {

        for (int iNode = 0; iNode < TO_MOVER(processor)->nodes->len; iNode++)
        {
            NODE *node = g_ptr_array_index(TO_MOVER(processor)->nodes, iNode);
            POINT point;

            set_point(&point,
                      TO_MOVER(processor)->offset.x + event->events.update_drag_event.offset_x,
                      TO_MOVER(processor)->offset.y + event->events.update_drag_event.offset_y);

            adjust_point(&point, 16);

            node->setPosition(node, point.x, point.y);

            g_ptr_array_foreach(TO_MOVER(processor)->sources, mover_source_arc_iterator, node);
            g_ptr_array_foreach(TO_MOVER(processor)->targets, mover_target_arc_iterator, node);
        }

        TO_MOVER(processor)->net->resize(TO_MOVER(processor)->net);
        TO_MOVER(processor)->controller->redraw(TO_MOVER(processor)->controller);
        TO_MOVER(processor)->release(TO_MOVER(processor));
    }
    break;
    }
}

/**
 * @brief deallocate a mover object
 *
 */
void release_mover(MOVER *mover)
{

    mover->net->controller->unmonitor(mover->net->controller, &mover->handler);

    g_free(mover);
}

/**
 * @brief move nodes and points
 *
 */
void mover_add_node(MOVER *mover, NODE *node)
{
    g_ptr_array_add(mover->nodes, node);
}

/**
 * @brief create a mover for nodes and points
 *
 */
MOVER *create_mover(CONTROLLER *controller, POINT *point, NET *net)
{
    MOVER *mover = g_malloc(sizeof(MOVER));

    mover->handler.handler = mover_event_handler;
    mover->handler.processor = mover;

    mover->release = release_mover;
    mover->addNode = mover_add_node;

    mover->controller = controller;
    mover->net = net;

    mover->offset.x = point->x;
    mover->offset.y = point->y;

    mover->nodes = g_ptr_array_new();
    mover->sources = g_ptr_array_new();
    mover->targets = g_ptr_array_new();

    net->controller->monitor(controller, &mover->handler);

    return mover;
}