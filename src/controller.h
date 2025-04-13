/**
 * @file controller.h
 * @author Dr. Neil Brittliff (brittliff.org)
 * @brief  prototype - the interface between the GDK event processing system and petri-net's implementation
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

/**
 * @brief casts an object to a controller
 *
 */
#define TO_CONTROLLER(controller) ((CONTROLLER *)(controller))

/**
 * @brief drag modes - based on the control key being pressed
 *
 */
enum MODES
{
  NORMAL = 0,
  CONNECT,
  MOVE,
  DRAG,
  FINALISE,
  END_MODES

};

/**
 * @brief controller interface
 *
 */
typedef struct _CONTROLLER
{

  GtkWidget *window;
  GtkWidget *scrolledWindow;
  GtkWidget *drawingArea;
  GtkWidget *viewPort;
  GtkGesture *click;
  GtkGesture *drag;
  GtkEventController *keyController;

  GtkWidget *selectButton;
  GtkWidget *placeButton;
  GtkWidget *transitionButton;

  GtkWidget *newToolbarButton;
  GtkWidget *openToolbarButton;
  GtkWidget *saveToolbarButton;
  GtkWidget *saveAsToolbarButton;
  GtkWidget *deleteToolbarButton;

  GtkListBox *fieldEditor;

  GPtrArray *handlers;

  enum MODES mode;

  /**
   * @brief this adds the handler(s) array to include the handler
   *
   */
  void (*monitor)(struct _CONTROLLER *controller, HANDLER *handler);

  /**
   * @brief this removes the handler(s) array to include the handler
   *
   */
  void (*unmonitor)(struct _CONTROLLER *controller, HANDLER *handler);

  /**
   * @brief notify the handler(s) to process the event
   *
   */
  void (*notify)(struct _CONTROLLER *controller, EVENT *event);

  /**
   * @brief this is send message
   *
   */
  void (*send)(struct _CONTROLLER *controller, EVENT *event);

  /**
   * @brief this is similar but only requires the nitifcation
   *
   */
  void (*message)(struct _CONTROLLER *controller, enum NOTIFICATION notification);

  /**
   * @brief this is called GTK to call all handlers to respond to the 'draw' event
   *
   */
  void (*redraw)(struct _CONTROLLER *controller);

  /**
   * @brief this is called to get a field editor
   *
   */
  EDITOR *(*edit)(struct _CONTROLLER *controller);

  /**
   * @brief this is called to get a field editor
   *
   */
  WRITER *(*writer)(struct _CONTROLLER *controller);

  /**
   * @brief this is called to get a field editor
   *
   */
  READER *(*reader)(struct _CONTROLLER *controller, char * filename);

  /**
   * @brief release the controller and return all resources
   *
   */
  void (*release)(struct _CONTROLLER *controller);

} CONTROLLER, *CONTROLLER_P;

extern CONTROLLER *create_controller(GtkApplication *gtkAppication, char *resourceURL);

#endif // CONTROLLER_H_INCLUDED