/*  This program composes a message given on the command line
 *  using a font described in a Bitmap Distribution Format
 *  (BDF) and outputs the resultant bitmap.
 *
 *  Coded by:      Ross Msloney
 *  Initial code:  August 2011
 */

#include  <stdio.h>
#include  <stdlib.h>    /* for exit() */
#include  <string.h>    /* for strcat() */

FILE   *fileIn, *fileOut, *fopen();
int    count, checkChars, ready, ii, k, attachx, attachy;
int    number, value;
int    FBBx, FBBy, Xoff, Yoff;  /* glyphs Boundingbox info. */
struct glyph {
  int   BBx, BBy, BBxoff, BByoff, dwx, dwy, number, lines;
  char  name[40], encoding[10], pattern[40][6];
}  pallet[200];   /* Information storage for glyph in font */

char  lineoftype[40][400];    /* Storage for composed message */

int main(int argc, char *argv[])
{
  char   c, line[300], filename[30];
  int    i;
  void   extract(char *);
  void   compose(char *);
  void   xbmout(char *, int, int);

             /* check the command line, then setup processing */
  if ( ( fileIn = fopen(argv[argc-1], "r") ) == NULL )  {
    printf("Name of BDF file needs to be supplied\n");
    exit(1);
  }
  count = 0;
  while ( fscanf(fileIn, "%[^\n]", line) != EOF )
    fscanf(fileIn, "%c", &c);  { /* Store glyphs */
    if ( line[0] != '\0' )  extract(line);  /* 0 length skip */
    line[0] = '\0';
  }
  compose(argv[1]);   /* compose the command line message */
  strcat(filename, argv[2]);
  strcat(filename, ".xbm");
  if ( ( fileOut = fopen(filename, "w") ) == NULL )  {
    printf("Could not open file for output\n");
    exit(1);
  }
  xbmout(argv[2], FBBy, attachx);   /* file composed message */
}

/*  Function to examine a BDF file and recovers required
 *  information associated with it's keywords.
 */

void extract(char *fileLine)
{
  char  command[40];
  int   i, j;
  void  printglyph(char);

  sscanf(fileLine, "%s", command);
  if ( !strcmp(command, "FONTBOUNDINGBOX") )  {
    sscanf(fileLine, "%s %d %d %d %d",
           command, &FBBx, &FBBy, &Xoff, &Yoff);
    return;
  }
  if ( !strcmp(command, "CHARS") )  {
    sscanf(fileLine, "%s %d", command, &checkChars);
    ii = 0;
    return;
  }
  if ( !strcmp(command, "STARTCHAR") )  {
    sscanf(fileLine, "%s %s", command, &pallet[ii].name);
    count++;
    return;
  }
  if ( !strcmp(command, "ENDCHAR") )  {
    pallet[ii].lines = k;
    ii++;
    ready = 0;
    return;
  }
  if ( !strcmp(command, "ENCODING") )  {
    sscanf(fileLine, "%s %s", command, &pallet[ii].encoding);
    ready = 1;
    checkChars--;
    return;
  }
  if ( !strcmp(command, "DWIDTH") )  {
    sscanf(fileLine, "%s %d %d",
           command, &pallet[ii].dwx, &pallet[ii].dwy);
    if ( ready != 1 )  {
      printf("ENCODING statement required before");
      printf("DWIDTH statement: %s\n", fileLine);
      exit(1);
    }
    ready = 2;
    return;
  }
  if ( !strcmp(command, "BBX") )  {
    sscanf(fileLine, "%s %d %d %d %d", command,
          &pallet[ii].BBx, &pallet[ii].BBy, &pallet[ii].BBxoff,
          &pallet[ii].BByoff);
    if ( ready != 2 )  {
      printf("DWIDTH statement required before");
      printf("BBX statement: %s\n", fileLine);
      exit(1);
    }
    ready = 3;
            /* Calculates number of data hex per line */
    pallet[ii].number = pallet[ii].BBx/4;
    if (pallet[ii].number*4 != pallet[ii].BBx)
      pallet[ii].number++;
    return;
  }
  if ( !strcmp(command, "BITMAP") )  {
    if ( ready != 3 )  {
      printf("No BBX statement for encoding %d\n",
             pallet[ii].encoding);
      exit(1);
    }
    ready = 4;
    k =0;
    return;
  }
  if ( ready == 4 )  {
    sscanf(fileLine, "%s", command);
    for (j=0; j<pallet[ii].number; j++)
      pallet[ii].pattern[k][j] = command[j];
    k++;
  }
  return;
}

