/*  The program displays a window coloured red.  When the
 *  left-hand mouse button is pressed while the pointer is in
 *  that window, a pattern patch is displayed at the location
 *  of the pointer.  The  pattern is recorded as a bitmap in the
 *  program and is displayed with a black foreground and a white
 *  background.
 *
 *  Coded by:  Ross Maloney
 *  Date:      May 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define shapes_width 50
#define shapes_height 25
static unsigned char shapes_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0xff, 0x00, 0x0e, 0x00, 0x00, 0x1f, 0xe0, 0xff, 0xc0,
   0x7f, 0x00, 0xc0, 0x60, 0xf0, 0xff, 0xe1, 0xff, 0x00, 0x30,
   0x80, 0xf1, 0xff, 0xf1, 0xff, 0x01, 0x08, 0x00, 0xf2, 0xff,
   0xf9, 0xff, 0x03, 0x08, 0x00, 0xf2, 0xff, 0xfd, 0xff, 0x03,
   0x04, 0x00, 0xf4, 0xff, 0xfd, 0xff, 0x03, 0x04, 0x00, 0xe4,
   0xff, 0xfc, 0xff, 0x03, 0xfa, 0x03, 0xe8, 0xff, 0xfe, 0xff,
   0x03, 0xfe, 0x07, 0xc8, 0x7f, 0xfe, 0xff, 0x03, 0xfe, 0x0f,
   0x08, 0x1f, 0xfe, 0xff, 0x03, 0xfe, 0x0f, 0x08, 0x00, 0xfc,
   0xff, 0x03, 0xfe, 0x0f, 0x08, 0x18, 0xfc, 0xff, 0x03, 0xfe,
   0x0f, 0x04, 0x3c, 0xfc, 0xff, 0x03, 0xfe, 0x0f, 0x04, 0x3e,
   0xf8, 0xff, 0x03, 0xfe, 0x0f, 0x02, 0x7f, 0xf0, 0xff, 0x01,
   0xfe, 0x0f, 0x02, 0xff, 0xe0, 0xff, 0x00, 0xfc, 0x87, 0x81,
   0xff, 0xc0, 0x7f, 0x00, 0xf8, 0x63, 0xc0, 0xff, 0x01, 0x0e,
   0x00, 0x00, 0x1f, 0xe0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0x03, 0x00, 0x00};

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
  Pixmap         pattern;
  char *window_name = "BWclick";
  char *icon_name   = "BW";
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

	       /* 4.  establish window resources */
  pattern = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                           shapes_bits, shapes_width,
			   shapes_height,
			   BlackPixel(mydisplay, screen_num),
			   WhitePixel(mydisplay, screen_num),
			   DefaultDepth(mydisplay, screen_num));
  mygc = XCreateGC(mydisplay, baseWindow, 0, NULL);
  XSetForeground(mydisplay, mygc,
                 WhitePixel(mydisplay, screen_num));
  XSetBackground(mydisplay, mygc,
                 BlackPixel(mydisplay, screen_num));

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
      if ( baseEvent.xbutton.button == Button1 )  {
        x = baseEvent.xbutton.x;
        y = baseEvent.xbutton.y;
        XCopyPlane(mydisplay, pattern, baseWindow, mygc, 0, 0,
                   shapes_width, shapes_height, x, y, 1);
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XDestroyWindow(mydisplay, baseWindow);
  XCloseDisplay(mydisplay);
}
