/*  This program converts colours between different Xcms colour
 *  spaces.  First a RGB colour is converted to its
 *  representation in the TekHVC colour space.  Then a colour
 *  defined in the TekHVC colour space is converted to RGB.
 *  The results of each conversion are printed on the terminal.
 *
 * Coded by:  Ross Maloney
 * Date:      13 September 2012
 */

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>
#include  <X11/Xcms.h>
#include  <stdio.h>

int main(int argc, char *argv[])
{
  Display        *mydisplay;
  XcmsColor      *exact, *available;
  Status         status;
  int            screen_num;
  int            red, green, blue;
  char           rgb[10], tekcolour[40];
  XcmsFloat      h, v, c;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  exact = malloc(sizeof(XcmsColor));
  available = malloc(sizeof(XcmsColor));

	       /* 3.  give the Window Manager hints */
	       /* 4.  establish window resources */
	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */

	       /* 8.  enter the event loop */
  printf("default white = %x\n", WhitePixel(mydisplay, screen_num));
  red = 0Xc4;
  green = 0xde;
  blue = 0x12;
  sprintf(rgb, "#%02x%02x%02x", red, green, blue);
  printf("rgb = %s\n", rgb);
  status = XcmsLookupColor(mydisplay,
                           XDefaultColormap(mydisplay, screen_num),
                           rgb, exact, available, XcmsTekHVCFormat);
  h = exact->spec.TekHVC.H;
  v = exact->spec.TekHVC.V;
  c = exact->spec.TekHVC.C;
  switch ( status )  {
  case XcmsSuccess:
    printf("Success: h = %lf  v = %lf  c = %lf\n", h, v, v);
    break;
  case XcmsSuccessWithCompression:
    printf("Compressed: h = %lf  v = %lf  c = %lf\n", h, v, v);
    break;
  case XcmsFailure:
    printf("Xcms failure\n");
    break;
  default:
    printf("This should never happen\n");
  }
  h = 192.4;
  v = 82.6;
  c = 56.1;
  sprintf(tekcolour, "TekHVC:%5.1f/%4.2f/%4.2f", h, v, c);
  printf("tekcolour = %s\n", tekcolour);
  status = XcmsLookupColor(mydisplay,
                        XDefaultColormap(mydisplay, screen_num),
                        rgb, exact, available, XcmsTekHVCFormat);
  red = exact->spec.RGB.red;
  green = exact->spec.RGB.green;
  blue = exact->spec.RGB.blue;
  switch ( status )  {
  case XcmsSuccess:
    printf("Success: red = %x  green = %x  blue = %x\n",
           red, green, blue);
    break;
  case XcmsSuccessWithCompression:
    printf("Compressed: red = %x  green = %x  blue = %x\n",
           red, green, blue);
    break;
  case XcmsFailure:
    printf("Xcms failure\n");
    break;
  default:
    printf("This should never happen\n");
  }

	       /* 9.  clean up before exiting */
  XCloseDisplay(mydisplay);
}
