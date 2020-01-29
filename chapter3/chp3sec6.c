/*  This utility program responds to all mouse generated events
 *  under the X Window System.  A message indicating the nature
 *  of each mouse event received is sent to the console from
 *  where this *  program was started.  However, the motion
 *  event without a button depressed is not used.  This can be
 *  used to determine the suitability and usefulness of the mouse
 *  under X which is plugged into the box running the X Window
 *  System.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseW;
  XSetWindowAttributes  baseat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevents;
  char *window_name = "Xclick";
  char *icon_name   = "Xc";
  int            screen_num, done;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  baseat.background_pixel = WhitePixel(mydisplay, screen_num);
  baseat.border_pixel = BlackPixel(mydisplay, screen_num);
  baseat.event_mask = ExposureMask | ButtonPressMask
                      | ButtonReleaseMask | ButtonMotionMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        100, 100, 200, 200, 2,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &baseat);

	       /* 3.  give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, baseW, &wmsize);
  wmhints.initial_state = NormalState;wmhints.flags = StateHint;
  XSetWMHints(mydisplay, baseW, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, baseW, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, baseW, &iconName);
  XMapWindow(mydisplay, baseW);

	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevents);
    switch (myevents.type)  {
    case  Expose:
      break;
    case  ButtonPress:
      printf("Button pressed:  button = %d   state = %o\n",
             myevents.xbutton.button, myevents.xbutton.state);
      break;
    case  ButtonRelease:
      printf("Button released: button = %d   state = %o\n",
             myevents.xbutton.button, myevents.xbutton.state);
      break;
    case  MotionNotify:
      printf("Motion event:    state = %o\n",
             myevents.xmotion.state);
      break;
    default:
      printf("This should not happen\n");
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
