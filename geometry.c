/**
 * @file geometry.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  A collection of geometric functions
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

int point_in_bounds(POINT * point, BOUNDS * bounds)
{

    return (point->x >= bounds->point.x &&
            point->x <= bounds->point.x + bounds->size.w &&
            point->y >= bounds->point.y &&
            point->y <= bounds->point.y + bounds->size.h);
}

POINT * set_point(POINT * point, double x, double y)
{
    point->x = x;
    point->y = y;

    return point;
}
