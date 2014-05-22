; XfaceClient.nsi
;
; This script is based on example2.nsi in NSIS examples
; Koray Balci 12.05.2004
;

; The name of the installer
Name "XfaceClient"

Icon "./res/XFaceLogo.ico"
UninstallIcon "./res/XFaceLogo.ico"

; The file to write
OutFile "../setup/XfaceClient-0.2-setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\XfaceClient
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\ITCirst_XfaceClient "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install the XfaceClient version 0.2 on your computer. Developed and provided by ITC-irst."
; The text to prompt the user to enter a directory
DirText "Choose a directory to install XfaceClient:"

; The stuff to install
Section "XfaceClient (required)"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\Bin"
  CreateDirectory "$INSTDIR\Bin\res"
  SetOutPath $INSTDIR\Bin
  ; Put main executable stuff
  File ".\Release\XfaceClient.exe"
  File "C:\WINDOWS\system32\msvcp71.dll"
  File "C:\WINDOWS\system32\msvcr71.dll"
  
  SetOutPath $INSTDIR\Bin\res
  File ".\res\inform.gif"
  File ".\res\open.gif"
  File ".\res\pause.gif"
  File ".\res\play.gif"
  File ".\res\stop.gif"
  File ".\res\TrafficGreen.gif"
  File ".\res\TrafficRed.gif"
  File ".\res\APML1.gif"
  File ".\res\APML2.gif"
  File ".\res\XfaceLogo.ico"
  
  SetOutPath $INSTDIR\Bin
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\ITCirst_XfaceClient "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfaceClient" "DisplayName" "ITC-irst XfaceClient (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfaceClient" "XfaceClient" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\XfaceClient"
  CreateShortCut "$SMPROGRAMS\XfaceClient\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\XfaceClient\XfaceClient.lnk" "$INSTDIR\Bin\XfaceClient.exe" "" "$INSTDIR\Bin\XfaceClient.exe" 0
SectionEnd

; uninstall stuff

UninstallText "This will uninstall XfaceClient from your PC. Hit next to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XfaceClient"
  DeleteRegKey HKLM SOFTWARE\ITCirst_XfaceClient
  ; remove files
  Delete $INSTDIR\Bin\XfaceClient.exe
  Delete $INSTDIR\Bin\msvcp71.dll
  Delete $INSTDIR\Bin\msvcr71.dll
  
  Delete $INSTDIR\Bin\res\inform.gif"
  Delete $INSTDIR\Bin\res\open.gif"
  Delete $INSTDIR\Bin\res\pause.gif"
  Delete $INSTDIR\Bin\res\play.gif"
  Delete $INSTDIR\Bin\res\stop.gif"
  Delete $INSTDIR\Bin\res\TrafficGreen.gif"
  Delete $INSTDIR\Bin\res\TrafficRed.gif"
  Delete $INSTDIR\Bin\res\APML1.gif"
  Delete $INSTDIR\Bin\res\APML2.gif"
  Delete $INSTDIR\Bin\res\XfaceLogo.ico"
  
  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\XfaceClient\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\XfaceClient"
  RMDir "$INSTDIR\Bin\res"
  RMDir "$INSTDIR\Bin"
  RMDir "$INSTDIR"
SectionEnd

; eof
