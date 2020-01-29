/*  A program to show lines of the same alphabetic character on a
 *  white window,  Each line of characters is drawn in the same
 *  font which is scaled larger with each successive line.
 *
 *  Coded by:  Ross Maloney
 *  Date:      October 2017
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>    /* for exit() */

char  message[] = "AaBbCcDdEeFfGgHhIiJjKk";
char  fontName1[] =
       "-adobe-courier-bold-i-normal--0-120-75-75-p-0-iso8859-1";
char  fontName2[] =
       "-adobe-courier-bold-i-normal--*-240-75-75-p-0-iso8859-1";
char  fontName3[] =
       "-adobe-courier-bold-i-normal--*-420-75-75-p-0-iso8859-1";
char  fontName4[] =
       "-adobe-courier-bold-i-normal--*-720-75-75-p-0-iso8859-1";

main()
{
  Display        *mydisplay;
  XSetWindowAttributes  myat;
  Window         mywindow;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  GC             myGC1, myGC2, myGC3, myGC4;
  XGCValues      myGCvalues;
  XFontStruct    *fontDetail;
  char *window_name = "Scaling";
  char *icon_name   = "Sc";
  int            screen_num, done, i;
  unsigned long  valuemask;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

               /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.event_mask = ExposureMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywindow = XCreateWindow(mydisplay,
                           RootWindow(mydisplay, screen_num),
                           60, 70, 500, 300, 3,
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

               /* 4.  establish window resources */
  myGCvalues.foreground = BlackPixel(mydisplay, screen_num);
  valuemask = GCForeground | GCFont;
  if ( ( fontDetail = XLoadQueryFont(mydisplay, fontName1) )
              == NULL )  {
    printf("Could not load font %s\n", fontName1);
    exit(1);
  }
  myGCvalues.font = fontDetail->fid;
  myGC1 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);
  if ( ( fontDetail = XLoadQueryFont(mydisplay, fontName2) )
              == NULL )  {
    printf("Could not load font %s\n", fontName2);
    exit(1);
  }
  myGCvalues.font = fontDetail->fid;
  myGC2 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);
  if ( ( fontDetail = XLoadQueryFont(mydisplay, fontName3) )
              == NULL )  {
    printf("Could not load font %s\n", fontName3);
    exit(1);
  }
  myGCvalues.font = fontDetail->fid;
  myGC3 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);
  if ( ( fontDetail = XLoadQueryFont(mydisplay, fontName4) )
              == NULL )  {
    printf("Could not load font %s\n", fontName2);
    exit(1);
  }
  myGCvalues.font = fontDetail->fid;
  myGC4 = XCreateGC(mydisplay, mywindow, valuemask, &myGCvalues);

               /* 5.  create all the other windows needed */
               /* 6.  select events for each window */
               /* 7.  map the windows */
  XMapWindow(mydisplay, mywindow);

               /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type)  {
    case  Expose:
      XDrawString(mydisplay, mywindow, myGC1, 10, 20, message,
                  strlen(message));
      XDrawString(mydisplay, mywindow, myGC2, 10, 90, message,
                  strlen(message));
      XDrawString(mydisplay, mywindow, myGC3, 10, 180, message,
                  strlen(message));
      XDrawString(mydisplay, mywindow, myGC4, 10, 280, message,
                  strlen(message));
      break;
    }
  }
               /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, mywindow);
  XDestroyWindow(mydisplay, mywindow);
  XCloseDisplay(mydisplay);
}
