/**
 * @file drawer.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief draws all graphical object - places, transitions and arcs on the canvas (drawingarea)
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "artifact.h"
#include "drawer.h"

#include "node.h"
#include "event.h"
#include "controller.h"
#include "arc.h"
#include "net.h"
#include "connector.h"

#include "node.h"

/**
 * @brief arc's arrow (-->--) drawer
 *
 */
void draw_arrow_head(ARC *arc, POINT *source, POINT *target, cairo_t *cr)
{
    gdouble slopy = atan2(target->y - source->y, target->x - source->x);
    gdouble cosy = cos(slopy);
    gdouble siny = sin(slopy);
    gdouble par = 14;

    POINT position;

    get_midpoint(source, target, &position);

    cairo_new_path(cr);
    if (arc->selected)
    {
        cairo_set_source_rgb(cr, 0, 0, 255);
    }
    else
    {
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    }

    cairo_set_line_width(cr, 1);

    cairo_move_to(cr, position.x, position.y);

    cairo_line_to(cr, position.x + (int)(-par * cosy - (par / 2.0 * siny)),
                  position.y + (int)(-par * siny + (par / 2.0 * cosy)));

    cairo_line_to(cr, position.x + (int)(-par * cosy + (par / 2.0 * siny)),
                  position.y - (int)(par / 2.0 * cosy + par * siny));

    cairo_line_to(cr, position.x, position.y);

    cairo_fill(cr);
    cairo_stroke(cr);

    cairo_close_path(cr);
}

/**
 * @brief draw the node's text
 *
 */
