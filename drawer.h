/**
 * @file drawer.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief prototype - draws all graphical object - places, transitions and arcs on the canvas (drawingarea)
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef DRAWER_H_INCLUDED
#define DRAWER_H_INCLUDED

enum PAINTER_TYPE
{
    PLACE_PAINTER = 0,
    TRANSITION_PAINTER,
    ARC_PAINTER,
    CONNECTOR_PAINTER,
    END_PAINTER_TYPES

};

/**
 * @brief painter prototype - instructs the drawer what painter method to use
 * 
 */
typedef struct _PAINTER
{
    enum PAINTER_TYPE type;

    union
    {
        struct
        {

            struct _NODE * node;

        } place_painter;

        struct
        {

             struct _NODE * node;

        } transition_painter;
      
        struct
        {

            struct _ARC * arc;
        } arc_painter;
     
        struct
        {

            struct _NET * net;
            struct _CONNECTOR * connector;
        } connector_painter;
  
  
    } painters;

} PAINTER, *PAINTER_P; 

/**
 * @brief casts an object to a drawer
 *
 */
#define TO_DRAWER(drawer) ((DRAWER *)(drawer))

typedef struct _DRAWER
{
    cairo_t *canvas;

    void (*release)(struct _DRAWER *drawer);

    void (*draw)(struct _DRAWER *drawer, PAINTER *painter);

    void (*drawers[END_PAINTER_TYPES])(struct _DRAWER *drawer, PAINTER * painter);

} DRAWER, *DRAWER_P;

extern DRAWER *create_drawer(cairo_t *canvas);

#endif // DRAWER_H_INCLUDED