/**
 * @file handler.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - the interface between events and the processor
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

 #ifndef HANDLER_H_INCLUDED
 #define HANDLER_H_INCLUDED
 
#define TO_HANDLER(handler) ((HANDLER *)(handler))

typedef void (*handler)(EVENT *event, void *processor);

/**
 * @brief a handler is an interface for implementation to processor events
 * 
 */
typedef void (*HANDLER_T)(EVENT *event, void *processor);

typedef struct _HANDLER
{

    /**
     * @brief function to process events
     * 
     */
    HANDLER_T handler;

    /**
     * @brief user-data to pass the data to the processor
     * 
     */
    void *processor;

    /**
     * @brief release the handler
     * 
     */
    void (*release) (struct _HANDLER * handler);

} HANDLER, HANDLER_P;

extern HANDLER * create_handler(HANDLER_T callback, void* processor);

#endif // HANDLER_H_INCLUDED