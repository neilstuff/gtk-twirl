/**
 * @file vertex.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  represent a point in the arc
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
#include "vertex.h"

/**
 * @brief release/free a vertex object
 *
 */
void destroy_vertex(VERTEX *vertex)
{
    g_free(vertex);
}

/**
 * @brief vertex constructor
 *
 */
VERTEX *create_vertex(enum POSITION position, POINT * point)
{
    VERTEX *vertex = g_malloc(sizeof(VERTEX));

    vertex->position = position;
    
    copy_point(point, &vertex->point);   

    vertex->destroy = destroy_vertex;

    return vertex;
}