#ifndef HANDLER_H_INCLUDED
#define HANDLER_H_INCLUDED

#include "event.h"

/**
 * HANDLER - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

typedef struct _HANDLER {

    int (*onSelectButtonPressed)(struct _EVENT * EVENT);
    int (*onPlaceButtonPressed)(struct _EVENT * EVENT);
    int (*onTransitionButtonPressed)(struct _EVENT * EVENT);

} HANDLER, * HANDLER_P;

#endif // HANDLER_H_INCLUDED