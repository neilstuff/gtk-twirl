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

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "geometry.h"

#include "artifact.h"
#include "container.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "vertex.h"
#include "node.h"
#include "arc.h"

#include "event.h"
#include "handler.h"
#include "editor.h"
#include "controller.h"

#include "net.h"
#include "vertex.h"
#include "arc.h"
#include "selector.h"

/**
 * @brief event handler for a mover
 *
 */
void selector_event_handler(EVENT *event, void *processor)
{
    switch (event->notification)
    {
    case UPDATE_DRAG:
    {
        TO_SELECTOR(processor)->offset.x = TO_SELECTOR(processor)->position.x + event->events.update_drag_event.offset_x;
        TO_SELECTOR(processor)->offset.y = TO_SELECTOR(processor)->position.y + event->events.update_drag_event.offset_y;
        TO_SELECTOR(processor)->controller->redraw(TO_SELECTOR(processor)->controller);
    }
    break;

    case END_DRAG:
    {

        BOUNDS bounds;

        bounds.point.x = TO_SELECTOR(processor)->position.x;
        bounds.point.y = TO_SELECTOR(processor)->position.y;

        bounds.size.w = TO_SELECTOR(processor)->offset.x - TO_SELECTOR(processor)->position.x;
        bounds.size.h = TO_SELECTOR(processor)->offset.y - TO_SELECTOR(processor)->position.y;

        GPtrArray *nodes = g_ptr_array_new();

        TO_SELECTOR(processor)->net->select(TO_SELECTOR(processor)->net, &bounds, nodes);
        TO_SELECTOR(processor)->controller->message(TO_SELECTOR(processor)->controller, CLEAR_EDITOR);

        EDITOR *editor = TO_SELECTOR(processor)->controller->edit(TO_SELECTOR(processor)->controller);

        for (int iNode = 0; iNode < nodes->len; iNode++)
        {
            NODE *node = g_ptr_array_index(nodes, iNode);

            node->edit(node, editor);
            node->artifact.selected = TRUE;
            
            TO_SELECTOR(processor)->controller->mode = FINALISE;
        }

        EVENT *activate = create_event(ACTIVATE_DELETE, TRUE);

        TO_SELECTOR(processor)->controller->send(TO_SELECTOR(processor)->controller, activate);
    
        activate->release(activate);
        
        TO_SELECTOR(processor)->controller->redraw(TO_SELECTOR(processor)->controller);

        TO_SELECTOR(processor)->release(TO_SELECTOR(processor));
    
    }


    break;

    case DRAW_REQUESTED:
    {
        DRAWER *drawer = create_drawer(event->events.draw_event.canvas);

        drawer->draw(drawer, &TO_SELECTOR(processor)->painter);
    }
    }
}

/**
 * @brief deallocate a selector object
 *
 */
void release_selector(SELECTOR *selector)
{

    selector->net->controller->unmonitor(selector->net->controller, &selector->handler);

    g_free(selector);
}

/**
 * @brief create a mover for nodes and points
 *
 */
SELECTOR *create_selector(CONTROLLER *controller, POINT *point, NET *net)
{
    SELECTOR *selector = g_malloc(sizeof(SELECTOR));

    selector->handler.handler = selector_event_handler;
    selector->handler.processor = selector;

    selector->release = release_selector;

    selector->controller = controller;
    selector->net = net;

    selector->position.x = point->x;
    selector->position.y = point->y;

    selector->offset.x = point->x;
    selector->offset.y = point->y;

    selector->nodes = g_ptr_array_new();

    selector->painter.type = SELECTOR_PAINTER;
    selector->painter.painters.selector_painter.selector = selector;
    selector->painter.painters.selector_painter.net = net;

    net->controller->monitor(controller, &selector->handler);

    return selector;
}