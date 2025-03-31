/**
 * @file reader.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief prototype - reads and reacreates a net
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
#include <libxml/xmlreader.h>

#include "artifact.h"
#include "editor.h"
#include "drawer.h"

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

#include "reader.h"

/**
 * Free the reader resources
 *
 */
void release_reader(READER *reader)
{

    xmlFreeDoc(reader->document);

    g_free(reader);
}

/**
 * Create a Reader - further initialisation required
 *
 * @return a newly created Reader
 *
 */
READER *new_reader()
{
    READER *reader = g_malloc(sizeof(READER));

    reader->release = release_reader;

    return reader;
}

/**
 * Create a Reader
 *
 * @param filename the filename the file to create
 *
 * @return a newly created Reader
 *
 */
READER *create_reader_from_file(char *filename)
{
    READER *reader = new_reader();

    reader->document = xmlReadFile(filename, NULL, 0);

    return reader;
}

/**
 * Create a Reader
 *
 * @param string the string that contains the xml
 *
 * @return a newly created Reader
 *
 */
READER *create_reader_from_string(GString *xml)
{
    READER *reader = new_reader();

    reader->document = xmlReadMemory(xml->str, xml->len, "twirl.xml", NULL, 0);

    return reader;
}
