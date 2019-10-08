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
//
// Log:
//
// DESCRIPTION:
//	Main loop menu stuff.
//	Default Config File.
//	PCX Screenshots.
//
//-----------------------------------------------------------------------------


#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <io.h>

#include <direct.h>

#include <ctype.h>


#include "doomdef.h"

#include "z_zone.h"

#include "m_swap.h"
#include "m_argv.h"

#include "w_wad.h"

#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#include "hu_stuff.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"

#include "m_misc.h"

//
// M_DrawText
// Returns the final X coordinate
// HU_Init must have been called to init the font
//
extern patch_t*		hu_font[HU_FONTSIZE];

int
M_DrawText
( int		x,
  int		y,
  boolean	direct,
  char*		string )
{
    int 	c;
    int		w;

    while (*string)
    {
	c = toupper(*string) - HU_FONTSTART;
	string++;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    x += 4;
	    continue;
	}
		
	w = SHORT (hu_font[c]->width);
	if (x+w > SCREENWIDTH)
	    break;
	if (direct)
	    V_DrawPatchDirect(x, y, 0, hu_font[c]);
	else
	    V_DrawPatch(x, y, 0, hu_font[c]);
	x+=w;
    }

    return x;
}



//
// M_GetFileSize
//
#ifndef O_BINARY
#define O_BINARY 0
#endif

int M_GetFileSize( char const*	name )
   {
    int		handle;
    int		count;
	
    handle = open ( name, O_RDWR | O_BINARY);

    if (handle == -1)
        return 0;

    count = lseek(handle, 0, SEEK_END);
    close (handle);
	
    return count;
   }

//
// M_WriteFile
//
boolean
M_WriteFile
( char const*	name,
  void*		source,
  int		length )
{
    int		handle;
    int		count;
	
    handle = open ( name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);

    if (handle == -1)
	return false;

    count = write (handle, source, length);
    close (handle);
	
    if (count < length)
	return false;
		
    return true;
}

//
// M_AppendFile
//
boolean M_AppendFile(char const *name, void *source, int length )
   {
    int		handle;
    int		count;
	
    handle = open( name, O_RDWR | O_BINARY);

    if (handle == -1)
        return false;

    lseek(handle, 0L, SEEK_END);

    count = write (handle, source, length);
    close (handle);
	
    if (count < length)
        return false;
		
    return true;
   }


//
// M_ReadFile
//
int
M_ReadFile
( char const*	name,
  byte**	buffer )
{
    int	handle, count, length;
    struct stat	fileinfo;
    byte		*buf;
	
    handle = open (name, O_RDONLY | O_BINARY, 0666);
    if (handle == -1)
	I_Error ("Couldn't read file %s", name);
    if (fstat (handle,&fileinfo) == -1)
	I_Error ("Couldn't read file %s", name);
    length = fileinfo.st_size;
    buf = Z_Malloc (length, PU_STATIC, NULL);
    count = read (handle, buf, length);
    close (handle);
	
    if (count < length)
	I_Error ("Couldn't read file %s", name);
		
    *buffer = buf;
    return length;
}


//
// DEFAULTS
//
int		usemouse;
int		usejoystick;

extern int	key_right;
extern int	key_left;
extern int	key_up;
extern int	key_down;

extern int	key_strafeleft;
extern int	key_straferight;

extern int	key_fire;
extern int	key_use;
extern int	key_strafe;
extern int	key_speed;

extern int  key_mvert;

extern int  always_run;
extern int  swap_stereo;
extern int  mvert;
extern int  keylink;

extern int	mousebfire;
extern int	mousebstrafe;
extern int	mousebforward;

extern int	mouseb1;
extern int	mouseb2;
extern int	mouseb3;

extern int	joybfire;
extern int	joybstrafe;
extern int	joybuse;
extern int	joybspeed;

extern int  joyb1;
extern int  joyb2;
extern int  joyb3;
extern int  joyb4;

extern int	viewwidth;
extern int	viewheight;

extern int	mouseSensitivity;
extern int	showMessages;


extern int	screenblocks;

extern int	showMessages;

// machine-independent sound params
extern	int	numChannels;



extern char*	chat_macros[];



int	numdefaults;
char*	defaultfile;

char DoomDir[128], szValue[32];
void GetININame(void);

