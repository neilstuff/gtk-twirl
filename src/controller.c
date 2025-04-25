/**
 * @file controller.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  the interface between the GDK event processing system and petri-net's implementation
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"
#include "container.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "vertex.h"
#include "node.h"
#include "arc.h"

#include "event.h"
#include "handler.h"

#include "controller.h"
#include "net.h"
#include "tracker.h"

/**
 * @brief iterates through the handlers for a specific event
 *
 */
void controller_handler_iterator(gpointer handler, gpointer event)
{

    TO_HANDLER(handler)->handler(event, TO_HANDLER(handler)->processor);
}

/**
 * @brief notify the event handlers with the event
 *
 */
void controller_notify(CONTROLLER *controller, EVENT *event)
{
    g_ptr_array_foreach(controller->handlers,
                        controller_handler_iterator, event);
}

/**
 * @brief send an event to the controller to process
 *
 */
void controller_send(CONTROLLER *controller, EVENT *event)
{

    switch (event->notification)
    {
    case ACTIVATE_TOOLBAR:
    {
        gtk_widget_set_sensitive(controller->saveToolbarButton, event->events.activate_toolbar.activate);
        gtk_widget_set_sensitive(controller->saveAsToolbarButton, event->events.activate_toolbar.activate);
    }
    break;

    case ACTIVATE_DELETE:
    {
        gtk_widget_set_sensitive(controller->deleteToolbarButton, event->events.activate_toolbar.activate);
        gtk_widget_set_sensitive(controller->cutToolbarButton, event->events.activate_toolbar.activate);
    }
    break;

    case SET_VIEW_SIZE:
    {
        gtk_widget_set_size_request(controller->drawingArea, event->events.set_view_size.size.w + 64,
                                    event->events.set_view_size.size.h + 64);
    }
    break;
    };
}

/**
 * @brief send an event to the controller to process
 *
 */
void controller_message(CONTROLLER *controller, enum NOTIFICATION notification)
{

    switch (notification)
    {
    case CLEAR_EDITOR:
    {
        gtk_list_box_remove_all(controller->fieldEditor);
    }
    break;
    }
}

/**
 * @brief manage the 'draw' event
 *
 */
