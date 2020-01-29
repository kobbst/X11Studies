/*  This program draws a target plate consisting of a square
 *  containing a square which is standing on its corners,
 *  extended diagonal lines of the inner square, and a circle
 *  centred at the intersection of those diagonal lines.  The
 *  squares are filled in pink and pale blue colour, one
 *  diagonal line is solid while the other is dotted, and the
 *  circle is a solid red coloured line.  This picture is
 *  drawn directly on its containing white coloured window.
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
  GC             baseGC;
  XGCValues      myGCValues;
  XColor         pink, blue, red, black, white;
  XPoint         corners[] = {{140,60},{230,150},{140,240},
                             {50,150}};
  char *window_name = "Tarplate";
  char *icon_name   = "Tp";
  int            screen_num, done;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  black.pixel = BlackPixel(mydisplay, screen_num);
  white.pixel = WhitePixel(mydisplay, screen_num);
  red.pixel = 0xff0000;
  pink.pixel = 0xffb6c1;
  blue.pixel = 0xacc8e6;
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
  myGCValues.foreground = blue.pixel;
  baseGC = XCreateGC(mydisplay, baseW, valuemask, &myGCValues);

	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch(myevent.type)  {
    case  Expose:
      XFillRectangle(mydisplay, baseW, baseGC,
                     50, 60, 180, 180);
      XSetForeground(mydisplay, baseGC, pink.pixel);
      XFillPolygon(mydisplay, baseW, baseGC,
                   corners, 4, Convex, CoordModeOrigin);
      XSetForeground(mydisplay, baseGC, black.pixel);
      XDrawLine(mydisplay, baseW, baseGC, 140, 30, 140, 270);
      XSetLineAttributes(mydisplay, baseGC,
                         2, LineOnOffDash, CapButt, JoinMiter);
      XDrawLine(mydisplay, baseW, baseGC, 20, 150, 260, 150);
      XSetForeground(mydisplay, baseGC, red.pixel);
      XSetLineAttributes(mydisplay, baseGC,
                         0, LineSolid, CapButt, JoinMiter);
      XDrawArc(mydisplay, baseW, baseGC,
               95, 105, 90, 90, 0, 360*64);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}

