/**
 * @file editor.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief prototype - editor for all graphical object - places, transitions and arcs
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

/**
 * @brief user selected tool from the tool pane; can be either - 'select', 'place', and 'transition'
 * 
 */
enum FIELD
{
    TEXT_FIELD,
    SPIN_BUTTON,
    ALIGNMENT_BOX,
    END_FIELD
};

typedef void (*Handler)(int id, void * value, void * object);

/**
 * @brief an editor structure 
 * 
 */
typedef struct _EDITOR
{

    void (*init)(struct _EDITOR *editor, void* object, Handler handler, enum FIELD field, ...);

    void (*release)(struct _EDITOR *editor);

    GtkListBox * listBox;

} EDITOR, *EDITOR_P;

extern EDITOR *create_editor(GtkListBox * listBox);

#endif // EDITOR_H_INCLUDED