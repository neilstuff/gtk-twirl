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
    }
    break;

    case END_DRAG:
    {
    }
    break;
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
SELECTOR * create_selector(CONTROLLER *controller, POINT *point, NET *net)
{
    SELECTOR *selector = g_malloc(sizeof(SELECTOR));

    selector->handler.handler = selector_event_handler;
    selector->handler.processor = selector;

    selector->release = release_selector;

    selector->controller = controller;
    selector->net = net;

    selector->offset.x = point->x;
    selector->offset.y = point->y;

    selector->nodes = g_ptr_array_new();

    net->controller->monitor(controller, &selector->handler);

    return selector;
}