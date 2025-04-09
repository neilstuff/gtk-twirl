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
                sscanf(value, "%lf", &x);
            }

            if (strcmp(attribute->name, "y") == 0)
            {
                sscanf(value, "%lf", &y);
            }

            xmlFree(value);

        }
        
        node->setPosition(node, x, y);
    }

}

/**
 * Process  the place nodes
 *
 */
void reader_process_place(READER *reader, NET *net, xmlNode *node)
{

    printf("node type: Place, name: %s\n", node->name);

    xmlAttr *attribute = node->properties;
    NODE * place = create_node(PLACE_NODE, net);

    while (attribute && attribute->name && attribute->children)
    {
        xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
       
        printf("Atributo %s: %s\n", attribute->name, value);

        if (strcmp(attribute->name, "name") == 0)
        {
            place->setName(place, value);
        }
       
        if  (strcmp(attribute->name, "id") == 0)
        {
            place->id = atoi(value);
        }

        if  (strcmp(attribute->name, "tokens") == 0)
        {
            place->place.marked = atoi(value);
        }

        xmlFree(value);
        
        attribute = attribute->next;
    }

}

/**
 * Process  the place nodes
 *
 */
void reader_process_nodes(READER *reader, NET *net, xmlNode *parent)
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
        }

        reader_process_nodes(reader, net, node->children);
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

    reader_process_nodes(reader, net, root);

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