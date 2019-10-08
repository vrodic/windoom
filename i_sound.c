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
//	System interface for sound.
//
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

#include <fcntl.h>
#include <io.h>


#include "z_zone.h"

#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"

#include "doomdef.h"


////////////////////////////////////////////////////////////////////////////
// WinDoom - DirectSound
////////////////////////////////////////////////////////////////////////////
#include <dsound.h>

char MsgText[256];
void WriteDebug(char *);

#define NUM_SOUND_FX 128
#define SB_SIZE      20480

void CreateSoundBuffer(int Channel, int length, unsigned char *data);
void I_PlaySoundEffect(int sfxid, int Channel, int volume, int pan);
void DS_Error( HRESULT hresult, char *msg );

#define NUM_DSBUFFERS 256

typedef enum { dsb_perm, dsb_temp } dsb_type;

extern LPDIRECTSOUND        lpDS;
extern LPDIRECTSOUNDBUFFER  lpDSBuffer[NUM_DSBUFFERS];

typedef struct
   {
    void   *origin;
    int     dsb_type;
    int     sfxid;
   }DSBControl_t;

DSBControl_t DSBControl[NUM_DSBUFFERS];

extern int swap_stereo;

////////////////////////////////////////////////////////////////////////////



// The number of internal mixing channels,
//  the samples calculated for each mixing step,
//  the size of the 16bit, 2 hardware channel (stereo)
//  mixing buffer, and the samplerate of the raw data.


// Needed for calling the actual sound output.
#define SAMPLECOUNT		512
#define NUM_CHANNELS		16
// It is 2 for 16bit, and 2 for two channels.


// The actual lengths of all sound effects.
int 		lengths[NUMSFX];

// The actual output device.
int	audio_fd;


//
// This function loads the sound data from the WAD lump,
//  for single sound.
//
void *getsfx( char *sfxname, int *len )
   {
    unsigned char*      sfx;
    unsigned char*      paddedsfx;
    int                 i;
    int                 size;
    int                 paddedsize;
    char                name[20];
    int                 sfxlump;

   
    sprintf(name, "ds%s", sfxname);

    // Get the sound data from the WAD, allocate lump
    //  in zone memory.
    // Now, there is a severe problem with the
    //  sound handling, in it is not (yet/anymore)
    //  gamemode aware. That means, sounds from
    //  DOOM II will be requested even with DOOM
    //  shareware.
    // The sound list is wired into sounds.c,
    //  which sets the external variable.
    // I do not do runtime patches to that
    //  variable. Instead, we will use a
    //  default sound for replacement.

    if ( W_CheckNumForName(name) == -1 )
      sfxlump = W_GetNumForName("dspistol");
    else
      sfxlump = W_GetNumForName(name);
    
    size = W_LumpLength( sfxlump );

//    sprintf(MsgText, "Getting sound effect : %s - %d\n", name, size);
//    WriteDebug(MsgText);

    // Debug.
    // fprintf( stderr, "." );
    //fprintf( stderr, " -loading  %s (lump %d, %d bytes)\n",
    //	     sfxname, sfxlump, size );
    //fflush( stderr );
    
    sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_STATIC );

    // Pads the sound effect out to the mixing buffer size.
    // The original realloc would interfere with zone memory.
    paddedsize = ((size-8 + (SAMPLECOUNT-1)) / SAMPLECOUNT) * SAMPLECOUNT;

    // Allocate from zone memory.
    paddedsfx = (unsigned char*)Z_Malloc( paddedsize+8, PU_STATIC, 0 );
    // ddt: (unsigned char *) realloc(sfx, paddedsize+8);
    // This should interfere with zone memory handling,
    //  which does not kick in in the soundserver.

    // Now copy and pad.
    memcpy(  paddedsfx, sfx, size );
    for (i = size; i < paddedsize+8; i++)
        paddedsfx[i] = 128;

    // Remove the cached lump.
    Z_Free( sfx );
    
    // Preserve padded length.
    *len = paddedsize;

    // Return allocated padded data.
    return (void *) (paddedsfx + 8);
   }




