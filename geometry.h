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

typedef struct
{

    int x;
    int y;

} POINT;

typedef struct
{

    int w;
    int h;

} SIZE;

typedef struct
{

    POINT point;
    SIZE size;

} BOUNDS;

#endif // POINT_H_INCLUDED