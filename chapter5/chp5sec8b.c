/*  This program scrolls vertically through a passage of text.  A
 *  vertical scroll bar is used to control the position of the
 *  viewing window, bringing in and removing a line of text as
 *  the viewing window is scrolled past each line of text.
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
  char *window_name = "Vscroll";
  char *icon_name   = "Vs";
  static char *lines[9] = {"Mary had a little lamb",
                           "Her father shot it dead",
			   "Now Mary takes the lamb to school",
			   "Between two hunks of bread",
			   "Now Mary is a very wise girl"
			   "And keeps her own counsel well",
			   "She never tells",
			   "That at home there is lamb stew",
			   "And fleece on the floor as well"};
  int            screen_num, done, i, y, newEnd, oldEnd;
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
                             10, 20, 140, 100, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  myat.background_pixel = grey.pixel;
  myat.event_mask = ExposureMask | Button1MotionMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderbedWindow = XCreateWindow(mydisplay, baseWindow,
                             160, 20, 11, 130, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  myat.background_pixel = black.pixel;
  myat.event_mask = ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  sliderWindow = XCreateWindow(mydisplay, sliderbedWindow,
                             1, 0, 7, 14, 1,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
  buffer = XCreatePixmap(mydisplay, baseWindow, 2000, 100,
                         DefaultDepth(mydisplay, screen_num));
  XFillRectangle(mydisplay, buffer, mygc, 0, 0, 2000, 100);
  XDrawImageString(mydisplay, buffer, mygc, 0, 14, lines[0],
                   strlen(lines[0]));
  for (i=1; i<5; i++)  {
    XDrawImageString(mydisplay, buffer, mygc,
                     0, 14 + 20*i, lines[i], strlen(lines[i]));
  }
  oldEnd = 4;
  newEnd = 4;

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
      if (newEnd  == oldEnd )  {
        XCopyArea(mydisplay, buffer, textWindow, mygc, 0, 0,
                  2000, 110, 0, 0);
      }
      if (newEnd > oldEnd)  {
	for (i=0; i<5; i++)  {
          XCopyArea(mydisplay, buffer, buffer, mygc, 0, 20*(i+1),
                    2000, 20, 0, 20*i);
	}
        XFillRectangle(mydisplay, buffer, mygc, 0, 80, 2000, 20);
	XDrawImageString(mydisplay, buffer, mygc,
                    0, 94, lines[newEnd], strlen(lines[newEnd]));
        XCopyArea(mydisplay, buffer, textWindow, mygc, 0, 80,
                  2000, 20, 0, 80);
	oldEnd = newEnd;
      }
      if (newEnd < oldEnd)  {
	for (i=4; i>0; i--)  {
          XCopyArea(mydisplay, buffer, buffer, mygc, 0, 20*(i-1),
                    2000, 20, 0, 20*i);
	}
        XFillRectangle(mydisplay, buffer, mygc, 0, 0, 2000, 20);
	XDrawImageString(mydisplay, buffer, mygc,
                  0, 14, lines[newEnd-4], strlen(lines[newEnd-4]));
        XCopyArea(mydisplay, buffer, textWindow, mygc, 0, 0,
                  2000, 20, 0, 0);
	oldEnd = newEnd;
      }
      break;
    case  ButtonPress:
      break;
    case  ButtonRelease:
      break;
    case  MotionNotify:
      XMoveWindow(mydisplay, sliderWindow, 1,
                  baseEvent.xmotion.y-7);
      y = baseEvent.xmotion.y;
      newEnd = 4 + (y + 7)/40;
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
}
