/**
 * @file tracker.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - represent red/undo tracker
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

 #ifndef TRACKER_H_INCLUDED
 #define TRACKER_H_INCLUDED

 /**
  * @brief casts an object to a tracker
  *
  */
 #define TO_TRACKER(tracker) ((TRACKER *)(tracker))
 
 typedef struct _TRACKER {
 
    void (*release)(struct _TRACKER * tracker);
    void (*snapshot)(struct _TRACKER * tracker, struct _NET * net);

    GPtrArray * history;

    GPtrArray * nodes;
  
 } TRACKER, *TRACKER_P;
 
 extern TRACKER * create_tracker();
 
 #endif // TRACKER_H_INCLUDED