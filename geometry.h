/**
 * Point - prototype
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#define TO_POINT(point) ((POINT*)(point))
#define TO_SIZE(size) ((POINT*)(size))
#define TO_BOUNDS(bounds) ((POINT*)(bounds))

typedef struct
{

    gdouble x;
    gdouble y;

} POINT;

typedef struct
{

    gdouble w;
    gdouble h;

} SIZE;

typedef struct
{

    POINT point;
    SIZE size;

} BOUNDS;

extern int point_in_bounds(POINT * point, BOUNDS * bounds);
extern POINT * set_point(POINT * point, double x, double y);

#endif // POINT_H_INCLUDED