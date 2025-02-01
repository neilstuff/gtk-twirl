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

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <math.h>

#include "geometry.h"

/**
 * @brief Get the distance between 2 points
 *
 */
int get_distance(POINT * source, POINT * target)
{
    double a = source->x - target->x;
    double b = source->y - target->y;

    return sqrt(a * a + b * b);
}

/**
 * @brief determine if a point is on a line
 * 
 */
int point_on_line(POINT * source, POINT * target, POINT * point, double tolerate)
{
    double d1 = get_distance(source, target);
    double d2 = get_distance(source, point);
    double d3 = get_distance(target, point);

    printf("Distances %f : %f : %f : %f \n", d1,d2,d3, trunc(d2 + d3));

    return (trunc(d2 + d3) >= trunc(d1) - tolerate) &&
           (trunc(d2 + d3) <= trunc(d1) + tolerate);
}

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
POINT *get_point_on_line(LINE *line, int distance, POINT *point)
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
 * @brief get the midpoint on a line give two points
 *
 */
POINT *get_midpoint(POINT *source, POINT *target, POINT *midpoint)
{
    midpoint->x = (source->x + target->x) / 2;
    midpoint->y = (source->y + target->y) / 2;

    return midpoint;
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

/**
 * @brief clone a point
 *
 */
POINT *clone_point(POINT *clone)
{
    POINT *point = g_malloc(sizeof(POINT));
    
    point->x = clone->x;
    point->y = clone->y;

    return point;
}
