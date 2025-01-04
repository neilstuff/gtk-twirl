#include <windows.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "controller.h"

CONTROLLER * contoller;

static void on_activate(GtkApplication *app, gpointer user_data)
{
    contoller = controller_create(app, "/org/twirl/twirl.ui");
}

static void on_open (GtkApplication *app, GFile **files, gint n_files, gchar *hint, gpointer user_data) {
    on_activate(app, user_data);
}

/**
 * Main Menu
 *
 * @param arc The number of arguments
 * @param argv The actual arguments
 *
 */
int main(int argc, char *argv[])
{
    GtkApplication *app;
    int stat;

    app = gtk_application_new ("org.brittliff.twirl", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    g_signal_connect (app, "open", G_CALLBACK (on_open), NULL);
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    
    return stat;
}