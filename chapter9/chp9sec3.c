/* Creating of a window using the minimum of xlib calls.
 *
 * Coded by:  Ross Maloney
 * Date:      March 2016
 */

#include  <X11/Xlib.h>

int  main(int argc, char *argv[])
{
  Display         *mydisplay;
  XSetWindowAttributes  myat;
  Window          mywindow;
  XEvent          myevent;
  GC              gc;
  XGCValues       values;
  unsigned long   valuemask;
  int             screen_num, done;

                 /* 1. open connection to the server */
  mydisplay = XOpenDisplay("");

                 /* 2. create a top-level window */
  screen_num = 0;
  myat.background_pixel = 0xffffff;
  myat.border_pixel = 0;
  myat.event_mask = ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           200, 440, 350, 250, 2,
			   24, InputOutput,
			   DefaultVisual(mydisplay, screen_num),
			   valuemask, &myat);

                 /* 3. give the Window Manager hints */
                 /* 4. establish window resources */
  values.foreground = 0;
  values.background = 0xffffff;
  values.line_width = 6;
  gc = XCreateGC(mydisplay, mywindow, 28, &values);

                 /* 5. create all the other windows needed */
                 /* 6. select events for each window */
                 /* 7. map the windows */
  XMapWindow(mydisplay, mywindow);

                 /* 8. enter the event loop */
  while ( 1 )  {
    XNextEvent(mydisplay, &myevent);
    switch ( myevent.type )  {
    case Expose:
      XDrawLine(mydisplay, mywindow, gc, 20, 30, 80, 200);
      break;
    }
  }

		 /* 9. clean up before exiting */
}
