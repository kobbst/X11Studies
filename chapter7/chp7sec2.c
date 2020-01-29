/*  This program creates and displays a basic window.  The window
 *  has a default white background.
 *
 *  Coded by:  Ross Maloney
 *  Date:      October 2017
 */

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>
#include  <stdio.h>
#include  <stdlib.h>    /* for exit() */

int main(int argc, char *argv[])
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  char *window_name = "Netprog";
  char *icon_name   = "Net";
  int            screen_num, done;
  unsigned long  valuemask;


                 /* 1. open connection to the server */
  mydisplay = XOpenDisplay("192.168.14.9:0.0");
  if ( mydisplay == NULL )  {
    printf("Error: cannot open display\n");
    exit(1);
  }

                 /* 2. create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ButtonPressMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           200, 200, 350, 250, 2,
			   DefaultDepth(mydisplay, screen_num),
			   InputOutput,
			   DefaultVisual(mydisplay, screen_num),
			   valuemask, &myat);

                 /* 3. give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, mywindow, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, mywindow, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, mywindow, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, mywindow, &iconName);

                 /* 4. establish window resources */
                 /* 5. create all the other windows needed */
                 /* 6. select events for each window */
                 /* 7. map the windows */
  XMapWindow(mydisplay, mywindow);

                 /* 8. enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type) {
    case ButtonPress:
      break;
    }
  }

		 /* 9. clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
