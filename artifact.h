/**
 * @file artifact.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - an artifact is a drawable object
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef ARTIFACT_H_INCLUDED
#define ARTIFACT_H_INCLUDED

#include "geometry.h"
#include "drawer.h"

/**
 * @brief casts an object to an ARTIFACT
 * 
 */
#define TO_ARTIFACT(artifact) ((ARTIFACT*)(artifact))

typedef struct _ARTIFACT {



} ARTIFACT, * ARTIFACT_P;


#endif // ARTIFACT_H_INCLUDED
