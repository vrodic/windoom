//-----------------------------------------------------------------------------
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// Log:
//
// DESCRIPTION:
//	Gamma correction LUT stuff.
//	Functions to draw patches (by post) directly to screen.
//	Functions to blit a block to the screen.
//
//-----------------------------------------------------------------------------


#include "i_system.h"
#include "r_local.h"

#include "doomdef.h"
#include "doomdata.h"

#include "m_bbox.h"
#include "m_swap.h"

#include "v_video.h"

#include "hu_stuff.h"	//vrodic:19-03-98:for V_DrawString ()

#include "w_wad.h"		//vrodic:19-03-98 for V_DrawFlatFill ()
#include "z_zone.h"		//and this

extern patch_t* 	hu_font[HU_FONTSIZE]; //vrodic

void WriteDebug(char *);
char MsgText[256];

// Each screen is [SCREENWIDTH*SCREENHEIGHT]; 
byte*				screens[5];	

int				dirtybox[4]; 



// Now where did these came from?
byte gammatable[5][256] =
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
     17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
     81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
     97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
     144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
     160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
     176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
     192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
     208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

    {2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,
     32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,
     56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,
     78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
     99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,
     130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
     146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,
     161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,
     175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,
     190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,
     205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,
     219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,
     233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,
     247,248,249,250,251,252,252,253,254,255},

    {4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,
     43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,
     70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,
     94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,
     144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,
     160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,
     174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,
     188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,
     202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,
     216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,
     229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,
     242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,
     255},

    {8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,
     57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,
     86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,
     108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,
     141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
     155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,
     169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,
     183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,
     195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,
     207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,
     219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,
     231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,
     242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,
     253,253,254,254,255},

    {16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,
     78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,
     107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
     142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
     156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,
     169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,
     182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,
     193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,
     204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,
     214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,
     224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,
     234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,
     243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,
     251,252,252,253,254,254,255,255}
};



int	usegamma;
			 
//
// V_MarkRect 
// 
void V_MarkRect( int x, int y, int width, int height ) 
   { 
    M_AddToBox (dirtybox, x, y); 
    M_AddToBox (dirtybox, x+width-1, y+height-1); 
   } 
 

//
// V_CopyRect 
// 
void V_CopyRect( int srcx, int srcy, int srcscrn, int width, int height, int destx, int desty, int destscrn ) 
   { 
    byte*	src;
    byte*	dest; 
	 
#ifdef RANGECHECK 
    if (srcx < 0 || srcx+width > SCREENWIDTH || srcy < 0 || srcy+height > SCREENHEIGHT || destx < 0 ||
        destx+width > SCREENWIDTH || desty < 0 || desty+height > SCREENHEIGHT || (unsigned)srcscrn > 4 || (unsigned)destscrn > 4)
       {
        I_Error ("Bad V_CopyRect");
       }
#endif 
    V_MarkRect (destx, desty, width, height); 
	 
    src = screens[srcscrn]+SCREENWIDTH*srcy+srcx; 
    dest = screens[destscrn]+SCREENWIDTH*desty+destx; 
    for ( ; height > 0 ; height--) 
       { 
        memcpy (dest, src, width); 
        src += SCREENWIDTH; 
        dest += SCREENWIDTH; 
       } 
   } 
 
//
//  V_DrawMappedPatch : like V_DrawScaledPatch, but with a colormap.
//
//
//vrodic:19-03-98:
void V_DrawMappedPatch ( int	       x,
			 int	       y,
			 int	       scrn,
			 patch_t*      patch,
			 byte*	       colormap )
{
    int 	count;
    int 	col;
    column_t*	column;
    byte*	desttop;
    byte*	dest;
    byte*	source;
    int 	w;

    int 	dupx,dupy;
    int 	ofs;
    int 	colfrac,rowfrac;

    dupx = SCREENWIDTH / BASEVIDWIDTH;
    dupy = SCREENHEIGHT / BASEVIDHEIGHT;

    y -= SHORT(patch->topoffset);
    x -= SHORT(patch->leftoffset);

    if (!scrn)
	V_MarkRect (x, y, SHORT(patch->width)*dupx, SHORT(patch->height)*dupy);

    col = 0;
    colfrac  = FixedDiv (FRACUNIT, dupx<<FRACBITS);
    rowfrac  = FixedDiv (FRACUNIT, dupy<<FRACBITS);

    desttop = screens[scrn] + (y*dupy*SCREENWIDTH) + (x*dupx) +
	      ((SCREENHEIGHT%BASEVIDHEIGHT)>>1)*SCREENWIDTH +
	      ((SCREENWIDTH%BASEVIDWIDTH)>>1);

    w = SHORT(patch->width)<<FRACBITS;

    for ( ; col<w ; col+=colfrac, desttop++)
    {
	column = (column_t *)((byte *)patch + LONG(patch->columnofs[col>>FRACBITS]));

	while (column->topdelta != 0xff )
	{
	    source = (byte *)column + 3;
	    dest   = desttop + column->topdelta*dupy*SCREENWIDTH;
	    count  = column->length*dupy;

	    ofs = 0;
	    while (count--)
	    {
		*dest = *(colormap + source[ofs>>FRACBITS] );
		dest += SCREENWIDTH;
		ofs += rowfrac;
	    }

	    column = (column_t *)( (byte *)column + column->length + 4 );
	}
    }

}

