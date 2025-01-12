#ifndef DRAWER_H_INCLUDED
#define DRAWER_H_INCLUDED

#define TO_DRAWER(drawer) ((DRAWER*)(drawer))

typedef struct _DRAWER 
{
    cairo_t *canvas;

    void (*release) (struct _DRAWER * drawer);

    void (*draw)(struct _DRAWER* drawer, NODE *node);

    void (*drawers[END_NODE_TYPES]) (struct _DRAWER* drawer, NODE * node);
    
} DRAWER, * DRAWER_P;

extern DRAWER * create_drawer(cairo_t *canvas);

#endif // DRAWER_H_INCLUDED