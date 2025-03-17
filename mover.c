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
#include "mover.h"

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

            adjust_point(&point, 15);

            node->setPosition(node, point.x, point.y);

        }

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
MOVER * create_mover(CONTROLLER *controller, POINT *point, NET *net)
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

    net->controller->monitor(controller, &mover->handler);

    return mover;
}