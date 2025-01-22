/**
 * @file connector.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief An connector is a graphical way to connect a place with a transition or a transition to a place
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

#include "connector.h"

void connect_connector(CONNECTOR *connector, NODE *target)
{

    connector->target = NULL;

    if (target != NULL && connector->source != target && connector->source->type != target->type)
    {

        connector->target = target;
    }
}

void connector_event_handler(EVENT *event, void *processor)
{
}

void release_connector(void *connector)
{

    g_free(connector);
}

CONNECTOR *create_connector(NODE *source)
{
    CONNECTOR *connector = g_malloc(sizeof(CONNECTOR));

    connector->handler.handler = connector_event_handler;
    connector->handler.processor = connector;

    connector->release = release_connector;
    connector->connect = connect_connector;

    connector->source = source;
    connector->target = NULL;

    connector->position.x = connector->source->position.x;
    connector->position.y = connector->source->position.y;

    return connector;
}
