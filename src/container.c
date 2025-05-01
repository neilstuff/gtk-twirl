#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"
#include "container.h"

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

/**
 * Cleanuo the container
 * 
 */
void container_clean(CONTAINER *container)
{
    WRITER * writer = create_writer();

    for (int iArc = 0; iArc < container->sources->len; iArc++)
    {
        ARC *arc = g_ptr_array_index(container->sources, iArc);

        int iSource;
        int iTarget;

        int bSource = g_ptr_array_find (
            container->places,
            arc->source,
            &iSource
        );

        int bTarget = g_ptr_array_find (
            container->transitions,
            arc->target,
            &iTarget
        );

        if (bSource && bTarget)
        {

            g_ptr_array_add(container->arcs, arc);

        }
    }
    
    for (int iArc = 0; iArc < container->targets->len; iArc++)
    {
        ARC *arc = g_ptr_array_index(container->targets, iArc);
        int iSource;
        int iTarget;

        int bSource = g_ptr_array_find (
            container->places,
            arc->source,
            &iSource
        );

        int bTarget = g_ptr_array_find (
            container->transitions,
            arc->target,
            &iTarget
        );

        if (bSource && bTarget)
        {

            g_ptr_array_add(container->arcs, arc);

        }
    }

}

/**
 * Free the writer resources
 *
 */
void container_release(CONTAINER *container)
{

    g_free(container);
}

/**
 * Create a Writer
 *
 */
CONTAINER *create_container()
{
    CONTAINER *container = g_malloc(sizeof(CONTAINER));

    container->release = container_release;
    container->clean = container_clean;

    container->places = g_ptr_array_new();
    container->transitions = g_ptr_array_new();

    container->sources = g_ptr_array_new();
    container->targets = g_ptr_array_new();

    container->arcs = g_ptr_array_new();

    return container;
}