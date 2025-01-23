/**
 * @file connector.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  header file - connector is a graphical way to connect a place with a transition or a transition to a place
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef CONNECTOR_H_INCLUDED
#define CONNECTOR_H_INCLUDED

#include "node.h"
#include "event.h"
#include "geometry.h"

#define TO_CONNECTOR(connector) ((CONNECTOR*)(connector))

typedef struct _CONNECTOR {

    void (*release)(void * connector);
    void (*connect)(struct _CONNECTOR * connector, NODE* target);
    void (*draw)(struct _CONNECTOR * connector, cairo_t * cr);

    HANDLER handler;

    NODE * source;
    NODE * target;

    POINT position;

} CONNECTOR, * CONNECTOR_P;

extern CONNECTOR * create_connector(NODE * source);

#endif // CONNECTOR_H_INCLUDED
