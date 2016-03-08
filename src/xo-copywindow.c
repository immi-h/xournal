#include "xo-copywindow.h"

#include "xournal.h"
#include "xo-support.h"
#include "xo-misc.h"
#include "xo-support.h"

#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>



GtkWidget* winView;


struct {
    GtkToolButton* cursorVisible;
    GtkToolButton* windowVisible;
    GtkComboBox  * scrollMode;
    GtkToolButton* zoomIn;
    GtkToolButton* zoomOut;
} menuItems;



///  CALLBACKS (these won't appear in the header)
void on_tooltoggle_toggled (GtkToolButton *menuitem, gpointer user_data);
void on_scroll_combo_changed (GtkComboBox *combobox, gpointer user_data);
void on_copy_window_zoombutton_clicked(GtkToolButton* button, gpointer userData);

void set_copy_cursor_visible(gboolean visible);

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

GtkWidget* create_copy_toolbar(){
    GtkHBox* toolbar = gtk_hbox_new(FALSE, 3);

    GtkWidget* image;

    gtk_widget_show(toolbar);
    gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
    
    

    menuItems.windowVisible = gtk_toggle_button_new_with_label("gtk-copy");
    gtk_button_set_use_stock(menuItems.windowVisible, TRUE);
    menuItems.cursorVisible = gtk_toggle_button_new();
    menuItems.scrollMode    = gtk_combo_box_new_text();
    menuItems.zoomIn        = gtk_tool_button_new_from_stock("gtk-zoom-in");
    menuItems.zoomOut	    = gtk_tool_button_new_from_stock("gtk-zoom-out");

    image = create_pixmap(0, "pencilIndicator.png");
    gtk_button_set_image (menuItems.cursorVisible, image);

    gtk_signal_connect(menuItems.windowVisible, "toggled", G_CALLBACK(&on_tooltoggle_toggled),NULL);
    gtk_signal_connect(menuItems.cursorVisible, "toggled", G_CALLBACK(&on_tooltoggle_toggled),NULL);
    gtk_signal_connect(menuItems.scrollMode   , "changed", G_CALLBACK(&on_scroll_combo_changed),NULL);
    gtk_signal_connect(menuItems.zoomIn       , "clicked", G_CALLBACK(&on_copy_window_zoombutton_clicked), NULL);
    gtk_signal_connect(menuItems.zoomOut      , "clicked", G_CALLBACK(&on_copy_window_zoombutton_clicked), NULL);


    gtk_combo_box_append_text(menuItems.scrollMode, "Detached");
    gtk_combo_box_append_text(menuItems.scrollMode, "Scroll together");
    gtk_combo_box_append_text(menuItems.scrollMode, "Top left corner");
    gtk_combo_box_append_text(menuItems.scrollMode, "Center");
    gtk_combo_box_append_text(menuItems.scrollMode, "Same height");
    gtk_combo_box_append_text(menuItems.scrollMode, "same width");
    gtk_combo_box_append_text(menuItems.scrollMode, "Always fit");

    gtk_widget_show(menuItems.windowVisible);
    gtk_widget_show(menuItems.cursorVisible);
    gtk_widget_show(menuItems.scrollMode);
    gtk_widget_show(menuItems.zoomIn);
    gtk_widget_show(menuItems.zoomOut);

    gtk_container_add(GTK_CONTAINER(toolbar), menuItems.cursorVisible);
    gtk_container_add(GTK_CONTAINER(toolbar), menuItems.windowVisible);
    gtk_container_add(GTK_CONTAINER(toolbar), menuItems.scrollMode);
    gtk_container_add(GTK_CONTAINER(toolbar), menuItems.zoomIn);
    gtk_container_add(GTK_CONTAINER(toolbar), menuItems.zoomOut);

    return toolbar;
}


