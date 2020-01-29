/*  This program implements hierarchical menus.  The base 400x400
 *  pixel window contains two menu-bar buttons.  The button on
 *  the left hand side is connected to a menu of three menu items.
 *  The bottom item of that menu is connected to a menu of two
 *  items, and the top one of those menu items is connected to
 *  another three item menu.  Each menu item is a blank window
 *  which changes colour when the mouse pointer moves over it.
 *
 *  As the mouse pointer enters a menu item window, it is
 *  highlighted and if a menu leads from it, that is displayed.
 *  When the mouse pointer leaves a menu item, it ceases to be
 *  highlighted and any menu of menu items leading from it are
 *  removed from the display.  The left-hand mouse button is used
 *  to select a menu item.
 *
 *  Coded by:  Ross Maloney
 *  Date:      June 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseW;
  struct {
    Window  id;
    int     homemenu;
    int     menudepth;
    int     shown;
    int     action;
  } W[13] = {
        {0, 1, 1, 0, 2},
	{1, 1, 1, 0, 1100},
	{2, 2, 2, 0, 1000},
	{3, 2, 2, 0, 3},
	{4, 2, 2, 0, 4},
	{5, 3, 3, 0, 1000},
	{6, 3, 3, 0, 1000},
	{7, 4, 3, 0, 5},
	{8, 4, 3, 0, 1100},
	{9, 5, 4, 0, 1000},
	{10, 5, 4, 0, 1000},
	{11, 5, 4, 0, 1000},
	{12, 0, 0, 0, 0}
    };
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  char *window_name = "Hierarchy";
  char *icon_name   = "Hie";
  int            screen_num, done, status, i, window;
  unsigned long  mymask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.border_pixel = 0xFF0000;    /* red */
  myat.background_pixel = 0xFFDEAD; /* navajo white */
  myat.event_mask = ExposureMask | EnterWindowMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        350, 400, 400, 400, 2,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			mymask, &myat);

	       /* 3.  give the Window Manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisplay, baseW, &wmsize);
  wmhints.initial_state = NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisplay, baseW, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisplay, baseW, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisplay, baseW, &iconName);

	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
  myat.background_pixel = 0xFFFFFF; /* white */
  myat.event_mask = ButtonPressMask | ButtonReleaseMask
             | ExposureMask | EnterWindowMask | LeaveWindowMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  W[0].id = XCreateWindow(mydisplay, baseW,
                          50, 50, 90, 20, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[1].id = XCreateWindow(mydisplay, baseW, 250, 100, 70, 30, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[2].id = XCreateWindow(mydisplay, baseW, 70, 60, 90, 20, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[3].id = XCreateWindow(mydisplay, baseW, 70, 80, 90, 20, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[4].id = XCreateWindow(mydisplay, baseW, 70, 100, 90, 20, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[5].id = XCreateWindow(mydisplay, baseW, 140, 90, 60, 10, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[6].id = XCreateWindow(mydisplay, baseW, 140, 100, 60, 10, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[7].id = XCreateWindow(mydisplay, baseW, 140, 110, 60, 10, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[8].id = XCreateWindow(mydisplay, baseW, 140, 120, 60, 10, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[9].id = XCreateWindow(mydisplay, baseW, 200, 110, 100, 30, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[10].id = XCreateWindow(mydisplay, baseW, 200, 140, 100, 30, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[11].id = XCreateWindow(mydisplay, baseW, 200, 170, 100, 30, 2,
			  DefaultDepth(mydisplay, screen_num),
			  InputOutput,
			  DefaultVisual(mydisplay, screen_num),
			  mymask, &myat);
  W[12].id = baseW;

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);
  for (i=0; i<2; i++)  {
    XMapWindow(mydisplay, W[i].id);
    W[i].shown = 1;
  }

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    window = -1;
    for (i=0; i<13; i++)
      if ( W[i].id == baseEvent.xany.window )  {
        window = i;
	break;
      }
    switch (baseEvent.type)  {
    case  Expose:
      XMapWindow(mydisplay, baseW);
      for (i=0; i<12; i++)
        if ( W[i].shown == 1 )  XMapWindow(mydisplay, W[i].id);
      break;
    case  ButtonPress:
      XUngrabPointer(mydisplay, CurrentTime);
      switch ( W[window].action )  {
      case 1000:
         XBell(mydisplay, 50);
	 break;
      case 1100:
         done = 1;
	 break;
      }
      break;
    case  ButtonRelease:
      break;
    case  EnterNotify:
      if ( i == 12 ) 
        for (i=2; i<12; i++)  W[i].shown = 0;
      else  {
        XSetWindowBackground(mydisplay, W[window].id, 0xFF0000);
        XClearWindow(mydisplay, W[window].id);
        for (i=0; i<12; i++)  {
	  if ( W[i].menudepth > W[window].menudepth )
	          W[i].shown = 0;
	  if ( W[i].homemenu == W[window].action )
	          W[i].shown = 1;
	}
      }
      for (i=0; i<12; i++)
        if ( W[i].shown == 1 )  XMapWindow(mydisplay, W[i].id);
	else  XUnmapWindow(mydisplay, W[i].id);
      XFlush(mydisplay);
      break;
    case  LeaveNotify:
      XSetWindowBackground(mydisplay, W[window].id, 0xFFFFFF);
      XClearWindow(mydisplay, W[window].id);
      break; 
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
