/*  This program creates a main window on which is a selection
 *  button.  That button is green in colour with the label
 *  'Selection' in pink characters.  By clicking the left mouse
 *  button on this button an option menu of 'flowers', 'pets',
 *  and 'quit' appears.  Each option is labelled in blue with a
 *  pink background.  On moving the mouse pointer over each
 *  option, the pink background changes to red.  Clicking the
 *  right-hand mouse button over the 'quit' option terminates
 *  the program.
 *
 *  Coded by:  Ross Maloney
 *  Date:      July 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include "labels.h"    /* bitmap representing all labels */

int main(int argc, char *arcv)
{
  Display        *mydisplay;
  XSetWindowAttributes  myat, buttonat, popat;
  Window         baseW, buttonW, optionW, panelsW[3];
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         myevent;
  XColor         exact, closest;
  GC             myGC1, myGC2, myGC3;
  Pixmap         pattern;
  XImage         *buttonL, *image2panels[3];
  unsigned long  valuemask;
  char *window_name = "Select";
  char *icon_name   = "Sel";
  int            screen_num, done, i;
  char *colours[] = {"white", "black", "green", "pink",
                     "blue", "red"};
  unsigned long  colourBits[6];

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  for (i=0; i<6; i++)  {
    XAllocNamedColor(mydisplay,
                     XDefaultColormap(mydisplay, screen_num),
                     colours[i], &exact, &closest);
    colourBits[i] = exact.pixel;
  }
  myat.background_pixel = colourBits[0];
  myat.border_pixel = colourBits[1];
  valuemask = CWBackPixel | CWBorderPixel;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        300, 300, 350, 400, 3,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			valuemask, &myat);

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
  myGC1 = XCreateGC(mydisplay, baseW, 0, NULL);
  XSetBackground(mydisplay, myGC1, colourBits[2]);
  XSetForeground(mydisplay, myGC1, colourBits[3]);
  myGC2 = XCreateGC(mydisplay, baseW, 0, NULL);
  XSetBackground(mydisplay, myGC2, colourBits[3]);
  XSetForeground(mydisplay, myGC2, colourBits[4]);
  myGC3 = XCreateGC(mydisplay, baseW, 0, NULL);
  XSetBackground(mydisplay, myGC3, colourBits[5]);
  XSetForeground(mydisplay, myGC3, colourBits[4]);

	       /* 5.  create all the other windows needed */
  buttonW = XCreateSimpleWindow(mydisplay, baseW, 20, 50,
                           selection_width, selection_height, 3,
                           colourBits[1], colourBits[0]);
  pattern = XCreateBitmapFromData(mydisplay, buttonW,
                           selection_bits, selection_width,
			   selection_height);
  buttonL = XGetImage(mydisplay, pattern, 0, 0,
                      selection_width, selection_height,
		      1, XYPixmap);
  buttonL->format = XYBitmap;
  optionW = XCreateSimpleWindow(mydisplay, baseW, 70, 80,
                                quit_width, 3*quit_height, 1,
                                colourBits[1], colourBits[1]);
  for (i=0; i<3; i++)
    panelsW[i] = XCreateSimpleWindow(mydisplay, optionW, 0,
                          i*quit_height, quit_width, quit_height,
			  1, colourBits[1], colourBits[0]);
  pattern = XCreateBitmapFromData(mydisplay, buttonW, flowers_bits,
                                  flowers_width, flowers_height);
  image2panels[0] = XGetImage(mydisplay, pattern, 0, 0,
                              flowers_width, flowers_height,
			      1, XYPixmap);
  image2panels[0]->format = XYBitmap;
  pattern = XCreateBitmapFromData(mydisplay, buttonW, pets_bits,
                                  pets_width, pets_height);
  image2panels[1] = XGetImage(mydisplay, pattern, 0, 0,
                              pets_width, pets_height, 1,
			      XYPixmap);
  image2panels[1]->format = XYBitmap;
  pattern = XCreateBitmapFromData(mydisplay, buttonW, quit_bits,
                                  quit_width, quit_height);
  image2panels[2] = XGetImage(mydisplay, pattern, 0, 0,
                              quit_width, quit_height, 1,
			      XYPixmap);
  image2panels[2]->format = XYBitmap;

	       /* 6.  select events for each window */
  myat.event_mask = ButtonPressMask | ExposureMask;
  valuemask = CWEventMask;
  XChangeWindowAttributes(mydisplay, buttonW, valuemask, &myat);
  myat.event_mask = ButtonPressMask | EnterWindowMask
                    | LeaveWindowMask;
  for ( i=0; i<3; i++)
    XChangeWindowAttributes(mydisplay, panelsW[i],
                            valuemask, &myat);

	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);
  XMapWindow(mydisplay, buttonW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &myevent);
    switch (myevent.type)  {
    case  Expose:
      XPutImage(mydisplay, buttonW, myGC1, buttonL, 0, 0, 0, 0,
                selection_width, selection_height);
      break;
    case  ButtonPress:
      if ( myevent.xbutton.button == Button1
           && myevent.xbutton.window == buttonW )  {
        printf("that is the button\n");
	XMapWindow(mydisplay, optionW);
	for (i=0; i<3; i++)  {
	  XMapWindow(mydisplay, panelsW[i]);
	  XPutImage(mydisplay, panelsW[i], myGC2, image2panels[i],
	            0, 0, 0, 0, quit_width, quit_height);
	}
      }
      if (myevent.xbutton.button == Button3
          && myevent.xbutton.window == panelsW[2] )
	  done = 1;   /* exit */
      break;
    case  EnterNotify:
      printf("window entered\n");
      for (i=0; i<3; i++)  {
        if ( myevent.xcrossing.window == panelsW[i] )  {
          XPutImage(mydisplay, panelsW[i], myGC3, image2panels[i],
	            0, 0, 0, 0, quit_width, quit_height);
	break;
	}
      }
      break;
    case  LeaveNotify:
      printf("window just left\n");
      for (i=0; i<3; i++)  {
        if ( myevent.xcrossing.window == panelsW[i] )  {
          XPutImage(mydisplay, panelsW[i], myGC2, image2panels[i],
	            0, 0, 0, 0, quit_width, quit_height);
	break;
	}
      }
      break;
    }
  }

	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
