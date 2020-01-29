/*  This program consists of a main window and a single text entry
 *  window.  An insertion cursor is created using a pixmap.  With
 *  a foreground colour of red, this pixmap is used to show where
 *  the next character entered from the keyboard will be placed.
 *  A 18 pixel typewriter text font is used to show the keyboard
 *  characters entered.  The mouse pointer, triggered by the
 *  release of any mouse button can be used to position this
 *  insertion cursor.
 *
 *  Coded by:  Ross Maloney
 *  Date:      June 2011
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define cursoricon_width 6
#define cursoricon_height 24
static unsigned char cursoricon_bits[] = {
   0x21, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
   0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
   0x0c, 0x0c, 0x1e, 0x21};

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, textWindow;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             myGC;
  XGCValues      myGCvalues;
  XFontStruct    *font1;
  XColor         exact, closest;
  char *window_name = "Insertion Cursor";
  char *icon_name = "IC";
  int            screen_num, done, lightcyan, red, count;
  int            charinc, position, end, current, i;
  unsigned long  mymask;
  char           data[20], bytes[3];
  KeySym         character;
  XComposeStatus cs;
  Pixmap         cursor;

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
                             100, 100, 300, 200, 2,
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
  myGC = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);
  font1 = XLoadQueryFont(mydisplay,
             "-b&h-lucidatypewriter-*-*-*-*-18-*-*-*-*-*-*");
  XSetFont(mydisplay, myGC, font1->fid);
  charinc = font1->per_char->width;
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "LightCyan2", &exact, &closest);
  lightcyan = closest.pixel;
  myat.background_pixel = lightcyan;
  XChangeWindowAttributes(mydisplay, baseWindow, CWBackPixel,
                          &myat);
  XAllocNamedColor(mydisplay,
                   XDefaultColormap(mydisplay, screen_num),
                   "red", &exact, &closest);
  red = closest.pixel;
  cursor = XCreatePixmapFromBitmapData(mydisplay, baseWindow,
                        cursoricon_bits, cursoricon_width,
			cursoricon_height,
			red, WhitePixel(mydisplay, screen_num),
			DefaultDepth(mydisplay, screen_num));

	       /* 5.  create all the other windows needed */
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  myat.event_mask = ButtonReleaseMask | KeyPressMask
                    | ExposureMask ;
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  textWindow = XCreateWindow(mydisplay, baseWindow,
                             60, 40, 220, 26, 2,
                             DefaultDepth(mydisplay, screen_num),
			     InputOutput,
			     DefaultVisual(mydisplay, screen_num),
			     mymask, &myat);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, textWindow);

	       /* 8.  enter the event loop */
  current = end = 0;
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch ( baseEvent.type )  {
    case  Expose:
      break;
    case  ButtonRelease:
      position = baseEvent.xbutton.x/charinc;
      current = position;
      if ( position > end )  {
        position = end;
	current = end;
      }
      XClearWindow(mydisplay, textWindow);
      XCopyArea(mydisplay, cursor, textWindow, myGC, 0, 0, 6, 24,
         position * charinc, 2);
      XDrawString(mydisplay, textWindow, myGC, 0, 17,
                  &data[0], end);
      break;
    case  KeyPress:
      count = XLookupString(&baseEvent.xkey, bytes, 3,
                            &character, &cs);
      switch ( count )  {
      case 0:      /* Control character */
        break;
      case 1:      /* Printable character */
        switch ( bytes[0] )  {
	case 8:    /* Backspace */
	  current--;
	  XClearWindow(mydisplay, textWindow);
          XCopyArea(mydisplay, cursor, textWindow, myGC,
	            0, 0, 6, 24,
             current * charinc, 2);
	  for (i=current; i<end; i++)  data[i] = data[i+1];
	  end--;
          XDrawString(mydisplay, textWindow, myGC, 0, 17,
                      &data[0], end);
	  if ( current < 1 )  XBell(mydisplay, 50);
	  break;
	case 13:   /* Enter */
	  XBell(mydisplay, 50);
	  break;
	default:
	  end++;
	  for (i=end; i>current; i--)  data[i] = data[i-1];
	  data[current] = bytes[0];
	  current++;
	  XClearWindow(mydisplay, textWindow);
          XCopyArea(mydisplay, cursor, textWindow, myGC,
	            0, 0, 6, 24,
             current * charinc, 2);
          XDrawString(mydisplay, textWindow, myGC, 0, 17,
	              &data[0], end);
	}
        break;
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XDestroyWindow(mydisplay, baseWindow);
  XCloseDisplay(mydisplay);
}
