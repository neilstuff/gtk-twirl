#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "node.h"
#include "connector.h"
#include "drawer.h"

/**
 * @brief Draw the text
 * 
 * @param drawer the drawer
 * @param node the node's text to paint
 */
void draw_text(DRAWER* drawer, NODE *node)
{
    gdouble x;
    gdouble y;
    cairo_text_extents_t extents;

    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_select_font_face(drawer->canvas, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(drawer->canvas, 12);
    cairo_text_extents(drawer->canvas, node->name->str, &extents);

    cairo_move_to(drawer->canvas, (int)node->position.x - 15 + (int)extents.width/2, (int)node->position.y + 30);
    cairo_show_text(drawer->canvas, node->name->str);
    cairo_get_current_point(drawer->canvas, &x, &y);

    node->textLength = (int)extents.width;

}

/**
 * @brief Draw the selection box
 * 
 * @param DRAWER the DRAWER
 * @param node the node's selection box to paint
 */
void draw_selection_box(DRAWER* drawer, NODE *node)
{

    if (node->selected && node->state == INACTIVE || node->enabled && node->state == ACTIVE)
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
 * Draw the Place Node
 *
 * @param drawer the drawer
 * @param node the node's selection box to paint
 */
void draw_place(DRAWER* drawer, NODE *node)
{

    cairo_set_line_width(drawer->canvas, 2);
    cairo_set_source_rgba(drawer->canvas, 0, 0, 0, 0.1);
 
    cairo_arc(drawer->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_fill(drawer->canvas);
 
    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_arc(drawer->canvas, node->position.x, node->position.y, 9, 0, 2 * M_PI);
    cairo_stroke(drawer->canvas);

    // Draw the marking
    if ((node->place.marked && node->state == INACTIVE) || node->place.occupied && node->state == ACTIVE)
    {
        cairo_set_source_rgb(drawer->canvas, 0, 0, 0);

        cairo_arc(drawer->canvas, node->position.x, node->position.y, 4, 0, 2 * M_PI);
        cairo_fill(drawer->canvas);
    }
 
    draw_selection_box(drawer, node);
    draw_text(drawer, node);
 
}

/**
 * @brief Draw the Transition Node
 * 
 * @param drawer the drawer
 * @param node the node's selection box to paint
 */
void draw_transition(DRAWER* drawer, NODE *node)
{

    cairo_set_line_width(drawer->canvas, 2);
    cairo_set_source_rgba(drawer->canvas, 0, 0, 0, 0.1);

    cairo_rectangle(drawer->canvas, (int)node->position.x - 9, (int)node->position.y - 9, 17, 17);
    cairo_fill(drawer->canvas);

    cairo_set_source_rgb(drawer->canvas, 0, 0, 0);
    cairo_rectangle(drawer->canvas, (int)node->position.x - 9, (int)node->position.y - 9, 18, 18);
    cairo_stroke(drawer->canvas);

    draw_selection_box(drawer, node);
    draw_text(drawer, node);

}

/**
 * @brief Draw the node
 *
 * @param net the drawer
 * @param node the node to draw
 */
void drawer_draw(DRAWER * drawer, NODE * node) 
{ 

    drawer->drawers[node->type](drawer, node);

}

/**
 * @brief Paint the Connector
 *
 * @param drawer the drawer
 * @param connector the connector between nodes to draw
 */
void draw_connector(DRAWER * drawer, CONNECTOR * connector)
{

    cairo_set_line_width (drawer->canvas, 1);
    cairo_set_source_rgb (drawer->canvas, 255, 0, 0);

    cairo_move_to(drawer->canvas, connector->source->position.x, connector->source->position.y);
    cairo_line_to(drawer->canvas, connector->position.x, connector->position.y);

    cairo_stroke (drawer->canvas);

}

/**
 * @brief Reealse a drawer and free any resources
 *
 * @param net the drawer to release
 */
void drawer_release(DRAWER * drawer) 
{ 
    
    g_free(drawer); 
    
}

/**
 * @brief Create a drawer
 *
 * @param canvas The cairo canvas
 *
 * @return a newly created DRAWER
 *
 */
DRAWER *create_drawer(cairo_t *canvas)
{
    DRAWER *drawer = g_malloc(sizeof(DRAWER));

    drawer->release = drawer_release;
    drawer->canvas = canvas;

    drawer->draw = drawer_draw;

    drawer->drawers[PLACE_NODE] = draw_place;
    drawer->drawers[TRANSITION_NODE] = draw_transition;

    return drawer;

}