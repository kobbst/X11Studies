/*  A program to scroll a line of text horizontally.  This is
 *  done to view portions of the line which is too long to fit
 *  into the viewing window.  A slider is used to move the
 *  viewing window along the line of text to bring the required
 *  continuous section of text into view.
 *
 *  Coded by:  Ross Maloney
 *  Date:      February 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>


int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, textWindow, sliderWindow,
                  sliderbedWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  XGCValues      myGCvalues;
  XFontStruct    *font1;
  XColor         white, black, grey;
  Pixmap         buffer;
  char *window_name = "Hscroll";
  char *icon_name   = "Hs";
  char *textline =
         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int            screen_num, done, x;
  unsigned long  mymask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  black.pixel = BlackPixel(mydisplay, screen_num);
  white.pixel = WhitePixel(mydisplay, screen_num);
  grey.pixel = 0xd3d3d3;
  myat.border_pixel = black.pixel;
  myat.background_pixel = white.pixel;
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
  myGCvalues.background = white.pixel;
  myGCvalues.foreground = black.pixel;
  mymask = GCForeground | GCBackground;
  mygc = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);
  font1 = XLoadQueryFont(mydisplay,
            "-adobe-times-bold-r-normal--0-0-0-0-p-0-iso8859-1");

	       /* 5.  create all the other windows needed */
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  myat.background_pixel = black.pixel;
  textWindow = XCreateWindow(mydisplay, baseWindow,
                             30, 40, 140, 26, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  myat.background_pixel = grey.pixel;
  myat.event_mask = ExposureMask | Button1MotionMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderbedWindow = XCreateWindow(mydisplay, baseWindow,
                             30, 80, 140, 11, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  myat.background_pixel = black.pixel;
  myat.event_mask = ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderWindow = XCreateWindow(mydisplay, sliderbedWindow,
                               0, 1, 14, 7, 1,
                               DefaultDepth(mydisplay, screen_num),
			       InputOutput,
			       DefaultVisual(mydisplay, screen_num),
			       mymask, &myat);
  buffer = XCreatePixmap(mydisplay, baseWindow, 1000, 26,
                         DefaultDepth(mydisplay, screen_num));
  XFillRectangle(mydisplay, buffer, mygc,
                       0, 0, 1000, 26);
  XDrawImageString(mydisplay, buffer, mygc,
                       0, 20, textline, strlen(textline));

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, textWindow);
  XMapWindow(mydisplay, sliderbedWindow);
  XMapWindow(mydisplay, sliderWindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch ( baseEvent.type )  {
    case  Expose:
      XCopyArea(mydisplay, buffer, textWindow, mygc, x, 0,
                140, 20, 0, 0);
      break;
    case  ButtonPress:
      break;
    case  ButtonRelease:
      break;
    case  MotionNotify:
      XMoveWindow(mydisplay, sliderWindow,
                  baseEvent.xmotion.x-7, 1);
      x = baseEvent.xmotion.x;
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
}
