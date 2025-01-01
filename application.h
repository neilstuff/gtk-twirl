#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

/**
 * @brief Application Main Header 
 * 
 * 
 */

typedef struct _APPLICATION {

    GtkWidget *window;

} APPLICATION, * APPLICATION_P;

extern APPLICATION * application_create (GtkApplication * gtkAppication, char * resourceURL);

#endif // APPLICATION_H_INCLUDED