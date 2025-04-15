/**
 * @file editor.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief editor for all graphical object - places, transitions and arcs
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "editor.h"

const char *alignments[5] = {
    "top",
    "bottom",
    "left",
    "right",
    NULL
  };

/**
 * @brief private structure used to call functions with a general parameter container
 *
 */
typedef struct _LISTENER
{

    Handler handler;
    void *object;

    int id;

} LISTENER, *LISTENER_P;

void edit_on_entry_changed(GtkEditable *self, gpointer user_data)
{
    LISTENER *listener = (LISTENER *)user_data;

    listener->handler(listener->id, (void*)gtk_editable_get_text(self), listener->object);
}

void edit_on_value_changed(GtkSpinButton* self, gpointer user_data)
{
    LISTENER *listener = (LISTENER *)user_data;
    int value = (int) gtk_spin_button_get_value(self);

    listener->handler(listener->id, (void*)&value, listener->object);
    
}

void edit_on_alignment_changed(GtkDropDown *self, GParamSpec *pspec, gpointer user_data)
{
    LISTENER *listener = (LISTENER *)user_data;
    
    int value = (int) gtk_drop_down_get_selected(self);

    listener->handler(listener->id, (void*)&value, listener->object);
    
}

/**
 * @brief initialise the field editor
 *
 */
void editor_init(EDITOR *editor, void *object, Handler handler, enum FIELD field, ...)
{
    va_list args;
    va_start(args, field);

    while (field != END_FIELD)
    {
        switch (field)
        {
        case TEXT_FIELD:
        {

            GtkWidget *listBoxRow = gtk_list_box_row_new();
            GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
            int id = va_arg(args, int);

            GtkWidget *label = gtk_label_new(va_arg(args, char *));
            gtk_widget_set_size_request (label, 40, 32);

            gtk_label_set_xalign(GTK_LABEL(label), 0);
            GtkWidget *entry = gtk_entry_new();

            gtk_editable_set_text(GTK_EDITABLE(entry), va_arg(args, char *));
            gtk_widget_set_hexpand (label, 1);

            gtk_box_append(GTK_BOX(box), label);
            gtk_box_append(GTK_BOX(box), entry);

            gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(listBoxRow), box);

            gtk_list_box_append(editor->listBox, listBoxRow);

            {
                LISTENER *listener = g_malloc(sizeof(LISTENER));

                listener->id = id;
                listener->object = object;
                listener->handler = handler;

                g_signal_connect(GTK_EDITABLE(entry), "changed", G_CALLBACK(edit_on_entry_changed), listener);
            }
        }
        break;
        case SPIN_BUTTON:
        {

            GtkWidget *listBoxRow = gtk_list_box_row_new();
            GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
            int id = va_arg(args, int);

            GtkWidget *label = gtk_label_new(va_arg(args, char *));
            gtk_widget_set_size_request (label, 40, 32);

            gtk_label_set_xalign(GTK_LABEL(label), 0);

            GtkAdjustment *adjustment = gtk_adjustment_new ((double)va_arg(args, int), 0.0, 1000, 1, 0.1, 0.0);

            GtkWidget *button = gtk_spin_button_new (adjustment, 1.0, 0);    
            gtk_widget_set_hexpand (button, 1);  

            gtk_box_append(GTK_BOX(box), label);
            gtk_box_append(GTK_BOX(box), button);

            gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(listBoxRow), box);

            gtk_list_box_append(editor->listBox, listBoxRow);

            {
                LISTENER *listener = g_malloc(sizeof(LISTENER));

                listener->id = id;
                listener->object = object;
                listener->handler = handler;

                g_signal_connect(GTK_SPIN_BUTTON(button), "value_changed", G_CALLBACK(edit_on_value_changed), listener);
            }
        }
        break;
        case ALIGNMENT_BOX:
        {
            GtkWidget *listBoxRow = gtk_list_box_row_new();
            GtkWidget* comboBox = gtk_drop_down_new_from_strings (alignments);
            GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

            int id = va_arg(args, int);

            GtkWidget *label = gtk_label_new(va_arg(args, char *));
            gtk_drop_down_set_selected (GTK_DROP_DOWN(comboBox), va_arg(args, int)); 

            gtk_widget_set_size_request (label, 40, 32);

            gtk_label_set_xalign(GTK_LABEL(label), 0);

            gtk_widget_set_hexpand (comboBox, 1);  

            gtk_box_append(GTK_BOX(box), label);
            gtk_box_append(GTK_BOX(box), comboBox);
            
            gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(listBoxRow), box);

            gtk_list_box_append(editor->listBox, listBoxRow);
           
            {
                LISTENER *listener = g_malloc(sizeof(LISTENER));

  
                listener->id = id;

                listener->object = object;
                listener->handler = handler;

                g_signal_connect(GTK_DROP_DOWN(comboBox), "notify::selected-item", G_CALLBACK(edit_on_alignment_changed), listener);
            }

        }
        break;
        }

        field = va_arg(args, enum FIELD);
    }

}

/**
 * @brief deallocate an editor's storage
 *
 */
void editor_release(EDITOR *editor)
{
    g_free(editor);
}

/**
 * @brief event constructor - based on the notification type
 *
 */
EDITOR *create_editor(GtkListBox *listBox)
{

    EDITOR *editor = g_malloc(sizeof(EDITOR));

    editor->listBox = listBox;

    editor->release = editor_release;
    editor->init = editor_init;
}