//
// M_SaveDefaults
//
void M_SaveDefaults (void)
   {
    // Win95 specific
	
    GetININame();
    sprintf(szValue, "%d", mouseSensitivity);
    WritePrivateProfileString("DEFAULTS", "mouse_sensitivity", szValue, DoomDir);
    sprintf(szValue, "%d", snd_SfxVolume);
    WritePrivateProfileString("DEFAULTS", "sfx_volume", szValue, DoomDir);
    sprintf(szValue, "%d", snd_MusicVolume);
    WritePrivateProfileString("DEFAULTS", "music_volume", szValue, DoomDir);
    sprintf(szValue, "%d", showMessages);
    WritePrivateProfileString("DEFAULTS", "show_messages", szValue, DoomDir);

    sprintf(szValue, "%d", key_right);
    WritePrivateProfileString("DEFAULTS", "key_right", szValue, DoomDir);
    sprintf(szValue, "%d", key_left);
    WritePrivateProfileString("DEFAULTS", "key_left", szValue, DoomDir);
    sprintf(szValue, "%d", key_up);
    WritePrivateProfileString("DEFAULTS", "key_up", szValue, DoomDir);
    sprintf(szValue, "%d", key_down);
    WritePrivateProfileString("DEFAULTS", "key_down", szValue, DoomDir);

    sprintf(szValue, "%d", key_mvert);
    WritePrivateProfileString("DEFAULTS", "key_mvert", szValue, DoomDir);

    sprintf(szValue, "%d", key_strafeleft);
    WritePrivateProfileString("DEFAULTS", "key_strafeleft", szValue, DoomDir);
    sprintf(szValue, "%d", key_straferight);
    WritePrivateProfileString("DEFAULTS", "key_straferight", szValue, DoomDir);

    sprintf(szValue, "%d", key_fire);
    WritePrivateProfileString("DEFAULTS", "key_fire", szValue, DoomDir);
    sprintf(szValue, "%d", key_use);
    WritePrivateProfileString("DEFAULTS", "key_use", szValue, DoomDir);
    sprintf(szValue, "%d", key_strafe);
    WritePrivateProfileString("DEFAULTS", "key_strafe", szValue, DoomDir);
    sprintf(szValue, "%d", key_speed);
    WritePrivateProfileString("DEFAULTS", "key_speed", szValue, DoomDir);

    sprintf(szValue, "%d", always_run);
    WritePrivateProfileString("DEFAULTS", "always_run", szValue, DoomDir);
    sprintf(szValue, "%d", swap_stereo);
    WritePrivateProfileString("DEFAULTS", "swap_stereo", szValue, DoomDir);
    sprintf(szValue, "%d", mvert);
    WritePrivateProfileString("DEFAULTS", "mvert", szValue, DoomDir);
    sprintf(szValue, "%d", keylink);
    WritePrivateProfileString("DEFAULTS", "keylink", szValue, DoomDir);


    sprintf(szValue, "%d", usemouse);
    WritePrivateProfileString("DEFAULTS", "use_mouse", szValue, DoomDir);
    sprintf(szValue, "%d", mousebfire);
    WritePrivateProfileString("DEFAULTS", "mouseb_fire", szValue, DoomDir);
    sprintf(szValue, "%d", mousebstrafe);
    WritePrivateProfileString("DEFAULTS", "mouseb_strafe", szValue, DoomDir);
    sprintf(szValue, "%d", mousebforward);
    WritePrivateProfileString("DEFAULTS", "mouseb_forward", szValue, DoomDir);

    sprintf(szValue, "%d", mouseb1);
    WritePrivateProfileString("DEFAULTS", "mouseb1", szValue, DoomDir);
    sprintf(szValue, "%d", mouseb2);
    WritePrivateProfileString("DEFAULTS", "mouseb2", szValue, DoomDir);
    sprintf(szValue, "%d", mouseb3);
    WritePrivateProfileString("DEFAULTS", "mouseb3", szValue, DoomDir);

    sprintf(szValue, "%d", usejoystick);
    WritePrivateProfileString("DEFAULTS", "use_joystick", szValue, DoomDir);
    sprintf(szValue, "%d", joybfire);
    WritePrivateProfileString("DEFAULTS", "joyb_fire", szValue, DoomDir);
    sprintf(szValue, "%d", joybstrafe);
    WritePrivateProfileString("DEFAULTS", "joyb_strafe", szValue, DoomDir);
    sprintf(szValue, "%d", joybuse);
    WritePrivateProfileString("DEFAULTS", "joyb_use", szValue, DoomDir);
    sprintf(szValue, "%d", joybspeed);
    WritePrivateProfileString("DEFAULTS", "joyb_speed", szValue, DoomDir);

    sprintf(szValue, "%d", joyb1);
    WritePrivateProfileString("DEFAULTS", "joyb1", szValue, DoomDir);
    sprintf(szValue, "%d", joyb2);
    WritePrivateProfileString("DEFAULTS", "joyb2", szValue, DoomDir);
    sprintf(szValue, "%d", joyb3);
    WritePrivateProfileString("DEFAULTS", "joyb3", szValue, DoomDir);
    sprintf(szValue, "%d", joyb4);
    WritePrivateProfileString("DEFAULTS", "joyb4", szValue, DoomDir);

    sprintf(szValue, "%d", screenblocks);
    WritePrivateProfileString("DEFAULTS", "screenblocks", szValue, DoomDir);

    sprintf(szValue, "%d", usegamma);
    WritePrivateProfileString("DEFAULTS", "usegamma", szValue, DoomDir);

    sprintf(szValue, "%d", numChannels);
    WritePrivateProfileString("DEFAULTS", "snd_channels", szValue, DoomDir);
   }


