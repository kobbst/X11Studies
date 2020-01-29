/*  This program consists of a main window on which is placed
 *  three text windows: two windows for text input and the
 *  other for display of all the text entered through the other
 *  two windows.  The text entered is also echoed in that
 *  window.  All four text streams have a different font.  All
 *  four windows have white backgrounds with the boundary of
 *  each text window shown by its border.  A text label is
 *  displayed against each text window.  The program is
 *  terminated by typing the 'down arrow' key.
 *
 *  Coded by:  Ross Maloney
 *  Date:      November 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

#define  BUFFER_LENGTH   10

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, textWindow1, textWindow2,
                   textWindow3;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc, myGC1, myGC2, myGC3;
  XGCValues      myGCvalues;
  KeySym         sym;
  XFontStruct    *font1, *font2;
  XTextItem      myText;
  char *window_name = "Echoing";
  char *icon_name   = "Ec";
  char *label1      = "input A:";
  char *label2      = "input B:";
  char *label3      = "All here:";
  int            screen_num, done;
  unsigned long  mymask;
  int            x, i;
  int            yWindow1, yWindow2, yWindow3, width;
  char           buffer[BUFFER_LENGTH];

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
                             300, 400, 550, 400, 2,
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
  XRebindKeysym(mydisplay, XK_Meta_L, NULL, 0, "MetaL", 5);
  myGCvalues.background = WhitePixel(mydisplay, screen_num);
  myGCvalues.foreground = BlackPixel(mydisplay, screen_num);
  mymask = GCForeground | GCBackground;
  mygc = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);
  font1 = XLoadQueryFont(mydisplay,
     "-adobe-palatino-medium-i-normal--0-0-0-0-p-0-iso8859-1");
  font2 = XLoadQueryFont(mydisplay,
     "-adobe-times-bold-r-normal--0-0-0-0-p-0-iso8859-1");
  myGC1 = XCreateGC(mydisplay, baseWindow, mymask, &myGCvalues);

	       /* 5.  create all the other windows needed */
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  textWindow3 = XCreateWindow(mydisplay, baseWindow,
                              140, 170, 300, 180, 2,
                              DefaultDepth(mydisplay, screen_num),
			      InputOutput,
			      DefaultVisual(mydisplay, screen_num),
			      mymask, &myat);
  myat.event_mask = KeyPressMask | ButtonPressMask | ExposureMask;
  textWindow1 = XCreateWindow(mydisplay, baseWindow,
                              140, 50, 200, 20, 2,
                              DefaultDepth(mydisplay, screen_num),
			      InputOutput,
			      DefaultVisual(mydisplay, screen_num),
			      mymask, &myat);
  textWindow2 = XCreateWindow(mydisplay, baseWindow,
                              140, 110, 200, 20, 2,
                              DefaultDepth(mydisplay, screen_num),
			      InputOutput,
			      DefaultVisual(mydisplay, screen_num),
			      mymask, &myat);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, textWindow1);
  XMapWindow(mydisplay, textWindow2);
  XMapWindow(mydisplay, textWindow3);

	       /* 8.  enter the event loop */
  done = 0;
  yWindow1 = yWindow2 = yWindow3 = 0;
  myText.chars = buffer;
  myText.nchars = 1;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch( baseEvent.type )  {
    case Expose:
      XDrawImageString(mydisplay, baseWindow, mygc,
                       85, 65, label1, strlen(label1));
      XDrawImageString(mydisplay, baseWindow, mygc,
                       85, 125, label2, strlen(label2));
      XDrawImageString(mydisplay, baseWindow, mygc,
                       78, 185, label3, strlen(label3));
      break;
    case ButtonPress:
      break;
    case KeyPress:
      if ( baseEvent.xkey.keycode == 88 )  {
        done = 1;
	break;
      }
      x = XLookupString(&baseEvent.xkey, buffer, BUFFER_LENGTH,
                        &sym, NULL);
      sym = XKeycodeToKeysym(mydisplay, baseEvent.xkey.keycode,
                             1);
      if ( baseEvent.xkey.window == textWindow1 )  {
	myText.font = font1 -> fid;
	XDrawText(mydisplay, textWindow1, myGC1, yWindow1, 15,
	          &myText, 1);
	width = XTextWidth(font1, buffer, 1);
	yWindow1 += width;
      }
      if ( baseEvent.xkey.window == textWindow2 )  {
	myText.font = font2 -> fid;
	XDrawText(mydisplay, textWindow2, myGC1, yWindow2, 15,
	          &myText, 1);
	width = XTextWidth(font1, buffer, 1);
	yWindow2 += width;
      }
      XDrawText(mydisplay, textWindow3, myGC1, yWindow3, 15,
                &myText, 1);
      yWindow3 += width;
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XUnmapWindow(mydisplay, textWindow1);
  XUnmapWindow(mydisplay, textWindow2);
}
