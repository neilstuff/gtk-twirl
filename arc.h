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
    int (*isArcAtPoint)(struct _ARC *arc,  POINT * point);
    void (*setVertex)(struct _ARC * arc, POINT * point);
    void (*draw)(struct _ARC * arc, cairo_t * cr);

    struct _NODE * source;
    struct _NODE * target;

    GPtrArray * vertices;

    int weight;

} ARC, * ARC_P;

extern ARC * create_arc (struct _NODE * source, struct _NODE * target);

#endif // ARC_H_INCLUDED
