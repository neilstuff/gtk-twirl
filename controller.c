#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "controller.h"
#include "event.h"
#include "handler.h"
#include "net.h"

void controller_handler_iterator(gpointer handler, gpointer event) {

  printf("im here b");
  TO_HANDLER(handler)->notify(TO_EVENT(event));
}

/**
 * @brief
 *
 * @param widget the Controller
 * @param user_data a pointer to the handler Object
 */
void controller_select_clicked(GtkButton *button, gpointer user_data) {
  EVENT *event = event_create_tool_selected(SELECT_TOOL);

  g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                      controller_handler_iterator, event);
}

/**
 * @brief Register a handle with the controller
 *
 * @param controller the contoller that manages the handlers
 * @param handler the handler to register
 */
void controller_register_handler(CONTROLLER *controller, HANDLER *handler) {

  g_ptr_array_add(controller->handlers, handler);
  
}

/**
 * @brief Releasse a Controller and free any resources
 *
 * @param controller the Controller to release
 */
void controller_release(CONTROLLER *controller) {

  g_ptr_array_unref(controller->handlers);

  g_free(controller);
}

/**
 * Create and initialise the controller
 *
 * @return an initialised controller
 *
 */
CONTROLLER *controller_create(GtkApplication *gtkAppication,
                              char *resourceURL) {
  CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

  controller->release = controller_release;
  controller->registerHandler = controller_register_handler;

  controller->handlers = g_ptr_array_new();

  GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

  controller->window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));

  gtk_window_set_application(GTK_WINDOW(controller->window),
                             GTK_APPLICATION(gtkAppication));

  controller->selectButton =
      GTK_WIDGET(gtk_builder_get_object(builder, "selectButton"));
  controller->placeButton =
      GTK_WIDGET(gtk_builder_get_object(builder, "placeButton"));
  controller->transitionButton =
      GTK_WIDGET(gtk_builder_get_object(builder, "transitionButton"));

  g_signal_connect(controller->selectButton, "clicked",
                   G_CALLBACK(controller_select_clicked), controller);

  g_object_unref(builder);

  controller->registerHandler(controller, handler_create());

  gtk_window_present(GTK_WINDOW(controller->window));

  return controller;
}