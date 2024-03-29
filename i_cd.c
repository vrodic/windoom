// i_cd.c
// Copyright � 1998, Bruce A. Lewis
// This code may not be re-used in a commercial product
// of any kind without express written consent from
// the author, Bruce A. Lewis.
//
// Global define needed for calling these functions
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "i_cd.h"

// Local define needed only for error message capture and display
#define     MCIMSGLEN  256
char        CDmsg[MCIMSGLEN+32];

BOOL CDOpen(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_OPEN_PARMS     open;
    MCIERROR           MCI_Error;

    open.dwCallback       = (DWORD)hWnd;
    open.lpstrDeviceType  = "cdaudio";

    MCI_Error = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)&open);

    cdd->CDError = 0;
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nOpenCD");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDDevice = open.wDeviceID;
    return(TRUE);
   }

BOOL CDCheck(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_STATUS_PARMS   status;
    MCIERROR           MCI_Error;

    status.dwCallback = (DWORD)hWnd;
    status.dwItem     = MCI_STATUS_MEDIA_PRESENT;

    cdd->CDError = 0;
    MCI_Error = mciSendCommand(cdd->CDDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCheckCD");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDMedia = status.dwReturn;
    return(TRUE);
   }

BOOL CDMediaIdentity(HWND hWnd, CD_Data_t *cdd, char *szIdent)
   {
    MCI_INFO_PARMS     info;
    MCIERROR           MCI_Error;

    info.dwCallback       = (DWORD)hWnd;
    info.lpstrReturn      = szIdent;
    info.dwRetSize        = 30;

    cdd->CDError = 0;
    MCI_Error = mciSendCommand(cdd->CDDevice, MCI_INFO, MCI_INFO_MEDIA_IDENTITY, (DWORD)&info);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nMediaIdentityCD");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    return(TRUE);
   }


BOOL CDTrackCount(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_STATUS_PARMS   status;
    MCIERROR           MCI_Error;

    status.dwCallback       = (DWORD)hWnd;
    status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;

    cdd->CDError = 0;
    cdd->CDTrackCount = 0;
    MCI_Error = mciSendCommand(cdd->CDDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCDTracks");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDTrackCount = status.dwReturn;
    return(TRUE);
   }

BOOL CDTrackType(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_STATUS_PARMS   status;
    MCIERROR           MCI_Error;

    status.dwCallback  = (DWORD)hWnd;
    status.dwItem      = MCI_CDA_STATUS_TYPE_TRACK;
    status.dwTrack     = cdd->CDTrack+1;

    cdd->CDError = 0;
    MCI_Error = mciSendCommand(cdd->CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD)&status);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCDTrackType");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDTrackList[cdd->CDTrack] = status.dwReturn;
    return(TRUE);
   }

BOOL CDTrackPlay(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_PLAY_PARMS     play;
    MCI_SET_PARMS      set;
    MCIERROR           MCI_Error;
    char               tstr[16];

    cdd->CDError = 0;
    set.dwTimeFormat = MCI_FORMAT_TMSF;
    mciSendCommand(cdd->CDDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &set);

    play.dwCallback = (DWORD)hWnd;
    play.dwTo = 0L;
    play.dwFrom = MCI_MAKE_TMSF(cdd->CDTrack+1, 0, 0, 0);
    if (cdd->CDTrack == (cdd->CDTrackCount-1))
        MCI_Error = mciSendCommand(cdd->CDDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)(LPVOID)&play);
    else
       {
        play.dwTo = MCI_MAKE_TMSF(cdd->CDTrack+2, 0, 0, 0);
        MCI_Error = mciSendCommand(cdd->CDDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID)&play);
       }
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCDTrackPlay - ");
        sprintf(tstr, "No: %d", cdd->CDTrack);
        strcat(CDmsg, tstr);
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDStatus = cd_play;
    return(TRUE);
   }

BOOL CDPause(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_SET_PARMS      set;
    MCI_STATUS_PARMS   status;
    MCIERROR           MCI_Error;

    cdd->CDError = 0;
    set.dwTimeFormat   = MCI_FORMAT_TMSF;
    mciSendCommand(cdd->CDDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&set);

    status.dwCallback  = (DWORD)hWnd;
    status.dwItem = MCI_STATUS_POSITION;

    MCI_Error = mciSendCommand(cdd->CDDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCDPause");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    cdd->CDPosition = status.dwReturn;
    cdd->CDTrack = MCI_TMSF_TRACK(cdd->CDPosition);
    mciSendCommand(cdd->CDDevice, MCI_STOP, 0, (DWORD)NULL);
    return(TRUE);
   }

BOOL CDResume(HWND hWnd, CD_Data_t *cdd)
   {
    MCI_SET_PARMS      set;
    MCI_PLAY_PARMS     play;
    MCIERROR           MCI_Error;

    cdd->CDError = 0;
    set.dwTimeFormat   = MCI_FORMAT_TMSF;
    mciSendCommand(cdd->CDDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &set);

    play.dwCallback = (DWORD)hWnd;
    play.dwTo = 0;
    play.dwFrom = cdd->CDPosition;
    if (cdd->CDTrack == (cdd->CDTrackCount-1))
        MCI_Error = mciSendCommand(cdd->CDDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)(LPVOID)&play);
    else
       {
        play.dwTo = MCI_MAKE_TMSF(MCI_TMSF_TRACK(cdd->CDPosition)+1, 0, 0, 0);
        MCI_Error = mciSendCommand(cdd->CDDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID)&play);
       }
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, CDmsg, MCIMSGLEN);
        strcat(CDmsg, "\nCDResume");
        MessageBox(hWnd, CDmsg, NULL, MB_ICONSTOP | MB_OK);
        cdd->CDError = MCI_Error;
        return(FALSE);
       }
    return(TRUE);
   }

void CDStop(CD_Data_t *cdd)
   {
    mciSendCommand(cdd->CDDevice, MCI_STOP, 0, (DWORD)NULL);
    cdd->CDStatus = cd_stop;
   }

void CDClose(CD_Data_t *cdd)
   {
    mciSendCommand(cdd->CDDevice, MCI_CLOSE, 0, (DWORD)NULL);
   }

