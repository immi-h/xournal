#include "gtk/gtk.h"
#include "libgnomecanvas/libgnomecanvas.h"
#include "utils.h"
#ifndef XOCOPYWINDOW
#define XOCOPYWINDOW

/**
 * How should the copyWindow behave while scrolling
 */
typedef enum {
    Detached=0,
    TopLeft,
    Center,
    SameHeight, //copy the entire width of the main window
    SameWidth,  //
    AlwaysFit,
    //etc
} FollowScrollMode;

/**
  * the transparent rectangle indicating what the copyWindows is showing
  */
typedef struct ViewIndicator{
    BBox bbox;
    GnomeCanvasItem* item;
} ViewIndicator;

/**
  * All the information about the copyWindow.
  */

typedef struct CopyWindow{
    gboolean visible;
    FollowScrollMode scrollMode;
    GnomeCanvas* canvas;
    ViewIndicator viewIndicator;
    GnomeCanvasItem* cursorIndicator;
} CopyWindows;

/**
 * @brief update_copy_scroll updates the scroll position of the copyWindow according to settings
 */
void update_copy_scroll();


/**
 * @brief create_winView
 * creates the viewer window
 */
GtkWidget* create_winView();

/**
 * @brief create_viewIndicator makes the nice rectangle that shows what the seconds screen is looking at.
 */
void create_viewIndicator();
/**
 * @brief update_viewIndicator move the ViewIndicator to the right position.
 */
void update_viewIndicator();

/**
 * @brief create_cursorIndicator creates the thing on the seconds screen that shows where the mouse is at.
 */
void create_cursorIndicator();
/**
 * @brief update_cursor_indicator moves the cursorIndicator to where it should be.
 */
void update_cursor_indicator(GdkEventMotion* event);


#endif // XOCOPYWINDOW

