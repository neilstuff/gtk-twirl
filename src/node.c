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

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "geometry.h"
#include "artifact.h"

#include "drawer.h"
#include "editor.h"
#include "reader.h"
#include "writer.h"

#include "node.h"
#include "event.h"
#include "handler.h"
#include "controller.h"
#include "net.h"

void node_edit_handler(int id, void * value, void * object)
{
 
    switch (id) {
        case 0:
            {
                TO_NODE(object)->setName(TO_NODE(object), (char *)value);
                TO_NODE(object)->net->redraw(TO_NODE(object)->net);
  
            }
            break;

        case 1: {
            int * tokens = (int*)value;
            TO_NODE(object)->place.marked = *tokens;
            TO_NODE(object)->net->redraw(TO_NODE(object)->net);
        }
        break;

    }
 
}

/**
 * @brief deallocate the node storage
 *
 */
void release_node(NODE *node)
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
 * @brief get the node's bounds
 *
 */
gint is_node_in_bounds(NODE *node, BOUNDS *bounds)
{

    return point_in_bounds(&node->position, bounds);

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

    node->id = 0;

    node->release = release_node;
    node->setPosition = set_position;
    node->getBounds = get_bounds;
    node->isNodeAtPoint = is_node_at_point;

    node->isTransition = is_transition;
    node->isPlace = is_place;

    setup_artifact(&node->artifact, FALSE, ACTIVE, TRUE);

    node->textLength = 0;

    node->setName = set_name;
    node->setDefaultName = set_default_name;

    return node;
}

/**
 * @brief edit place node
 *
 */

 void node_place_editor(NODE* node, EDITOR * editor)
 {

    editor->init(editor, node, node_edit_handler, 
                 TEXT_FIELD, 0, "Name", node->name->str, 
                 SPIN_BUTTON, 1, "Tokens", node->place.marked,
                 END_FIELD);
 }

/**
 * @brief create an initialised "place" node
 *
 */
NODE *new_place(NODE * node)
{

    node->place.marked = 0;
    node->place.occupied = FALSE;

    node->artifact.state = INACTIVE;
    node->type = PLACE_NODE;

    node->painter.type = PLACE_PAINTER;
    node->painter.painters.place_painter.node = node;
    node->edit = node_place_editor;

    set_default_name(node);

    return node;
}

/**
 * @brief edit place node
 *
 */

 void node_transition_editor(NODE* node, EDITOR * editor)
 {
    editor->init(editor, node, node_edit_handler, TEXT_FIELD, 0, "Name", node->name->str, END_FIELD);
 }

/**
 * @brief create an initialised "transition" node
 *
 */
NODE *new_transition(NODE *node)
{

    node->transition.duration = 0;

    node->artifact.state = INACTIVE;
    node->type = TRANSITION_NODE;

    node->painter.type = TRANSITION_PAINTER;
    node->painter.painters.transition_painter.node = node;

    node->edit = node_transition_editor;

    set_default_name(node);

    return node;
}


/**
 * @brief release/free the node object
 *
 */
void node_release(NODE *node)
{
    g_free(node);
}

/**
 * @brief create an initialised node common to both a place and transition node
 *
 */
NODE *create_node(int type, NET * net)
{
    NODE *node = new_node();

    node->net = net;

    return type == PLACE_NODE ? new_place(node) : new_transition(node);
}