//
// This function adds a sound to the
//  list of currently active sounds,
//  which is maintained as a given number
//  (eight, usually) of internal channels.
// Returns a handle.
//
int addsfx( int sfxid, int volume, int seperation, void *origin )
   {
    int		i;

    /*static unsigned short	handlenums = 0; // vrodic
    int		rc = -1; 
    
    int		oldest = gametic;
    int		oldestnum = 0;*/
    /*int		slot;

    int		rightvol;
    int		leftvol;*/

    int     iVolume, iPan;
    DWORD   dwDSBStatus;
    int     dsbchannel;

    // Chainsaw troubles.
    // Play these sound effects only one at a time.
    if ( sfxid == sfx_sawup  || sfxid == sfx_sawidl || sfxid == sfx_sawful ||
         sfxid == sfx_sawhit || sfxid == sfx_stnmov )
        dsbchannel = sfxid;
    else
       {
        dsbchannel = sfxid;
        if (lpDSBuffer[0] != 0)
            lpDSBuffer[sfxid]->lpVtbl->GetStatus(lpDSBuffer[sfxid], &dwDSBStatus);
        else
            dwDSBStatus = DS_OK;
        if (dwDSBStatus == DSBSTATUS_PLAYING)
           {
            for (i = NUMSFX; i < NUM_DSBUFFERS; i++)
               {
                if ((DSBControl[i].origin == origin) && (DSBControl[i].sfxid == sfxid))
                   {
                    dsbchannel = i;
                    break;
                   }
                if (DSBControl[i].sfxid == -1)
                   {
                    dsbchannel = i;
                    DSBControl[i].origin = origin;
                    break;
                   }
                if (DSBControl[i].sfxid >= 0)
                   {
                    lpDSBuffer[i]->lpVtbl->GetStatus(lpDSBuffer[i], &dwDSBStatus);
                    if (dwDSBStatus != DSBSTATUS_PLAYING)
                       {
                        dsbchannel = i;
                        DSBControl[i].origin = origin;
                        break;
                       }
                   }
                if (DSBControl[i].origin == origin)
                   {
                    dsbchannel = i;
                    break;
                   }
               }
           }
       }

    iVolume = 0-(128*(15-volume));
    if (iVolume < -10000)
       iVolume == -10000;
    iPan = (seperation-128)*20;
    if (iPan < -10000)
       iPan = -10000;
    if (iPan > 10000)
       iPan = 10000;

    if (swap_stereo == TRUE)
       iPan *= -1;


    I_PlaySoundEffect(sfxid, dsbchannel, iVolume, iPan);

    // You tell me.
    //return rc;
    return dsbchannel;
   }

