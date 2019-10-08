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
// DESCRIPTION:
//	Gamma correction LUT.
//	Functions to draw patches (by post) directly to screen.
//	Functions to blit a block to the screen.
//
//-----------------------------------------------------------------------------


#ifndef __V_VIDEO__
#define __V_VIDEO__

#include "doomtype.h"

#include "doomdef.h"

// Needed because we are refering to patches.
#include "r_data.h"

//
// VIDEO
//

#define CENTERY			(SCREENHEIGHT/2)


// Screen 0 is the screen updated by I_Update screen.
// Screen 1 is an extra buffer.

// vrodic : MAX video width and height
#define MAXVIDWIDTH     1024  //dont set this too high because actually
#define MAXVIDHEIGHT    768  // lots of tables are allocated with the MAX
                            // size.
#define BASEVIDWIDTH	320   //NEVER CHANGE THIS! this is the original
#define BASEVIDHEIGHT	200  // resolution of the graphics.
//////////////////////////////

extern	byte *screens[5];

extern  int	dirtybox[4];

extern	byte	gammatable[5][256];
extern	int	usegamma;



// Allocates buffer screens, call before R_Init.
void V_Init (void);


void
V_CopyRect
( int		srcx,
  int		srcy,
  int		srcscrn,
  int		width,
  int		height,
  int		destx,
  int		desty,
  int		destscrn );

//vrodic:19-03-98:like V_DrawPatch, + using a colormap.
void V_DrawMappedPatch ( int	       x,
			 int	       y,
			 int	       scrn,
			 patch_t*      patch,
			 byte*	       colormap );

//vrodic:18-03-98:V_DrawPatch scaled 2,3,4 times size and position.
void V_DrawScaledPatch ( int	       x,
			 int	       y,
			 int	       scrn,
			 patch_t*      patch );
//vrodic:19-03-98: like V_DrawPatch, but uses a colormap, see comments in .c
void V_DrawTranslationPatch ( int	    x,
			      int	    y,
			      int	    scrn,
			      patch_t*	    patch,
			      byte*	    colormap );


void
V_DrawPatch
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch);

void
V_DrawPatchDirect
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch );


// Draw a linear block of pixels into the view buffer.
void
V_DrawBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		src );

// Reads a linear block of pixels into the view buffer.
void
V_GetBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		dest );


void
V_MarkRect
( int		x,
  int		y,
  int		width,
  int		height );

//vrodic:19-03-98: fill a box with a single color
void V_DrawFill (int x, int y, int w, int h, int c);
//vrodic:19-03-98: fill a box with a flat as a pattern
void V_DrawFlatFill (int x, int y, int w, int h, char* flat);

//vrodic:18-03-98: fade down the screen buffer before drawing the menu over
void V_DrawFadeScreen (void);

//added:19-03-98: draw a string using the hu_font
void V_DrawString (int x, int y, char* string);
//vrodic:19-03-98: V_DrawString which remaps text color to whites
/*void V_DrawStringWhite (int x, int y, char* string);*/

// Find string width from hu_font chars
int  V_StringWidth (char* string);

#endif
