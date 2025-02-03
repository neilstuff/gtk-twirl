/**
 * @file geometry.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - a collection of geometric functions
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

/**
 * @brief casts an object to a point
 * 
 */
#define TO_POINT(point) ((POINT*)(point))

/**
 * @brief casts an object to a size
 * 
 */
#define TO_SIZE(size) ((SIZE*)(size))

/**
 * @brief casts an object to a bounds
 * 
 */
#define TO_BOUNDS(bounds) ((BOUNDS*)(bounds))

/**
 * @brief casts an object to a line
 * 
 */
#define TO_LINE(line) ((LINE*)(line))

/**
 * @brief a point is an x,y coordinate
 * 
 */
typedef struct
{

    double x;
    double y;

} POINT;

/**
 * @brief a size is a width (h) and height (w)
 * 
 */
typedef struct
{

    double w;
    double h;

} SIZE;

/**
 * @brief a bounds is rectange with cooridnates (x,y) its size
 * 
 */
typedef struct
{

    POINT point;
    SIZE size;

} BOUNDS;

/**
 * @brief a line is 2 points - the source and target locations
 * 
 */
typedef struct
{

    POINT source;
    POINT target;

} LINE;

extern double get_length(LINE * line);
extern int point_on_line(POINT * source, POINT * target, POINT * point, double tolerate);
extern POINT * get_point_on_line(LINE *line, int distance, POINT *point);
extern int point_in_bounds(POINT * point, BOUNDS * bounds);
extern POINT * get_midpoint(POINT *source, POINT *target, POINT *midpoint);
extern POINT * set_point(POINT * point, double x, double y);
extern LINE * set_line(LINE * line, POINT * source, POINT * target);
extern void copy_point(POINT *from, POINT *to);

#endif // POINT_H_INCLUDED