/**
 * Writer - prototype
 *
 * Writes out the PNML to a document
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

#ifndef WRITER_H_INCLUDED
#define WRITER_H_INCLUDED

#define TO_WRITER(writer) ((WRITER*)(writer))

#define ENCODING "ISO-8859-1"

#define NET_ELEMENT "net"

#define PLACE_ELEMENT "place"
#define TRANSITION_ELEMENT "transition"
#define GRAPHICS_ELEMENT "graphics"
#define ARC_ELEMENT "arc"
#define VERTEX_ELEMENT "vertex"

#define NODE_NAME_ATTRIBUTE "name"
#define NODE_ALIGNMENT_ATTRIBUTE "alignment"

#define SOURCE_ATTRIBUTE "source"
#define TARGET_ATTRIBUTE "target"

#define SOURCE_ATTRIBUTE "source"
#define TARGET_ATTRIBUTE "target"


#define TOKENS_ATTRIBUTE "tokens"
#define WEIGHT_ATTRIBUTE "weight"
#define X_ATTRIBUTE "x"
#define Y_ATTRIBUTE "y"

typedef struct _WRITER {

    void (*write)(struct _WRITER * writer, struct _NET * net);
    void (*save)(struct _WRITER * writer, char * filename);
    void (*release)(struct _WRITER * writer);

    xmlDocPtr document;
    xmlBufferPtr buffer;
    xmlTextWriterPtr writer;

} WRITER, * WRITER_P;

extern WRITER * create_writer();
extern WRITER * create_writer_from_file(char * file);

#endif // WRITER_H_INCLUDED