//
// V_DrawScaledPatch
//   like V_DrawPatch, but scaled 2,3,4 times the original size and position
//   this is used for menu and title screens, with high resolutions
//
//vrodic:18-03-98:
void V_DrawScaledPatch ( int	       x,
			 int	       y,
			 int	       scrn,
			 patch_t*      patch )
{
    int 	count;
    int 	col;
    column_t*	column;
    byte*	desttop;
    byte*	dest;
    byte*	source;
    int 	w;

    int 	dupx,dupy;
    int 	ofs;
    int 	colfrac,rowfrac;

    dupx = SCREENWIDTH / BASEVIDWIDTH;
    dupy = SCREENHEIGHT / BASEVIDHEIGHT;

    y -= SHORT(patch->topoffset);
    x -= SHORT(patch->leftoffset);

    if (!scrn)
	V_MarkRect (x, y, SHORT(patch->width)*dupx, SHORT(patch->height)*dupy);

    col = 0;
    colfrac  = FixedDiv (FRACUNIT, dupx<<FRACBITS);
    rowfrac  = FixedDiv (FRACUNIT, dupy<<FRACBITS);

    desttop = screens[scrn] + (y*dupy*SCREENWIDTH) + (x*dupx) +
	      ((SCREENHEIGHT%BASEVIDHEIGHT)>>1)*SCREENWIDTH +
	      ((SCREENWIDTH%BASEVIDWIDTH)>>1);

    w = SHORT(patch->width)<<FRACBITS;

    for ( ; col<w ; col+=colfrac, desttop++)
    {
	column = (column_t *)((byte *)patch + LONG(patch->columnofs[col>>FRACBITS]));

	while (column->topdelta != 0xff )
	{
	    source = (byte *)column + 3;
	    dest   = desttop + column->topdelta*dupy*SCREENWIDTH;
	    count  = column->length*dupy;

	    ofs = 0;
	    while (count--)
	    {
		*dest = source[ofs>>FRACBITS];
		dest += SCREENWIDTH;
		ofs += rowfrac;
	    }

	    column = (column_t *)( (byte *)column + column->length + 4 );
	}
    }
}

//
//  Masks a column based masked pic to the screen, USING A COLORMAP
//
//  This is just like V_DrawMappedPatch, but it doesn't scale since it's
//  used for the status bar right now...
//
//  This is called 'V_DrawTranslationPatch' because it is used to draw
//  the facebacks of statusbar and intermission screens with the right
//  player colors.
//
//vrodic : 19-03-1998
void V_DrawTranslationPatch ( int	    x,
			      int	    y,
			      int	    scrn,
			      patch_t*	    patch,
			      byte*	    colormap )
{
    int 	count;
    int 	col;
    column_t*	column;
    byte*	desttop;
    byte*	dest;
    byte*	source;
    int 	w;

    y -= SHORT(patch->topoffset);
    x -= SHORT(patch->leftoffset);
#ifdef RANGECHECK
    if (x<0
	||x+SHORT(patch->width) >SCREENWIDTH
	|| y<0
	|| y+SHORT(patch->height)>SCREENHEIGHT
	|| (unsigned)scrn>4)
    {
      fprintf( stderr, "Patch at %d,%d exceeds LFB\n", x,y );
      // No I_Error abort - what is up with TNT.WAD?
      fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
      return;
    }
#endif

    if (!scrn)
	V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height));

    col = 0;
    desttop = screens[scrn]+y*SCREENWIDTH+x;

    w = SHORT(patch->width);

    for ( ; col<w ; x++, col++, desttop++)
    {
	column = (column_t *)((byte *)patch + LONG(patch->columnofs[col]));

	// step through the posts in a column
	while (column->topdelta != 0xff )
	{
	    source = (byte *)column + 3;
	    dest = desttop + column->topdelta*SCREENWIDTH;
	    count = column->length;

	    while (count--)
	    {
		*dest = *(colormap + *(source++) );
		dest += SCREENWIDTH;
	    }
	    column = (column_t *)(  (byte *)column + column->length + 4 );
	}
    }
}


