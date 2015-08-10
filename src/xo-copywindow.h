#include "gtk/gtk.h"

#ifndef XOCOPYWINDOW
#define XOCOPYWINDOW




/**
 * @brief copyScrollPosition copies the scroll position of the main window to the viewing window
 *
 */
void copyScrollPosition();

/**
 * @brief create_winView
 * creates the viewer window
 */
GtkWidget* create_winView();

void create_viewIndicator();
void update_viewIndicator();

#endif // XOCOPYWINDOW

