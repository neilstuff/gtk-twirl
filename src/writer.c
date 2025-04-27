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
#include "container.h"

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
#include "tracker.h"
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
    char buffer[36];

    xmlTextWriterStartElement(TO_WRITER(writer)->writer, BAD_CAST ARC_ELEMENT);
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, SOURCE_ATTRIBUTE, "%s",
                                      TO_ARC(arc)->source->generate(TO_ARC(arc)->source, sizeof(buffer), buffer));
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, TARGET_ATTRIBUTE, "%s",
                                      TO_ARC(arc)->target->generate(TO_ARC(arc)->target, sizeof(buffer), buffer));
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
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_ALIGNMENT_ATTRIBUTE, "%d", TO_NODE(node)->alignment);
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
    xmlTextWriterWriteFormatAttribute(TO_WRITER(writer)->writer, NODE_ALIGNMENT_ATTRIBUTE, "%d", TO_NODE(node)->alignment);

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

    g_ptr_array_foreach(net->places,
                        writer_place_iterator, writer);

    g_ptr_array_foreach(net->transitions,
                        writer_transition_iterator, writer);

    g_ptr_array_foreach(net->arcs,
                        writer_arc_iterator, writer);
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
}

/**
 * @brief save to a file
 *
 */
char *writer_dump(WRITER *writer)
{
    if (writer->buffer != NULL) 
    {
        xmlBufferFree(writer->buffer);
    }

    writer->buffer = xmlBufferCreate();

    xmlNodeDump(writer->buffer, NULL, xmlDocGetRootElement(writer->document), 1, 1);
    printf("%s", (char *)writer->buffer->content);

    return writer->buffer->content;

}


/**
 * @brief write a container to a buffer
 *
 */
char *writer_snap(WRITER *writer, CONTAINER * container)
{

    printf("Write SNAP\n");

    if (writer->buffer != NULL) 
    {
        xmlBufferFree(writer->buffer);
    }

    xmlTextWriterStartDocument(writer->writer, NULL, ENCODING, NULL);

    xmlTextWriterStartElement(writer->writer, BAD_CAST SNIPPET_ELEMENT);

 
    g_ptr_array_foreach(container->places,
                        writer_place_iterator, writer);

    g_ptr_array_foreach(container->transitions,
                        writer_transition_iterator, writer);

    xmlTextWriterEndElement(writer->writer);

    xmlTextWriterEndDocument(writer->writer);
    
    writer->buffer = xmlBufferCreate();
                   
    xmlNodeDump(writer->buffer, NULL, xmlDocGetRootElement(writer->document), 1, 1);     
    printf("%s", (char *)writer->buffer->content);

    return (char *)writer->buffer->content;

}


/**
 * @brief save to a file
 *
 */
void writer_save(WRITER *writer, char *filename)
{

    if (writer->buffer != NULL) 
    {
        xmlBufferFree(writer->buffer);
    }

    writer->buffer = xmlBufferCreate();

    xmlNodeDump(writer->buffer, NULL, xmlDocGetRootElement(writer->document), 1, 1);
    printf("%s", (char *)writer->buffer->content);

    xmlSaveFileEnc(filename, writer->document, ENCODING);
}

/**
 * Free the writer resources
 *
 */
void release_writer(WRITER *writer)
{

    xmlFreeDoc(writer->document);

    if (writer->buffer != NULL) 
    {
        xmlBufferFree(writer->buffer);
    }

    g_free(writer);
}

/**
 * create and initalise a writer
 *
 */
WRITER *create_writer()
{
    WRITER *writer = g_malloc(sizeof(WRITER));

    writer->release = release_writer;
    writer->write = writer_write;
    writer->save = writer_save;
    writer->snap = writer_snap;

    writer->writer = xmlNewTextWriterDoc(&writer->document, 0);
    writer->buffer = NULL;

    return writer;
}
