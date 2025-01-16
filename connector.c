#include <math.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "connector.h"

/**
 * Connector Manager - implementation
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

/**
 * Destroy the Connector
 *
 * @param connector the Connector
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
 * @brief Release the Connector
 *
 * @param connector the Connector to destroy
 *
 */
void release_connector(void *connector)
{

    g_free(connector);
}

/**
 * Create a Connector
 *
 * @param source the Source Node - Place/Target
 * @return a newly created Connector
 *
 */
CONNECTOR *create_connector(NODE *source)
{
    CONNECTOR *connector = g_malloc(sizeof(CONNECTOR));

    connector->release = release_connector;
    connector->connect = connect_connector;

    connector->source = source;
    connector->target = NULL;

    connector->position.x = connector->source->position.x;
    connector->position.y = connector->source->position.y;

    return connector;
}
