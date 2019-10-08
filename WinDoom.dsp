# Microsoft Developer Studio Project File - Name="WinDoom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WinDoom - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinDoom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinDoom.mak" CFG="WinDoom - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinDoom - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WinDoom - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinDoom - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinDoom_"
# PROP BASE Intermediate_Dir ".\WinDoom_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib kernel32.lib user32.lib gdi32.lib dsound.lib ddraw.lib dinput.lib winmm.lib wsock32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "WinDoom - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDoom0"
# PROP BASE Intermediate_Dir ".\WinDoom0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib ddraw.lib dinput.lib winmm.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "WinDoom - Win32 Release"
# Name "WinDoom - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\am_map.c
# End Source File
# Begin Source File

SOURCE=.\d_console.c
# End Source File
# Begin Source File

SOURCE=.\d_items.c
# End Source File
# Begin Source File

SOURCE=.\d_main.c
# End Source File
# Begin Source File

SOURCE=.\d_net.c
# End Source File
# Begin Source File

SOURCE=.\doomdef.c
# End Source File
# Begin Source File

SOURCE=.\doomstat.c
# End Source File
# Begin Source File

SOURCE=.\dstrings.c
# End Source File
# Begin Source File

SOURCE=.\dxerr.c
# End Source File
# Begin Source File

SOURCE=.\f_finale.c
# End Source File
# Begin Source File

SOURCE=.\f_wipe.c
# End Source File
# Begin Source File

SOURCE=.\g_game.c
# End Source File
# Begin Source File

SOURCE=.\hu_lib.c
# End Source File
# Begin Source File

SOURCE=.\hu_stuff.c
# End Source File
# Begin Source File

SOURCE=.\i_cd.c
# End Source File
# Begin Source File

SOURCE=.\i_main.c
# End Source File
# Begin Source File

SOURCE=.\i_midi.c
# End Source File
# Begin Source File

SOURCE=.\i_net.c
# End Source File
# Begin Source File

SOURCE=.\i_sound.c
# End Source File
# Begin Source File

SOURCE=.\i_system.c
# End Source File
# Begin Source File

SOURCE=.\i_video.c
# End Source File
# Begin Source File

SOURCE=.\info.c
# End Source File
# Begin Source File

SOURCE=.\m_argv.c
# End Source File
# Begin Source File

SOURCE=.\m_bbox.c
# End Source File
# Begin Source File

SOURCE=.\m_cheat.c
# End Source File
# Begin Source File

SOURCE=.\m_fixed.c
# End Source File
# Begin Source File

SOURCE=.\m_menu.c
# End Source File
# Begin Source File

SOURCE=.\m_misc.c
# End Source File
# Begin Source File

SOURCE=.\m_music.c
# End Source File
# Begin Source File

SOURCE=.\m_random.c
# End Source File
# Begin Source File

SOURCE=.\m_swap.c
# End Source File
# Begin Source File

SOURCE=.\p_ceilng.c
# End Source File
# Begin Source File

SOURCE=.\p_doors.c
# End Source File
# Begin Source File

SOURCE=.\p_enemy.c
# End Source File
# Begin Source File

SOURCE=.\p_floor.c
# End Source File
# Begin Source File

SOURCE=.\p_inter.c
# End Source File
# Begin Source File

SOURCE=.\p_lights.c
# End Source File
# Begin Source File

SOURCE=.\p_map.c
# End Source File
# Begin Source File

SOURCE=.\p_maputl.c
# End Source File
# Begin Source File

SOURCE=.\p_mobj.c
# End Source File
# Begin Source File

SOURCE=.\p_plats.c
# End Source File
# Begin Source File

SOURCE=.\p_pspr.c
# End Source File
# Begin Source File

SOURCE=.\p_saveg.c
# End Source File
# Begin Source File

SOURCE=.\p_setup.c
# End Source File
# Begin Source File

SOURCE=.\p_sight.c
# End Source File
# Begin Source File

SOURCE=.\p_spec.c
# End Source File
# Begin Source File

SOURCE=.\p_switch.c
# End Source File
# Begin Source File

SOURCE=.\p_telept.c
# End Source File
# Begin Source File

SOURCE=.\p_tick.c
# End Source File
# Begin Source File

SOURCE=.\p_user.c
# End Source File
# Begin Source File

SOURCE=.\qmus2mid.c
# End Source File
# Begin Source File

SOURCE=.\r_bsp.c
# End Source File
# Begin Source File

SOURCE=.\r_data.c
# End Source File
# Begin Source File

SOURCE=.\r_draw.c
# End Source File
# Begin Source File

SOURCE=.\r_main.c
# End Source File
# Begin Source File

SOURCE=.\r_plane.c
# End Source File
# Begin Source File

SOURCE=.\r_segs.c
# End Source File
# Begin Source File

SOURCE=.\r_sky.c
# End Source File
# Begin Source File

SOURCE=.\r_things.c
# End Source File
# Begin Source File

SOURCE=.\s_sound.c
# End Source File
# Begin Source File

SOURCE=.\sounds.c
# End Source File
# Begin Source File

SOURCE=.\st_lib.c
# End Source File
# Begin Source File

SOURCE=.\st_stuff.c
# End Source File
# Begin Source File

