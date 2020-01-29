/*  This program displays a window coloured red.  When the
 *  right-hand mouse button is pressed while the pointer is
 *  in that window, a pattern patch is displayed at the
 *  location of the pointer.  The pattern is of an arrow
 *  pointing to the top-left which is coloured black,
 *  surrounded by a thin white border.  This pattern is
 *  recorded as a bitmap in the program and is displayed
 *  using a clipping mask which also is stored as a pixmap.
 *  A transparent pixmap pattern results.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define arrow_width 16
#define arrow_height 16
static unsigned char arrow_bits[] = {
   0x00, 0x00, 0x06, 0x00, 0x1e, 0x00, 0x7c, 0x00, 0xfc, 0x01,
   0xf8, 0x07, 0xf8, 0x1f, 0xf8, 0x7f, 0xf0, 0x7f, 0xf0, 0x03,
   0xe0, 0x07, 0xe0, 0x06, 0xc0, 0x0c, 0xc0, 0x18, 0x80, 0x30,
   0x00, 0x00};

#define mask_width 16
#define mask_height 16
static unsigned char mask_bits[] = {
   0x07, 0x00, 0x1f, 0x00, 0x7f, 0x00, 0xf6, 0x01, 0xc6, 0x07,
   0x8e, 0x1f, 0x0c, 0x3e, 0x1c, 0xfc, 0x38, 0xfc, 0x38, 0xfc,
   0x78, 0x0f, 0xf0, 0x1f, 0xf0, 0x3f, 0xe0, 0x7d, 0xe0, 0x79,
   0xc0, 0x71};

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  XColor         exact, closest;
  GC             mygc;
  XGCValues      myGCValues;
  Pixmap         pattern, mask;
  char *window_name = "Transparent";
  char *icon_name   = "Tr";
  int            screen_num, done;
  unsigned long  mymask;
  int            x, y;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "red", &exact, &closest);
  myat.background_pixel = closest.pixel;
  myat.event_mask = ButtonPressMask | ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseWindow = XCreateWindow(mydisplay,
                             RootWindow(mydisplay, screen_num),
                             300, 300, 350, 400, 3,
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

	       /* 4.  establish window reqources */
  pattern = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                         arrow_bits, arrow_width, arrow_height,
			 WhitePixel(mydisplay, screen_num),
			 BlackPixel(mydisplay, screen_num),
			 DefaultDepth(mydisplay, screen_num));
  mask = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                         mask_bits, mask_width, mask_height,
			 1, 0, 1);
  mymask = GCForeground | GCBackground | GCClipMask;
  myGCValues.background = WhitePixel(mydisplay, screen_num);
  myGCValues.foreground = BlackPixel(mydisplay, screen_num);
  myGCValues.clip_mask = mask;
  mygc = XCreateGC(mydisplay, baseWindow, mymask, &myGCValues);

	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */

	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch( baseEvent.type )  {
    case Expose:
      break;
    case ButtonPress:
      if ( baseEvent.xbutton.button == Button3 )  {
        x = baseEvent.xbutton.x;
        y = baseEvent.xbutton.y;
	XSetClipOrigin(mydisplay, mygc, x, y);
        XCopyPlane(mydisplay, pattern, baseWindow, mygc, 0, 0,
                   arrow_width, arrow_height, x, y, 1);
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XDestroyWindow(mydisplay, baseWindow);
  XCloseDisplay(mydisplay);
}