void draw_text(DRAWER *drawer, NODE *node)
{
    gdouble x;
    gdouble y;
    cairo_text_extents_t extents;

    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_select_font_face(drawer->canvas, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(drawer->canvas, 12);
    cairo_text_extents(drawer->canvas, node->name->str, &extents);

    cairo_move_to(drawer->canvas, (int)node->position.x - 17 + (int)extents.width / 2, (int)node->position.y + 26);
    cairo_show_text(drawer->canvas, node->name->str);
    cairo_get_current_point(drawer->canvas, &x, &y);

    node->textLength = (int)extents.width;
}

/**
 * @brief draw the node's selection box
 *
 */
void draw_selection_box(DRAWER *drawer, NODE *node)
{

    if (node->artifact.selected && node->artifact.state == INACTIVE 
        || node->artifact.enabled && node->artifact.state == ACTIVE)
    {
        const double dashes[] = {1.0, 1.0, 1.0};

        cairo_set_source_rgba(drawer->canvas, 0, 0, 255, 0.2);
        cairo_set_dash(drawer->canvas, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);
        cairo_rectangle(drawer->canvas, (int)node->position.x - 13, (int)node->position.y - 13, 26, 26);
        cairo_stroke(drawer->canvas);
        cairo_set_dash(drawer->canvas, dashes, 0, 0);
    }
}

/**
 * @brief draw the 'place' node
 *
 */
void draw_place(DRAWER *drawer, PAINTER *painter)
{
    NODE *node = painter->painters.transition_painter.node;

    printf("draw_place: from: %s\n", node->name->str);

    cairo_set_line_width(drawer->canvas, 2);
    cairo_set_source_rgb(drawer->canvas, 0.75, 0.75, 0.75);

    cairo_arc(drawer->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_fill(drawer->canvas);

    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_arc(drawer->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_stroke(drawer->canvas);

    // Draw the marking
    if ((node->place.marked && node->artifact.state == INACTIVE) || node->place.occupied && node->artifact.state == ACTIVE)
    {
        cairo_set_source_rgb(drawer->canvas, 0, 0, 0);

        cairo_arc(drawer->canvas, node->position.x, node->position.y, 4, 0, 2 * M_PI);
        cairo_fill(drawer->canvas);
    }

    draw_selection_box(drawer, node);
    draw_text(drawer, node);
}

/**
 * @brief draw the 'transition' node
 *
 */
void draw_transition(DRAWER *drawer, PAINTER *painter)
{
    NODE *node = painter->painters.place_painter.node;

    printf("draw_transition: from: %s\n", node->name->str);
    cairo_set_line_width(drawer->canvas, 2);
    cairo_set_source_rgb(drawer->canvas, 0.75, 0.75, 0.75);

    cairo_rectangle(drawer->canvas, (int)node->position.x - 9, (int)node->position.y - 9, 17, 17);
    cairo_fill(drawer->canvas);

    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_rectangle(drawer->canvas, (int)node->position.x - 9, (int)node->position.y - 9, 18, 18);
    cairo_stroke(drawer->canvas);

    draw_selection_box(drawer, node);
    draw_text(drawer, node);
}

/**
 * @brief draw the node - will pass it on to the drawer for that type of node
 *
 */
void drawer_draw(DRAWER *drawer, PAINTER *painter)
{

    drawer->drawers[painter->type](drawer, painter);
}

/**
 * @brief draw an 'arc' between a place to a transition or transition to place
 *
 */
void draw_arc(DRAWER *drawer, PAINTER *painter)
{
    ARC *arc = painter->painters.arc_painter.arc;
    int point = 0;

    if (arc->selected)
    {
        cairo_set_source_rgb(drawer->canvas, 0, 0, 255);
        cairo_set_line_width(drawer->canvas, 2);
    }
    else
    {
        cairo_set_source_rgba(drawer->canvas, 0, 0, 0, 1.0);
        cairo_set_line_width(drawer->canvas, 1);
    }

    for (point = 0; point < arc->points->len; point++)
    {

        if (point % 2 == 0)
        {

            printf("draw_arc: from: %d:%d\n", (int)TO_POINT(arc->points->pdata[point])->x,
                   (int)TO_POINT(arc->points->pdata[point])->y);
            cairo_move_to(drawer->canvas, (int)TO_POINT(arc->points->pdata[point])->x,
                          (int)TO_POINT(arc->points->pdata[point])->y);
        }
        else
        {

            printf("draw_arc: to: %d:%d\n", (int)TO_POINT(arc->points->pdata[point])->x,
                   (int)TO_POINT(arc->points->pdata[point])->y);

            cairo_line_to(drawer->canvas, (int)TO_POINT(arc->points->pdata[point])->x,
                          (int)TO_POINT(arc->points->pdata[point])->y);

            cairo_stroke(drawer->canvas);

            draw_arrow_head(arc, TO_POINT(arc->points->pdata[point - 1]),
                            TO_POINT(arc->points->pdata[point]),
                            drawer->canvas);
        }
    }
}

/**
 * @brief draw the connector between '2' nodes' must be (p -> t or t -> p)
 *
 */
void draw_connector(DRAWER *drawer, PAINTER *painter)
{
    CONNECTOR *connector = painter->painters.connector_painter.connector;

    const double dashes[] = {1.0, 1.0, 1.0};
    cairo_set_line_width(drawer->canvas, 2);
    cairo_set_dash(drawer->canvas, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);

    cairo_set_source_rgba(drawer->canvas, 255, 0, 0, 0.3);

    cairo_move_to(drawer->canvas, (int)connector->source->position.x, (int)(connector->source->position.y));
    cairo_line_to(drawer->canvas, (int)(connector->source->position.x + connector->offset.x),
                  (int)(connector->source->position.y + connector->offset.y));

    cairo_stroke(drawer->canvas);
    cairo_set_dash(drawer->canvas, dashes, 0, 0);
}

/**
 * @brief deallocate the drawer's storage
 *
 */
void drawer_release(DRAWER *drawer)
{

    g_free(drawer);
}

/**
 * @brief create a drawer object
 *
 */
DRAWER *create_drawer(cairo_t *canvas)
{
    DRAWER *drawer = g_malloc(sizeof(DRAWER));

    drawer->release = drawer_release;
    drawer->canvas = canvas;

    drawer->draw = drawer_draw;

    drawer->drawers[PLACE_PAINTER] = draw_place;
    drawer->drawers[TRANSITION_PAINTER] = draw_transition;
    drawer->drawers[ARC_PAINTER] = draw_arc;
    drawer->drawers[CONNECTOR_PAINTER] = draw_connector;

    return drawer;
}