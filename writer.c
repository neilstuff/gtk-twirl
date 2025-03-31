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

#include "writer.h"


/**
 * @brief find the arc by a given point
 *
 */
void writer_arc_iterator(gpointer node, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST ARC_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, SOURCE_ATTRIBUTE, "%d", (int)TO_ARC(node)->source->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, TARGET_ATTRIBUTE, "%d", (int)TO_ARC(node)->source->id);;
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, WEIGHT_ATTRIBUTE, "%d", (int)TO_ARC(node)->weight);;
	xmlTextWriterEndElement(TO_WRITER(writer)->writer); 

}


/**
 * @brief find the arc by a given point
 *
 */
void writer_place_iterator(gpointer node, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST PLACE_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_ID_ATTRIBUTE, "%d", (int)TO_NODE(node)->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_NAME_ATTRIBUTE, "%s", TO_NODE(node)->name->str);
 
    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST GRAPHICS_ELEMENT);

    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, X_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.x);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, Y_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.y);

	xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Graphics
	xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Place

}

/**
 * @brief find the arc by a given point
 *
 */
void writer_transition_iterator(gpointer node, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST TRANSITION_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_ID_ATTRIBUTE, "%d", (int)TO_NODE(node)->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_NAME_ATTRIBUTE, "%s", TO_NODE(node)->name->str);
 
    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST GRAPHICS_ELEMENT);

    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, X_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.x);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, Y_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.y);

	xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Graphics
	xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Transition

}

/**
 * Free the writer resources
 *
 */
void writer_generate_nodes (WRITER * writer, NET * net)
{

    g_ptr_array_foreach(net->places,
        writer_place_iterator, writer);

    g_ptr_array_foreach(net->transitions,
            writer_transition_iterator, writer);
    
    
}

/**
 * Create a Writer
 *
 */
void writer_write(WRITER * writer, NET * net)
{
	xmlTextWriterStartDocument (writer->writer, NULL, ENCODING, NULL);

	xmlTextWriterStartElement (writer->writer, BAD_CAST NET_ELEMENT);

    writer_generate_nodes(writer, net);

	xmlTextWriterEndElement (writer->writer);
	xmlTextWriterEndElement (writer->writer);

	xmlTextWriterEndDocument (writer->writer);

    xmlFreeTextWriter (writer->writer);

}

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
    writer->write = writer_write;
    writer->writer = xmlNewTextWriterDoc(&writer->document, 0);

    return writer;

}