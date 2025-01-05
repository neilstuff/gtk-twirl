#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "handler.h"
#include "controller.h"
#include "net.h"

/**
 * @brief 
 *
 * @param widget the Controller
 * @param user_data a pointer to the handler Object
 */
void controller_select_clicked(GtkButton *button, gpointer user_data)
{

}

/**
 * @brief Register a handle with the controller
 *
 * @param controller the contoller that manages the handlers
 * @param handler the handler to register
 */
void controller_register_handler(CONTROLLER  * controller, HANDLER * handler)
{

    g_ptr_array_add(controller->processors, handler);

}

/**
 * @brief Releasse a Controller and free any resources
 * 
 * @param controller the Controller to release
 */
void controller_release(CONTROLLER  * controller) 
{

    g_ptr_array_unref(controller->processors);

    g_free(controller);

}

/**
 * Create and initialise the controller
 *
 * @return an initialised controller
 *
 */
CONTROLLER * controller_create(GtkApplication * gtkAppication, char * resourceURL)
{
    CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

    controller->release = controller_release;
    controller->registerHandler = controller_register_handler;

    controller->processors = g_ptr_array_new ();

    GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

    controller->window = GTK_WIDGET(gtk_builder_get_object (builder, "main"));

    gtk_window_set_application(GTK_WINDOW (controller->window), GTK_APPLICATION (gtkAppication));

    controller->selectButton = GTK_WIDGET(gtk_builder_get_object (builder, "selectButton"));
    controller->placeButton = GTK_WIDGET(gtk_builder_get_object (builder, "placeButton"));
    controller->transitionButton = GTK_WIDGET(gtk_builder_get_object (builder, "transitionButton"));

 
    g_signal_connect (controller->selectButton, "clicked", G_CALLBACK (controller_select_clicked), controller);
 
    g_object_unref(builder);

    gtk_window_present(GTK_WINDOW (controller->window));

    return controller;

}