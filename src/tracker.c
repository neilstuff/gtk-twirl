#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include "artifact.h"

#include "editor.h"
#include "drawer.h"
#include "reader.h"
#include "writer.h"

#include "event.h"
#include "handler.h"

#include "node.h"
#include "vertex.h"
#include "arc.h"

#include "editor.h"
#include "controller.h"

#include "net.h"

#include "connector.h"
#include "mover.h"
#include "selector.h"

#include "tracker.h"


/**
 * Free the writer resources
 *
 */
char * copy_to_clipboard(TRACKER *tracker, enum TYPE type, GPtrArray * artifacts)
{
    WRITER * writer = create_writer();


}

/**
 * Free the writer resources
 *
 */
char * snapshot(TRACKER *tracker, NET* net)
{
    WRITER * writer = create_writer();

    writer->write(writer, net);
}

/**
 * Free the writer resources
 *
 */
void tracker_release(TRACKER *tracker)
{

    g_free(tracker);
}

/**
 * Create a Writer
 *
 */
TRACKER *create_tracker()
{
    TRACKER *tracker = g_malloc(sizeof(TRACKER));

    tracker->release = tracker_release;

    tracker->history = g_ptr_array_new();

    return tracker;
}