// This function call starts a sound playing in a DirectSound buffer...
void I_PlaySoundEffect(int sfxid, int Channel, int iVolume, int iPan)
   {
    HRESULT hresult;
    //DWORD   dwDSBStatus; // vrodic

    if (Channel > NUM_SOUND_FX)
       {
        //WriteDebug("Invalid sound channel...\n");
        return;
       }
    if (lpDSBuffer[Channel] == 0)
       {
        //WriteDebug("Trying to play sound without DirectSound working...\n");
        return;
       }

    if ((DSBControl[Channel].dsb_type == dsb_temp) && (DSBControl[Channel].sfxid != sfxid))
       {
        if (DSBControl[Channel].sfxid > 0)
           {
/*
            lpDSBuffer[Channel]->lpVtbl->GetStatus(lpDSBuffer[Channel], &dwDSBStatus);
            if (dwDSBStatus == DSBSTATUS_PLAYING)
               {
                hresult = lpDSBuffer[Channel]->lpVtbl->Stop(lpDSBuffer[Channel]);
                if (hresult != DS_OK)
                    DS_Error(hresult, "lpDSBuffer.Stop");
               }
*/
            lpDSBuffer[Channel]->lpVtbl->Release(lpDSBuffer[Channel]);
            DSBControl[Channel].sfxid = -1;
           }

        if (DSBControl[Channel].sfxid < 0)
           {
            // clone temp buffer here...
            lpDS->lpVtbl->DuplicateSoundBuffer(lpDS, lpDSBuffer[sfxid], &lpDSBuffer[Channel]);
            DSBControl[Channel].sfxid = sfxid;
           }
       }
/*
    else
       {
        lpDSBuffer[Channel]->lpVtbl->GetStatus(lpDSBuffer[Channel], &dwDSBStatus);
        if (dwDSBStatus == DSBSTATUS_PLAYING)
           {
            hresult = lpDSBuffer[Channel]->lpVtbl->Stop(lpDSBuffer[Channel]);
            if (hresult != DS_OK)
                DS_Error(hresult, "lpDSBuffer.Stop");
           }
       }
*/
    hresult = lpDSBuffer[Channel]->lpVtbl->SetCurrentPosition(lpDSBuffer[Channel], 0);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.SetCurrentPosition");
    hresult = lpDSBuffer[Channel]->lpVtbl->SetVolume(lpDSBuffer[Channel], iVolume );
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.SetVolume");
    hresult = lpDSBuffer[Channel]->lpVtbl->SetPan(lpDSBuffer[Channel], iPan);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.SetPan");
    hresult = lpDSBuffer[Channel]->lpVtbl->Play(lpDSBuffer[Channel], 0, 0, 0);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.Play");
   }


void I_SetSfxVolume(int volume)
{
  // Identical to DOS.
  // Basically, this should propagate
  //  the menu/config file setting
  //  to the state variable used in
  //  the mixing.
  snd_SfxVolume = volume;
}

// MUSIC API - dummy. Some code from DOS version.
void I_SetMusicVolume(int volume)
{
  // Internal state variable.
  snd_MusicVolume = volume;
  // Now set volume on output device.
  // Whatever( snd_MusciVolume );
}


//
// Retrieve the raw data lump index
//  for a given SFX name.
//
int I_GetSfxLumpNum(sfxinfo_t* sfx)
{
    char namebuf[9];
    sprintf(namebuf, "ds%s", sfx->name);
    return W_GetNumForName(namebuf);
}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//
int I_StartSound( int id, int vol, int sep, int pitch, int priority, void *origin )
   {
    // UNUSED
    priority = 0;
  
    // Debug.
//    sprintf(MsgText, "starting sound %d", id );
//    WriteDebug(MsgText);
    
    // Returns a handle (not used).
    id = addsfx( id, vol, sep, origin );

//    sprintf(MsgText, "/handle is %d\n", id );
//    WriteDebug(MsgText);

    return id;
   }



void I_StopSound (int handle)
   {
    HRESULT hresult;
  // You need the handle returned by StartSound.
  // Would be looping all channels,
  //  tracking down the handle,
  //  an setting the channel to zero.
  
  // UNUSED.
//    handle = 0;

    if (handle > NUM_DSBUFFERS)
       {
        WriteDebug("Invalid sound channel...\n");
        return;
       }
    if (lpDSBuffer[handle] == 0)
       {
        WriteDebug("Trying to stop sound without DirectSound working...\n");
        return;
       }

    hresult = lpDSBuffer[handle]->lpVtbl->Stop(lpDSBuffer[handle]);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.Stop");
/*
    else
    if (DSBControl[handle].dsb_type == dsb_temp)
       {
        lpDSBuffer[handle]->lpVtbl->Release(lpDSBuffer[handle]);
        DSBControl[handle].origin = NULL;
        DSBControl[handle].sfxid = -1;
       }
*/
   }


int I_SoundIsPlaying(int handle)
   {
    DWORD dwStatus;
    HRESULT hresult;

    if (lpDSBuffer[handle] == 0)
       {
        //WriteDebug("Trying to get status of a sound without DirectSound working...\n");
        return FALSE;
       }

    hresult = lpDSBuffer[handle]->lpVtbl->GetStatus(lpDSBuffer[handle], &dwStatus);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.GetStatus");
    return (dwStatus == DSBSTATUS_PLAYING);
    // Ouch.
//    return gametic < handle;
   }