/*  Function to typeset the glyph pattern.
 */

void  compose(char *message)
{
  int   i, j, k, n, topx, topy, currentx, currenty;
  void putglyph(char, int, int);

  for (i=0; i<FBBy; i++)
    for (j=0; j<400; j++)  lineoftype[i][j] = '.';
  attachx = -Xoff;     /* Calculate location of initial point */
  attachy = FBBy + Yoff;
  k = 0;
  lineoftype[attachy][attachx] = 'M'; /* Show attachment point */
  while ( message[k] != '\0' )  {     /* Get each character */
    for (j=0; j<ii; j++)
      if ( pallet[j].name[0] == message[k] )  break;
    topy = attachy - (pallet[j].BBy + pallet[j].BByoff);
    topx = attachx - pallet[j].BBxoff;
    currentx = topx;
    currenty = topy;
    if ( topx < 0 )  currentx = 0;
          /* Show top-left glyph pattern position */
    lineoftype[topy][topx] = 'T';
    for (n=0; n<pallet[j].lines; n++)  {
      currentx = topx;
      for (i=0; i<pallet[j].number; i++)  {
        putglyph(pallet[j].pattern[n][i], currenty, currentx);
        currentx = currentx + 4;
      }
      currenty++;
    }
    k++;
    attachx = attachx + pallet[j].dwx;
    attachy = attachy + pallet[j].dwy;
    lineoftype[attachy][attachx] = 'M';  /* Show next attachment */
  }
}

/*  Function to insert the black/white bits contained in single
 *  glyph into the bitmap of the overall composition */

void  putglyph(char hex, int y, int x)
{
  int  value;

  switch ( hex )  {
  case '0':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = '+';
             break;
  case '1':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = 'm';
             break;
  case '2':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = '+';
             break;
  case '3':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = 'm';
             break;
  case '4':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = '+';
             break;
  case '5':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = 'm';
             break;
  case '6':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = '+';
             break;
  case '7':  lineoftype[y][x] = '+';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = 'm';
             break;
  case '8':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = '+';
             break;
  case '9':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = 'm';
             break;
  case 'A':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = '+';
             break;
  case 'B':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = '+';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = 'm';
             break;
  case 'C':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = '+';
             break;
  case 'D':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = '+';  lineoftype[y][x+3] = 'm';
             break;
  case 'E':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = '+';
             break;
  case 'F':  lineoftype[y][x] = 'm';    lineoftype[y][x+1] = 'm';
	     lineoftype[y][x+2] = 'm';  lineoftype[y][x+3] = 'm';
             break;
  default:   printf("Error in printing a hex value\n");
  }
}

/*  Function to write the glyph composition as an X Window bitmap
 *  (XBM) file using the naming information supplied on the
 *  command line which invoked this program.
 *
 *  Note with respect to xbm files:
 *  .  the least significant bit is on the left
 *  .  the most significant hex digit is on the right of a hex-pair
 *  .  each row of bits are completely contained in bytes
 *        representing that row
 */

void  xbmout(char *message, int height, int width)
{
  int  i, j, result, value, k, bit;
  int  copy, swing;

  fprintf(fileOut, "#define %s_width %d\n", message, width);
  fprintf(fileOut, "#define %s_height %d\n", message, height);
  fprintf(fileOut, "static char %s_bits[] = {\n", message);
       /* main test part */
  for (i=0; i<height; i++)  {
    k = 0;
    bit = 1;
    value = 0;
    copy = 0;
    swing = 1;
    for (j=0; j<width; j++)  {
      if ( lineoftype[i][j] == 'm' )  value = value | bit;
      bit = bit*2;
      k++;
      if ( k == 4 )  {
        if ( swing > 0 )  copy = value;
	else  fprintf(fileOut, " 0x%x%x,", value, copy);
	swing = -swing;
	value = 0;
	bit = 1;
	k = 0;
      }
    }
    if ( k == 0 )    /* selecting end of row output */
      if ( swing > 0 )  fprintf(fileOut, "\n");
      else  fprintf(fileOut, " 0x0%x,\n", copy);
    else 
      if ( swing > 0 )  fprintf(fileOut, " 0x0%x,\n", value);
      else  fprintf(fileOut, " 0x%x%x,\n", value, copy);
  }
  fprintf(fileOut, "};\n");
}
