/*  First a window with a black and white checker-board pattern is
 *  drawn.  Two rectangles are then drawn on that window.  The
 *  background of each of these two windows is white in colour.
 *  A GC is then created having a foreground colour of black.
 *  This GC is used to paint the foreground of the two windows
 *  black in colour.  A third is created with a black background
 *  and is displayed overlaying the two windows.  This overlaying
 *  window is then removed.  This process is event driven with a 2
 *  second delay in the event loop.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2011
 */

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>
#include  <unistd.h>

#define backing_width 16
#define backing_height 16
static unsigned char backing_bits[] = {
   0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
   0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff,
   0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
   0x00, 0xff};

int main(int argc, char *argv[])
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow, win1, win2, ontop;
  XWindowChanges alter;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  GC             gc;
  char *window_name = "Uncover";
  char *icon_name = "Uc";
  int            screen_num, done;
  unsigned long  valuemask;
  Pixmap         back;
  int            count;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

               /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ExposureMask;
  myat.save_under = True;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask
              | CWSaveUnder;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           200, 300, 350, 250, 2,
			   DefaultDepth(mydisplay, screen_num),
			   InputOutput,
			   DefaultVisual(mydisplay, screen_num),
			   valuemask, &myat);
  back = XCreatePixmapFromBitmapData(mydisplay, mywindow,
			      backing_bits, backing_width,
			      backing_height,
			      BlackPixel(mydisplay, screen_num),
			      WhitePixel(mydisplay, screen_num),
			      DefaultDepth(mydisplay, screen_num));
  XSetWindowBackgroundPixmap(mydisplay, mywindow, back);

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
  gc = XCreateGC(mydisplay, mywindow, 0, NULL);
  XSetForeground(mydisplay, gc, BlackPixel(mydisplay, screen_num));
  XSetBackground(mydisplay, gc, WhitePixel(mydisplay, screen_num));

               /* 5.  create all the other windows needed */
  win1 = XCreateWindow(mydisplay, mywindow,
                        100, 30, 50, 70, 2,
		        DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &myat);
  win2 = XCreateWindow(mydisplay, mywindow,
                        100, 150, 150, 30, 2,
		        DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &myat);
  myat.background_pixel = BlackPixel(mydisplay, screen_num);
  ontop = XCreateWindow(mydisplay, mywindow,
                        120, 40, 80, 130, 2,
		        DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &myat);

               /* 6.  select events for each window */
               /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);
  XMapWindow(mydisplay, win1);
  XMapWindow(mydisplay, win2);

               /* 8.  enter the event loop */
  done = 0;
  count = 0;
  while ( done == 0 )  {
    XFlush(mydisplay);
    XNextEvent(mydisplay, &myevent);
    sleep(2);
    switch (myevent.type) {
    case  Expose:
      count++;
      switch (count)  {
      case 1:
        XFillRectangle(mydisplay, win1, gc, 0, 0, 50, 70);
        XFillRectangle(mydisplay, win2, gc, 0, 0, 150, 30);
	break;
      case 3:
        XMapWindow(mydisplay, ontop);
	break;
      case 6:
        XUnmapWindow(mydisplay, ontop);
	break;
      case 9:
        XUnmapWindow(mydisplay, win2);
	break;
      default:
        break;
      }
      break;
    }
  }

               /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
