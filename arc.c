/**
 * @file arc.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  an arc joins places to transitions or transitions to places
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

#include "artifact.h"
#include "drawer.h"
#include "vertex.h"
#include "arc.h"
#include "node.h"

/**
 * @brief does this arc contain the point
 *
 */
int is_arc_at_point(ARC *arc, POINT *point)
{
    int iVertex = 0;
 
    for (iVertex = 0; iVertex < arc->vertices->len; iVertex++)
    {
        POINT *source;

        if (iVertex % 2 != 0)
        {
            int intersects = point_on_line(&TO_VERTEX(arc->vertices->pdata[iVertex - 1])->point,  
                                           &TO_VERTEX(arc->vertices->pdata[iVertex])->point, 
                                           point, 4);

            printf("Intersects: %d\n", intersects);

            if (intersects)
            {

                return TRUE;
            }
        }
    }
 
    return FALSE;
}

/**
 * @brief release/free an arc object
 *
 */
void destroy_arc(ARC *arc)
{
    g_free(arc);
}

/**
 * @brief arc constructor
 *
 */
ARC *create_arc(NODE *source, NODE *target)
{
    ARC *arc = g_malloc(sizeof(ARC));

    arc->source = source;
    arc->target = target;

    arc->painter.type = ARC_PAINTER;
    arc->painter.painters.arc_painter.arc = arc;

    arc->vertices = g_ptr_array_new();

    g_ptr_array_add(arc->vertices, create_vertex(&source->position));
    g_ptr_array_add(arc->vertices, create_vertex(&target->position));

    arc->destroy = destroy_arc;
    arc->isArcAtPoint = is_arc_at_point;

    return arc;
}