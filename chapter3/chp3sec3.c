/*  This program consists of a base window coloured yellow.  When
 *  the mouse pointer is over this window and a mouse button is
 *  pressed, the coordinates of the pointer relative to the
 *  window is printed on the console window and a red window
 *  containing a green window is drawn at that point.  If the
 *  mouse button pressed is the left-hand mouse button, then the
 *  beep of the computer is also sounded.  If the right-hand mouse
 *  button is clicked over the green window, the text 'ouch!' is
 *  also printed on the display console window.
 *
 *  Coded by:  Ross Maloney
 *  Date:      June 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  XSetWindowAttributes  baseat, redat, greenat;
  Window         baseW, redW, greenW;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         abc, myevent;
  XColor         exact, closest;
  GC             baseGC;
  XGCValues      myGCValues;
  char *window_name = "Events";
  char *icon_name   = "Ev";
  int            screen_num, done;
  int            x, y;
  unsigned long  valuemask, red, green;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "yellow", &exact, &closest);
  baseat.background_pixel = closest.pixel;
  baseat.border_pixel = BlackPixel(mydisplay, screen_num);
  baseat.event_mask = ButtonPressMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        300, 300, 350, 400, 3,
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

	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &abc);
    switch(abc.type)  {
    case ButtonPress:
      if (abc.xbutton.button == Button1) XBell(mydisplay, 100);
      if (abc.xbutton.button == Button3
           && abc.xbutton.window == greenW) printf("ouch!\n");
      x = abc.xbutton.x;
      y = abc.xbutton.y;
      if (abc.xbutton.window == baseW)  printf("Yellow window: ");
      if (abc.xbutton.window == redW)  printf("Red window: ");
      if (abc.xbutton.window == greenW)  printf("Green window: ");
      printf("x = %d  y = %d\n", x, y);
      redW = XCreateSimpleWindow(mydisplay, baseW, x, y,
                100, 50, 1,
		BlackPixel(mydisplay, screen_num), red);
      XMapWindow(mydisplay, redW);
      XSelectInput(mydisplay, redW, ButtonPressMask);
      greenW = XCreateSimpleWindow(mydisplay, redW, 10, 20,
                40, 20, 1,
		BlackPixel(mydisplay, screen_num), green);
      XMapWindow(mydisplay, greenW);
      XSelectInput(mydisplay, greenW, ButtonPressMask);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
