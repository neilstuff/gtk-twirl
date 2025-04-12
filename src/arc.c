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

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "event.h"
#include "handler.h"
#include "vertex.h"
#include "arc.h"
#include "node.h"

#include "editor.h"
#include "controller.h"
#include "net.h"

typedef struct _PATH
{
    POINT *point;

} PATH, *PATH_P;

/**
 * @brief  iterator of nodes and arcs - the context determines the processor to apply to the artifact
 *
 */
void arc_vertex_path_iterator(gpointer artifact, gpointer context)
{
    PATH *path = (PATH *)context;

    path->point->x = TO_VERTEX(artifact)->point.x > path->point->x ? TO_VERTEX(artifact)->point.x : path->point->x;
    path->point->y = TO_VERTEX(artifact)->point.y > path->point->y ? TO_VERTEX(artifact)->point.y : path->point->y;

}

/**
 * @brief get the bounds of th arc - used to calculate the scrollbars for the primary view
 *
 */
POINT *arc_get_path_bounds(ARC *arc, POINT *point)
{
    PATH path;

    set_point(point, 0, 0);

    path.point = point;

    g_ptr_array_foreach(arc->vertices,
                        arc_vertex_path_iterator, &path);

    return point;
}

/**
 * @brief arc edit handler called from the editor
 *
 */
void arc_edit_handler(int id, void *value, void *object)
{

    switch (id)
    {

    case 1:
    {
        int *tokens = (int *)value;
        TO_ARC(object)->weight = *tokens;
        TO_ARC(object)->net->redraw(TO_ARC(object)->net);
    }
    break;
    }
}

/**
 * @brief create a vertex
 *
 */
VERTEX *arc_get_vertex(ARC *arc, POINT *point)
{
    int iVertex = 0;
    int located = FALSE;

    for (iVertex = 0; iVertex < arc->vertices->len && !located; iVertex++)
    {
        POINT *vertex = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;

        if (TO_VERTEX(arc->vertices->pdata[iVertex])->position == CONTROL_POSITION && point_on_point(vertex, point, 4))
        {
            return arc->vertices->pdata[iVertex];
        }
    }

    return NULL;
}

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

    if (located)
    {
        g_ptr_array_insert(arc->vertices, iVertex, create_vertex(CONTROL_POSITION, point));
    }
}

/**
 * @brief add a vertex
 *
 */
void arc_add_vertex(ARC *arc, VERTEX * vertex)
{

    g_ptr_array_add(arc->vertices, vertex);
 
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
 * @brief edit place node
 *
 */

void arc_editor(ARC *arc, EDITOR *editor)
{

    editor->init(editor, arc, arc_edit_handler,
                 SPIN_BUTTON, 1, "Tokens", arc->weight,
                 END_FIELD);
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
ARC *new_arc(NET *net)
{
    ARC *arc = g_malloc(sizeof(ARC));
    
    arc->net = net;

    arc->source = NULL;
    arc->target = NULL;

    arc->painter.type = ARC_PAINTER;
    arc->painter.painters.arc_painter.arc = arc;

    arc->weight = 1;

    arc->vertices = g_ptr_array_new();

    arc->release = release_arc;
    arc->isArcAtPoint = is_arc_at_point;
    arc->getPathBounds = arc_get_path_bounds;
    arc->setVertex = arc_set_vertex;
    arc->getVertex = arc_get_vertex;
    arc->addVertex = arc_add_vertex;
    arc->edit = arc_editor;

    return arc;

}

/**
 * @brief arc constructor
 *
 */
ARC *create_arc(NET *net, NODE *source, NODE *target)
{
    ARC *arc = new_arc(net);

    arc->source = source;
    arc->target = target;

    g_ptr_array_add(arc->vertices, create_vertex(SOURCE_POSITION, &source->position));
    g_ptr_array_add(arc->vertices, create_vertex(TARGET_POSITION, &target->position));

    return arc;
}