//
// M_LoadDefaults
//
extern byte	scantokey[128];

void WriteDebug(char *);
char MsgText[256];

void GetININame()
   {
    GetProfileString("WINDOOM", "DIRECTORY", "", DoomDir, 128 );
    if (strlen(DoomDir) == 0)
       {
        getcwd(DoomDir, 128);
        WriteProfileString("WINDOOM", "DIRECTORY", DoomDir );
       }
    strcat(DoomDir, "\\WinDoom.INI");
   }

void M_LoadDefaults (void)
{
    // Win95 specific

    GetININame();

    mouseSensitivity = GetPrivateProfileInt("DEFAULTS", "mouse_sensitivity", 5, DoomDir);
    snd_SfxVolume = GetPrivateProfileInt("DEFAULTS", "sfx_volume", 15, DoomDir);
    snd_MusicVolume = GetPrivateProfileInt("DEFAULTS", "music_volume", 8, DoomDir);
    showMessages = GetPrivateProfileInt("DEFAULTS", "show_messages", 1, DoomDir);

    key_right = GetPrivateProfileInt("DEFAULTS", "key_right", KEY_RIGHTARROW, DoomDir);
    key_left = GetPrivateProfileInt("DEFAULTS", "key_left", KEY_LEFTARROW, DoomDir);
    key_up = GetPrivateProfileInt("DEFAULTS", "key_up", KEY_UPARROW, DoomDir);
    key_down = GetPrivateProfileInt("DEFAULTS", "key_down", KEY_DOWNARROW, DoomDir);

    key_strafeleft = GetPrivateProfileInt("DEFAULTS", "key_strafeleft", KEY_COMMA, DoomDir);
    key_straferight = GetPrivateProfileInt("DEFAULTS", "key_straferight", KEY_PERIOD, DoomDir);

    key_fire = GetPrivateProfileInt("DEFAULTS", "key_fire", KEY_RCTRL, DoomDir);
    key_use = GetPrivateProfileInt("DEFAULTS", "key_use", KEY_SPACE, DoomDir);
    key_strafe = GetPrivateProfileInt("DEFAULTS", "key_strafe", KEY_RALT, DoomDir);
    key_speed = GetPrivateProfileInt("DEFAULTS", "key_speed", KEY_RSHIFT, DoomDir);

    key_mvert = GetPrivateProfileInt("DEFAULTS", "key_mvert", KEY_SLASH, DoomDir);

    always_run = GetPrivateProfileInt("DEFAULTS", "always_run", FALSE, DoomDir);
    swap_stereo = GetPrivateProfileInt("DEFAULTS", "swap_stereo", FALSE, DoomDir);
    mvert = GetPrivateProfileInt("DEFAULTS", "mvert", TRUE, DoomDir);
    keylink = GetPrivateProfileInt("DEFAULTS", "keylink", TRUE, DoomDir);

    usemouse = GetPrivateProfileInt("DEFAULTS", "use_mouse", TRUE, DoomDir);
    mousebfire = GetPrivateProfileInt("DEFAULTS", "mouseb_fire", 0, DoomDir);
    mousebstrafe = GetPrivateProfileInt("DEFAULTS", "mouseb_strafe", 1, DoomDir);
    mousebforward = GetPrivateProfileInt("DEFAULTS", "mouseb_forward", 2, DoomDir);

    mouseb1 = GetPrivateProfileInt("DEFAULTS", "mouseb1", key_fire, DoomDir);
    mouseb2 = GetPrivateProfileInt("DEFAULTS", "mouseb2", key_strafe, DoomDir);
    mouseb3 = GetPrivateProfileInt("DEFAULTS", "mouseb3", key_up, DoomDir);

    usejoystick = GetPrivateProfileInt("DEFAULTS", "use_joystick", FALSE, DoomDir);
    joybfire = GetPrivateProfileInt("DEFAULTS", "joyb_fire", 0, DoomDir);
    joybstrafe = GetPrivateProfileInt("DEFAULTS", "joyb_strafe", 1, DoomDir);
    joybuse = GetPrivateProfileInt("DEFAULTS", "joyb_use", 3, DoomDir);
    joybspeed = GetPrivateProfileInt("DEFAULTS", "joyb_speed", 2, DoomDir);

    joyb1 = GetPrivateProfileInt("DEFAULTS", "joyb1", key_fire, DoomDir);
    joyb2 = GetPrivateProfileInt("DEFAULTS", "joyb2", key_strafe, DoomDir);
    joyb3 = GetPrivateProfileInt("DEFAULTS", "joyb3", key_use, DoomDir);
    joyb4 = GetPrivateProfileInt("DEFAULTS", "joyb4", key_speed, DoomDir);

    screenblocks = GetPrivateProfileInt("DEFAULTS", "screenblocks", 10, DoomDir);

    usegamma = GetPrivateProfileInt("DEFAULTS", "usegamma", 0, DoomDir);

    numChannels = GetPrivateProfileInt("DEFAULTS", "snd_channels", 256, DoomDir);
   }

