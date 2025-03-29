/**
 * @file writer.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief prototype - writes a net to a file or memory
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

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
void release_writer (WRITER * writer)
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

    writer->release = release_writer;
	writer->writer = xmlNewTextWriterDoc(&writer->document, 0);

    return writer;

}