SOURCE=.\tables.c
# End Source File
# Begin Source File

SOURCE=.\v_video.c
# End Source File
# Begin Source File

SOURCE=.\w_wad.c
# End Source File
# Begin Source File

SOURCE=.\wi_stuff.c
# End Source File
# Begin Source File

SOURCE=.\windoom.rc
# End Source File
# Begin Source File

SOURCE=.\z_zone.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\am_map.h
# End Source File
# Begin Source File

SOURCE=.\d_console.h
# End Source File
# Begin Source File

SOURCE=.\d_englsh.h
# End Source File
# Begin Source File

SOURCE=.\d_event.h
# End Source File
# Begin Source File

SOURCE=.\d_french.h
# End Source File
# Begin Source File

SOURCE=.\d_items.h
# End Source File
# Begin Source File

SOURCE=.\d_main.h
# End Source File
# Begin Source File

SOURCE=.\d_net.h
# End Source File
# Begin Source File

SOURCE=.\d_player.h
# End Source File
# Begin Source File

SOURCE=.\d_think.h
# End Source File
# Begin Source File

SOURCE=.\d_ticcmd.h
# End Source File
# Begin Source File

SOURCE=.\doomdata.h
# End Source File
# Begin Source File

SOURCE=.\doomdef.h
# End Source File
# Begin Source File

SOURCE=.\doomstat.h
# End Source File
# Begin Source File

SOURCE=.\doomtype.h
# End Source File
# Begin Source File

SOURCE=.\dstrings.h
# End Source File
# Begin Source File

SOURCE=.\dxerr.h
# End Source File
# Begin Source File

SOURCE=.\f_finale.h
# End Source File
# Begin Source File

SOURCE=.\f_wipe.h
# End Source File
# Begin Source File

SOURCE=.\g_game.h
# End Source File
# Begin Source File

SOURCE=.\hu_lib.h
# End Source File
# Begin Source File

SOURCE=.\hu_stuff.h
# End Source File
# Begin Source File

SOURCE=.\i_cd.h
# End Source File
# Begin Source File

SOURCE=.\i_midi.h
# End Source File
# Begin Source File

SOURCE=.\i_net.h
# End Source File
# Begin Source File

SOURCE=.\i_sound.h
# End Source File
# Begin Source File

SOURCE=.\i_system.h
# End Source File
# Begin Source File

SOURCE=.\i_video.h
# End Source File
# Begin Source File

SOURCE=.\info.h
# End Source File
# Begin Source File

SOURCE=.\m_argv.h
# End Source File
# Begin Source File

SOURCE=.\m_bbox.h
# End Source File
# Begin Source File

SOURCE=.\m_cheat.h
# End Source File
# Begin Source File

SOURCE=.\m_fixed.h
# End Source File
# Begin Source File

SOURCE=.\m_menu.h
# End Source File
# Begin Source File

SOURCE=.\m_misc.h
# End Source File
# Begin Source File

SOURCE=.\m_music.h
# End Source File
# Begin Source File

SOURCE=.\m_random.h
# End Source File
# Begin Source File

SOURCE=.\m_swap.h
# End Source File
# Begin Source File

SOURCE=.\p_inter.h
# End Source File
# Begin Source File

SOURCE=.\p_local.h
# End Source File
# Begin Source File

SOURCE=.\p_mobj.h
# End Source File
# Begin Source File

SOURCE=.\p_pspr.h
# End Source File
# Begin Source File

SOURCE=.\p_saveg.h
# End Source File
# Begin Source File

SOURCE=.\p_setup.h
# End Source File
# Begin Source File

SOURCE=.\p_spec.h
# End Source File
# Begin Source File

SOURCE=.\p_tick.h
# End Source File
# Begin Source File

SOURCE=.\qmus2mid.h
# End Source File
# Begin Source File

SOURCE=.\r_bsp.h
# End Source File
# Begin Source File

SOURCE=.\r_data.h
# End Source File
# Begin Source File

SOURCE=.\r_defs.h
# End Source File
# Begin Source File

SOURCE=.\r_draw.h
# End Source File
# Begin Source File

SOURCE=.\r_local.h
# End Source File
# Begin Source File

SOURCE=.\r_main.h
# End Source File
# Begin Source File

SOURCE=.\r_plane.h
# End Source File
# Begin Source File

SOURCE=.\r_segs.h
# End Source File
# Begin Source File

SOURCE=.\r_sky.h
# End Source File
# Begin Source File

SOURCE=.\r_state.h
# End Source File
# Begin Source File

SOURCE=.\r_things.h
# End Source File
# Begin Source File

SOURCE=.\s_sound.h
# End Source File
# Begin Source File

SOURCE=.\sounds.h
# End Source File
# Begin Source File

SOURCE=.\st_lib.h
# End Source File
# Begin Source File

SOURCE=.\st_stuff.h
# End Source File
# Begin Source File

SOURCE=.\tables.h
# End Source File
# Begin Source File

SOURCE=.\v_video.h
# End Source File
# Begin Source File

SOURCE=.\w_wad.h
# End Source File
# Begin Source File

SOURCE=.\wi_stuff.h
# End Source File
# Begin Source File

SOURCE=.\z_zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
