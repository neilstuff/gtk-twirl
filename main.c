/**
 * @file main.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief The main class - it all starts here...
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

/*! \mainpage Twirl Overview
 *
 * \section Introduction
 *
 * TWIRL is an example petri-net implementation.  There are 3 main artifacts which are: 
 * \li \c place (represents a state); 
 * \li \c transition (represent a process); and
 * \li \c arc connects place to a transition or a transition to a place 
 *
 */
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "node.h"

#include "event.h"
#include "handler.h"
#include "editor.h"
#include "controller.h"

CONTROLLER *contoller;

/**
 * @brief establishes the controller
 *
 */
static void on_activate(GtkApplication *app, gpointer user_data)
{
    contoller = create_controller(app, "/org/twirl/twirl.ui");
}

/**
 * @brief activate the application
 *
 */
static void on_open(GtkApplication *app, GFile **files, gint n_files,
                    gchar *hint, gpointer user_data)
{
    on_activate(app, user_data);
}

/**
 * @brief the main section
 *
 */
int main(int argc, char *argv[])
{
    GtkApplication *app;
    int stat;

    app = gtk_application_new("org.brittliff.twirl", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    g_signal_connect(app, "open", G_CALLBACK(on_open), NULL);
    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return stat;
}