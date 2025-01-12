#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "controller.h"
#include "event.h"
#include "net.h"

void controller_handler_iterator(gpointer net, gpointer event)
{

    printf("Notification Received: %d\n", TO_EVENT(event)->notification);

    TO_NET(net)->processors[TO_EVENT(event)->notification](TO_NET(net),  TO_EVENT(event));

}

/**
 * @brief Manage the Draw Event
 *
 * @param area the Widget - Drawing Area
 * @param cr the Cairo context
 * @param width the width of the drawing Area
 * @param height the height of the drawing Area
 */
static void controller_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height,
                            gpointer user_data)
{
    EVENT *event = create_draw_event(cr, width, height);


    g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                        controller_handler_iterator, event);
}

/**
 * @brief Select Tool Clicked
 *
 * @param button the Select Tool Button
 * @param user_data a pointer to the Controller
 */
void controller_select_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_tool_selected_event(SELECT_TOOL);
    GdkCursor *cursor = gdk_cursor_new_from_name("default", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                        controller_handler_iterator, event);
}

/**
 * @brief Place Tool Clicked
 *
 * @param button the Place Tool Button
 * @param user_data a pointer to the Controller
 */
void controller_place_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_tool_selected_event(PLACE_TOOL);
    GdkCursor *cursor = gdk_cursor_new_from_name("copy", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                        controller_handler_iterator, event);
}

/**
 * @brief Transition Tool Clicked
 *
 * @param button the Place Tool Button
 * @param user_data a pointer to the Controller
 */
void controller_transition_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_tool_selected_event(TRANSITION_TOOL);

    GdkCursor *cursor = gdk_cursor_new_from_name("copy", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                        controller_handler_iterator, event);
}

/**
 * @brief Release gesture processing
 *
 * @param gesture The release gesture
 * @param n_press what button was pressed
 * @param x the 'x' coordinate
 * @param y the 'y' coordinate
 * @param user_data the Controller
 */
void controller_gesture_released(GtkGestureClick *gesture,
                                 int n_press,
                                 double x,
                                 double y,
                                 gpointer user_data)
{

     EVENT *event = create_node_event(n_press, x, y);
    
    g_ptr_array_foreach(TO_CONTROLLER(user_data)->handlers,
                        controller_handler_iterator, event);

}

/**
 * @brief Redraw the Drawing Area
 * 
 * @param controller the Controller
 */
void controller_redraw(CONTROLLER *controller) 
{

    gtk_widget_queue_draw(controller->drawingArea);

}

/**
 * @brief Register a handle with the controller
 *
 * @param controller the contoller that manages the handlers
 * @param net the net to register
 */
void controller_monitor(CONTROLLER *controller, void *net)
{

    g_ptr_array_add(controller->handlers, net);

}

/**
 * @brief Releasse a Controller and free any resources
 *
 * @param controller the Controller to release
 */
void controller_release(CONTROLLER *controller)
{

    g_ptr_array_unref(controller->handlers);

    g_free(controller);
}

/**
 * Create and initialise the controller
 *
 * @return an initialised controller
 *
 */
CONTROLLER *create_controller(GtkApplication *gtkAppication,
                              char *resourceURL)
{
    CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

    controller->release = controller_release;
    controller->monitor = controller_monitor;
    controller->redraw = controller_redraw;

    controller->handlers = g_ptr_array_new();

    GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

    controller->window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    controller->scrolledWindow =
        GTK_WIDGET(gtk_builder_get_object(builder, "scrolledWindow"));
    controller->drawingArea =
        GTK_WIDGET(gtk_builder_get_object(builder, "drawingArea"));

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

    g_signal_connect(controller->placeButton, "clicked",
                     G_CALLBACK(controller_place_clicked), controller);

    g_signal_connect(controller->transitionButton, "clicked",
                     G_CALLBACK(controller_transition_clicked), controller);

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(controller->drawingArea), controller_draw, controller,
                                   NULL);
    controller->gesture = gtk_gesture_click_new();

    g_signal_connect(controller->gesture, "released",
                     G_CALLBACK(controller_gesture_released),
                     controller);

    gtk_widget_add_controller(controller->drawingArea, GTK_EVENT_CONTROLLER(controller->gesture));

    controller->monitor(controller, net_create(controller));

    gtk_window_present(GTK_WINDOW(controller->window));

    return controller;

}