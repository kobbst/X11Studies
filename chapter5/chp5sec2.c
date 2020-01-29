/*  This program prints the name of all fonts available on the
 *  current X server.
 *
 *  Coded by:  Ross Maloney
 *  Date:      December 2008
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(int argc, char *carv)
{
  Display        *mydisplay;
  char           **fontNames;
  int            i, present;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");
  fontNames = XListFonts(mydisplay, "*", 4000, &present);
  for (i=0; i<present; i++)  printf("%s\n", fontNames[i]);
  printf("Number of those fonts present = %d\n", present);

	       /* 2.  create a top-level window */
	       /* 3.  give the Window Manager hints */
	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
	       /* 8.  enter the event loop */
	       /* 9.  clean up before exiting */
}
