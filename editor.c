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
EDITOR *create_editor(GtkListBox *listBox, ...) 
{

    EDITOR *editor = g_malloc(sizeof(EDITOR));

    editor->release = editor_release;

    gtk_list_box_remove_all(listBox);

}