//
// V_DrawPatch
// Masks a column based masked pic to the screen. 
//
void V_DrawPatch ( int x, int y, int scrn, patch_t* patch ) 
   { 
    int		count;
    int		col; 
    column_t*	column; 
    byte*	desttop;
    byte*	dest;
    byte*	source; 
    int		w; 
	 
    y -= SHORT(patch->topoffset); 
    x -= SHORT(patch->leftoffset); 
#ifdef RANGECHECK 
    if (x<0 ||x+SHORT(patch->width) >SCREENWIDTH || y<0 || y+SHORT(patch->height)>SCREENHEIGHT || (unsigned)scrn>4)
       {
        fprintf( stderr, "Patch at %d,%d exceeds LFB\n", x,y );
        // No I_Error abort - what is up with TNT.WAD?
        fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
        return;
       }
#endif 
 
    if (!scrn)
       {
        //WriteDebug("V_MarkRect...\n");
        V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 
       }

    col = 0; 
    desttop = screens[scrn]+y*SCREENWIDTH+x; 

    w = SHORT(patch->width);

    //sprintf(MsgText, "width = %d, x/y = %d/%d\n", w, x, y);
    //WriteDebug(MsgText);

    for ( ; col<w ; x++, col++, desttop++)
       { 
        //sprintf(MsgText, "col = %d length = %d\n", col, column->length);
        //WriteDebug(MsgText);
        column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 
 
        // step through the posts in a column 
        while (column->topdelta != 0xff ) 
           { 
            source = (byte *)column + 3; 
            dest = desttop + column->topdelta*SCREENWIDTH; 
            count = column->length; 

            //sprintf(MsgText, "count = %d\n", count);
            //WriteDebug(MsgText);
			 
            while (count--) 
               { 
                *dest = *source++; 
                dest += SCREENWIDTH; 
               } 
            column = (column_t *)((byte *)column + column->length + 4 );
           } 
       }
    //WriteDebug("Finished V_DrawPatch...\n");
   } 
 
//
// V_DrawPatchFlipped 
// Masks a column based masked pic to the screen.
// Flips horizontally, e.g. to mirror face.
//
void
V_DrawPatchFlipped
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch ) 
{ 

    int		count;
    int		col; 
    column_t*	column; 
    byte*	desttop;
    byte*	dest;
    byte*	source; 
    int		w; 
	 
    y -= SHORT(patch->topoffset); 
    x -= SHORT(patch->leftoffset); 
#ifdef RANGECHECK 
    if (x<0
	||x+SHORT(patch->width) >SCREENWIDTH
	|| y<0
	|| y+SHORT(patch->height)>SCREENHEIGHT 
	|| (unsigned)scrn>4)
    {
      fprintf( stderr, "Patch origin %d,%d exceeds LFB\n", x,y );
      I_Error ("Bad V_DrawPatch in V_DrawPatchFlipped");
    }
#endif 
 
    if (!scrn)
	V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 

    col = 0; 
    desttop = screens[scrn]+y*SCREENWIDTH+x; 
	 
    w = SHORT(patch->width); 

    for ( ; col<w ; x++, col++, desttop++) 
    { 
	column = (column_t *)((byte *)patch + LONG(patch->columnofs[w-1-col])); 
 
	// step through the posts in a column 
	while (column->topdelta != 0xff ) 
	{ 
	    source = (byte *)column + 3; 
	    dest = desttop + column->topdelta*SCREENWIDTH; 
	    count = column->length; 
			 
	    while (count--) 
	    { 
		*dest = *source++; 
		dest += SCREENWIDTH; 
	    } 
	    column = (column_t *)(  (byte *)column + column->length 
				    + 4 ); 
	} 
    }			 
} 
 


