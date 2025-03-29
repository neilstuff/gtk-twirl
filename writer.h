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

typedef struct _WRITER {

    void (*release)(struct _WRITER * writer);

    xmlDocPtr document;
    xmlTextWriterPtr writer;

} WRITER, * WRITER_P;

extern WRITER * create_writer();

#endif // WRITER_H_INCLUDED