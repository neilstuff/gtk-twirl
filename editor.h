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
    SPIN_FIELD,
    END_FIELD
};


/**
 * @brief an editor structure 
 * 
 */
typedef struct _EDITOR
{

    void (*listener)(int id, char * value);

    void (*init)(struct _EDITOR *editor, enum FIELD field, ...);

    void (*release)(struct _EDITOR *editor);

    GtkListBox * listBox;

} EDITOR, *EDITOR_P;

extern EDITOR *create_editor(GtkListBox * listBox);

#endif // EDITOR_H_INCLUDED