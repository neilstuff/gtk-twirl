/**
 * Node - implementation
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#include <math.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "geometry.h"
#include "event.h"
#include "node.h"

/**
 * @brief Destroy the Node
 *
 * @param node the Node to destroy
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
 * @brief Is the Node a Transition?
 *
 * @param node the Node
 *
 * @return TRUE if it is a transition, FALSE otherwise
 */
gint is_transition(NODE *node)
{

    return (node->type == 1) ? 1 : 0;
}

/**
 * Is the Node a Place
 *
 * @param node the Node
 *
 * @return TRUE if it is a place, FALSE otherwise
 */
gint is_place(NODE *node)
{

    return (node->type == 0) ? 1 : 0;
}

/**
 * @brief Is the point (x,y) within the node's bounds
 *
 * @param node the node
 * @param point determine if the point is in the bounds of the node
 */
gint is_node_at_point(NODE *node, POINT *point)
{

    return point_in_bounds(point, &node->bounds);
}

/**
 * @brief Set the Node Position
 *
 * @param node the Node
 * @param x the 'x' coordinate
 * @param y the 'y' coordinate
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
 * @brief Set the Node
 *
 * @param node the Node
 * @param name the Name of the Node
 */
void set_name(NODE *node, gchar *name)
{

    node->name = g_string_new(name);
    node->textLength = node->name->len * DEFAULT_CHAR_LENGTH;
}

/**
 * @brief Set the Default Node
 *
 * @param node the Node
 */
void set_default_name(NODE *node)
{
    GString *name = g_string_new("");

    g_string_printf(name, "%c-%d", node->type == TRANSITION_NODE ? 't' : 'p', node->id);

    node->setName(node, name->str);

    g_string_free(name, TRUE);
}

/**
 * @brief Get the Bounds
 *
 * @param node the Node
 * @param bounds receives the nodes bounds
 */
void get_bounds(NODE *node, BOUNDS *bounds)
{

    bounds->point.x = node->bounds.point.x;
    bounds->point.y = node->bounds.point.y;
    bounds->size.w = node->bounds.size.w;
    bounds->size.h = node->bounds.size.h;
}

/**
 * @brief Initialise and create a 'BLANK' node
 *
 * Node: the methods applicable apply to both Places/Transitions
 *       the actual node type is applied later
 *
 * @return a node
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

    set_default_name(node);

    return node;
}

/**
 * @brief Create a PLACE Node
 *
 * Node: the methods applicable apply to PLACE only
 *
 * @return a 'PLACE' Node
 */
NODE *new_place()
{
    NODE *node = new_node();

    node->place.marked = 0;
    node->place.occupied = FALSE;

    node->state = INACTIVE;
    node->type = PLACE_NODE;

    return node;
}

/**
 * @brief Create a TRANSITION Node
 *
 * Node: the methods applicable apply to TRANSITION only
 *
 * @return a 'TRANSITION' Node
 */
NODE *new_transition()
{
    NODE *node = new_node();

    node->transition.duration = 0;
    node->enabled = 0;

    node->state = INACTIVE;
    node->type = TRANSITION_NODE;

    return node;
}

/**
 * @brief Create a Node or Place
 *
 * @param type The Type of Node to Create 0 - place, 1 - transition
 *
 * @return a newly created Node
 */
NODE *create_node(int type)
{

    return type == PLACE_NODE ? new_place() : new_transition();
}