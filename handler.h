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

    int (*onButtonPressed)(struct _EVENT * EVENT);

    void (*release) (struct _HANDLER * handler);

} HANDLER, * HANDLER_P;

#endif // HANDLER_H_INCLUDED