/*  This program prints the display request limitation of the
 *  current X server.
 *
 *  Coded by:  Ross Maloney
 *  Date:      March 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(int argc, char *carv)
{
  Display        *mydisplay;
  long           size;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
	       /* 3.  give the Window Manager hints */
	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
	       /* 8.  enter the event loop */
  size = XMaxRequestSize(mydisplay);
  printf("Single protocol size limit = %d\n", size);
  size -= 3;
  printf("Upper limits:\n");
  printf("  points   < %d\n", size);
  printf("  lines    < %d\n", size/2);
  printf("  arcs     < %d\n", size/3);
  printf("  polygons < %d\n", size+1);

	       /* 9.  clean up before exiting */
  XCloseDisplay(mydisplay);
}
