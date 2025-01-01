
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "application.h"

/**
 * Create and initialise the Application
 *
 * @return an initialised Application
 *
 */
APPLICATION * application_create (GtkApplication * gtkAppication, char * resourceURL)
{
    APPLICATION *application = g_malloc(sizeof(APPLICATION));

    GtkBuilder *builder = gtk_builder_new_from_resource (resourceURL);

    application->window = GTK_WIDGET (gtk_builder_get_object (builder, "main"));

    gtk_window_set_application (GTK_WINDOW ( application->window), GTK_APPLICATION (gtkAppication));

    g_object_unref(builder);

    gtk_window_present(GTK_WINDOW (application->window));

    return application;

}

void application_destroy(APPLICATION  * application) 
{

    g_free(application);

}