//
// V_DrawPatchDirect
// Draws directly to the screen on the pc. 
//
void
V_DrawPatchDirect
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch ) 
{
    V_DrawPatch (x,y,scrn, patch); 

    /*
    int		count;
    int		col; 
    column_t*	column; 
    byte*	desttop;
    byte*	dest;
    byte*	source; 
    int		w; 
	 
    y -= SHORT(patch->topoffset); 
    x -= SHORT(patch->leftoffset); 

#ifdef RANGECHECK 
    if (x<0
	||x+SHORT(patch->width) >SCREENWIDTH
	|| y<0
	|| y+SHORT(patch->height)>SCREENHEIGHT 
	|| (unsigned)scrn>4)
    {
	I_Error ("Bad V_DrawPatchDirect");
    }
#endif 
 
    //	V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 
    desttop = destscreen + y*SCREENWIDTH/4 + (x>>2); 
	 
    w = SHORT(patch->width); 
    for ( col = 0 ; col<w ; col++) 
    { 
	outp (SC_INDEX+1,1<<(x&3)); 
	column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 
 
	// step through the posts in a column 
	 
	while (column->topdelta != 0xff ) 
	{ 
	    source = (byte *)column + 3; 
	    dest = desttop + column->topdelta*SCREENWIDTH/4; 
	    count = column->length; 
 
	    while (count--) 
	    { 
		*dest = *source++; 
		dest += SCREENWIDTH/4; 
	    } 
	    column = (column_t *)(  (byte *)column + column->length 
				    + 4 ); 
	} 
	if ( ((++x)&3) == 0 ) 
	    desttop++;	// go to next byte, not next plane 
    }*/ 
} 
 


//
// V_DrawBlock
// Draw a linear block of pixels into the view buffer.
//
void
V_DrawBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		src ) 
{ 
    byte*	dest; 
	 
#ifdef RANGECHECK 
    if (x < 0 || x+width > SCREENWIDTH || y < 0 || y+height > SCREENHEIGHT || (unsigned)scrn > 4 )
       {
        I_Error ("Bad V_DrawBlock");
       }
#endif 
 
    V_MarkRect (x, y, width, height); 
 
    dest = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    { 
	memcpy (dest, src, width); 
	src += width; 
	dest += SCREENWIDTH; 
    } 
} 
 


//
// V_GetBlock
// Gets a linear block of pixels from the view buffer.
//
void
V_GetBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		dest ) 
{ 
    byte*	src; 
	 
#ifdef RANGECHECK 
    if (x<0
	||x+width >SCREENWIDTH
	|| y<0
	|| y+height>SCREENHEIGHT 
	|| (unsigned)scrn>4 )
    {
	I_Error ("Bad V_DrawBlock");
    }
#endif 
 
    src = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    { 
	memcpy (dest, src, width); 
	src += SCREENWIDTH; 
	dest += width; 
    } 
} 

//
//  Fills a box of pixels with a single color, NOTE: scaled to screen size
//
//vrodic:19-03-98:
void V_DrawFill (int x, int y, int w, int h, int c)
{
    byte      *dest;
    int       u, v;
    int       dupx,dupy;

    dupx = SCREENWIDTH / BASEVIDWIDTH;
    dupy = SCREENHEIGHT / BASEVIDHEIGHT;

    dest = screens[0] + y*dupy*SCREENWIDTH + x*dupx +
	   ((SCREENHEIGHT%BASEVIDHEIGHT)>>1)*SCREENWIDTH +
	   ((SCREENWIDTH%BASEVIDWIDTH)>>1);

    w *= dupx;
    h *= dupy;

    for (v=0 ; v<h ; v++, dest += SCREENWIDTH)
	for (u=0 ; u<w ; u++)
	    dest[u] = c;
}



//
//  Fills a box of pixels using a flat texture as a pattern,
//  scaled to screen size.
//
//added:19-03-98:
void V_DrawFlatFill (int x, int y, int w, int h, char* flat)
{
    byte      *dest;
    int       u, v;
    int       dupx,dupy;
    fixed_t   dx,dy,xfrac,yfrac;
    byte      *psrc,*src;

    dupx = SCREENWIDTH / BASEVIDWIDTH;
    dupy = SCREENHEIGHT / BASEVIDHEIGHT;

    psrc = W_CacheLumpName (flat, PU_CACHE);

    dest = screens[0] + y*dupy*SCREENWIDTH + x*dupx +
	   ((SCREENHEIGHT%BASEVIDHEIGHT)>>1)*SCREENWIDTH +
	   ((SCREENWIDTH%BASEVIDWIDTH)>>1);

    w *= dupx;
    h *= dupy;

    dx = FixedDiv(FRACUNIT,dupx<<FRACBITS);
    dy = FixedDiv(FRACUNIT,dupy<<FRACBITS);

    yfrac = 0;
    for (v=0; v<h ; v++, dest += SCREENWIDTH)
    {
	xfrac = 0;
	src = psrc + (((yfrac>>FRACBITS)&63)<<6);
	for (u=0 ; u<w ; u++)
	{
	    dest[u] = src[(xfrac>>FRACBITS)&63];
	    xfrac += dx;
	}
	yfrac += dy;
    }
}


