/**
 * @file connector.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - connector is a graphical way to connect a place with a transition or a transition to a place
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef CONNECTOR_H_INCLUDED
#define CONNECTOR_H_INCLUDED

#include "geometry.h"

/**
 * @brief casts an object to a connector
 * 
 */
#define TO_CONNECTOR(connector) ((CONNECTOR*)(connector))

/**
 * @brief connector interface
 * 
 */
typedef struct _CONNECTOR {

    /**  
     * @brief define how the painter should draw the connector
     * 
     */
   struct _PAINTER painter;

    /**
     * @brief release the connector and deallocate resources
     * 
     */
    void (*release)(struct _CONNECTOR * connector);

    /**
     * @brief connect the two nodes together
     * 
     */
    void (*connect)(struct _CONNECTOR * connector, NODE * target);

    /**
     * @brief draw the connector line and possible selected object
     * 
     */
    void (*draw)(struct _CONNECTOR * connector, cairo_t * cr);

    HANDLER handler;

    CONTROLLER * controller;
    NET * net;

    NODE * source;
    NODE * target;

    POINT offset;

} CONNECTOR, * CONNECTOR_P;

/**
 * @brief create a connector object
 *  
 */
extern CONNECTOR * create_connector(CONTROLLER *controller, NET *net, NODE *source);

#endif // CONNECTOR_H_INCLUDED
