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

#include "geometry.h"
#include "artifact.h"
#include "editor.h"
#include "drawer.h"

#include "node.h"
#include "event.h"
#include "handler.h"

#include "artifact.h"
#include "controller.h"
#include "net.h"
#include "vertex.h"

/**
 * @brief copy the point
 * 
 */
void vertex_set_point(VERTEX *vertex, POINT *point)
{
    copy_point(point, &vertex->point);   
}

/**
 * @brief release/free a vertex object
 *
 */
void vertex_release(VERTEX *vertex)
{
    g_free(vertex);
}

/**
 * @brief vertex constructor
 *
 */
VERTEX *create_vertex(enum POSITION position, NET * net, POINT * point)
{
    VERTEX *vertex = g_malloc(sizeof(VERTEX));

    vertex->net = net;
    vertex->position = position;
    
    vertex->artifact.state = INACTIVE;
    vertex->artifact.selected = FALSE;
    
    copy_point(point, &vertex->point);   

    vertex->release = vertex_release;    
    vertex->setPoint = vertex_set_point;    


    return vertex;
}