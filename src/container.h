/**
 * @file container.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - represents container of artifacts
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

 #ifndef CONTAINER_H_INCLUDED
 #define CONTAINER_H_INCLUDED

 /**
  * @brief casts an object to a container
  *
  */
 #define TO_CONTAINER(container) ((CONTAINER *)(container))
 
 typedef struct _CONTAINER {
 
    void (*snap)(struct _CONTAINER * container);
    void (*release)(struct _CONTAINER * container);
 
    GPtrArray * history;

    GPtrArray * places;
    GPtrArray * transitions;

    GPtrArray * sources;
    GPtrArray * targets;

    GPtrArray * arcs;
  
 } CONTAINER, *CONTAINER_P;
 
 extern CONTAINER * create_container();
 
 #endif // CONTAINER_H_INCLUDED