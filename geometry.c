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

/**
 * @brief calculate the length of a line
 *
 */
double get_length(LINE *line)
{
    float dx = line->source.x - line->target.x;
    float dy = line->source.y - line->target.y;

    return sqrt(dx * dx + dy * dy);
}

/**
 * @brief determine if a point (x,y) is on the specified line
 *
 */
POINT * get_point_on_line(LINE *line, int distance, POINT *point)
{

    // calculate a point on the line x1-y1 to x2-y2 that is distance from x2-y2

    double vx = line->target.x - line->source.x; // x vector
    double vy = line->target.y - line->source.y; // y vector

    point->x = ((double)line->target.x + vx * (double)distance);
    point->y = ((double)line->target.y + vy * (double)distance);

    return point;
}

/**
 * @brief calculates the distance between 2 points
 *
 */
double distance_between(POINT *a, POINT *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;

    return sqrt(dx * dx + dy * dy);
}

/**
 * @brief determine if a point is in the bounding rectangle
 *
 */
int point_in_bounds(POINT *point, BOUNDS *bounds)
{

    return (point->x >= bounds->point.x &&
            point->x <= bounds->point.x + bounds->size.w &&
            point->y >= bounds->point.y &&
            point->y <= bounds->point.y + bounds->size.h);
}

/**
 * @brief set (x, y) coordinates of a point and return that paint
 *
 */
POINT *set_point(POINT *point, double x, double y)
{
    point->x = x;
    point->y = y;

    return point;
}

/**
 * @brief set the source and target of a line and return that line
 *
 */
LINE *set_line(LINE *line, POINT *source, POINT *target)
{
    line->source.x = source->x;
    line->source.y = source->y;

    line->target.x = target->x;
    line->target.y = target->y;

    return line;
}