static void controller_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height,
                            gpointer user_data)
{
    EVENT *event = create_event(DRAW_REQUESTED, cr, width, height);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief 'select' tool selected
 *
 */
void controller_select_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(TOOL_SELECTED, SELECT_TOOL);
    GdkCursor *cursor = gdk_cursor_new_from_name("default", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief 'place' tool selected
 *
 */
void controller_place_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(TOOL_SELECTED, PLACE_TOOL);
    GdkCursor *cursor = gdk_cursor_new_from_name("copy", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief 'transition' tool selected
 *
 */
void controller_transition_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(TOOL_SELECTED, TRANSITION_TOOL);

    GdkCursor *cursor = gdk_cursor_new_from_name("copy", NULL);

    gtk_widget_set_cursor(TO_CONTROLLER(user_data)->scrolledWindow, cursor);

    controller_notify(TO_CONTROLLER(user_data), event);
}

void controller_open(GObject *source_object, GAsyncResult *res, gpointer data)
{
    GError *error = NULL;

    GFile *file = gtk_file_dialog_open_finish((GtkFileDialog *)source_object, res, &error);

    if (file != NULL)
    {
        char *path = g_file_get_path(file);

        EVENT *event = create_event(READ_NET, create_reader_from_file(path), path);

        controller_notify(TO_CONTROLLER(data), event);
    }
}

void controller_save(GObject *source_object, GAsyncResult *res, gpointer data)
{
    GError *error = NULL;

    GFile *file = gtk_file_dialog_save_finish((GtkFileDialog *)source_object, res, &error);

    if (file != NULL)
    {
        char *path = g_file_get_path(file);
        WRITER *writer = create_writer();

        EVENT *event = create_event(WRITE_NET, writer, path);

        controller_notify(TO_CONTROLLER(data), event);
    }
}

/**
 * @brief 'new' toolbar button selected
 *
 */
void controller_new_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(CLEAR_NET);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief 'open' toolbar button selected
 *
 */
void controller_open_clicked(GtkButton *button, gpointer user_data)
{

    GtkFileDialog *filedialog = gtk_file_dialog_new();

    GtkFileFilter *filefilter = gtk_file_filter_new();
    gtk_file_filter_add_suffix(filefilter, "xml");
    gtk_file_filter_set_name(filefilter, "XML File");

    GListStore *liststore = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(liststore, filefilter);

    gtk_file_dialog_set_filters(filedialog, G_LIST_MODEL(liststore));

    gtk_file_dialog_open(filedialog, GTK_WINDOW(TO_CONTROLLER(user_data)->window), NULL, controller_open, user_data);
}

/**
 * @brief 'save' toolbar selected
 *
 */
void controller_save_clicked(GtkButton *button, gpointer user_data)
{

    GtkFileDialog *filedialog = gtk_file_dialog_new();

    GtkFileFilter *filefilter = gtk_file_filter_new();
    gtk_file_filter_add_suffix(filefilter, "xml");
    gtk_file_filter_set_name(filefilter, "XML File");

    GListStore *liststore = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(liststore, filefilter);

    gtk_file_dialog_set_filters(filedialog, G_LIST_MODEL(liststore));

    gtk_file_dialog_save(filedialog, GTK_WINDOW(TO_CONTROLLER(user_data)->window), NULL, controller_save, user_data);
}

/**
 * @brief delete tool selected
 *
 */
void controller_delete_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(DELETE_SELECTED);

    controller_notify(TO_CONTROLLER(user_data), event);
}


/**
 * @brief cut tool selected
 *
 */
void controller_cut_clicked(GtkButton *button, gpointer user_data)
{
    EVENT *event = create_event(CUT_SELECTED);

    controller_notify(TO_CONTROLLER(user_data), event);
}


/**
 * @brief 'release' gesture processing
 *
 */
void controller_gesture_released(GtkGestureClick *gesture,
                                 int n_press,
                                 double x,
                                 double y,
                                 gpointer user_data)
{

    if (TO_CONTROLLER(user_data)->mode != FINALISE)
    {
        EVENT *event = create_event(CREATE_NODE, n_press, x, y);

        controller_notify(TO_CONTROLLER(user_data), event);
    }
    else
    {
        TO_CONTROLLER(user_data)->mode = NORMAL;
    }
}

/**
 * @brief capture the 'control' key
 *
 */
gboolean controller_key_pressed(GtkEventControllerKey *self,
                                guint keyval, guint keycode,
                                GdkModifierType state,
                                gpointer user_data)
{

    if (state & (GDK_CONTROL_MASK))
    {
        TO_CONTROLLER(user_data)->mode = CONNECT;
    }

    if (state & (GDK_SHIFT_MASK))
    {
        TO_CONTROLLER(user_data)->mode = MOVE;
    }

    return TRUE;
}

/**
 *  @brief release the 'control' key
 *
 */
void controller_key_released(GtkEventControllerKey *self,
                             guint keyval, guint keycode,
                             GdkModifierType state,
                             gpointer user_data)
{

    TO_CONTROLLER(user_data)->mode = NORMAL;
}

/**
 * @brief begin the drag and notify hanlders
 *
 */
void controller_drag_begin(GtkGestureDrag *gesture, double x, double y, gpointer user_data)
{
    EVENT *event = create_event(START_DRAG, x, y, TO_CONTROLLER(user_data)->mode);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief update the drag and notify hanlders
 *
 */
void controller_drag_update(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data)
{
    EVENT *event = create_event(UPDATE_DRAG, offset_x, offset_y, TO_CONTROLLER(user_data)->mode);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief end the drag and notify hanlders
 *
 */
void controller_drag_end(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data)
{
    EVENT *event = create_event(END_DRAG, offset_x, offset_y, TO_CONTROLLER(user_data)->mode);

    controller_notify(TO_CONTROLLER(user_data), event);
}

/**
 * @brief Redraw the Drawing Area
 *
 */
void controller_redraw(CONTROLLER *controller)
{
    gtk_widget_queue_draw(controller->drawingArea);
}

/**
 * @brief register an event handler with the controller
 *
 */
void controller_monitor(CONTROLLER *controller, HANDLER *handler)
{
    g_ptr_array_add(controller->handlers, handler);
}

/**
 * @brief register an event handler with the controller
 *
 */
void controller_unmonitor(CONTROLLER *controller, HANDLER *handler)
{
    g_ptr_array_remove(controller->handlers, handler);
}

/**
 * @brief get a field editor
 */
EDITOR *controller_edit(CONTROLLER *controller)
{
    return create_editor(controller->fieldEditor);
}

/**
 * @brief release the controller and free any resources
 *
 */
void controller_release(CONTROLLER *controller)
{

    g_ptr_array_unref(controller->handlers);

    g_free(controller);
}

/**
 * @brief create and initialise the controller
 *
 */
CONTROLLER *create_controller(GtkApplication *gtkAppication,
                              char *resourceURL)
{
    CONTROLLER *controller = g_malloc(sizeof(CONTROLLER));

    {
        controller->mode = NORMAL;

        controller->release = controller_release;
        controller->monitor = controller_monitor;
        controller->unmonitor = controller_unmonitor;
        controller->redraw = controller_redraw;
        controller->notify = controller_notify;
        controller->send = controller_send;
        controller->message = controller_message;
        controller->edit = controller_edit;

        controller->handlers = g_ptr_array_new();
    }
    {
        GtkBuilder *builder = gtk_builder_new_from_resource(resourceURL);

        controller->window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));

        controller->scrolledWindow =
            GTK_WIDGET(gtk_builder_get_object(builder, "scrolledWindow"));
        controller->drawingArea =
            GTK_WIDGET(gtk_builder_get_object(builder, "drawingArea"));
        controller->viewPort =
            GTK_WIDGET(gtk_builder_get_object(builder, "viewPort"));

        controller->fieldEditor =
            GTK_LIST_BOX(gtk_builder_get_object(builder, "fieldEditor"));

        gtk_window_set_application(GTK_WINDOW(controller->window),
                                   GTK_APPLICATION(gtkAppication));

        controller->selectButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "selectButton"));
        controller->placeButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "placeButton"));
        controller->transitionButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "transitionButton"));

        controller->newToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "newToolbarButton"));
        controller->openToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "openToolbarButton"));
        controller->saveToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "saveToolbarButton"));
        controller->saveAsToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "saveAsToolbarButton"));
        controller->deleteToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "deleteToolbarButton"));

        controller->undoToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "undoToolbarButton"));
        controller->redoToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "redoToolbarButton"));
   
        controller->cutToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "cutToolbarButton"));
        controller->copyToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "copyToolbarButton"));
        controller->pasteToolbarButton =
            GTK_WIDGET(gtk_builder_get_object(builder, "pasteToolbarButton"));
    }
    {
        g_signal_connect(controller->selectButton, "clicked",
                         G_CALLBACK(controller_select_clicked), controller);

        g_signal_connect(controller->placeButton, "clicked",
                         G_CALLBACK(controller_place_clicked), controller);

        g_signal_connect(controller->transitionButton, "clicked",
                         G_CALLBACK(controller_transition_clicked), controller);

        g_signal_connect(controller->newToolbarButton, "clicked",
                         G_CALLBACK(controller_new_clicked), controller);

        g_signal_connect(controller->openToolbarButton, "clicked",
                         G_CALLBACK(controller_open_clicked), controller);

        g_signal_connect(controller->saveToolbarButton, "clicked",
                         G_CALLBACK(controller_save_clicked), controller);

        g_signal_connect(controller->deleteToolbarButton, "clicked",
                         G_CALLBACK(controller_delete_clicked), controller);

        g_signal_connect(controller->cutToolbarButton, "clicked",
                            G_CALLBACK(controller_cut_clicked), controller);

        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(controller->drawingArea), controller_draw, controller,
                                       NULL);

        controller->click = gtk_gesture_click_new();
        g_signal_connect(controller->click, "released",
                         G_CALLBACK(controller_gesture_released),
                         controller);

        gtk_widget_add_controller(controller->drawingArea, GTK_EVENT_CONTROLLER(controller->click));
    }
    /* Initialise the Net */
    {
        NET *net = net_create(controller);
        EVENT *event = create_event(CREATE_NET, SELECT_TOOL);

        gtk_toggle_button_set_active((GtkToggleButton *)controller->selectButton, TRUE);

        controller->monitor(controller, &net->handler);
        net->processors[event->notification](net, event);
    }

    {
        controller->keyController = gtk_event_controller_key_new();

        g_signal_connect(controller->keyController, "key-pressed",
                         G_CALLBACK(controller_key_pressed),
                         controller);

        g_signal_connect(controller->keyController, "key-released",
                         G_CALLBACK(controller_key_released),
                         controller);

        gtk_widget_add_controller(controller->window, GTK_EVENT_CONTROLLER(controller->keyController));
    }
    {
        controller->drag = gtk_gesture_drag_new();
        g_signal_connect(controller->drag, "drag-begin", G_CALLBACK(controller_drag_begin), controller);
        g_signal_connect(controller->drag, "drag-update", G_CALLBACK(controller_drag_update), controller);
        g_signal_connect(controller->drag, "drag-end", G_CALLBACK(controller_drag_end), controller);

        gtk_widget_add_controller(controller->drawingArea, GTK_EVENT_CONTROLLER(controller->drag));
    }

    gtk_window_present(GTK_WINDOW(controller->window));

    return controller;
}