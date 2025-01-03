#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

/**
 * @brief Controller Main Header 
 * 
 * 
 */

typedef struct _CONTROLLER {

    GtkWidget *window;

    GtkWidget *selectButton;
    GtkWidget *placeButton;
    GtkWidget *transitionButton;
    
} CONTROLLER, * CONTROLLER_P;

extern CONTROLLER * create_controller(GtkApplication * gtkAppication, char * resourceURL);

#endif // CONTROLLER_H_INCLUDED