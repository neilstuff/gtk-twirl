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
 * @brief node type
 *
 */
enum TYPE
{
    PLACE_NODE = 0,
    TRANSITION_NODE,
    END_NODE_TYPES

};

/**
 * @brief node name alignment
 *
 */
enum ALIGNMENT
{
    TOP = 0,
    BOTTOM,
    LEFT,
    RIGHT,
    END_NODE_ALIGNMENT
};

/**
 * @brief a place node (state container)
 * 
 */
typedef struct
{

    int marked;
    int occupied;

} PLACE;

/**
 * @brief a transition node (process container)
 * 
 */
typedef struct
{

    int duration;

} TRANSITION;

typedef struct _NODE
{
    
    /**  
     * @brief base structure for a node and arc
     * 
     */
    struct _ARTIFACT artifact;

    /**  
     * @brief define how the painter should draw the node
     * 
     */
    struct _PAINTER painter;
    
    /**
     * @brief node's destructor
     * 
     */
    void (*release)(struct _NODE *node);

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
     * @brief returns the id prefixed by the type
     * 
     */
    char * (*generate)(struct _NODE *node, int length, char *buffer);

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
     * @brief edit the node's properties and attributes
     * 
     */
    void (*edit)(struct _NODE *node,  EDITOR * editor);

    /**
     * @brief the owning 'net'
     * 
     */
    struct _NET * net;

    /**
     * @brief private (the nodes type - place/transition)
     * 
     */
    enum TYPE type;

    /**
     * @brief id 
     * 
     */
    int id;

    /**
     * @brief the node's name/title
     * 
     */
    GString *name;

    /**
     * @brief private (the node's name alignment)
     * 
     */
    enum ALIGNMENT alignment;
    
    /**
     * @brief the node's text length in pixels
     * 
     */
    int textLength;

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

extern NODE *create_node(int type, struct _NET * net);

#endif // NODE_H_INCLUDED
