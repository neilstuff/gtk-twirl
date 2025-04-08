/**
 * Reader - prototype
 *
 * Reader out the PNML to a document
 *
 * @author Neil Brittliff
 *
 * (c) Neil Brittliff - all rights reserved
 *
 */

 #ifndef READER_H_INCLUDED
 #define READER_H_INCLUDED

 typedef struct _READER {

    void (*read)(struct _READER * reader, struct _NET * net);
    void (*release)(struct _READER * reader);

    xmlDocPtr  document;

} READER, * READER_P;

extern READER * create_reader_from_file (char * filename);
extern READER * create_reader_from_string (GString * xml);

#endif // READER_H_INCLUDED