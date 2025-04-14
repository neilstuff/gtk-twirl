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

#include "reader.h"

void reader_process_graphics(READER *reader, xmlNode *parent, NODE *node)
{

    if (parent->type == XML_ELEMENT_NODE && (strcmp(parent->name, GRAPHICS_ELEMENT) == 0))
    {
        xmlAttr *attribute = parent->properties;
        double x = 0;
        double y = 0;

        while (attribute && attribute->name && attribute->children)
        {
            xmlChar *value = xmlNodeListGetString(parent->doc, attribute->children, 1);

            if (strcmp(attribute->name, "x") == 0)
            {
                printf("Atributo %s: %s\n", attribute->name, value);
                sscanf(value, "%lf", &x);
            }

            if (strcmp(attribute->name, "y") == 0)
            {
                printf("Atributo %s: %s\n", attribute->name, value);
                sscanf(value, "%lf", &y);
            }

            xmlFree(value);

            attribute = attribute->next;
        }

        node->setPosition(node, x, y);
    }
}

/**
 * Process the vertex nodes
 *
 */
VERTEX * reader_process_vertix(READER *reader, enum POSITION position,  xmlNode *node)
{
    POINT point;

    set_point(&point, 0, 0);

    xmlAttr *attribute = node->properties;

    while (attribute && attribute->name && attribute->children)
    {
        xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);

        if (strcmp(attribute->name, "x") == 0) {

            point.x = atoi(value);

        }

        if (strcmp(attribute->name, "y") == 0) {

            point.y = atoi(value);

        }

        xmlFree(value);

        attribute = attribute->next;

    }

    return create_vertex(position, &point);

}

/**
 * Process all the vertices
 *
 */
void reader_process_vertices(READER *reader, ARC * arc, int nVertices, xmlNode *parent)
{
    xmlNode *node = NULL;
    VERTEX  *vertex = NULL; 
    int iVertex = 0;

    for (node = parent; node; node = node->next, iVertex += 1)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (strcmp(node->name, VERTEX_ELEMENT) == 0)
            {
                vertex = reader_process_vertix(reader, iVertex == 0 ? SOURCE_POSITION : 
                    iVertex == nVertices ? TARGET_POSITION : CONTROL_POSITION, node);

                arc->addVertex(arc, vertex);
            }

        }

    }

}

/**
 * @brief count the number vertices assigned to the arc
 * 
 */
int reader_count_vertices(READER *reader, xmlNode *parent)
{
    int counter = 0;
    xmlNode *node = NULL;

    for (node = parent; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (strcmp(node->name, VERTEX_ELEMENT) == 0)
            {
               counter += 1;
                
            }

        }
    }

    return counter;
}

/**
 * Process the transition nodes
 *
 */
void reader_process_arc(READER *reader, NET *net, xmlNode *node)
{
    xmlAttr *attribute = node->properties;
    ARC * arc = new_arc(net);

    while (attribute && attribute->name && attribute->children)
    {
        xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
   

        printf("Atributo %s: %s\n", attribute->name, value);

        if (strcmp(attribute->name, "weight") == 0) {

            arc->weight = atoi(value);

        }

        if (strcmp(attribute->name, "source") == 0) {

            arc->source = net->findNode(net, value);

        }

        if (strcmp(attribute->name, "target") == 0) {
            
            arc->target = net->findNode(net, value);

        }

        xmlFree(value);

        attribute = attribute->next;

    }

    reader_process_vertices(reader, arc, reader_count_vertices(reader, node->children), node->children);

    net->addArc(net, arc);

}

/**
 * Process the transition nodes
 *
 */
void reader_process_transition(READER *reader, NET *net, xmlNode *node)
{
    xmlAttr *attribute = node->properties;
    NODE *transition = create_node(TRANSITION_NODE, net);


    while (attribute && attribute->name && attribute->children)
    {
        xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);

        printf("Atributo %s: %s\n", attribute->name, value);

        if (strcmp(attribute->name, "name") == 0)
        {
            transition->setName(transition, value);
        }

        if (strcmp(attribute->name, "id") == 0)
        {
            transition->id = atoi(value);
        }

        xmlFree(value);

        attribute = attribute->next;
    }

    reader_process_graphics(reader, node->children, transition);

    net->addNode(net, transition);
}

/**
 * Process the place nodes
 *
 */
void reader_process_place(READER *reader, NET *net, xmlNode *node)
{

    printf("node type: Place, name: %s\n", node->name);

    xmlAttr *attribute = node->properties;
    NODE *place = create_node(PLACE_NODE, net);

    while (attribute && attribute->name && attribute->children)
    {
        xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);

        printf("Atributo %s: %s\n", attribute->name, value);

        if (strcmp(attribute->name, "name") == 0)
        {
            place->setName(place, value);
        }

        if (strcmp(attribute->name, "id") == 0)
        {
            place->id = atoi(value);
        }

        if (strcmp(attribute->name, "tokens") == 0)
        {
            place->place.marked = atoi(value);
        }

        xmlFree(value);

        attribute = attribute->next;
    }

    reader_process_graphics(reader, node->children, place);

    net->addNode(net, place);
}

/**
 * Process  the place nodes
 *
 */
void reader_process_elements(READER *reader, NET *net, xmlNode *parent)
{
    xmlNode *node = NULL;

    for (node = parent; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {

            if (strcmp(node->name, PLACE_ELEMENT) == 0)
            {
                reader_process_place(reader, net, node);
            }

            if (strcmp(node->name, TRANSITION_ELEMENT) == 0)
            {
                reader_process_transition(reader, net, node);
            }

            if (strcmp(node->name, ARC_ELEMENT) == 0)
            {
                reader_process_arc(reader, net, node);
            }
        }

        reader_process_elements(reader, net, node->children);
    }
}

/**
 * Free the reader resources
 *
 */
void reader_read(READER *reader, NET *net)
{

    xmlNode *root = xmlDocGetRootElement(reader->document);

    xmlBufferPtr buf = xmlBufferCreate();

    xmlNodeDump(buf, NULL, root, 1, 1);

    reader_process_elements(reader, net, root);

    printf("%s", (char *)buf->content);
}

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
    reader->read = reader_read;

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