/**
 * @file arc.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - an arc joins places to transitions or transitions to places
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include "geometry.h"

/**
 * @brief casts an object to an arc
 * 
 */
#define TO_ARC(arc) ((ARC*)(arc))

typedef struct _ARC
 {
     struct _ARTIFACT artifact;

    struct _PAINTER painter;

    void (*release)(struct _ARC * arc);
    POINT * (*getPathBounds)(struct _ARC * arc,  POINT * point);
    int (*isArcAtPoint)(struct _ARC * arc,  POINT * point);
    VERTEX * (*getVertex)(struct _ARC * arc, POINT * point);
    void (*setVertex)(struct _ARC * arc, POINT * point);
    void (*addVertex)(struct _ARC * arc, VERTEX * vertex);

    /**
     * @brief draw the arc
     * 
     */
    void (*draw)(struct _ARC * arc, cairo_t * cr);

    /**
     * @brief edit the arc's properties and attributes
     * 
     */
    void (*edit)(struct _ARC * arc,  EDITOR * editor);

    struct _NET * net;

    struct _NODE * source;
    struct _NODE * target;

    GPtrArray * vertices;

    int weight;

} ARC, * ARC_P;


extern ARC * new_arc(struct _NET * net);
extern ARC * create_arc(struct _NET * net,  struct _NODE * source, struct _NODE * target);

#endif // ARC_H_INCLUDED
