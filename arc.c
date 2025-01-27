/**
 * @file arc.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - an arc joins places to transitions or transitions to places
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

#include "drawer.h"
#include "arc.h"
#include "node.h"

/**
 * @brief release/free an arc object
 *
 */
void destroy_arc (ARC * arc)
{
    g_free (arc);

}

/**
 * @brief arc constructor
 *
 */
ARC * create_arc (NODE * source, NODE * target)
{
    ARC * arc = g_malloc(sizeof(ARC));

    printf("Creating ARC: %s:%s", source->name, target->name);

    arc->source = source;
    arc->target = target;
    arc->selected = 0;

    arc->painter.type = ARC_PAINTER;
    arc->painter.painters.arc_painter.arc = arc;

    arc->points = g_ptr_array_new();

    g_ptr_array_add(arc->points, clone_point(&source->position));
    g_ptr_array_add(arc->points, clone_point(&target->position));

    arc->destroy = destroy_arc;

    return arc;

}