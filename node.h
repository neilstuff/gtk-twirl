/**
 * @file node.h
 * @author Dr. Neil Brittliff (brittliff.org) 
 * @brief prototype - a representation of a petri-net node 'Place' or 'Transition
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "geometry.h"

#define TO_NODE(node) ((NODE *)(node))
#define TO_PLACE(node) ((NODE *)(node))->place
#define TO_TRANSITION(node) ((NODE *)(node))->transition

#define DEFAULT_CHAR_LENGTH 10

#define PLACE_ELEMENT "place"
#define TRANSITION_ELEMENT "transition"
#define NODE_NAME_ELEMENT "name"
#define INITIAL_MARKING_ELEMENT "initialMarking"
#define GRAPHICS_ELEMENT "graphics"

#define X_ATTRIBUTE "x"
#define Y_ATTRIBUTE "y"
#define NODE_ID_ATTRIBUTE "id"

/**
 * Node Definition File
 *
 * @author Neil Brittliff
 */

enum TYPE
{
    PLACE_NODE = 0,
    TRANSITION_NODE,
    END_NODE_TYPES

};

enum STATE
{
    ACTIVE,
    INACTIVE
};

typedef struct
{

    int marked;
    int occupied;

} PLACE;

typedef struct
{

    int duration;

} TRANSITION;


typedef struct _NODE
{

    /**
     * @brief node's destructor
     * 
     */
    void (*destroy)(struct _NODE *node);

    /**
     * @brief set the nodes position within the drawing canvas
     * 
     */
    void (*setPosition)(struct _NODE *, double x, double y);

    /**
     * @brief returns true if the node is a transition, false othewise
     * 
     */
    int (*isTransition)(struct _NODE *node);

    /**
     * @brief returns true if the node is a place, false othewise
     * 
     */
    int (*isPlace)(struct _NODE *node);

    /**
     * @brief set/replace the node's name
     * 
     */
    void (*setName)(struct _NODE *node, gchar *name);

    /**
     * @brief set the node's default name - format: [p|t]-[0-9]*
     * 
     */
    void (*setDefaultName)(struct _NODE *node);

    /**
     * @brief get the nodes bounds (bounds is the input to receive the coordinates)
     * 
     */
    void (*getBounds)(struct _NODE *node, BOUNDS * bounds);

    /**
     * @brief returns true if the node's bounds are contained within the poinr, false otherwise
     * 
     */
    int (*isNodeAtPoint)(struct _NODE *node,  POINT * point);

    /**
     * @brief private (the nodes type - place/transition)
     * 
     */
    enum TYPE type;

    /**
     * @brief private (the nodes unique id)
     * 
     */
    int id;

    /**
     * @brief the node's name/title
     * 
     */
    GString *name;

    /**
     * @brief if '1'the node is selected, '0'not selected
     * 
     */
    int selected;

    /**
     * @brief the node's text length in pixels
     * 
     */
    int textLength;

    /**
     * @brief '1' the node is enabled, '0' not enabled
     * 
     */
    int enabled;

    /**
     * @brief the node's state either ACTIVE or INACTIVE
     * 
     */
    int state;

    /**
     * @brief the node's central point
     * 
     */
    POINT position;

    /**
     * @brief the node's bound
     * 
     */
    BOUNDS bounds;

    /**
     * @brief a node can be one type - PLACE or TRANSITION
     * 
     */
    union
    {

        PLACE place;
        TRANSITION transition;
    };

} NODE, *NODE_P;

extern NODE *create_node(int type);

#endif // NODE_H_INCLUDED
