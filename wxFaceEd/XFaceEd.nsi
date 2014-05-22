; XFaceEd.nsi
;
; This script is based on example2.nsi in NSIS examples
; Koray Balci 02.10.2003
;

; The name of the installer
Name "XFaceEd"

Icon "XFaceLogo.ico"
UninstallIcon "XFaceLogo.ico"


; The file to write
OutFile "XFaceEd-0.6-setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\XFaceEd
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\ITCirst_XFaceEd "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install the XFaceEd v0.6 Beta 1 on your computer. It is provided by ITC-irst. Select which optional things you want installed."
; The text to prompt the user to enter a directory
DirText "Choose a directory to install XFaceEd:"

; The stuff to install
Section "XFaceEd (required)"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\Faces\alice"
  CreateDirectory "$INSTDIR\Faces\john"
  CreateDirectory "$INSTDIR\Faps"
  CreateDirectory "$INSTDIR\Bin"
  CreateDirectory "$INSTDIR\Bin\res"
  SetOutPath $INSTDIR\Bin
  ; Put file there
  File "..\bin\XFaceEd.exe"
  File "C:\WINDOWS\system32\msvcp71.dll"
  File "C:\WINDOWS\system32\msvcr71.dll"
  
  SetOutPath $INSTDIR\Bin\res
  File "..\wxFaceEd\res\fdp_lips.bmp"
  File "..\wxFaceEd\res\fdp_face.bmp"
  File "..\wxFaceEd\res\fdp_eyes.bmp"
  File "..\wxFaceEd\res\fdp_nose.bmp"
  File "..\wxFaceEd\res\fdp_tongue.bmp"
  File "..\wxFaceEd\res\fapu.bmp"
  File "..\wxFaceEd\res\XFaceLogo.ico"
  
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
  
  SetOutPath $INSTDIR\Faces\john
  File "..\Faces\john_hi_obj\john_hi_obj.obj"
  File "..\Faces\john_hi_obj\john_hi_obj.mtl"
  File "..\Faces\john_hi_obj\john.xml"
  File "..\Faces\john_hi_obj\john_hi_obj00.bmp"
  File "..\Faces\john_hi_obj\john_hi_obj01.bmp"
  File "..\Faces\john_hi_obj\john_hi_obj02.bmp"
  
  ; Put some faps
  SetOutPath $INSTDIR\Faps
  File "..\Faps\anger.fap"
  File "..\Faps\beg.fap"
  File "..\Faps\disgust.fap"
  File "..\Faps\happy-for.fap"
  File "..\Faps\hate.fap"
  File "..\Faps\joy.fap"
  File "..\Faps\pride.fap"
  File "..\Faps\sample.wav"
  
  SetOutPath $INSTDIR\Bin
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\ITCirst_XFaceEd "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XFaceEd" "DisplayName" "ITC-irst XFaceEd (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XFaceEd" "XFaceEd" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\XFaceEd"
  CreateShortCut "$SMPROGRAMS\XFaceEd\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\XFaceEd\XFaceEd.lnk" "$INSTDIR\Bin\XFaceEd.exe" "" "$INSTDIR\Bin\XFaceEd.exe" 0
SectionEnd

; uninstall stuff

UninstallText "This will uninstall XFaceEd from your PC. Hit next to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XFaceEd"
  DeleteRegKey HKLM SOFTWARE\ITCirst_XFaceEd
  ; remove files
  Delete $INSTDIR\Bin\XFaceEd.exe
  Delete $INSTDIR\Bin\msvcp71.dll
  Delete $INSTDIR\Bin\msvcr71.dll
  
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
  
  Delete $INSTDIR\Faces\john\john_hi_obj.obj"
  Delete $INSTDIR\Faces\john\john_hi_obj00.bmp"
  Delete $INSTDIR\Faces\john\john_hi_obj01.bmp"
  Delete $INSTDIR\Faces\john\john_hi_obj02.bmp"
  Delete $INSTDIR\Faces\john\john_hi_obj.mtl"
  Delete $INSTDIR\Faces\john\john.xml"
  
  Delete $INSTDIR\Bin\res\fdp_lips.bmp"
  Delete $INSTDIR\Bin\res\fdp_face.bmp"
  Delete $INSTDIR\Bin\res\fdp_eyes.bmp"
  Delete $INSTDIR\Bin\res\fdp_nose.bmp"
  Delete $INSTDIR\Bin\res\fdp_tongue.bmp"
  Delete $INSTDIR\Bin\res\fapu.bmp"
  Delete $INSTDIR\Bin\res\XFaceLogo.ico"
  
  Delete $INSTDIR\Faps\anger.fap"
  Delete $INSTDIR\Faps\beg.fap"
  Delete $INSTDIR\Faps\disgust.fap"
  Delete $INSTDIR\Faps\happy-for.fap"
  Delete $INSTDIR\Faps\hate.fap"
  Delete $INSTDIR\Faps\joy.fap"
  Delete $INSTDIR\Faps\pride.fap"
  Delete $INSTDIR\Faps\sample.wav"
  
  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\XFaceEd\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\XFaceEd"
  RMDir "$INSTDIR\Faces\alice"
  RMDir "$INSTDIR\Faces\john"
  RMDir "$INSTDIR\Faces"
  RMDir "$INSTDIR\Faps"
  RMDir "$INSTDIR\Bin\res"
  RMDir "$INSTDIR\Bin"
  RMDir "$INSTDIR"
SectionEnd

; eof
