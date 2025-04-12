/**
 * @file vertex.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - represent a point in the arc
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include "geometry.h"

/**
 * @brief casts an object to a vertex
 *
 */
#define TO_VERTEX(vertex) ((VERTEX *)(vertex))

enum POSITION
{
    SOURCE_POSITION = 0,
    TARGET_POSITION,
    CONTROL_POSITION,
    END_POSITION_TYPES

};

typedef struct _VERTEX {

    /**
     * @brief base structure for a node and arc
     *
     */
    struct _ARTIFACT artifact;
    POINT point;

    enum POSITION position;

    void (*release)(struct _VERTEX * vertex);
    void (*setPoint)(struct _VERTEX * vertex, POINT * point);
 
} VERTEX, *VERTEX_P;

extern VERTEX * create_vertex(enum POSITION position, POINT * point);

#endif // VERTEX_H_INCLUDED