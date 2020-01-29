/*  The simplest Xlib program possible which produces a window.
 *  A Window coloured white is placed on the screen.
 *
 *  Coded by:  Ross Maloney
 *  Date:      April 2012
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow;
  XEvent         myevent;
  int            screen_num, done;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  valuemask = CWBackPixel;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           200, 200, 350, 250, 2,
			   DefaultDepth(mydisplay, screen_num),
			   InputOutput,
			   DefaultVisual(mydisplay, screen_num),
			   valuemask, &myat);

	       /* 3.  give the Window Manager hints */
	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);

	       /* 8.  enter the event loop */
  XNextEvent(mydisplay, &myevent);

	       /* 9.  clean up before exiting */
}
