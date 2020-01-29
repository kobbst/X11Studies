/*  This program first displays a 300x300 pixel window coloured
 *  purple.  When the left-hand mouse button is clicked in this
 *  window, a 6 coloured smiley face appears on the screen to
 *  indicate when the mouse pointer was located when the button
 *  was pressed.  The smiley face is created using a XPM pixmap.
 *
 *  Coded by:  Ross Maloney
 *  Date:      April 2009
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>

/* XPM */
static char *smile[] = {
/* columns rows colors chars-per-pixel */
"51 51 6 1",
"  c None",
". c yellow",
"b c blue",
"x c black",
"w c white",
"r c red",
/* pixels */
"                    ...........                    ",
"                 .................                 ",
"              .......................              ",
"             .........................             ",
"           .............................           ",
"          ...............................          ",
"        ...................................        ",
"       .....................................       ",
"      .......................................      ",
"      .......................................      ",
"     .........................................     ",
"    ..........w..................wwwww.........    ",
"    .......wwwwwww..............wwwwwww........    ",
"   .......wwwwwwwww............wwwwwwwww........   ",
"  .......wwwwwwwwwww..........wwwwwwwwwww........  ",
"  .......wwwwwwwwwww.........wwwwwwwwwwwww.......  ",
"  .......wwwwwwwwwww.........wwwwwwwwwwwww.......  ",
" .......wwwbbbbbwwwww........wwwwbbbbwwwww........ ",
" ........wbbbbbbbwww.........wwwbbbbbbwwww........ ",
" ........bbbxxxbbwww.........wwbbxxxxbbwww........ ",
".........bbbxxxbbwww..........wbbbxxxbbww..........",
".........bbbbbbbbww............bbbbxbbbw...........",
"..........bbbbbbww.............bbbbbbbb............",
"............bbb..................bbbbb.............",
"...................................................",
"...................................................",
"...................................................",
"...................................................",
"...................................................",
"...................................................",
"...................................................",
" ................................................. ",
" ................................................. ",
" ................................................. ",
"  ...............................................  ",
"  ...............................................  ",
"  ...............................................  ",
"   .............................................   ",
"    ......r.............................r......    ",
"    .......r...........................r.......    ",
"     .......rr.......................rr.......     ",
"      ........rrr.................rrr........      ",
"      ...........rrrr.........rrrr...........      ",
"       ..............rrrrrrrrr..............       ",
"        ...................................        ",
"          ...............................          ",
"           .............................           ",
"             .........................             ",
"              .......................              ",
"                 .................                 ",
"                    ...........                    "
};

int main(int argc, char *argv)
{
  Display        *mydisplay;
  Window         baseW;
  XSetWindowAttributes  myat;
  XSizeHints     wmsize;
  XWMHints       wmhints;
  XTextProperty  windowName, iconName;
  XEvent         baseEvent;
  GC             mygc;
  Pixmap         pattern, clipper;
  char *window_name = "ColourClick";
  char *icon_name   = "CCl";
  int            screen_num, done, status;
  unsigned long  mymask;
  int            x, y;
  XpmAttributes  faceAt;

               /* 1.  open connection to the server */
  mydisplay = XOpenDisplay("");

	       /* 2.  create a top-level window */
  screen_num = DefaultScreen(mydisplay);
  myat.border_pixel = 0x0;             /* black */
  myat.background_pixel = 0xA020F0;    /* purple */
  myat.event_mask = ButtonPressMask | ExposureMask;
  mymask = CWBackPixel | CWBorderPixel | CWEventMask;
  baseW = XCreateWindow(mydisplay,
                        RootWindow(mydisplay, screen_num),
                        350, 400, 300, 300, 2,
			DefaultDepth(mydisplay, screen_num),
			InputOutput,
			DefaultVisual(mydisplay, screen_num),
			mymask, &myat);

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
  faceAt.color_key = XPM_COLOR;
  faceAt.valuemask = XpmColorKey | XpmColorTable;
  status = XpmCreatePixmapFromData(mydisplay, baseW,
                           smile, &pattern, &clipper, &faceAt);
  mygc = XCreateGC(mydisplay, baseW, 0, NULL);
  XSetForeground(mydisplay, mygc,
                 WhitePixel(mydisplay, screen_num));
  XSetBackground(mydisplay, mygc,
                 BlackPixel(mydisplay, screen_num));
  XSetClipMask(mydisplay, mygc, clipper);
  XSetClipOrigin(mydisplay, mygc, 0, 0);

	       /* 5.  create all the other windows needed */
	       /* 6.  select events for each window */
	       /* 7.  map the windows */
  XMapWindow(mydisplay, baseW);

	       /* 8.  enter the event loop */
  done = 0;
  while ( done == 0 )  {
    XNextEvent(mydisplay, &baseEvent);
    switch( baseEvent.type )  {
    case Expose:
      break;
    case ButtonPress:
      if ( baseEvent.xbutton.button == Button1 )  {
        x = baseEvent.xbutton.x;
	y = baseEvent.xbutton.y;
	XSetClipOrigin(mydisplay, mygc, x, y);
	XCopyArea(mydisplay, pattern, baseW, mygc, 0, 0,
	           51, 51, x, y);
      }
      break;
    }
  }
    
	       /* 9.  clean up before exiting */
  XUnmapWindow(mydisplay, baseW);
  XDestroyWindow(mydisplay, baseW);
  XCloseDisplay(mydisplay);
}
