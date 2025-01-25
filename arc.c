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

#include "arc.h"
#include "node.h"
#include "controller.h"
#include "net.h"
#include "event.h"


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

    arc->source = source;
    arc->target = target;
    arc->selected = 0;

    arc->points = g_ptr_array_new();

    arc->destroy = destroy_arc;

    return arc;

}