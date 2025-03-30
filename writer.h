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

#define PLACE_ELEMENT "place"
#define TRANSITION_ELEMENT "transition"
#define GRAPHICS_ELEMENT "graphics"
#define ARC_ELEMENT "arc"
#define NODE_NAME_ATTRIBUTE "name"

#define SOURCE_ATTRIBUTE "source"
#define TARGET_ATTRIBUTE "target"

#define TOKENS_ATTRIBUTE "tokens"
#define WEIGHT_ATTRIBUTE "tokens"
#define X_ATTRIBUTE "x"
#define Y_ATTRIBUTE "y"

typedef struct _WRITER {

    void (*release)(struct _WRITER * writer);

    xmlDocPtr document;
    xmlTextWriterPtr writer;

} WRITER, * WRITER_P;

extern WRITER * create_writer();

#endif // WRITER_H_INCLUDED