/*
chatmacro0		"no macro"
chatmacro1		"no macro"
chatmacro2		"no macro"
chatmacro3		"no macro"
chatmacro4		"no macro"
chatmacro5		"no macro"
chatmacro6		"no macro"
chatmacro7		"no macro"
chatmacro8		"no macro"
chatmacro9		"no macro"
*/

//
// SCREEN SHOTS
//


typedef struct
{
    char		manufacturer;
    char		version;
    char		encoding;
    char		bits_per_pixel;

    unsigned short	xmin;
    unsigned short	ymin;
    unsigned short	xmax;
    unsigned short	ymax;
    
    unsigned short	hres;
    unsigned short	vres;

    unsigned char	palette[48];
    
    char		reserved;
    char		color_planes;
    unsigned short	bytes_per_line;
    unsigned short	palette_type;
    
    char		filler[58];
    unsigned char	data;		// unbounded
} pcx_t;


//
// WritePCXfile
//
void
WritePCXfile
( char*		filename,
  byte*		data,
  int		width,
  int		height,
  byte*		palette )
{
    int		i;
    int		length;
    pcx_t*	pcx;
    byte*	pack;
	
    pcx = Z_Malloc (width*height*2+1000, PU_STATIC, NULL);

    pcx->manufacturer = 0x0a;		// PCX id
    pcx->version = 5;			// 256 color
    pcx->encoding = 1;			// uncompressed
    pcx->bits_per_pixel = 8;		// 256 color
    pcx->xmin = 0;
    pcx->ymin = 0;
    pcx->xmax = SHORT(width-1);
    pcx->ymax = SHORT(height-1);
    pcx->hres = SHORT(width);
    pcx->vres = SHORT(height);
    memset (pcx->palette,0,sizeof(pcx->palette));
    pcx->color_planes = 1;		// chunky image
    pcx->bytes_per_line = SHORT(width);
    pcx->palette_type = SHORT(2);	// not a grey scale
    memset (pcx->filler,0,sizeof(pcx->filler));


    // pack the image
    pack = &pcx->data;
	
    for (i=0 ; i<width*height ; i++)
    {
	if ( (*data & 0xc0) != 0xc0)
	    *pack++ = *data++;
	else
	{
	    *pack++ = 0xc1;
	    *pack++ = *data++;
	}
    }
    
    // write the palette
    *pack++ = 0x0c;	// palette ID byte
    for (i=0 ; i<768 ; i++)
	*pack++ = gammatable[usegamma][*palette++];
    
    // write output file
    length = pack - (byte *)pcx;
    M_WriteFile (filename, pcx, length);

    Z_Free (pcx);
}


//
// M_ScreenShot
//
void M_ScreenShot (void)
{
    int		i;
    byte*	linear;
    char	lbmname[12];
    
    // munge planar buffer to linear
    linear = screens[2];
    I_ReadScreen (linear);
    
    // find a file name to save it to
    strcpy(lbmname,"DOOM00.pcx");
		
    for (i=0 ; i<=99 ; i++)
    {
	lbmname[4] = i/10 + '0';
	lbmname[5] = i%10 + '0';
	if (access(lbmname,0) == -1)
	    break;	// file doesn't exist
    }
    if (i==100)
	I_Error ("M_ScreenShot: Couldn't create a PCX");
    
    // save the pcx file
    WritePCXfile (lbmname, linear,
		  SCREENWIDTH, SCREENHEIGHT,
		  W_CacheLumpName ("PLAYPAL",PU_CACHE));
	
	players[consoleplayer].message = "screen shot";
}


