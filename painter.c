#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "node.h"
#include "painter.h"

/**
 * @brief Draw the text
 * 
 * @param painter the painter
 * @param node the node's text to paint
 */
void draw_text(PAINTER* painter, NODE *node)
{
    gdouble x;
    gdouble y;

    cairo_set_source_rgb(painter->canvas, 0, 0, 0);
    cairo_move_to(painter->canvas, (int)node->position.x + 15, (int)node->position.y);

    cairo_select_font_face(painter->canvas, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(painter->canvas, 12);
    cairo_show_text(painter->canvas, node->name->str);
    cairo_get_current_point(painter->canvas, &x, &y);

    node->textLength = x - node->position.x + 35;

}

/**
 * @brief Draw the selection box
 * 
 * @param painter the painter
 * @param node the node's selection box to paint
 */
void draw_selection_box(PAINTER* painter, NODE *node)
{

    if (node->selected && node->state == INACTIVE || node->enabled && node->state == ACTIVE)
    {
        cairo_set_source_rgb(painter->canvas, 0, 0, 255);

        cairo_rectangle(painter->canvas, (int)node->position.x - 12, (int)node->position.y - 12, 24, 24);
        cairo_stroke(painter->canvas);
    }

}

/**
 * Draw the Place Node
 *
 * @param painter the painter
 * @param node the node's selection box to paint
 */
void draw_place(PAINTER* painter, NODE *node)
{

    cairo_set_line_width(painter->canvas, 2);
    cairo_set_source_rgb(painter->canvas, 255, 255, 0);

    cairo_arc(painter->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_fill(painter->canvas);

    cairo_set_source_rgb(painter->canvas, 0, 0, 0);
    cairo_arc(painter->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_stroke(painter->canvas);

    // Draw the marking
    if ((node->place.marked && node->state == INACTIVE) || node->place.occupied && node->state == ACTIVE)
    {
        cairo_set_source_rgb(painter->canvas, 0, 0, 0);

        cairo_arc(painter->canvas, node->position.x, node->position.y, 4, 0, 2 * M_PI);
        cairo_fill(painter->canvas);
    }

    draw_selection_box(painter, node);
    draw_text(painter, node);

}

/**
 * @brief Draw the Transition Node
 * 
 * @param painter the painter
 * @param node the node's selection box to paint
 */
void draw_transition(PAINTER* painter, NODE *node)
{

    cairo_set_line_width(painter->canvas, 2);
    cairo_set_source_rgb(painter->canvas, 0, 255, 0);

    cairo_rectangle(painter->canvas, (int)node->position.x - 10, (int)node->position.y - 5, 20, 8);
    cairo_fill(painter->canvas);

    cairo_set_source_rgb(painter->canvas, 0, 0, 0);
    cairo_rectangle(painter->canvas, (int)node->position.x - 10, (int)node->position.y - 5, 20, 9);
    cairo_stroke(painter->canvas);

    draw_selection_box(painter, node);
    draw_text(painter, node);

}

/**
 * @brief Reealse a painter and free any resources
 *
 * @param net the painter to release
 */
void painter_release(PAINTER * painter) 
{ 
    
    g_free(painter); 
    
}

/**
 * @brief Create a Painter
 *
 * @param canvas The cairo canvas
 *
 * @return a newly created painter
 *
 */
PAINTER *create_painter(cairo_t *canvas)
{
    PAINTER *painter = g_malloc(sizeof(PAINTER));

    painter->release = painter_release;

    painter->drawPlace = draw_place;
    painter->drawTransition = draw_transition;

}