/*  This program consists of a main window on which is placed a
 *  second window.  Initially the main window is coloured red.
 *  When the mouse pointer enters this window, that background
 *  changes to yellow and then back to red when the mouse
 *  pointer exits this window.  Onto this first window a second
 *  window is placed.  This second window carries a
 *  checker-board bitmap which only covers the background of the
 *  whole window.  When the mouse pointer is inside this window
 *  the background of that checker-board is coloured green and
 *  blue, and when outside it is coloured black and blue.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2012
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define b_width 32
#define b_height 32
static char b_bits[] = {
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0xff, 0xff, 0x00, 0x00,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff
};

int main(int argc, char *argv)
{
  Display       *mydisplay;
  XSetWindowAttributes  baseat, secondat;
  Window        baseW, secondW;
  XSizeHints    wmsize;
  XWMHints      wmhints;
  XTextProperty windowName, iconName;
  XEvent        myevent;
  XColor        exact, closest;
  GC            baseGC;
  XGCValues     myGCValues;
  Pixmap        ck_board1, ck_board2;
  char *window_name = "Background";
  char *icon_name   = "Bk";
  int           screen_num, done;
  unsigned long valuemask, red, green, yellow, blue;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  baseat.background_pixel = WhitePixel(mydisplay, screen_num);
  baseat.border_pixel = BlackPixel(mydisplay, screen_num);
  baseat.event_mask = EnterWindowMask | LeaveWindowMask
                      | ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        300, 300, 350, 200, 3,
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
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "red", &exact, &closest);
  red = closest.pixel;
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "green", &exact, &closest);
  green = closest.pixel;
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "yellow", &exact, &closest);
  yellow = closest.pixel;
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "blue", &exact, &closest);
  blue = closest.pixel;

	       /* 5.  create all the other windows needed */
  XSetWindowBackground(mydisplay, baseW, red);
  secondat.background_pixel = green;
  secondat.border_pixel = BlackPixel(mydisplay, screen_num);
  secondat.event_mask = EnterWindowMask | LeaveWindowMask
                       | ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  secondW = XCreateWindow(mydisplay, baseW,
                        100, 50, 96, 80, 1,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &secondat);
  ck_board1 = XCreatePixmapFromBitmapData(mydisplay, secondW,
                   b_bits, b_width, b_height,
		   BlackPixel(mydisplay, screen_num),
		   blue, DefaultDepth(mydisplay, screen_num));
  XSetWindowBackgroundPixmap(mydisplay, secondW, ck_board1);
  ck_board2 = XCreatePixmapFromBitmapData(mydisplay, secondW,
                   b_bits, b_width, b_height, blue,
		   green, DefaultDepth(mydisplay, screen_num));

	       /* 6.  select events for each window */

	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);
  XMapWindow(mydisplay, secondW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type)  {
    case  EnterNotify:
      if ( myevent.xcrossing.window == baseW ) {
        XSetWindowBackground(mydisplay, baseW, yellow);
        XClearWindow(mydisplay, baseW);
      }
      if ( myevent.xcrossing.window == secondW ) {
        XSetWindowBackgroundPixmap(mydisplay, secondW, ck_board2);
        XClearWindow(mydisplay, secondW);
      }
      break;
    case  LeaveNotify:
      if ( myevent.xcrossing.window == baseW ) {
        XSetWindowBackground(mydisplay, baseW, red);
        XClearWindow(mydisplay, baseW);
      }
      if ( myevent.xcrossing.window == secondW ) {
        XSetWindowBackgroundPixmap(mydisplay, secondW, ck_board1);
        XClearWindow(mydisplay, secondW);
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
