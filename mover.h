/**
 * @file mover.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - mover - moves objects in a net node, arc control poiints
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MOVER_H_INCLUDED
#define MOVER_H_INCLUDED

#include "geometry.h"

/**
 * @brief casts an object to a mover
 * 
 */
#define TO_MOVER(mover) ((MOVER*)(mover))

/**
 * @brief mover interface
 * 
 */
typedef struct _MOVER {

    /**
     * @brief release the mover and deallocate resources
     * 
     */
    void (*release)(struct _MOVER * mover);
    void (*addNode)(struct _MOVER * mover, NODE *node);
    
    /**  
     * @brief define how the painter should draw the connector
     * 
     */
    struct _PAINTER painter;

    HANDLER handler;

    CONTROLLER * controller;
    NET * net;

    GPtrArray * nodes;
    GPtrArray * targets;
    GPtrArray * sources;

    POINT offset;

} MOVER, * MOVER_P;

/**
 * @brief create a mover object
 *  
 */
extern MOVER *create_mover(CONTROLLER *controller, POINT * point, NET *net);

#endif // MOVER_H_INCLUDED