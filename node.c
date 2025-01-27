/**
 * @file node.c
 * @author Dr. Neil Brittliff (brittliff.org) 
 * @brief A representation of a petri-net node 'Place' or 'Transition
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
#include "drawer.h"

#include "node.h"
#include "event.h"

/**
 * @brief deallocate the node storage
 *
 */
void destroy_node(NODE *node)
{

    if (node->name != NULL)
    {

        g_string_free(node->name, TRUE);
    }

    g_free(node);
}

/**
 * @brief is the node a 'transition' node
 *
 */
int is_transition(NODE *node)
{

    return (node->type == TRANSITION_NODE) ? 1 : 0;
}

/**
 * @brief is the node a 'place' node
 * 
 */
int is_place(NODE *node)
{

    return (node->type == PLACE_NODE) ? 1 : 0;
}

/**
 * @brief is a point within the node's bounds 
 * 
 */
gint is_node_at_point(NODE *node, POINT *point)
{

    return point_in_bounds(point, &node->bounds);
}

/**
 * @brief set the node's position ('x', 'y' coordinates)
 * 
 */
void set_position(NODE *node, double x, double y)
{

    node->position.x = x;
    node->position.y = y;

    node->bounds.point.x = x - 15;
    node->bounds.point.y = y - 15;

    node->bounds.size.w = 30;
    node->bounds.size.h = 30;
}

/**
 * @brief set the node's name - default name usually 'P' or 'T' followed by a number
 * 
 */
void set_name(NODE *node, gchar *name)
{

    node->name = g_string_new(name);
    node->textLength = node->name->len * DEFAULT_CHAR_LENGTH;
}

/**
 * @brief set the node's 'default' name - default name either 'P' or 'T' followed by a number
 * 
 */
void set_default_name(NODE *node)
{
    GString *name = g_string_new("");

    g_string_printf(name, "%c-%d", node->type == TRANSITION_NODE ? 't' : 'p', node->id);

    node->setName(node, name->str);

    g_string_free(name, TRUE);
}

/**
 * @brief get the node's bounds
 * 
 */
void get_bounds(NODE *node, BOUNDS *bounds)
{

    bounds->point.x = node->bounds.point.x;
    bounds->point.y = node->bounds.point.y;
    bounds->size.w = node->bounds.size.w;
    bounds->size.h = node->bounds.size.h;
}

/**
 * @brief create an initialised node common to both a place and transition node
 * 
 */
NODE *new_node()
{
    NODE *node = g_malloc(sizeof(NODE));

    node->destroy = destroy_node;
    node->setPosition = set_position;
    node->getBounds = get_bounds;
    node->isNodeAtPoint = is_node_at_point;

    node->isTransition = is_transition;
    node->isPlace = is_place;

    node->selected = TRUE;
    node->id = 0;
    node->state = INACTIVE;

    node->textLength = 0;

    node->setName = set_name;
    node->setDefaultName = set_default_name;

    return node;
}

/**
 * @brief create an initialised "place" node
 * 
 */
NODE *new_place()
{
    NODE *node = new_node();

    node->place.marked = 0;
    node->place.occupied = FALSE;

    node->state = INACTIVE;
    node->type = PLACE_NODE;

    node->painter.type = PLACE_PAINTER;
    node->painter.painters.place_painter.node = node;

    set_default_name(node);

    return node;
}

/**
 * @brief create an initialised "transition" node
 * 
 */
NODE *new_transition()
{
    NODE *node = new_node();

    node->transition.duration = 0;
    node->enabled = 0;

    node->state = INACTIVE;
    node->type = TRANSITION_NODE;

    node->painter.type = TRANSITION_PAINTER;
    node->painter.painters.transition_painter.node = node;

    set_default_name(node);

    return node;
}

/**
 * @brief create an initialised node common to both a place and transition node
 * 
 */
NODE *create_node(int type)
{

    return type == PLACE_NODE ? new_place() : new_transition();
}