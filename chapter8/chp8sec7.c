/*  Program to create a window coloured white and print the
 *  phrase "Hello, World" on it.  Then as the mouse is moved
 *  around the window, pressing of the mouse button causes the
 *  phrase "Hi!" to be shown under the current mouse pointer
 *  position.  Typing the character 'q' on the keyboard quits
 *  the program.  The window is initially to be sited at the
 *  point (200,200) on the screen.
 *
 *  Coded by:  Ross Maloney
 *  Date:      October 2017
 */

#include <xcb/xcb.h>
#include <string.h>

int  main(int argc,  char *argv)
{

  xcb_connection_t       *mypath;
  xcb_screen_t           *myscreen;
  xcb_generic_event_t    *myevent;
  int                    mywindow, mygc, myfont, done;
  int                    mask, values[3];
  int                    x, y;
  int                    key, how;
  char  fontname[] =
    "-adobe-courier-bold-r-normal--24-240-75-75-m-150-iso10646-1";
  char  hello[] = "Hello, World.";
  char  hi[] = "Hi!";
  char  pountain[] = "Pountain";

                 /* 1.  open connection to the server */
  mypath = xcb_connect(NULL, NULL);

                 /* 2.  create a top-level window */
  myscreen = xcb_setup_roots_iterator( xcb_get_setup(mypath) ).data;
  mywindow = xcb_generate_id(mypath);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = myscreen->white_pixel; /* white window background */
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS
              | XCB_EVENT_MASK_BUTTON_PRESS;
  xcb_create_window(mypath, XCB_COPY_FROM_PARENT, mywindow,
                    myscreen->root,
		    200, 200, 350, 250, 2,
		    XCB_WINDOW_CLASS_INPUT_OUTPUT,
		    myscreen->root_visual,
		    mask, values);

                 /* 3.  give the Window Manager hints */

                 /* 4.  establish window resources */
  mygc = xcb_generate_id(mypath);
  myfont = xcb_generate_id(mypath);
  xcb_open_font(mypath, myfont, strlen(fontname), fontname);
  mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  values[0] = myscreen->black_pixel; /* foreground colour */
  values[1] = myscreen->white_pixel; /* background colour */
  values[2] = myfont;
  xcb_create_gc(mypath, mygc, mywindow, mask, values);
  xcb_close_font(mypath, myfont);

                 /* 5.  create all the other windows needed */
                 /* 6.  select events for each window */
                 /* 7.  map the windows */
  xcb_map_window(mypath, mywindow);
  xcb_flush(mypath);

                 /* 8.  enter the even loop */
  done = 1;
  while (done)  {
    myevent = xcb_wait_for_event(mypath);
    switch ( myevent->response_type )  {
    case XCB_EXPOSE:
      xcb_image_text_8(mypath, strlen(hello), mywindow,
                       mygc, 50, 50, hello);
      xcb_flush(mypath);
      break;
    case XCB_MAPPING_NOTIFY:
      break;
    case XCB_KEY_PRESS:
      key = ((xcb_key_press_event_t *)myevent)->detail;
      how = ((xcb_key_press_event_t *)myevent)->state;
      if ( key == 24 && how == 0 )  done = 0;
      break;
    case XCB_BUTTON_PRESS:
      x = ((xcb_button_press_event_t *)myevent)->event_x;
      y = ((xcb_button_press_event_t *)myevent)->event_y;
      xcb_image_text_8(mypath, strlen(hi), mywindow,
                       mygc, x, y, hi);
      xcb_flush(mypath);
      break;
    }
  }

                 /* 9.  clean up before exiting */
  xcb_disconnect(mypath);
}
