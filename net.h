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

    void (*invalidateBounds)(struct _NET * net, GdkRectangle * bounds);
    void (*invalidate)(struct _NET * net);

   
    GPtrArray * places;
    GPtrArray * transitions;
    GPtrArray * arcs;

    int position;

} NET, * NET_P;

extern NET * net_create(CONTROLLER * controller);
extern void net_release(CONTROLLER  * controller);

#endif // NET_H_INCLUDED
