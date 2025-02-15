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
 * @brief an event structure is a union of the object editor
 * 
 */
typedef struct _EDITOR
{

    void (*release)(struct _EDITOR *editor);

} EDITOR, *EDITOR_P;

#endif // EDITOR_H_INCLUDED