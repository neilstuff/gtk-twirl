#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#define TO_CONTROLLER(controller) ((CONTROLLER*)(controller))

typedef struct _CONTROLLER 
{

    GtkWidget *window;
    GtkWidget *scrolledWindow;
    GtkWidget *drawingArea;
    GtkGesture *gesture;

    GtkWidget *selectButton;
    GtkWidget *placeButton;
    GtkWidget *transitionButton;

    GPtrArray * handlers;

   void (*monitor) (struct _CONTROLLER * controller, void * net);
   void (*release) (struct _CONTROLLER * controller);
    
} CONTROLLER, * CONTROLLER_P;

extern CONTROLLER * create_controller(GtkApplication * gtkAppication, char * resourceURL);

#endif // CONTROLLER_H_INCLUDED