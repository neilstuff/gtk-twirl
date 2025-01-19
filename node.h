/**
 * Node - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
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

    void (*destroy)(struct _NODE *node);
    void (*setPosition)(struct _NODE *, double x, double y);
    gint (*isTransition)(struct _NODE *node);
    int (*isPlace)(struct _NODE *node);
    void (*setName)(struct _NODE *node, gchar *name);
    void (*setDefaultName)(struct _NODE *node);

    void (*getBounds)(struct _NODE *node, BOUNDS * bounds);
    gint (*isNodeAtPoint)(struct _NODE *node,  POINT * point);

    enum TYPE type;
    gint id;
    GString *name;

    int selected;
    int textLength;
    int enabled;
    int state;

    POINT position;
    BOUNDS bounds;

    union
    {

        PLACE place;
        TRANSITION transition;
    };

} NODE, *NODE_P;

extern NODE *create_node(int type);

#endif // NODE_H_INCLUDED
