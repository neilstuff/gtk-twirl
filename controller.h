#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#define TO_CONTROLLER(controller) ((CONTROLLER*)(controller))

enum MODES
{
    NORMAL = 0,
    CONNECT,
    END_MODES
    
};

typedef struct _CONTROLLER 
{

    GtkWidget *window;
    GtkWidget *scrolledWindow;
    GtkWidget *drawingArea;
    GtkGesture *gesture;
    GtkEventController * keyController;

    GtkWidget *selectButton;
    GtkWidget *placeButton;
    GtkWidget *transitionButton;

    GPtrArray * handlers;

    enum MODES mode;

   void (*monitor) (struct _CONTROLLER * controller, void * net);  
   void (*redraw) (struct _CONTROLLER * controller);

   void (*release) (struct _CONTROLLER * controller);
    
} CONTROLLER, * CONTROLLER_P;

extern CONTROLLER * create_controller(GtkApplication * gtkAppication, char * resourceURL);

#endif // CONTROLLER_H_INCLUDED