#ifndef HANDLER_H_INCLUDED
#define HANDLER_H_INCLUDED

#include "event.h"

#define TO_HANDLER(handler) ((HANDLER*)(handler))

/**
 * HANDLER - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

typedef struct _HANDLER {

    void (*notify)(EVENT * EVENT);

    void (*release) (struct _HANDLER * handler);

} HANDLER, * HANDLER_P;

extern HANDLER * handler_create();

#endif // HANDLER_H_INCLUDED