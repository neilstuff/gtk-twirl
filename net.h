/**
 * @file net.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief header file - a 'Petri Net' representation
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "event.h"
/**
 * Net - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#define TO_NET(net) ((NET*)(net))

typedef struct _NET {
    CONTROLLER * controller;
    GPtrArray * places;
    GPtrArray * transitions;
    GPtrArray * arcs;

    enum TOOL tool;

    HANDLER handler;

    void (*processors[END_NOTIFICATION]) (struct _NET *net, EVENT * event);

    void (*release) (struct _NET * net);




} NET, * NET_P;

extern NET * net_create(CONTROLLER * controller);

#endif // NET_H_INCLUDED
