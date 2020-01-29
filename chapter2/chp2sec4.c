/*  An elementary X Window program.  A display is linked to this
 *  program, the keyboard bell is then sounded, then the program
 *  terminates.
 *
 *  Coded by:  Ross Maloney
 *  Date:      January 2012
 */

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>

int  main(int argc, char *argv[])
{
  Display         *mydisplay;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

               /* 2.  create a top-level window */
               /* 3.  give the Window Manager hints */
               /* 4.  establish window resources */
               /* 5.  create all the other windows needed */
               /* 6.  select events for each window */
               /* 7.  map the windows */
               /* 8.  enter the event loop */
  XBell(mydisplay, 0);
               /* 9.  clean up before exiting */
  XCloseDisplay(mydisplay);
}
