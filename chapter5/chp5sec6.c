/*  This program consists of a main window on which is placed four
 *  text input windows.  These windows are to hold the date, name
 *  of the receiver, subject, and the message.  Each window
 *  contains a single line of text 20 characters in length.
 *  There is no editing facilities nor insertion cursor on any of
 *  these *  windows.  However, the up arrow and down arrow
 *  keyboard keys move the keyboard focus the next window above or
 *  below, respectively, for receiving the next character from the
 *  keyboard.  These four windows are connected to form a ring.
 *
 *  Coded by:  Ross Maloney
 *  Date:      June 2011
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  XGCValues      myGCvalues;
  XFontStruct    *font1;
  char *window_name = "Text window switching";
  char *icon_name = "Swt";
  int            screen_num, done, y, i, index, charinc, count;
  unsigned long  mymask;
  char           bytes[3];
  KeySym         character;
  XComposeStatus cs;
  struct  {                   /* Input window ring structure */
    Window     id;
    int        last;
    char       array[20];
  }  ring[4];

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
                             100, 100, 250, 270, 2,
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
  myGCvalues.background = WhitePixel(mydisplay, screen_num);
  myGCvalues.foreground = BlackPixel(mydisplay, screen_num);
  mymask = GCForeground | GCBackground;
  mygc = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);
  font1 = XLoadQueryFont(mydisplay,
                "-b&h-lucidatypewriter-*-*-*-*-14-*-*-*-*-*-*");
  XSetFont(mydisplay, mygc, font1->fid);
  charinc = font1->per_char->width;

	       /* 5.  create all the other windows needed */
  y = 30;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  myat.event_mask = KeyPressMask;
  for (i=0; i<4; i++)  {
    ring[i].id = XCreateWindow(mydisplay, baseWindow, 70,
                             y, 20*charinc, 20, 2,
			     DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);
    ring[i].last = 0;
    y += 60;
  }

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  for (i=0; i<4; i++) XMapWindow(mydisplay, ring[i].id);

	       /* 8.  enter the event loop */
  index = 0;
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch ( baseEvent.type )  {
    case  Expose:
      break;
    case  KeyPress:
      count = XLookupString(&baseEvent.xkey, bytes, 3,
                            &character, &cs);
      index = 0;
      for (i=0; i<4; i++)
        if ( ring[i].id == baseEvent.xkey.window ) index = i;
      switch ( count )  {
      case 0:       /* Control character */
	switch ( character )  {
	case XK_Up:           /* Up arrow key */
	case XK_KP_Up:
	  index--;
	  if (index < 0 )  index = 3;
	  XWarpPointer(mydisplay, None, ring[index].id,
	               0, 0, 0, 0, ring[index].last*charinc, 10);
	  break;
	case XK_Down:         /* Down arrow key */
	case XK_KP_Down:
	  index++;
	  if (index > 3 )  index = 0;
	  XWarpPointer(mydisplay, None, ring[index].id,
	               0, 0, 0, 0, ring[index].last*charinc, 10);
	  break;
	}
        break;
      case 1:       /* Printable character */
	ring[index].array[ring[index].last] = bytes[0];
	XDrawString(mydisplay, ring[index].id, mygc,
	            ring[index].last*charinc, 15, bytes, 1);
	ring[index].last++;
        break;
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
}
