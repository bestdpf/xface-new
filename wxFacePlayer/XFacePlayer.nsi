; XfacePlayer.nsi
;
; This script is based on example2.nsi in NSIS examples
; Koray Balci 12.05.2004
;

; The name of the installer
Name "XfacePlayer"

Icon "./res/XFaceLogo.ico"
UninstallIcon "./res/XFaceLogo.ico"

; The file to write
OutFile "../setup/XfacePlayer-0.4b-setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\XfacePlayer
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\ITCirst_XfacePlayer "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install the XfacePlayer version 0.4b on your computer. Developed and provided by ITC-irst."
; The text to prompt the user to enter a directory
DirText "Choose a directory to install XfacePlayer:"

; The stuff to install
Section "XfacePlayer (required)"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\Faces\alice"
  ;CreateDirectory "$INSTDIR\Faces\john"
  CreateDirectory "$INSTDIR\Faps"
  CreateDirectory "$INSTDIR\Bin"
  CreateDirectory "$INSTDIR\Bin\res"
  SetOutPath $INSTDIR\Bin
  ; Put main executable stuff
  File ".\Release\XfacePlayer.exe"
  File "C:\WINDOWS\system32\msvcp71.dll"
  File "C:\WINDOWS\system32\msvcr71.dll"
  File "C:\WINDOWS\system32\sdl.dll"
  
  SetOutPath $INSTDIR\Bin\res
  File ".\res\camera.gif"
  File ".\res\inform.gif"
  File ".\res\open.gif"
  File ".\res\pause.gif"
  File ".\res\play.gif"
  File ".\res\stop.gif"
  File ".\res\TrafficGreen.gif"
  File ".\res\TrafficRed.gif"
  File ".\res\volume.gif"
  File ".\res\volumeNo.gif"
  File ".\res\XfaceLogo.ico"
  
  ; Put Alice
  SetOutPath $INSTDIR\Faces\alice
  File "..\Faces\alice\alice.wrl"
  File "..\Faces\alice\alice_tongue.bmp"
  File "..\Faces\alice\alice_teethUpper.bmp"
  File "..\Faces\alice\alice_teethLower.bmp"
  File "..\Faces\alice\alice_HairGraduatedBob.bmp"
  File "..\Faces\alice\alice_sock.bmp"
  File "..\Faces\alice\alice_skin_hi.bmp"
  File "..\Faces\alice\alice_eyeR_hi.bmp"
  File "..\Faces\alice\alice_eyeL_hi.bmp"
  File "..\Faces\alice\alice.xml"
  
  ; Put some faps
  SetOutPath $INSTDIR\Faps
  File "..\Faps\anger.fap"
  File "..\Faps\beg.fap"
  File "..\Faps\disgust.fap"
  File "..\Faps\fear.fap"
  File "..\Faps\happy-for.fap"
  File "..\Faps\hate.fap"
  File "..\Faps\joy.fap"
  File "..\Faps\pride.fap"
  File "..\Faps\sample.wav"
  
  SetOutPath $INSTDIR\Bin
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\ITCirst_XfacePlayer "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfacePlayer" "DisplayName" "ITC-irst XfacePlayer (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfacePlayer" "XfacePlayer" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\XfacePlayer"
  CreateShortCut "$SMPROGRAMS\XfacePlayer\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\XfacePlayer\XfacePlayer.lnk" "$INSTDIR\Bin\XfacePlayer.exe" "" "$INSTDIR\Bin\XfacePlayer.exe" 0
SectionEnd

; uninstall stuff

UninstallText "This will uninstall XfacePlayer from your PC. Hit next to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfacePlayer"
  DeleteRegKey HKLM SOFTWARE\ITCirst_XfacePlayer
  ; remove files
  Delete $INSTDIR\Bin\XfacePlayer.exe
  Delete $INSTDIR\Bin\msvcp71.dll
  Delete $INSTDIR\Bin\msvcr71.dll
  Delete $INSTDIR\Bin\SDL.dll
  
  Delete $INSTDIR\Bin\res\camera.gif"
  Delete $INSTDIR\Bin\res\inform.gif"
  Delete $INSTDIR\Bin\res\open.gif"
  Delete $INSTDIR\Bin\res\pause.gif"
  Delete $INSTDIR\Bin\res\play.gif"
  Delete $INSTDIR\Bin\res\stop.gif"
  Delete $INSTDIR\Bin\res\TrafficGreen.gif"
  Delete $INSTDIR\Bin\res\TrafficRed.gif"
  Delete $INSTDIR\Bin\res\volume.gif"
  Delete $INSTDIR\Bin\res\volumeNo.gif"
  Delete $INSTDIR\Bin\res\XfaceLogo.ico"
  
  Delete $INSTDIR\Faces\alice\alice.wrl"
  Delete $INSTDIR\Faces\alice\alice_tongue.bmp"
  Delete $INSTDIR\Faces\alice\alice_teethUpper.bmp"
  Delete $INSTDIR\Faces\alice\alice_teethLower.bmp"
  Delete $INSTDIR\Faces\alice\alice_sock.bmp"
  Delete $INSTDIR\Faces\alice\alice_skin_hi.bmp"
  Delete $INSTDIR\Faces\alice\alice_eyeR_hi.bmp"
  Delete $INSTDIR\Faces\alice\alice_eyeL_hi.bmp"
  Delete $INSTDIR\Faces\alice\alice_HairGraduatedBob.bmp"
  Delete $INSTDIR\Faces\alice\alice.xml"
  
  Delete $INSTDIR\Faps\anger.fap"
  Delete $INSTDIR\Faps\beg.fap"
  Delete $INSTDIR\Faps\disgust.fap"
  Delete $INSTDIR\Faps\fear.fap"
  Delete $INSTDIR\Faps\happy-for.fap"
  Delete $INSTDIR\Faps\hate.fap"
  Delete $INSTDIR\Faps\joy.fap"
  Delete $INSTDIR\Faps\pride.fap"
  Delete $INSTDIR\Faps\sample.wav"
  
  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\XfacePlayer\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\XfacePlayer"
  RMDir "$INSTDIR\Faces\alice"
  ;RMDir "$INSTDIR\Faces\john"
  RMDir "$INSTDIR\Faces"
  RMDir "$INSTDIR\Faps"
  RMDir "$INSTDIR\Bin\res"
  RMDir "$INSTDIR\Bin"
  RMDir "$INSTDIR"
SectionEnd

; eof
