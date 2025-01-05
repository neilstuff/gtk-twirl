#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

typedef struct _CONTROLLER 
{

    GtkWidget *window;

    GtkWidget *selectButton;
    GtkWidget *placeButton;
    GtkWidget *transitionButton;

    GPtrArray * processors;

   void (*registerHandler)(struct _CONTROLLER * controller, HANDLER * handler);
   void (*release) (struct _CONTROLLER * controller);
    
} CONTROLLER, * CONTROLLER_P;

extern CONTROLLER * controller_create(GtkApplication * gtkAppication, char * resourceURL);

#endif // CONTROLLER_H_INCLUDED