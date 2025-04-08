/**
 * @file selector.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - selector - select multiple objects with a dragt square
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef SELECTOR_H_INCLUDED
#define SELECTOR_H_INCLUDED

#include "geometry.h"

/**
 * @brief casts an object to a mover
 * 
 */
#define TO_SELECTOR(selector) ((SELECTOR*)(selector))

/**
 * @brief selector interface
 * 
 */
typedef struct _SELECTOR {

    /**  
     * @brief define how the painter should draw the connector
     * 
     */
    struct _PAINTER painter;

    /**
     * @brief release the selector and deallocate resources
     * 
     */
    void (*release)(struct _SELECTOR * selector);

    HANDLER handler;

    CONTROLLER * controller;
    NET * net;

    GPtrArray * nodes;

    POINT position;
    POINT offset;

} SELECTOR, * SELECTOR_P;

/**
 * @brief create a selector object
 *  
 */
extern SELECTOR *create_selector(CONTROLLER *controller, POINT * point, NET *net);

#endif // SELECTOR_H_INCLUDED