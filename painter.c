

/**
 * Draw the Node's Text
 *
 * @param node the node (may not be selected
 * @param cr the Cairo Context
 *
 */
void draw_text (NODE * node, cairo_t *cr)
{
    gdouble x;
    gdouble y;

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_move_to (cr, (int)node->position.x + 15, (int)node->position.y);

    cairo_select_font_face (cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size (cr, 12);
    cairo_show_text (cr, node->name->str);
    cairo_get_current_point (cr, &x, &y);

    node->textLength = x - node->position.x + 35;

 }

/**
 * Draw the Selection box
 *
 * @param node the node (may not be selected
 * @param cr the Cairo Context
 *
 */
void draw_selection_box (NODE * node, cairo_t *cr)
{

    if (node->selected && node->state == INACTIVE
    || node->enabled && node->state == ACTIVE) {
        cairo_set_source_rgb (cr, 0, 0, 255);

        cairo_rectangle (cr, (int)node->position.x - 12, (int)node->position.y - 12, 24, 24);
        cairo_stroke (cr);

   }

}


/**
 * Draw the Node's Text
 *
 * @param node the node (may not be selected
 * @param cr the Cairo Context
 *
 */
void draw_text (NODE * node, cairo_t *cr)
{
    gdouble x;
    gdouble y;

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_move_to (cr, (int)node->position.x + 15, (int)node->position.y);

    cairo_select_font_face (cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size (cr, 12);
    cairo_show_text (cr, node->name->str);
    cairo_get_current_point (cr, &x, &y);

    node->textLength = x - node->position.x + 35;

 }

/**
 * Draw the Transition Node
 *
 * @param node the Node
 * @param cr the Cairo Canvas
 *
 */
void draw_box(NODE * node, cairo_t *cr)
{

    cairo_set_line_width (cr, 2);
    cairo_set_source_rgb (cr, 0, 255, 0);

    cairo_rectangle (cr, (int)node->position.x-10, (int)node->position.y-5, 20, 8);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, (int)node->position.x-10, (int)node->position.y-5, 20, 9);
    cairo_stroke (cr);

    draw_selection_box(node, cr);
    draw_text(node, cr);

}


/**
 * Draw the Place Node
 *
 * @param node the Node
 * @param cr the Cairo Canvas
 *
 */
void draw_place (NODE * node, cairo_t *cr)
{

    cairo_set_line_width (cr, 2);
    cairo_set_source_rgb (cr, 255, 255, 0);

    cairo_arc (cr, node->position.x, node->position.y, 9, 0, 2*M_PI);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_arc (cr, node->position.x, node->position.y, 9, 0, 2*M_PI);
    cairo_stroke (cr);

    // Draw the marking
    if ((node->place.marked && node->state == INACTIVE)
    || node->place.occupied && node->state == ACTIVE) {
        cairo_set_source_rgb (cr, 0, 0, 0);

        cairo_arc (cr, node->position.x, node->position.y, 4, 0, 2*M_PI);
        cairo_fill (cr);
    }

    draw_selection_box(node, cr);
    draw_text(node, cr);

}

/**
 * @brief Draw the Transition Node
 *
 * @param node the Node
 * @param cr the Cairo Canvas
 *
 */
void draw_transition(NODE * node, cairo_t *cr)
{

    cairo_set_line_width (cr, 2);
    cairo_set_source_rgb (cr, 0, 255, 0);

    cairo_rectangle (cr, (int)node->position.x-10, (int)node->position.y-5, 20, 8);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, (int)node->position.x-10, (int)node->position.y-5, 20, 9);
    cairo_stroke (cr);

    draw_selection_box(node, cr);
    draw_text(node, cr);

}