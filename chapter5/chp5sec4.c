/*  This program demonstrates placement of a single line of text
 *  in a window which is setup for that purpose.  The line of
 *  text is too long to be displayed in the window.
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
  Window         baseWindow, textWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  XGCValues      myGCvalues;
  XFontStruct    *font1;
  char *window_name = "Text";
  char *icon_name   = "Te";
  char *textline = "Six white boomers, Snow white boomers, Racing";
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
                             100, 100, 300, 200, 2,
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
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, baseWindow, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, baseWindow, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, baseWindow, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);

	       /* 4.  establish window resources */
  myGCvalues.background = WhitePixel(mydisplay, screen_num);
  myGCvalues.foreground = BlackPixel(mydisplay, screen_num);
  mymask = GCForeground | GCBackground;
  mygc = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);
  font1 = XLoadQueryFont(mydisplay,
            "-adobe-times-bold-r-normal--0-0-0-0-p-0-iso8859-1");
  printf("ascent = %d\ndescent = %d\n",
           font1->ascent, font1->descent);
  XSetFont(mydisplay, mygc, font1->fid);


	       /* 5.  create all the other windows needed */
  myat.background_pixel = BlackPixel(mydisplay, screen_num);
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  textWindow = XCreateWindow(mydisplay, baseWindow,
                             30, 40, 140, 26, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, textWindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch ( baseEvent.type )  {
    case  Expose:
      XDrawImageString(mydisplay, textWindow, mygc,
                       30, 20, textline, strlen(textline));
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
}