void
I_UpdateSoundParams
( int	handle,
  int	vol,
  int	sep,
  int	pitch)
{
  // I fail too see that this is used.
  // Would be using the handle to identify
  //  on which channel the sound might be active,
  //  and resetting the channel parameters.
 int iVolume, iPan;
 HRESULT hresult;

 if (lpDSBuffer[handle] == 0)
    return;

    iVolume = 0-(128*(15-vol));
    if (iVolume < -10000)
       iVolume == -10000;
    iPan = (sep-128)*20;
    if (iPan < -10000)
       iPan = -10000;
    if (iPan > 10000)
       iPan = 10000;

    if (swap_stereo == TRUE)
       iPan *= -1;

    hresult = lpDSBuffer[handle]->lpVtbl->SetVolume(lpDSBuffer[handle], iVolume );
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.SetVolume");
    hresult = lpDSBuffer[handle]->lpVtbl->SetPan(lpDSBuffer[handle], iPan);
    if (hresult != DS_OK)
        DS_Error(hresult, "lpDSBuffer.SetPan");
  // UNUSED.
  //handle = vol = sep = pitch = 0;
}




void I_ShutdownSound(void)
{    
  // Wait till all pending sounds are finished.
  /*int done = 0;
  int i;
  
  // FIXME (below).
  fprintf( stderr, "I_ShutdownSound: NOT finishing pending sounds\n");
  fflush( stderr );
  
  while ( !done )
  {
    //for( i=0 ; i<8 && !channels[i] ; i++);
    
    // FIXME. No proper channel output.
    //if (i==8)
    done=1;
  } 
  // Cleaning up -releasing the DSP device.*/
  close ( audio_fd );
  // Done.
  return;
}



void
I_InitSound()
{ 
  int i;

  for (i = 1; i < NUMSFX; i++)
     { 
      // Alias? Example is the chaingun sound linked to pistol.
      if (!S_sfx[i].link)
         {
          // Load data from WAD file.
          S_sfx[i].data = getsfx( S_sfx[i].name, &lengths[i] );
         }	
      else
         {
          // Previously loaded already?
          S_sfx[i].data = S_sfx[i].link->data;
          lengths[i] = lengths[(S_sfx[i].link - S_sfx)/sizeof(sfxinfo_t)];
         }
      CreateSoundBuffer(i, lengths[i], S_sfx[i].data);
      DSBControl[i].origin = NULL;
      DSBControl[i].sfxid = i;
      DSBControl[i].dsb_type = dsb_perm;
     }
  for (; i < NUM_DSBUFFERS; i++)
     {
      DSBControl[i].origin = NULL;
      DSBControl[i].sfxid = -1;
      DSBControl[i].dsb_type = dsb_temp;
     }

}




//
// MUSIC API.
// Still no music done.
// Remains. Dummies.
//
void I_InitMusic(void)		{ }
void I_ShutdownMusic(void)	{ }

static int	looping=0;
static int	musicdies=-1;

void I_PlaySong(int handle, int looping)
{
  // UNUSED.
  handle = looping = 0;
  musicdies = gametic + TICRATE*30;
}

void I_PauseSong (int handle)
{
  // UNUSED.
  handle = 0;
}

void I_ResumeSong (int handle)
{
  // UNUSED.
  handle = 0;
}

void I_StopSong(int handle)
{
  // UNUSED.
  handle = 0;
  
  looping = 0;
  musicdies = 0;
}

void I_UnRegisterSong(int handle)
{
  // UNUSED.
  handle = 0;
}

int I_RegisterSong(void* data)
{
  // UNUSED.
  data = NULL;
  
  return 1;
}

// Is the song playing?
int I_QrySongPlaying(int handle)
{
  // UNUSED.
  handle = 0;
  return looping || musicdies > gametic;
}
