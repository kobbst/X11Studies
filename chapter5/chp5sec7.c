/*  A program which produces a window containing a vertical slider
 *  bar.  The slider is picked up by clicking the left-hand mouse
 *  button over the slider.  While that button is depressed the
 *  slider can be moved along the slider bed with the end of the
 *  motion indicated by releasing that mouse button.  The
 *  coordinates of the slider are printed on the terminal screen
 *  as the slider is moved.
 *
 *  Coded by:  Ross Maloney
 *  Date:      February 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, sliderWindow, sliderbedWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  XGCValues      myGCvalues;
  XFontStruct    *font1;
  char *window_name = "Slider";
  char *icon_name   = "Sb";
  int            screen_num, done;
  unsigned long  mymask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.event_mask = ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseWindow = XCreateWindow(mydisplay,
                             RootWindow(mydisplay, screen_num),
                             100, 100, 200, 200, 2,
			     DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);

	       /* 3.  give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, baseWindow, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, baseWindow, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, baseWindow, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, baseWindow, &iconName);

	       /* 4.  establish window resources */

	       /* 5.  create all the other windows needed */
  myat.background_pixel = 0xd3d3d3;
  myat.event_mask = ExposureMask | Button1MotionMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderbedWindow = XCreateWindow(mydisplay, baseWindow,
                             90, 30, 11, 140, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  myat.background_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderWindow = XCreateWindow(mydisplay, sliderbedWindow,
                               1, 0, 7, 14, 1,
                               DefaultDepth(mydisplay, screen_num),
			       InputOutput,
			       DefaultVisual(mydisplay, screen_num),
			       mymask, &myat);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, sliderbedWindow);
  XMapWindow(mydisplay, sliderWindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch ( baseEvent.type )  {
    case  Expose:
      break;
    case  ButtonPress:
      break;
    case  ButtonRelease:
      break;
    case  MotionNotify:
      printf("Moving to: x = %d   y = %d\n",
             baseEvent.xmotion.x, baseEvent.xmotion.y);
      XMoveWindow(mydisplay, sliderWindow, 1,
                  baseEvent.xmotion.y - 7);
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XUnmapWindow(mydisplay, sliderbedWindow);
  XUnmapWindow(mydisplay, sliderWindow);
}
