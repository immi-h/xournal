#include "xo-copywindow.h"

#include <stdio.h>
#include <gtk/gtk.h>
#include "xournal.h"
#include "xo-support.h"
#include "xo-misc.h"


GtkWidget* winView;

//borrowing this from selection.c
extern void make_dashed(GnomeCanvasItem* item);

GtkWidget* create_winView(){

    memset(&ui.copyWindow, 0, sizeof ui.copyWindow);

    ui.copyWindow.canvas = GNOME_CANVAS (gnome_canvas_new_aa ());

    GtkWidget* winView = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_icon(GTK_WINDOW(winView), create_pixbuf("xournal.png"));
    GtkWidget* vbox =gtk_vbox_new(FALSE, 0);

    gtk_window_set_default_size(GTK_WINDOW (winView), 500, 500);

    scrolledWindowView=gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolledWindowView), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_window_set_title (GTK_WINDOW (winView), _("Xournal viewing window"));

    gtk_container_add(GTK_CONTAINER(winView), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindowView, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(scrolledWindowView), ui.copyWindow.canvas);

    gnome_canvas_set_pixels_per_unit (ui.copyWindow.canvas, ui.zoomView);

    gnome_canvas_set_center_scroll_region (ui.copyWindow.canvas, TRUE);
    gtk_layout_get_hadjustment(GTK_LAYOUT (ui.copyWindow.canvas))->step_increment = ui.scrollbar_step_increment;
    gtk_layout_get_vadjustment(GTK_LAYOUT (ui.copyWindow.canvas))->step_increment = ui.scrollbar_step_increment;

    ui.copyWindow.canvas = ui.copyWindow.canvas;

    gtk_widget_show_all(winView);


    return winView;
}


void update_copy_scroll()
{
  fprintf(stderr, "DEBUG: copy scroll\n");
  GtkAdjustment* mainAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(GET_COMPONENT("scrolledwindowMain")));
  GtkAdjustment* viewAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));

  float pageSize = (gtk_adjustment_get_page_size(mainAdj)/2);
  float pageSizeView = (gtk_adjustment_get_page_size(mainAdj)/2);

  double value = gtk_adjustment_get_value(mainAdj) + (gtk_adjustment_get_page_size(mainAdj)/2);

  fprintf(stderr, "pageSize: %f\n", pageSize);
  fprintf(stderr, "pageSizeView: %f\n", pageSizeView);
  fprintf(stderr, "value: %f\n", value);
  
  switch(ui.copyWindow.scrollMode){
    default:
  	case Detached:
      //TODO
      break;
  	case TopLeft:
      //TODO
      break;
    case Center:
      //TODO
      break;
  case SameHeight:
      //TODO
      break;
  case SameWidth:
      //TODO
      break;
  case AlwaysFit:
      //TODO
      break;
  }

  gtk_adjustment_set_value(viewAdj, value - (gtk_adjustment_get_page_size(viewAdj)/2));
  
  update_viewIndicator();
}



void create_viewIndicator()
{
  ui.copyWindow.viewIndicator.item = gnome_canvas_item_new(
      gnome_canvas_root(canvas),
      gnome_canvas_rect_get_type(),
      "width-pixels",		5,
      "outline-color-rgba", 0x00110020,
      "fill-color-rgba",    0x00800020,
      NULL);

  gnome_canvas_item_show(ui.copyWindow.viewIndicator.item);
  gnome_canvas_item_raise_to_top(ui.copyWindow.viewIndicator.item);
}

void update_viewIndicator(){
    if(!scrolledWindowView) return;
    GtkAdjustment* vAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));
    GtkAdjustment* hAdj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));

    double left =  gtk_adjustment_get_value(hAdj) / ui.zoomView;
    double top  =  gtk_adjustment_get_value(vAdj) / ui.zoomView;
    
    double xsize=scrolledWindowView->allocation.width / ui.zoomView;
    double ysize=scrolledWindowView->allocation.height / ui.zoomView;

    ui.copyWindow.viewIndicator.bbox.bottom = top+xsize;
    ui.copyWindow.viewIndicator.bbox.right = left+ysize;

    gnome_canvas_item_set ( ui.copyWindow.viewIndicator.item,
      "x1", left,
      "x2", left+xsize ,
      "y1", top,
      "y2", top+ysize,
    NULL);

    gnome_canvas_item_show(ui.copyWindow.viewIndicator.item);

    make_dashed(ui.copyWindow.viewIndicator.item);
    gnome_canvas_item_raise_to_top(ui.copyWindow.viewIndicator.item);
}

void on_copyWindow_visibility_toggled (GtkMenuItem *menuitem, gpointer user_data);

GtkWidget* makeCopywindowToolbar(){
    GtkToolbar* toolbar = gtk_toolbar_new();
    GtkToolButton* copyWindowVisibleButton = gtk_toggle_button_new();
    gtk_signal_connect(copyWindowVisibleButton, "activate", G_CALLBACK(on_copyWindow_visibility_toggled),NULL);

    GtkToolButton* copyWindowPointerVisible = gtk_toggle_button_new();
    gtk_container_add(GTK_CONTAINER(toolbar), copyWindowVisibleButton);
}





///////////////////////////////////
///  CALLBACKS

///////////////////////////////////
/// \brief on_copyWindow_visibility_toggled callback for when th visibility button is clicked
/// \param menuitem  the vilibilty toggle switch
/// \param user_data nothing usefull
///
void on_copyWindow_visibility_toggled (GtkMenuItem *menuitem, gpointer user_data){
    //todo
    printf("[copywindow]visibility toggled");
}


void create_cursorIndicator()
{
    //for now we just make a cross.

    GnomeCanvasGroup* group = (GnomeCanvasGroup*) gnome_canvas_item_new(
                gnome_canvas_root(ui.copyWindow.canvas),
                gnome_canvas_group_get_type(),
                "x", 100.,
                "y", 100.,
                NULL
            );
    GnomeCanvasPoints* points;
    double coords[] =  {-5.,-5., 5., 5.};

    points = gnome_canvas_points_new(2);
    g_memmove(points->coords, coords,
              2*2*sizeof(double));

    gnome_canvas_item_new(
        group,
        gnome_canvas_line_get_type(),

        "points",			points,
        "fill-color-rgba",	0x000000ff,
        "width-units",		1.,
        NULL
    );

    coords[0] =  5;
    coords[2] = -5;
    g_memmove(points->coords, coords,
              2*2*sizeof(double));

    gnome_canvas_item_new(
        group,
        gnome_canvas_line_get_type(),

        "points",			points,
        "fill-color-rgba",	0x000000ff,
        "width-units",		1.,
        NULL
    );



    ui.copyWindow.cursorIndicator = group;
}

void update_cursor_indicator(GdkEventMotion* event){

    double newPos[2];
    get_pointer_coords(event, newPos);
    fprintf(stderr, "x: %f\ny:%f\n", newPos[0], newPos[1] );
    if(ui.copyWindow.cursorIndicator==NULL){
        create_cursorIndicator();
    }
    gnome_canvas_item_set(ui.copyWindow.cursorIndicator,
                          "x", newPos[0],
                          "y", newPos[1],
                          NULL);
    gnome_canvas_item_move(ui.copyWindow.cursorIndicator, 0., 0.);
}
