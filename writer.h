#ifndef WRITER_H_INCLUDED
#define WRITER_H_INCLUDED

#define TO_WRITER(writer) ((WRITER*)(writer))

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

typedef struct _WRITER {

    void (*destroy)(struct _WRITER * writer);

    xmlDocPtr document;
    xmlTextWriterPtr writer;

} WRITER, * WRITER_P;

extern WRITER * create_writer();

#endif // WRITER_H_INCLUDED