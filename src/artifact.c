/**
 * @file artifact.c
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  base class for artifacts - nodes (places and transitions) and arcs.
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "artifact.h"

ARTIFACT *setup_artifact(ARTIFACT *artifact, int enabled, enum STATE state, int selected)
{

    artifact->enabled = enabled;
    artifact->state = state;
    artifact->selected = selected;
}