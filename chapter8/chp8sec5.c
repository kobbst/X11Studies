/*  A Xcb program which draws a thick black line across a window
 *  previously created using a created GC.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2016
 */

#include <xcb/xcb.h>
#include <string.h>

int main(int argc, char *argv)
{
  xcb_connection_t       *mypath;
  xcb_screen_t           *myscreen;
  xcb_generic_event_t    *myevents;
  xcb_screen_iterator_t  iter;
  int                    mywindow, mygc;
  int                    mask, values[3];
  char                   winName[] = "Xcb line";
  char                   winIcon[] = "Li";
  xcb_point_t            data[] =  { {20, 20}, {167, 247} };

               /* 1.  open connection to the server */
  mypath = xcb_connect(NULL, NULL);

	       /* 2.  create a top-level window */
  iter = xcb_setup_roots_iterator( xcb_get_setup(mypath) );
  myscreen = iter.data;
  mywindow = xcb_generate_id(mypath);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = myscreen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE;
  xcb_create_window(mypath, XCB_COPY_FROM_PARENT, mywindow,
                    myscreen->root,
                    100, 120, 200, 260, 2,
		    XCB_WINDOW_CLASS_INPUT_OUTPUT,
		    myscreen->root_visual,
		    mask, values);

	       /* 3.  give the Window Manager hints */
  xcb_change_property(mypath, XCB_PROP_MODE_REPLACE, mywindow,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		      strlen(winName), winName);
  xcb_change_property(mypath, XCB_PROP_MODE_REPLACE, mywindow,
                      XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
		      strlen(winIcon), winIcon);

	       /* 4.  establish window resources */
  mygc = xcb_generate_id(mypath);
  mask = XCB_GC_FOREGROUND | XCB_GC_LINE_WIDTH;
  values[0] = myscreen->black_pixel;
  values[1] = 6;  /* line thickness */
  xcb_create_gc(mypath, mygc, mywindow, mask, values);

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
      xcb_poly_line(mypath, XCB_COORD_MODE_ORIGIN, mywindow,
                    mygc, 2, data);
      xcb_flush(mypath);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  xcb_disconnet(mypath);
}
