#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "writer.h"
/**
 * Free the writer resources
 *
 */
void destroy_writer (WRITER * writer)
{

    xmlFreeDoc(writer->document);

    g_free (writer);

}

/**
 * Create a Writer
 *
 */
WRITER * create_writer ()
{
    WRITER * writer = g_malloc(sizeof(WRITER));

    writer->destroy = destroy_writer;
	writer->writer = xmlNewTextWriterDoc(&writer->document, 0);

    return writer;

}