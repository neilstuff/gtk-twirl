/**
 * @file geometry.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief Geometrycollection of geometric functions
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "geometry.h"

/**
 * @brief Determine if a point is in bounds
 *
 * @param point the point (x, y)
 * @param bounds the bounds (width, height)
 * @return int 1 - in bounds (true), 0 (false) - otherwise
 */
int point_in_bounds(POINT * point, BOUNDS * bounds)
{

    return (point->x >= bounds->point.x &&
            point->x <= bounds->point.x + bounds->size.w &&
            point->y >= bounds->point.y &&
            point->y <= bounds->point.y + bounds->size.h);
}

/**
 * @brief Set the point object
 * 
 * @param point the receiving point
 * @param x the point's x value
 * @param y the point's y value
 * @return POINT* return the populated point - fluent style
 */
POINT * set_point(POINT * point, double x, double y)
{
    point->x = x;
    point->y = y;

    return point;
}
