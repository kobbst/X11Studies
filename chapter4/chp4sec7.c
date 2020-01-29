/*  This program draws a 100 by 200 pixel base window.  An image
 *  is created from a bitmap pattern of the character E that had
 *  been created externally to this program.  That bitmap
 *  pattern is stored in this program.  The program converts that
 *  pattern to the X Window System pixmap format and that pixmap
 *  format is written onto the base window using two different
 *  sets of foreground and background colours.
 *
 *  Coded by:  Ross Maloney
 *  Date:      July 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>


#define e_width 45
#define e_height 35
static char e_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff,
  0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0xe0,
  0x0f, 0xf8, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xe0, 0x00, 0x00, 
  0x00, 0xc0, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xc6,
  0x00, 0x00, 0x00, 0xc0, 0x0f, 0xc6, 0x00, 0x00, 0x00, 0xc0,
  0x0f, 0x06, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x07, 0x00, 0x00,
  0x00, 0xc0, 0x0f, 0x07, 0x00, 0x00, 0x00, 0xc0, 0xcf, 0x07,
  0x00, 0x00, 0x00, 0xc0, 0xff, 0x07, 0x00, 0x00, 0x00, 0xc0,
  0xff, 0x07, 0x00, 0x00, 0x00, 0xc0, 0x8f, 0x07, 0x00, 0x00, 
  0x00, 0xc0, 0x0f, 0x07, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x06,
  0x00, 0x00, 0x00, 0xc0, 0x0f, 0x06, 0x01, 0x00, 0x00, 0xc0,
  0x0f, 0x86, 0x01, 0x00, 0x00, 0xc0, 0x0f, 0xc0, 0x01, 0x00,
  0x00, 0xc0, 0x0f, 0xc0, 0x01, 0x00, 0x00, 0xc0, 0x0f, 0xe0,
  0x01, 0x00, 0x00, 0xe0, 0x0f, 0xf8, 0x01, 0x00, 0x00, 0xf8,
  0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 

int main(int argc, char *argv)
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  char *window_name = "Image";
  char *icon_name = "Im";
  XEvent         myevent;
  XGCValues      myGCvalues;
  GC             imageGC;
  Pixmap         pattern;
  XImage         *local;
  int            screen_num, done, x, y;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ButtonPressMask | ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           300, 50, 100, 200, 3,
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

	       /* 4.  establish window reqources */
  pattern = XCreatePixmapFromBitmapData(mydisplay, mywindow,
                        e_bits, e_width, e_height,
			WhitePixel(mydisplay, screen_num),
			BlackPixel(mydisplay, screen_num),
			DefaultDepth(mydisplay, screen_num));
  imageGC = XCreateGC(mydisplay, mywindow, 0, NULL);

	       /* 5.  create all the other windows needed */

	       /* 6.  select events for each window */

	       /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type)  {
    case Expose:
      XSetBackground(mydisplay, imageGC, 0xff00);
      XSetForeground(mydisplay, imageGC,
                     BlackPixel(mydisplay, screen_num));
      XCopyPlane(mydisplay, pattern, mywindow, imageGC, 0, 0,
                 e_width, e_height, 10, 10, 1);
      XSetForeground(mydisplay, imageGC, 0xff00);
      XSetBackground(mydisplay, imageGC,
                 BlackPixel(mydisplay, screen_num));
      XCopyPlane(mydisplay, pattern, mywindow, imageGC, 0, 0,
                 e_width, e_height, 10, 100, 1);
      break;
    case ButtonPress:
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
