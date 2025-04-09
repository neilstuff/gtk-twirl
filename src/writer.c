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
#include <libxml/xmlreader.h>
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
#include "reader.h"
#include "writer.h"
#include "controller.h"

#include "net.h"

#include "connector.h"
#include "mover.h"
#include "selector.h"

/**
 * @brief iterate through the vertices
 *
 */
void writer_vertex_iterator(gpointer arc, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST VERTEX_ELEMENT);

    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, X_ATTRIBUTE, "%d", (int)TO_VERTEX(arc)->point.x);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, Y_ATTRIBUTE, "%d", (int)TO_VERTEX(arc)->point.y);
 
    xmlTextWriterEndElement(TO_WRITER(writer)->writer);
    
}

/**
 * @brief iterate through the arcs
 *
 */
void writer_arc_iterator(gpointer arc, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST ARC_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, SOURCE_ATTRIBUTE, "%d", (int)TO_ARC(arc)->source->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, TARGET_ATTRIBUTE, "%d", (int)TO_ARC(arc)->source->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, WEIGHT_ATTRIBUTE, "%d", (int)TO_ARC(arc)->weight);

    g_ptr_array_foreach(TO_ARC(arc)->vertices,
        writer_vertex_iterator, writer);

    xmlTextWriterEndElement(TO_WRITER(writer)->writer);

}

/**
 * @brief iterate through the place
 *
 */
void writer_place_iterator(gpointer node, gpointer writer)
{

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST PLACE_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_ID_ATTRIBUTE, "%d", (int)TO_NODE(node)->id);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_NAME_ATTRIBUTE, "%s", TO_NODE(node)->name->str);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, TOKENS_ATTRIBUTE, "%d", (int)TO_PLACE(node).marked);

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST GRAPHICS_ELEMENT);

    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, X_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.x);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, Y_ATTRIBUTE, "%d", (int)TO_NODE(node)->position.y);

 
    xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Graphics
    xmlTextWriterEndElement(TO_WRITER(writer)->writer); // Place
}

/**
 * @brief iterate through the transitions
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
void writer_generate_nodes(WRITER *writer, NET *net)
{

    printf("writer_generate_nodes : ENTER\n");

    g_ptr_array_foreach(net->places,
                        writer_place_iterator, writer);

    g_ptr_array_foreach(net->transitions,
                        writer_transition_iterator, writer);

    g_ptr_array_foreach(net->arcs,
                        writer_arc_iterator, writer);

    printf("writer_generate_nodes : EXIT\n");
}

/**
 * Write out the NET
 *
 */
void writer_write(WRITER *writer, NET *net)
{
    xmlTextWriterStartDocument(writer->writer, NULL, ENCODING, NULL);

    xmlTextWriterStartElement(writer->writer, BAD_CAST NET_ELEMENT);

    writer_generate_nodes(writer, net);

    xmlTextWriterEndElement(writer->writer);

    xmlTextWriterEndDocument(writer->writer);

    xmlFreeTextWriter(writer->writer);

    xmlBufferPtr buf = xmlBufferCreate();

    xmlNodeDump(buf, NULL, xmlDocGetRootElement(writer->document), 1, 1);
    printf("%s", (char *)buf->content);

    xmlSaveFileEnc(writer->filename, writer->document, ENCODING);
}

/**
 * Free the writer resources
 *
 */
void release_writer(WRITER *writer)
{

    xmlFreeDoc(writer->document);

    g_free(writer);
}

/**
 * Create a Writer
 *
 */
WRITER *create_writer_from_file(char *filename)
{
    WRITER *writer = g_malloc(sizeof(WRITER));

    writer->release = release_writer;
    writer->write = writer_write;

    writer->filename = filename;

    writer->writer = xmlNewTextWriterDoc(&writer->document, 0);

    return writer;
}