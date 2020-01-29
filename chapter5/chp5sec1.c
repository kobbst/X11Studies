/*  This program consists of a main window on which is placed two
 *  text input windows.  All three windows have white backgrounds
 *  with the boundary of each text window shown by its border.
 *  Each window responses to keyboard key presses and mouse
 *  button presses.  The nature of each press is printed on the
 *  console *  screen.
 *
 *  Coded by:  Ross Maloney
 *  Date:      October 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#define  BUFFER_LENGTH   10

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseWindow, textWindow1, textWindow2;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  KeySym         sym;
  char *window_name = "Inout";
  char *icon_name   = "IO";
  int            screen_num, done;
  unsigned long  mymask;
  int            x, i;
  char           buffer[BUFFER_LENGTH];

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.border_pixel = BlackPixel(mydisplay, screen_num);
  myat.background_pixel = WhitePixel(mydisplay, screen_num);
  myat.event_mask = KeyPressMask | ButtonPressMask | ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseWindow = XCreateWindow(mydisplay,
                             RootWindow(mydisplay, screen_num),
                             300, 300, 350, 400, 2,
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
	       /* 5.  create all the other windows needed */
  textWindow1 = XCreateWindow(mydisplay, baseWindow,
                              30, 80, 200, 20, 2,
                              DefaultDepth(mydisplay, screen_num),
			      InputOutput,
			      DefaultVisual(mydisplay, screen_num),
			      mymask, &myat);
  textWindow2 = XCreateWindow(mydisplay, baseWindow,
                              30, 200, 200, 20, 2,
                              DefaultDepth(mydisplay, screen_num),
			      InputOutput,
			      DefaultVisual(mydisplay, screen_num),
			      mymask, &myat);

	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseWindow);
  XMapWindow(mydisplay, textWindow1);
  XMapWindow(mydisplay, textWindow2);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch( baseEvent.type )  {
    case Expose:
      break;
    case ButtonPress:
      printf("I got a button press\n");
      break;
    case KeyPress:
      printf("I got a key press\n");
      if ( baseEvent.xkey.window == textWindow1 )
        printf("In top window\n");
      if ( baseEvent.xkey.window == textWindow2 )  {
        printf("In bottom window\n");
	x = XLookupString(&baseEvent.xkey, buffer, BUFFER_LENGTH,
	                  &sym, NULL);
	printf("Keycode = %d\n", baseEvent.xkey.keycode);
	sym = XKeycodeToKeysym(mydisplay,
	                       baseEvent.xkey.keycode, 1);
	printf("x = %d\n", x);
        printf("Keysym = %x   character = %c", sym, buffer[0]);
	for (i=1; i<x; i++)  printf("%c", buffer[i]);
	printf("\n");
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseWindow);
  XUnmapWindow(mydisplay, textWindow1);
  XUnmapWindow(mydisplay, textWindow2);
}
