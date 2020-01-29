/*  This program creates a window coloured red a then two other
 *  windows contained inside it.  One of those additional windows
 *  is coloured white and the other is coloured black.  A cursor
 *  shaped, defined by two bitmaps created externally to this
 *  program are then linked to the mouse pointer which it is over
 *  the white window.
 *
 *  Coded by:  Ross Maloney
 *  Date:      May 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define arrow_width 16
#define arrow_height 16
static unsigned char arrow_bits[] = {
   0x00, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x3c, 0x00, 0xf8, 0x00,
   0xf8, 0x01, 0xf0, 0x07, 0xf0, 0x0f, 0xf0, 0x1f, 0xe0, 0x7f,
   0xe0, 0x7f, 0xc0, 0x7f, 0x80, 0x7f, 0x80, 0x7f, 0x00, 0x7f,
   0x00, 0x00};

#define arrowmask_width 16
#define arrowmask_height 16
#define arrowmask_x_hot 0
#define arrowmask_y_hot 0
static unsigned char arrowmask_bits[] = {
   0x1f, 0x00, 0x3f, 0x00, 0xff, 0x00, 0xff, 0x03, 0xff, 0x07,
   0xfe, 0x0f, 0xfc, 0x1f, 0xfc, 0x3f, 0xf8, 0x7f, 0xf8, 0xff,
   0xf0, 0xff, 0xf0, 0xff, 0xe0, 0xff, 0xc0, 0xff, 0x80, 0xff,
   0x80, 0xff};

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, wWindow, bWindow;
  XSetWindowAttributes  myat, wat, bat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  XColor         exact, closest, front, backing;
  Pixmap         backArrow, foreArrow;
  Cursor         cursor;
  char *window_name = "CursorPlay";
  char *icon_name   = "Play";
  int            screen_num, done;
  unsigned long  mymask;

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
                             400, 500, 600, 340, 3,
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
  backArrow = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                      arrowmask_bits, arrowmask_width,
		      arrowmask_height, 1, 0, 1);
  foreArrow = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                      arrow_bits, arrow_width, arrow_height,
		      1, 0, 1);
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "black", &exact, &front);
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "white", &exact, &backing);
  cursor = XCreatePixmapCursor(mydisplay, foreArrow, backArrow,
                      &front, &backing,
		      arrowmask_x_hot, arrowmask_y_hot);
  XDefineCursor(mydisplay, baseWindow, cursor);

	       /* 5.  create all the other windows needed */
  wat.event_mask = ButtonPressMask | ExposureMask;
  wat.background_pixel = WhitePixel(mydisplay, screen_num);
  bat.event_mask = ButtonPressMask | ExposureMask;
  bat.background_pixel = BlackPixel(mydisplay, screen_num);
  wWindow = XCreateWindow(mydisplay, baseWindow,
                          100, 50, 200, 200, 1,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &wat);
  bWindow = XCreateWindow(mydisplay, baseWindow,
                          400, 50, 100, 100, 1,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &bat);

	       /* 6.  select events for each window */

	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, wWindow);
  XMapWindow(mydisplay, bWindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch( baseEvent.type)  {
    case Expose:
      break;
    case ButtonPress:
      break;
    }
  } 
	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XDestroyWindow(mydisplay, baseWindow);
  XCloseDisplay(mydisplay);
}
