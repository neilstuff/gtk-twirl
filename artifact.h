
/**
 * @file artifact.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - base class for artifacts - nodes (places and transitions) and arcs.
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef ARTIFACT_H_INCLUDED
#define ARTIFACT_H_INCLUDED

/**
 * @brief casts an object to an artifact
 *
 */
#define TO_ARTIFACT(artifact) ((ARTIFACT *)(artifact))

/**
 * @brief enable states
 * 
 */
enum STATE
{
    ACTIVE,
    INACTIVE
};

/**
 * @brief Artifact is a node (place/transiton) and an arc.
 *
 */
typedef struct _ARTIFACT
{

    /**
     * @brief if '1'the artifact's is selected, '0'not selected
     *
     */
    int selected;

    /**
     * @brief '1' the artifact's is enabled, '0' not enabled
     *
     */
    int enabled;

    /**
     * @brief the artifact's state either ACTIVE or INACTIVE
     *
     */
    int state;

} ARTIFACT, *ARTIFACT_P;

extern ARTIFACT * setup_artifact(ARTIFACT *artifact, int enabled, enum STATE state, int selected);

#endif // ARTIFACT_H_INCLUDED
