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
//	DOOM graphics stuff for WIN95
//
//-----------------------------------------------------------------------------


#include <stdlib.h>

#include <stdarg.h>
#include <time.h>
#include <sys/types.h>

#include <signal.h>
#include <ddraw.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#define POINTER_WARP_COUNTDOWN	1

//FIXME
void RenderScene(void);


void I_ShutdownGraphics(void)
{


}



//
// I_StartFrame
//
void I_StartFrame (void)
{
    // er?

}


void I_GetEvent(void)
{

}


//
// I_StartTic
//
void I_StartTic (void)
{
	I_GetEvent();
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

//
// I_FinishUpdate
//
char MsgText[256];
void WriteDebug(char *);

void I_FinishUpdate(void)
{
    static int	lasttic;
    int		tics;
    int		i;

    // draws little dots on the bottom of the screen
    if (devparm)
    {
	i = I_GetTime();
	tics = i - lasttic;
	lasttic = i;
	if (tics > 20)
       tics = 20;

	for (i = 0; i < tics*2; i += 2)
	    screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
	for ( ; i < 20*2; i += 2)
	    screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;
    }
    RenderScene();
}


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

extern LPDIRECTDRAWPALETTE lpPalette;

//
// Palette stuff.
//
//
// I_SetPalette
//
void I_SetPalette(byte* palette)
   {
    PALETTEENTRY pe[256];
    int i;

    for (i = 0; i < 256; i++)
       {
        pe[i].peRed = gammatable[usegamma][*palette++];
        pe[i].peGreen = gammatable[usegamma][*palette++];
        pe[i].peBlue = gammatable[usegamma][*palette++];
       }
    if (lpPalette != 0)
        lpPalette->lpVtbl->SetEntries(lpPalette, 0, 0, 256, pe);
}

void I_InitGraphics(void)
{
    
}




