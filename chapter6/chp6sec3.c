/*  This program draws the Tao (or Tai-Chi) symbol in black on a
 *  300 by 300 white window.  The symbol is composed of 3
 *  semicircles, and 3 full circles.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  XSetWindowAttributes  baseat;
  Window         baseW;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  GC             gc1, gc2;
  XGCValues      myGCValues;
  XColor         black, white;
  Pixmap         pad;
  char *window_name = "Tao";
  char *icon_name   = "Ta";
  int            screen_num, done;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  black.pixel = BlackPixel(mydisplay, screen_num);
  white.pixel = WhitePixel(mydisplay, screen_num);
  baseat.background_pixel = white.pixel;
  baseat.border_pixel = black.pixel;
  baseat.event_mask = ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        100, 100, 300, 300, 2,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &baseat);

	       /* 3.  give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, baseW, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, baseW, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, baseW, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, baseW, &iconName);

	       /* 4.  establish window resources */
  valuemask = GCForeground | GCBackground;
  myGCValues.background = white.pixel;
  myGCValues.foreground = black.pixel;
  gc1 = XCreateGC(mydisplay, baseW, valuemask, &myGCValues);
  myGCValues.background = black.pixel;
  myGCValues.foreground = white.pixel;
  gc2 = XCreateGC(mydisplay, baseW, valuemask, &myGCValues);

	       /* 5.  create all the other windows needed */
  pad = XCreatePixmap(mydisplay, baseW, 300, 300, 
		      DefaultDepth(mydisplay, screen_num));
  XFillRectangle(mydisplay, pad, gc2, 0, 0, 300, 300),
  XFillArc(mydisplay, pad, gc1, 30, 30, 240, 240, 90*64, 180*64);
  XFillArc(mydisplay, pad, gc1, 90, 150, 120, 120, 270*64, 180*64);
  XFillArc(mydisplay, pad, gc2, 90, 30, 120, 120, 90*64, 180*64);
  XFillArc(mydisplay, pad, gc2, 140, 200, 20, 20, 0, 360*64);
  XFillArc(mydisplay, pad, gc1, 140, 80, 20, 20, 0, 360*64);
  XDrawArc(mydisplay, pad, gc1, 30, 30, 240, 240, 0, 360*64);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch(myevent.type)  {
    case  Expose:
      XCopyArea(mydisplay, pad, baseW, gc1, 0, 0, 300, 300, 0, 0);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
