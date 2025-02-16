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
#include "editor.h"

/**
 * @brief initialise the field editor
 *
 */
void editor_init(EDITOR *editor, enum FIELD field, ...)
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
            GtkWidget *entry = gtk_entry_new();

            gtk_entry_set_placeholder_text(GTK_ENTRY(entry), va_arg(args, char *));
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

    gtk_list_box_remove_all(listBox);
}