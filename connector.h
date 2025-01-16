#ifndef CONNECTOR_H_INCLUDED
#define CONNECTOR_H_INCLUDED

#include "node.h"

/**
 * Connector - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#define TO_CONNECTOR(connector) ((CONNECTOR*)(connector))

#include "point.h"

/**
 * Connector Definition File
 *
 * @author Neil Brittliff
 */

typedef struct _CONNECTOR {

    void (*release)(void * connector);
    void (*connect)(struct _CONNECTOR * connector, NODE* target);
    void (*draw)(struct _CONNECTOR * connector, cairo_t * cr);

    struct _NODE * source;
    struct _NODE * target;

    POINT position;

} CONNECTOR, * CONNECTOR_P;

extern CONNECTOR * create_connector(NODE * source);

#endif // CONNECTOR_H_INCLUDED
