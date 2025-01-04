
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "handler.h"
#include "controller.h"
#include "net.h"

NET * controller_create_net(CONTROLLER * controller) 
{


}

/**
 * Create and initialise the Controller
 *
 * @return an initialised Controller
 *
 */
CONTROLLER * controller_create(GtkApplication * gtkAppication, char * resourceURL)
{
    CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

    GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

    controller->window = GTK_WIDGET(gtk_builder_get_object (builder, "main"));

    gtk_window_set_application(GTK_WINDOW (controller->window), GTK_APPLICATION (gtkAppication));

    controller->selectButton = GTK_WIDGET(gtk_builder_get_object (builder, "selectButton"));
    controller->placeButton = GTK_WIDGET(gtk_builder_get_object (builder, "placeButton"));
    controller->transitionButton = GTK_WIDGET(gtk_builder_get_object (builder, "transitionButton"));

    controller->processors = g_ptr_array_new ();

    g_object_unref(builder);

    gtk_window_present(GTK_WINDOW (controller->window));

    return controller;

}

/**
 * @brief Reealse a Controller and free any resources
 * 
 * @param controller the Controller to release
 */
void controller_release(CONTROLLER  * controller) 
{

    g_free(controller);

}
