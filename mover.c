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
    }
    break;

    case END_DRAG:
    {
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
 * @brief create a coonector for a specific node
 *
 */
MOVER *create_mover(CONTROLLER *controller, NET *net, NODE *source)
{
    MOVER *mover = g_malloc(sizeof(MOVER));

    mover->handler.handler = mover_event_handler;
    mover->handler.processor = mover;

    mover->release = release_mover;

    mover->controller = controller;
    mover->net = net;

    mover->offset.x = 0;
    mover->offset.y = 0;

    net->controller->monitor(controller, &mover->handler);

    return mover;
}