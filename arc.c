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
#include "editor.h"
#include "drawer.h"
#include "vertex.h"
#include "arc.h"
#include "node.h"

/**
 * @brief create a vertex
 *
 */
void arc_set_vertex(ARC *arc, POINT *point)
{
    int iVertex = 0;
    int located = FALSE;
    POINT *source = NULL;
    POINT *target = NULL;

    for (iVertex = 0; iVertex < arc->vertices->len && !located; iVertex++)
    {

        if (source != NULL)
        {           
            target = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;

            if (point_on_line(source, target, point, 4))
            {
                located = TRUE;
                break;
            }
        }

        source = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;

    }

    if (located) {
        g_ptr_array_insert (arc->vertices, iVertex, create_vertex(CONTROL_POSITION, point));
    }
}

/**
 * @brief does this arc contain the point
 *
 */
int is_arc_at_point(ARC *arc, POINT *point)
{
    int iVertex = 0;
    POINT *source = NULL;
    POINT *target = NULL;
 
    for (iVertex = 0; iVertex < arc->vertices->len; iVertex++)
    {

        if ((source != NULL))
        {
            target = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;
            if (point_on_line(source, target, point, 4)) 
            {

                return TRUE;
            }
        }

        source = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;
    }
 
    return FALSE;
}

/**
 * @brief release/free an arc object
 *
 */
void release_arc(ARC *arc)
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

    g_ptr_array_add(arc->vertices, create_vertex(SOURCE_POSITION, &source->position));
    g_ptr_array_add(arc->vertices, create_vertex(TARGET_POSITION, &target->position));

    arc->release = release_arc;
    arc->isArcAtPoint = is_arc_at_point;
    arc->setVertex = arc_set_vertex;


    return arc;
}