/*  First a basic window with a white background is created.
 *  Then another window, a child of the first is created with
 *  a black background.  This second window is repeatedly
 *  mapped onto its parent window and then removed after 3
 *  seconds.  Each mapping is at different location.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2011
 */

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>
#include  <unistd.h>

int main(int argc, char *argv[])
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow, rover;
  XWindowChanges alter;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  char *window_name = "Walking";
  char *icon_name = "Wk";
  int            screen_num, done;
  unsigned long  valuemask;
  int            x, y;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

               /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ExposureMask | StructureNotifyMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           200, 300, 350, 250, 2,
			   DefaultDepth(mydisplay, screen_num),
			   InputOutput,
			   DefaultVisual(mydisplay, screen_num),
			   valuemask, &myat);

               /* 3.  give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, mywindow, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, mywindow, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, mywindow, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, mywindow, &iconName);

               /* 4.  establish window resources */
  myat.background_pixel = BlackPixel(mydisplay, screen_num);

               /* 5.  create all the other windows needed */
  rover = XCreateWindow(mydisplay, mywindow,
                        100, 30, 50, 70, 2,
		        DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &myat);

               /* 6.  select events for each window */
  valuemask = CWX | CWY;

               /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);

               /* 8.  enter the event loop */
  done = 0;
  x = 11;  y = 12;
  while ( done == 0 )  {
    alter.x = x;
    alter.y = y;
    XConfigureWindow(mydisplay, rover, valuemask, &alter);
    XFlush(mydisplay);
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type) {
    case  Expose:
      break;
    case  ConfigureNotify:
      XMapWindow(mydisplay, rover);
      sleep(3);
      x += 5;  y += 6;
    }
  }

               /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
