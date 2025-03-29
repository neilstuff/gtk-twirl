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

#include "math.h"

#include "artifact.h"
#include "drawer.h"
#include "editor.h"

#include "node.h"
#include "event.h"

#include "editor.h"
#include "handler.h"
#include "controller.h"

#include "vertex.h"
#include "arc.h"
#include "net.h"
#include "connector.h"
#include "selector.h"

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
    gdouble par = 12;

    POINT position;

    get_midpoint(source, target, &position);

    cairo_new_path(cr);
    if (arc->artifact.selected)
    {
        cairo_set_source_rgb(cr, 0, 0, 1.0);
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
 * @brief arc's arrow (-->--) drawer
 *
 */
void draw_arc_tokens(DRAWER *drawer, ARC *arc, POINT *source, POINT *target, cairo_t *cr)
{
    POINT position;
    cairo_text_extents_t extents;

    get_midpoint(source, target, &position);

    gdouble slopy = atan2(target->y - source->y, target->x - source->x);
    gdouble cosy = cos(slopy);
    gdouble siny = sin(slopy); 
    int par = 20;

    cairo_set_line_width(drawer->canvas, 1);
    cairo_set_source_rgb(drawer->canvas, 0.2, 0.2, 0.2);
    cairo_arc(drawer->canvas, position.x + (int)(-par * cosy + (par / 2.0 * siny)),
                              position.y + (int)(-par * siny - (par / 2.0 * cosy)), 6, 0, 2 * M_PI);
    cairo_stroke(drawer->canvas);

    cairo_set_source_rgb(drawer->canvas, 1.0, 1.0, 1.0);
    cairo_fill(drawer->canvas);
   
    GString *tokens = g_string_new("");

    g_string_printf(tokens, "%d", arc->weight);

    cairo_set_source_rgb(drawer->canvas, 0.1, 0.1, 0.1);
    cairo_select_font_face(drawer->canvas, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(drawer->canvas, 10);
    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_text_extents(drawer->canvas, tokens->str, &extents);

    int adjustment = atoi(tokens->str) > 9  && atoi(tokens->str) < 20 || atoi(tokens->str) == 1 ? 1 : 0;
    
    cairo_move_to(drawer->canvas, position.x + (int)(-par * cosy + (par / 2.0 * siny) - (int)extents.width / 2 - adjustment),
                                  position.y + (int)(-par * siny - (par / 2.0 * cosy) + 3));
    cairo_show_text(drawer->canvas, tokens->str);

    g_string_free(tokens, TRUE);

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

    cairo_move_to(drawer->canvas, (int)node->position.x - (int)extents.width / 2, (int)node->position.y + 26);
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

    if (node->artifact.selected && node->artifact.state == INACTIVE || node->artifact.enabled && node->artifact.state == ACTIVE)
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
        cairo_text_extents_t extents;

        cairo_set_source_rgb(drawer->canvas, 0, 0, 0);

        if (node->place.marked == 1)
        {
            cairo_arc(drawer->canvas, node->position.x, node->position.y, 4, 0, 2 * M_PI);
            cairo_fill(drawer->canvas);
        }
        else
        {
            GString *tokens = g_string_new("");

            g_string_printf(tokens, "%d", node->place.marked);

            cairo_arc(drawer->canvas, node->position.x, node->position.y - 5, 2, 0, 2 * M_PI);
            cairo_fill(drawer->canvas);

            cairo_set_source_rgb(drawer->canvas, 0.3, 0.3, 0.3);
            cairo_select_font_face(drawer->canvas, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(drawer->canvas, 11);

            cairo_text_extents(drawer->canvas, tokens->str, &extents);

            cairo_move_to(drawer->canvas, (int)node->position.x - (int)extents.width / 2, (int)node->position.y + 7);
            cairo_show_text(drawer->canvas, tokens->str);

            g_string_free(tokens, TRUE);
        }
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
    int iVertex = 0;
    const double dashes[] = {1.0, 1.0, 1.0};
    
    POINT *source = NULL;
    POINT *target = NULL;

    if (arc->artifact.selected)
    {
        cairo_set_source_rgb(drawer->canvas, 0, 0, 255);
        cairo_set_line_width(drawer->canvas, 1);
        cairo_set_dash(drawer->canvas, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);
    }
    else
    {
        cairo_set_source_rgba(drawer->canvas, 0, 0, 0, 1.0);
        cairo_set_line_width(drawer->canvas, 1);
    }

    for (iVertex = 0; iVertex < arc->vertices->len; iVertex++)
    {

        if (source) {

            target = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;

            cairo_line_to(drawer->canvas, (int)target->x, (int)target->y);

            cairo_stroke(drawer->canvas);

            draw_arrow_head(arc, source, target, drawer->canvas);   
            draw_arc_tokens(drawer, arc, source, target, drawer->canvas);

        } 

        if (target && iVertex < arc->vertices->len - 1) {

            if (TO_VERTEX(arc->vertices->pdata[iVertex])->artifact.selected) 
            {
                cairo_set_source_rgba(drawer->canvas, 0, 0, 255, 1.0);
            }
            else 
            {
                cairo_set_source_rgba(drawer->canvas, 0, 0, 0, 1.0);
            }
            cairo_arc(drawer->canvas, (int)target->x, (int)target->y, 3, 0, 2 * M_PI);
            cairo_fill(drawer->canvas);
 
        }

        source = &TO_VERTEX(arc->vertices->pdata[iVertex])->point;

        cairo_move_to(drawer->canvas, (int)source->x, (int)source->y);
    
    }

    cairo_set_dash(drawer->canvas, dashes, 0, 0);
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
 * @brief draw the node's selection box
 *
 */
void draw_selection(DRAWER *drawer, PAINTER *painter)
{
    const double dashes[] = {1.0, 1.0, 1.0}; 

    cairo_set_source_rgba(drawer->canvas, 0, 0, 1.0, 0.2);
 
    int width = (int)(painter->painters.selector_painter.selector->offset.x - 
                      painter->painters.selector_painter.selector->position.x);

    int height = (int)(painter->painters.selector_painter.selector->offset.y - 
                        painter->painters.selector_painter.selector->position.y);

    cairo_set_dash(drawer->canvas, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);
    cairo_rectangle(drawer->canvas, (int)(painter->painters.selector_painter.selector->position.x), 
                    (int)(painter->painters.selector_painter.selector->position.y),
                    width, height);
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
    drawer->drawers[SELECTOR_PAINTER] = draw_selection;

    return drawer;
}