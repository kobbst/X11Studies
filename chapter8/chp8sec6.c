/*  This program creates of a main window on which is placed a
 *  coloured text and a partial ellipse
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2016
 */

#include <xcb/xcb.h>
#include <string.h>

int main(int argc, char *argv)
{
  xcb_connection_t      *mypath;
  xcb_screen_t          *myscreen;
  xcb_generic_event_t   *myevents;
  int                   mywindow, mygc, myfont;
  int                   mask, values[3];
  char                  winName[] = "Xcb colours";
  char                  winIcon[] = "Col";
  xcb_arc_t        data[] = { {0, 150, 80, 140, 0, 120 << 6},
                              {70, 130, 155, 40, 0, 290 << 6} };
  char fontname[]=
  "-adobe-courier-bold-r-normal--24-240-75-75-m-150-iso10646-1";
  char                  message[] = "Hello";

               /* 1.  open connection to the server */
  mypath = xcb_connect(NULL, NULL);

	       /* 2.  create a top-level window */
  myscreen = xcb_setup_roots_iterator( xcb_get_setup(mypath) ).data;
  mywindow = xcb_generate_id(mypath);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = 0xffff00;
  values[1] = XCB_EVENT_MASK_EXPOSURE;
  xcb_create_window(mypath, XCB_COPY_FROM_PARENT, mywindow,
                    myscreen->root,
                    300, 400, 230, 270, 2,
		    XCB_WINDOW_CLASS_INPUT_OUTPUT,
		    myscreen->root_visual, mask, values);

	       /* 3.  give the Window Manager hints */
  xcb_change_property(mypath, XCB_PROP_MODE_REPLACE, mywindow,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		      strlen(winName), winName);
  xcb_change_property(mypath, XCB_PROP_MODE_REPLACE, mywindow,
                      XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
		      strlen(winIcon), winIcon);

	       /* 4.  establish window resources */
  mygc = xcb_generate_id(mypath);
  myfont = xcb_generate_id(mypath);
  xcb_open_font(mypath, myfont, strlen(fontname), fontname);
  mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  values[0] = 0xff0000;
  values[1] = 0x00ffff;
  values[2] = myfont;
  xcb_create_gc(mypath, mygc, mywindow, mask, values);
  xcb_close_font(mypath, myfont);

	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */

	       /* 7.  map the windows */
  xcb_map_window(mypath, mywindow);
  xcb_flush(mypath);

	       /* 8.  enter the event loop */
  while ( 1 )  {
    myevents = xcb_wait_for_event(mypath);
    switch ( myevents->response_type )  {
    case XCB_EXPOSE:
      xcb_poly_fill_arc(mypath, mywindow, mygc, 2, data);
      xcb_image_text_8(mypath, strlen(message), mywindow,
                       mygc, 80, 70, message);
      xcb_flush(mypath);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  xcb_disconnet(mypath);
}
