#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

typedef struct _CONTROLLER 
{

    GtkWidget *window;

    GtkWidget *selectButton;
    GtkWidget *placeButton;
    GtkWidget *transitionButton;

    GPtrArray * processors;
    
} CONTROLLER, * CONTROLLER_P;

extern CONTROLLER * controller_create(GtkApplication * gtkAppication, char * resourceURL);
extern void release_controller(CONTROLLER  * controller);

#endif // CONTROLLER_H_INCLUDED