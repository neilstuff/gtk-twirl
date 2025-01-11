#ifndef PAINTER_H_INCLUDED
#define PAINTER_H_INCLUDED

#define TO_PAINTER(painter) ((PAINTER*)(painter))

typedef struct _PAINTER 
{
    cairo_t *canvas;

    void (*release) (struct _PAINTER * PAINTER);

    void (*drawPlace)(struct _PAINTER* painter, NODE *node);
    void (*drawTransition)(struct _PAINTER* painter, NODE *node);
    
} PAINTER, * PAINTER_P;

extern PAINTER * create_painter(cairo_t *canvas);

#endif // PAINTER_H_INCLUDED