
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "CONTROLLER.h"

/**
 * Create and initialise the Controller
 *
 * @return an initialised Controller
 *
 */
CONTROLLER * create_controller(GtkApplication * gtkAppication, char * resourceURL)
{
    CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

    GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

    controller->window = GTK_WIDGET(gtk_builder_get_object (builder, "main"));

    gtk_window_set_application(GTK_WINDOW (controller->window), GTK_APPLICATION (gtkAppication));

    g_object_unref(builder);

    gtk_window_present(GTK_WINDOW (controller->window));

    return controller;

}

void destroy_controller(CONTROLLER  * controller) 
{

    g_free(controller);

}
