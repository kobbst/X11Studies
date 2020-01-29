/*  This program consists of a main window on which is placed a
 *  selection button.  The selection button is green in colour
 *  with the label 'selection' in pink characters.  By clicking
 *  the left mouse button on this selection button an option menu
 *  of 'flowers', 'pets', and 'quit' appears, each option
 *  labelled in blue with a pink background.  On moving the mouse
 *  pointer to each option, the pink background of the option
 *  changes to red.  Clicking the right mouse button on the 'quit'
 *  option terminates the program.
 *
 *  Coded by:  Ross Maloney
 *  Date:      July 2006
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

static char *labels[] = {"Selection", "flowers", "pets", "quit"};

static char *colours[] = {"green", "pink", "blue", "red"};

int main(int argc, char *argv)
{
  Display        *mydisplay;
  XSetWindowAttributes  myat, buttonat, popat;
  Window         mywindow, button, optA1, panes[3];
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  XColor         exact, closest;
  GC             myGC1, myGC2, myGC3;
  XGCValues      myGCvalues;
  char *window_name = "Select";
  char *icon_name   = "Sel";
  int            screen_num, done, i;
  unsigned long  valuemask;
  int            labelLength[4], currentWindow;
  unsigned long  colourBits[6];

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

               /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  for (i=0; i<4; i++)  labelLength[i] = strlen(labels[i]);
  colourBits[0] = WhitePixel(mydisplay, screen_num);
  colourBits[1] = BlackPixel(mydisplay, screen_num);
  myat.background_pixel = colourBits[0];
  myat.border_pixel = colourBits[1];
  valuemask = CWBackPixel | CWBorderPixel;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           300, 300, 350, 400, 3,
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
  for (i=0; i<4; i++) {
          XAllocNamedColor(mydisplay,
	                   XDefaultColormap(mydisplay, screen_num),
	                   colours[i], &exact, &closest);
	  colourBits[i+2] = exact.pixel;
  }
  myGCvalues.background = colourBits[2];  /* green */
  myGCvalues.foreground = colourBits[3];  /* pink */
  valuemask = GCForeground | GCBackground;
  myGC1 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);
  myGCvalues.background = colourBits[3];  /* pink */
  myGCvalues.foreground = colourBits[4];  /* blue */
  myGC2 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);
  myGCvalues.background = colourBits[5];  /*red */
  myGC3 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);

               /* 5.  create all the other windows needed */
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  buttonat.background_pixel = colourBits[2];  /* green */
  buttonat.border_pixel = colourBits[1];
  buttonat.event_mask = ButtonPressMask | ExposureMask
                        | Button1MotionMask;
  button = XCreateWindow(mydisplay, mywindow,
                         20, 50, 70, 30, 2,
			 DefaultDepth(mydisplay, screen_num),
			 InputOutput,
			 DefaultVisual(mydisplay, screen_num),
			 valuemask, &buttonat);
  popat.border_pixel = colourBits[1];
  popat.background_pixel = colourBits[3];  /* pink */
  popat.event_mask = 0;
  optA1 = XCreateWindow(mydisplay, mywindow,
                         50, 60, 100, 150, 2,
			 DefaultDepth(mydisplay, screen_num),
			 InputOutput,
			 DefaultVisual(mydisplay, screen_num),
			 valuemask, &popat);
  popat.event_mask = ButtonPressMask | EnterWindowMask
                     | LeaveWindowMask | ExposureMask;
  for (i=0; i<3; i++)
    panes[i] = XCreateWindow(mydisplay, optA1,
                             0, i*50, 100, 50, 2,
		             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
		             DefaultVisual(mydisplay, screen_num),
		             valuemask, &popat);

               /* 6.  select events for each window */
               /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);
  XMapWindow(mydisplay, button);

               /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type)  {
    case Expose:
      XDrawImageString(mydisplay, button, myGC1, 10, 17, labels[0],
                       labelLength[0]);
      break;
    case ButtonPress:
      XMapWindow(mydisplay, optA1);
      currentWindow = 0;
      for (i=0; i<3; i++)  {
        XMapWindow(mydisplay, panes[i]);
        XDrawImageString(mydisplay, panes[i],
                    myGC2, 0, 10, labels[i+1], labelLength[i+1]);
      }
      if ( myevent.xbutton.window == panes[2] )  done = 1;
      break;
    case EnterNotify:
      XSetWindowBackground(mydisplay, panes[currentWindow],
                           colourBits[3]);
      XClearWindow(mydisplay, panes[currentWindow]);
      XDrawImageString(mydisplay, panes[currentWindow],
                       myGC2, 0, 10, labels[currentWindow+1],
		       labelLength[currentWindow+1]);
      for (i=0; i<3; i++)
        if ( panes[i] == myevent.xcrossing.window )  {
	  currentWindow = i;
	  break;
	}
      XSetWindowBackground(mydisplay, myevent.xcrossing.window,
                           colourBits[5]);
      XClearWindow(mydisplay, myevent.xcrossing.window);
      XDrawImageString(mydisplay, panes[currentWindow],
                       myGC3, 0, 10, labels[currentWindow+1],
		       labelLength[currentWindow+1]);
      break;
    }
  }

               /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
