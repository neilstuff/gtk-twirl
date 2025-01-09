#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

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

    void (*release) (struct _NET * net);
    void (*notify) (struct _NET * net, EVENT * event);

    void (*invalidateBounds)(struct _NET * net, GdkRectangle * bounds);
    void (*invalidate)(struct _NET * net);


} NET, * NET_P;

extern NET * net_create(CONTROLLER * controller);

#endif // NET_H_INCLUDED
