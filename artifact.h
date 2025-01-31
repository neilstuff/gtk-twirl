
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
 * @brief artifact types - node (places and transitions) or arcs
 *
 */
enum ARTIFACT_TYPE
{
    NODE_ARTIFACT = 0,
    ARC_ARTIFACT,
    END_ARTIFACT_TYPES

};

/**
 * @brief casts an object to an artifact
 *
 */
#define TO_ARTIFACT(connector) ((ARTIFACT *)(artifact))

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

    /**
     * @brief artifact type
     * 
     */
    enum ARTIFACT_TYPE type;

    union
    {
        struct
        {

            struct _NODE *node;

        } node_artifact;

        struct
        {

            struct _ARC *arc;

        } arc_artifact;

    } artifacts;

} ARTIFACT, *ARTIFACT_P;

#endif // ARTIFACT_H_INCLUDED