void update_copy_scroll()
{
  GtkAdjustment* mainAdjV = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(GET_COMPONENT("scrolledwindowMain")));
  GtkAdjustment* mainAdjH = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(GET_COMPONENT("scrolledwindowMain")));

  GtkAdjustment* viewAdjV = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));
  GtkAdjustment* viewAdjH = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));

  float pageSizeV     = (gtk_adjustment_get_page_size(mainAdjV)/2);
  float pageSizeH     = (gtk_adjustment_get_page_size(mainAdjH)/2);

  float pageSizeViewV = (gtk_adjustment_get_page_size(viewAdjV)/2);
  float pageSizeViewH = (gtk_adjustment_get_page_size(viewAdjH)/2);




  struct {
      float x,y;
  } main;
  main.x = gtk_adjustment_get_value(mainAdjH);
  main.y = gtk_adjustment_get_value(mainAdjV);


  // zoom factor to fit the width
  double widthZoom = (GTK_WIDGET(ui.copyWindow.canvas))->allocation.width/ui.cur_page->width;
  double heightZoom= (GTK_WIDGET(ui.copyWindow.canvas))->allocation.height/ui.cur_page->height;

  //double widthscale =  (double) pageSizeViewH

  switch(ui.copyWindow.scrollMode){
    default:
  	case Detached:
      break; //don't copy anything
    case Center:
      main.y += pageSizeV - pageSizeViewV;
      main.x += pageSizeH - pageSizeViewH;
    case TopLeft:
      gtk_adjustment_set_value(viewAdjV, main.y);
      gtk_adjustment_set_value(viewAdjH, main.x);
      break;
  case SameHeight:
      ui.zoomView = heightZoom;
      break;
  case SameWidth:
      ui.zoomView = widthZoom;
      break;
  case AlwaysFit:
      ui.zoomView = MIN(heightZoom, widthZoom);
      //TODO
      break;
  }
  gnome_canvas_set_pixels_per_unit(ui.copyWindow.canvas, ui.zoomView);
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
    if(!ui.copyWindow.viewIndicator.item){
        create_viewIndicator();
    }
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
    if(ui.copyWindow.cursorIndicator==NULL){
        create_cursorIndicator();
    }
    gnome_canvas_item_set(ui.copyWindow.cursorIndicator,
                          "x", newPos[0],
                          "y", newPos[1],
                          NULL);
    gnome_canvas_item_move(ui.copyWindow.cursorIndicator, 0., 0.);
}



///////////////////////////////////
///  CALLBACKS

///////////////////////////////////
/// \brief on_copyWindow_visibility_toggled callback for when th visibility button is clicked
/// \param menuitem  the vilibilty toggle switch
/// \param user_data nothing usefull
///

void on_scroll_combo_changed (GtkComboBox *combobox, gpointer user_data){
    const char* newText = gtk_combo_box_get_active_text(combobox);
    int item = gtk_combo_box_get_active(menuItems.scrollMode);

    ui.copyWindow.scrollMode = item;

    update_copy_scroll();
}

void on_tooltoggle_toggled (GtkToolButton *item, gpointer user_data){
    //todo
    printf("[copywindow]visibility toggled");
    gboolean active = gtk_toggle_button_get_active(item);
    if(item == menuItems.windowVisible ){
        set_copy_window_visible(active);
    }
    else if (item == menuItems.cursorVisible){
        set_copy_cursor_visible(active);
    }
}

void set_copy_window_visible(gboolean visible){
    ui.copyWindow.visible = visible;
    if(visible == FALSE){
        gtk_widget_set_visible(winView, visible);
    }
    else
    {
        //restore will set the actual visibility
        copy_window_restore();
    }
}

void set_copy_cursor_visible(gboolean visible){
    ui.copyWindow.cursorVisible = visible;

    (visible? gnome_canvas_item_show : gnome_canvas_item_hide)
            (ui.copyWindow.cursorIndicator);
}

void copy_window_restore() {
    gtk_widget_set_visible (winView, ui.copyWindow.visible);
    set_copy_cursor_visible(ui.copyWindow.cursorIndicator);
}

void copy_window_cleanup()
{
    if(ui.copyWindow.cursorIndicator)
    {
        gtk_object_destroy(ui.copyWindow.cursorIndicator);
        ui.copyWindow.cursorIndicator = NULL;
    }
    if(ui.copyWindow.viewIndicator.item){
        gtk_object_destroy(ui.copyWindow.viewIndicator.item);
        ui.copyWindow.viewIndicator.item = NULL;
    }
}

void on_copy_window_zoombutton_clicked(GtkToolButton* button, gpointer userData){
    if(button == menuItems.zoomIn)
        ui.zoomView = ui.zoomView * COPY_WINDOW_ZOOM_PER_CLICK;
    else if(button==menuItems.zoomOut)
        ui.zoomView = ui.zoomView * (1/COPY_WINDOW_ZOOM_PER_CLICK);
    else
        fprintf(stderr, "warning, zoom callback from unknown source\n");
    gnome_canvas_set_pixels_per_unit(ui.copyWindow.canvas, ui.zoomView);
}