//
//  Fade all the screen buffer, so that the menu is more readable,
//  especially now that we use the small hufont in the menus...
//
//vrodic:18-03-98:
void V_DrawFadeScreen (void)
{
    int 	x,y,w;
    int 	*buf;
    unsigned	quad;
    byte	p1, p2, p3, p4;

    byte*	fadetable = (byte *) colormaps + 16*256;;

    w = SCREENWIDTH>>2;
    for (y=0 ; y<SCREENHEIGHT ; y++)
    {
	buf = (int *)(screens[0] + SCREENWIDTH*y);
	for (x=0 ; x<w ; x++)
	{
	    quad = buf[x];
	    p1 = fadetable[quad&255];
	    p2 = fadetable[(quad>>8)&255];
	    p3 = fadetable[(quad>>16)&255];
	    p4 = fadetable[quad>>24];
	    buf[x] = (p4<<24) | (p3<<16) | (p2<<8) | p1;
	}
    }

}


//
//  Write a string using the hu_font
//  NOTE: the text is centered for screens larger than the base width
//
//vrodic:19-03-98:
void V_DrawString (int x, int y, char* string)
{
    int 	w;
    char*	ch;
    int 	c;
    int 	cx;
    int 	cy;

    ch = string;
    cx = x;
    cy = y;

    while(1)
    {
	c = *ch++;
	if (!c)
	    break;
	if (c == '\n')
	{
	    cx = x;
	    cy += 12;
	    continue;
	}

	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c>= HU_FONTSIZE)
	{
	    cx += 4;
	    continue;
	}

	w = SHORT (hu_font[c]->width);
	if (cx+w > BASEVIDWIDTH)
	    break;
	V_DrawScaledPatch(cx, cy, 0, hu_font[c]);
	cx+=w;
    }
}
/*
// vrodic: TODO! (maybe)
extern byte* whitemap;	//m_menu.c

//
//vrodic:19-03-98: V_DrawString, using a colormap to display the text in
//		  brighter colors.
void V_DrawStringWhite (int x, int y, char* string)
{
    int 	w;
    char*	ch;
    int 	c;
    int 	cx;
    int 	cy;

    ch = string;
    cx = x;
    cy = y;
    while(1)
    {
	c = *ch++;
	if (!c)
	    break;
	if (c == '\n')
	{
	    cx = x;
	    cy += 12;
	    continue;
	}

	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c>= HU_FONTSIZE)
	{
	    cx += 4;
	    continue;
	}

	w = SHORT (hu_font[c]->width);
	if (cx+w > vid.width)
	    break;
	V_DrawMappedPatch(cx, cy, 0, hu_font[c], whitemap);
	cx+=w;
    }
}*/


//
// Find string width from hu_font chars
//
int V_StringWidth (char* string)
{
    int 	    i;
    int 	    w = 0;
    int 	    c;

    for (i = 0;i < strlen(string);i++)
    {
	c = toupper(string[i]) - HU_FONTSTART;
	if (c < 0 || c >= HU_FONTSIZE)
	    w += 4;
	else
	    w += SHORT (hu_font[c]->width);
    }

    return w;
}


//
// V_Init
// 
void V_Init (void) 
   { 
    int		i, j, x;
    //byte*	base;
    DWORD  *buff;
		
    // stick these in low dos memory on PCs

    //base = I_AllocLow (SCREENWIDTH*SCREENHEIGHT*4);

    for (i = 0; i < 5; i++)
       {
        x = SCREENWIDTH * SCREENHEIGHT;
        screens[i] = (byte *)malloc(x);
        buff = (DWORD *)screens[i];
        x /= 4;
        for (j = 0; j < x; j++)
           buff[j] = 0x00000000;
       }
//  screens[i] = base + i*SCREENWIDTH*SCREENHEIGHT;
   }
