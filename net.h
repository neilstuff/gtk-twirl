/**
 * @file net.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief prototype - a 'Petri Net' representation
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "event.h"


#define TO_NET(net) ((NET*)(net))

/**
 * @brief the Net's interface
 * 
 */
typedef struct _NET {
    CONTROLLER * controller;
    GPtrArray * places;
    GPtrArray * transitions;
    GPtrArray * arcs;
    GPtrArray * artifacts;

    enum TOOL tool;

    HANDLER handler;

    void (*connect) (struct _NET * net, NODE * source, POINT * point);

    void (*processors[END_NOTIFICATION]) (struct _NET * net, EVENT * event);

    void (*redraw) (struct _NET * net);
    void (*resize) (struct _NET * net);

    void (*release) (struct _NET * net);

} NET, * NET_P;

extern NET * net_create(CONTROLLER * controller);

#endif // NET_H_INCLUDED
