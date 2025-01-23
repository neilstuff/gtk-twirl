/**
 * @file connector.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  connector is a graphical way to connect a place with a transition or a transition to a place
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

#include "controller.h"
#include "net.h"
#include "connector.h"

/**
 * @brief connect to nodes together (p -> t or t -> p)
 * 
 */
void connect_connector(CONNECTOR *connector, NODE *target)
{

    connector->target = NULL;

    if (target != NULL && connector->source != target && connector->source->type != target->type)
    {

        connector->target = target;
    }
}

/**
 * @brief event handler for a connector
 * 
 */
void connector_event_handler(EVENT *event, void *processor)
{
    switch (event->notification)
    {
        case UPDATE_DRAG:
            printf("UPDATED DRAG\n");
            break;

        case END_DRAG:
            printf("END DRAG\n");
            TO_CONNECTOR(processor)->release(TO_CONNECTOR(processor));
            break;

        case DRAW_REQUESTED:
            printf("DRAW REQUESTED\n");
            break;
            
    }

}

/**
 * @brief deallocate a connector object
 * 
 */
void release_connector(CONNECTOR * connector)
{

    connector->net->controller->unmonitor(connector->net->controller, &connector->handler);

    g_free(connector);
}

/**
 * @brief create a coonector for a specific node
 * 
 */
CONNECTOR *create_connector(NET* net, NODE *source)
{
    CONNECTOR *connector = g_malloc(sizeof(CONNECTOR));

    connector->handler.handler = connector_event_handler;
    connector->handler.processor = connector;

    connector->release = release_connector;
    connector->connect = connect_connector;

    connector->net = net;

    connector->source = source;
    connector->target = NULL;

    connector->position.x = connector->source->position.x;
    connector->position.y = connector->source->position.y;

    net->controller->monitor(net->controller, &connector->handler);

    return connector;
}
