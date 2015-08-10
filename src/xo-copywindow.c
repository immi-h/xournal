#include "xo-copywindow.h"

#include <stdio.h>
#include <gtk/gtk.h>
#include "xournal.h"
#include "xo-support.h"


GtkWidget* winView;


GtkWidget* create_winView(){
    GtkWidget* winView = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_icon(GTK_WINDOW(winView), create_pixbuf("xournal.png"));
    GtkWidget* vbox =gtk_vbox_new(FALSE, 0);


    scrolledWindowView=gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolledWindowView), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewCanvas = gnome_canvas_new_aa();

    gtk_window_set_default_size(GTK_WINDOW (winView), ui.window_default_width, ui.window_default_height);
    gtk_window_set_title (GTK_WINDOW (winView), _("Xournal viewing window"));

    gtk_container_add(GTK_CONTAINER(winView), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindowView, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(scrolledWindowView), viewCanvas);

    gnome_canvas_set_pixels_per_unit (viewCanvas, ui.zoomView);

    gnome_canvas_set_center_scroll_region (viewCanvas, TRUE);
    gtk_layout_get_hadjustment(GTK_LAYOUT (viewCanvas))->step_increment = ui.scrollbar_step_increment;
    gtk_layout_get_vadjustment(GTK_LAYOUT (viewCanvas))->step_increment = ui.scrollbar_step_increment;

    gtk_widget_show(viewCanvas);

    gtk_widget_show_all(winView);


    return winView;

}


void copyScrollPosition()
{
    update_viewIndicator();
    return;
  fprintf(stderr, "DEBUG: copy scroll\n");
  GtkAdjustment* mainAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(GET_COMPONENT("scrolledwindowMain")));
  GtkAdjustment* viewAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));

  float pageSize = (gtk_adjustment_get_page_size(mainAdj)/2);
  float pageSizeView = (gtk_adjustment_get_page_size(mainAdj)/2);

  double value = gtk_adjustment_get_value(mainAdj) + (gtk_adjustment_get_page_size(mainAdj)/2);

  fprintf(stderr, "pageSize: %f\n", pageSize);
  fprintf(stderr, "pageSizeView: %f\n", pageSizeView);
  fprintf(stderr, "value: %f\n", value);

  gtk_adjustment_set_value(viewAdj, value - (gtk_adjustment_get_page_size(viewAdj)/2));
}


GnomeCanvasItem* viewIndicatorItem;

void create_viewIndicator()
{
  viewIndicatorItem = gnome_canvas_item_new(gnome_canvas_root(canvas),
      gnome_canvas_rect_get_type(),
      "width-pixels", 5,
      "outline-color-rgba", 0x00110080,
      "fill-color-rgba",    0x00800080,
      "x1", 100.,
      "x2", 5000.,
      "y1", 100.,
      "y2", 3000.,

      NULL);

  gnome_canvas_item_show(viewIndicatorItem);
  gnome_canvas_item_raise_to_top(viewIndicatorItem);
  update_viewIndicator();
}

void update_viewIndicator(){
    GtkAdjustment* vAdj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));
    GtkAdjustment* hAdj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolledWindowView));

    double left =  gtk_adjustment_get_value(hAdj) / ui.zoomView;
    double top  =  gtk_adjustment_get_value(vAdj) / ui.zoomView;
    
    double xsize=scrolledWindowView->allocation.width / ui.zoomView;
    double ysize=scrolledWindowView->allocation.height / ui.zoomView;

    fprintf(stderr, "x:%f\n", left);
    fprintf(stderr, "y:%f\n", top);
    fprintf(stderr, "w:%f\n", xsize);
    fprintf(stderr, "h:%f\n", ysize);

    ui.viewIndicator.bbox.bottom = top+xsize;
    ui.viewIndicator.bbox.right = left+ysize;

    gnome_canvas_item_set ( viewIndicatorItem,
      "x1", left  ,
      "x2", left+xsize ,
      "y1", top,
      "y2", top+ysize,
    NULL);
    gnome_canvas_item_show(viewIndicatorItem);

    make_dashed(viewIndicatorItem);
    gnome_canvas_item_raise_to_top(viewIndicatorItem);




}
