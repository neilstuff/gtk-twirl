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

/**
 * @brief casts an object to an arc
 * 
 */
#define TO_ARC(arc) ((ARC*)(arc))

typedef struct _ARC {

    void (*destroy)(struct _ARC * arc);
    void (*setBounds)(struct _ARC * arc);
    void (*draw)(struct _ARC * arc, cairo_t * cr);

    struct _NODE * source;
    struct _NODE * target;

    GPtrArray * points;

    int weight;
    int selected;

} ARC, * ARC_P;

extern ARC * create_arc (struct _NODE * source, struct _NODE * target);

#endif // ARC_H_INCLUDED
