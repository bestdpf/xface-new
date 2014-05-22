; Xface.nsi
;
; Koray Balci 12.04.2006
;

!define VER_DISPLAY "1.0"
;--------------------------------
;Include Modern UI
!include "MUI.nsh"

;--------------------------------
;General
	SetCompressor /SOLID /FINAL lzma
; The name of the installer
	Name "Xface Toolkit"
	Caption "Xface Toolkit ${VER_DISPLAY} Setup"

	!define MUI_ICON "XFaceLogo.ico"
	!define MUI_UNICON "win-uninstall.ico"

	; The file to write
	OutFile "Xface-${VER_DISPLAY}-setup.exe"

	; The default installation directory
	InstallDir $PROGRAMFILES\Xface
	; Registry key to check for directory (so if you install again, it will 
	; overwrite the old one automatically)
	InstallDirRegKey HKCU "Software\FBK-irst Xface" ""

	; The text to prompt the user to enter a directory
	ComponentText "This will install the Xface version ${VER_DISPLAY} on your computer. Developed and provided by FBK-irst."
	; The text to prompt the user to enter a directory
	DirText "Choose a directory to install Xface:"
	
;--------------------------------
;Interface Configuration
  !define MUI_HEADERIMAGE 
  !define MUI_HEADERIMAGE_BITMAP "XfaceLogo.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP "XfaceLogo.bmp"
  !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
  !define MUI_HEADERIMAGE_UNBITMAP_NOSTRETCH
  
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\llama.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\llama.bmp"
  ;!define MUI_BGCOLOR "CBCBCB"
  ;!define MUI_BGCOLOR "DA9358"
  !define MUI_ABORTWARNING


;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\FBK-irst Xface" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_LICENSE "License.txt"
	!insertmacro MUI_PAGE_COMPONENTS
	!insertmacro MUI_PAGE_DIRECTORY
	
	;Start Menu Folder Page Configuration
	Var STARTMENU_FOLDER
	!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
	!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Xface" 
	!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  	!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

	!insertmacro MUI_PAGE_INSTFILES
	
	!define MUI_FINISHPAGE_LINK "Visit Xface website for the latest news and support"
	!define MUI_FINISHPAGE_LINK_LOCATION "http://xface.fbk.eu/"
	!define MUI_FINISHPAGE_RUN "$INSTDIR\Bin\XfacePlayer"
	!define MUI_FINISHPAGE_RUN_TEXT "Run XfacePlayer"
	!insertmacro MUI_PAGE_FINISH
	
	!insertmacro MUI_UNPAGE_WELCOME
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
	!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Turkish"
  !insertmacro MUI_RESERVEFILE_LANGDLL

; The stuff to install
Section "XfacePlayer" XfacePlayer
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\Faps"
  CreateDirectory "$INSTDIR\Bin"
  CreateDirectory "$INSTDIR\Bin\res"
  CreateDirectory "$INSTDIR\Bin\lang"
  SetOutPath $INSTDIR\Bin
  ; Put main executable stuff
  File "..\wxFacePlayer\Release\XfacePlayer.exe"
  File "Microsoft.VC90.CRT.manifest"
  File "..\wxFacePlayer\xfaceplayer.ini"
  File "..\wxFacePlayer\scriptProcs.xml"
  File "..\wxFaceClient\test.smil"
  File "msvcp90.dll"
  File "msvcr90.dll"
  File "msvcm90.dll"
  File "C:\WINDOWS\system32\sdl.dll"
  File "C:\WINDOWS\system32\xerces-c_2_8.dll"
  
  SetOutPath $INSTDIR\Bin\lang
  File "..\wxFacePlayer\lang\enSAPI.dic"
  File "..\wxFacePlayer\lang\italian.dic"
  File "..\wxFacePlayer\lang\sapiEn.txt"
  
  ;File "Sp5TTIntXP.Msm"
  ;ExecWait 'msiexec /i Sp5TTIntXP.Msm /qn'
  ;Delete $INSTDIR\Bin\lang\Sp5TTIntXP.Msm
  
  SetOutPath $INSTDIR\Bin\res
  File "..\wxFacePlayer\res\camera.png"
  File "..\wxFacePlayer\res\inform.png"
  File "..\wxFacePlayer\res\open.png"
  File "..\wxFacePlayer\res\play.png"
  File "..\wxFacePlayer\res\stop.png"
  File "..\wxFacePlayer\res\TrafficGreen.png"
  File "..\wxFacePlayer\res\TrafficRed.png"
  File "..\wxFacePlayer\res\volume.png"
  File "..\wxFacePlayer\res\volumeNo.png"
  File "..\wxFacePlayer\res\XfaceLogo.ico"
  
  ; Write the installation path into the registry
  WriteRegStr HKCU "Software\FBK-irst Xface" "" "$INSTDIR"
  WriteUninstaller "uninstall.exe"
  
SectionEnd
; XfaceEd
Section "XfaceEd" XfaceEd
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\Bin
  File "..\wxFaceEd\Release\XFaceEd.exe"
  File "..\wxFaceEd\help.zip"
  SetOutPath $INSTDIR\Bin\res
  File "..\wxFaceEd\res\fdp_lips.png"
  File "..\wxFaceEd\res\fdp_face.png"
  File "..\wxFaceEd\res\fdp_eyes.png"
  File "..\wxFaceEd\res\fdp_nose.png"
  File "..\wxFaceEd\res\fdp_tongue.png"
  File "..\wxFaceEd\res\fapu.png"
  File "..\wxFaceEd\res\open.png"
  File "..\wxFaceEd\res\play.png"
  File "..\wxFaceEd\res\Plus.png"
  File "..\wxFaceEd\res\Delete.png"
  File "..\wxFaceEd\res\Eyeball.png"
  File "..\wxFaceEd\res\stop.png"
  File "..\wxFaceEd\res\volumeNo.png"
  File "..\wxFaceEd\res\volume.png"
  File "..\wxFaceEd\res\filesave.png"
  File "..\wxFaceEd\res\cursor_crosshair_target.xpm"
  File "..\wxFaceEd\res\cursor_move_outlined.xpm"
SectionEnd

; XfaceClient
Section "XfaceClient" XfaceClient
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\Bin
  File "..\wxFaceClient\Release\XFaceClient.exe"
  SetOutPath $INSTDIR\Bin\res
  File "..\wxFaceClient\res\proc_send.png"
  File "..\wxFaceClient\res\send_proc.png"
SectionEnd

Section "Sample FAP files" Faps
  ; Put some faps
  CreateDirectory "$INSTDIR\Faps"
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
  File "..\Faps\actionOnlySeq.smil"
  File "..\Faps\simpleEnglish.smil"
  File "..\Faps\withSilent.smil"
  File "..\Faps\withMarks.smil"
  File "..\Faps\italian.smil"
  File "..\Faps\externalWav.smil"
  SetOutPath $INSTDIR\Bin
SectionEnd

Section "Alice Face (required)" Alice
	SectionIn "RO"
  ; Put Alice
  CreateDirectory "$INSTDIR\Faces\alice"
  SetOutPath $INSTDIR\Faces\alice
  File "..\Faces\alice\alice_tongue.bmp"
  File "..\Faces\alice\alice_teethUpper.bmp"
  File "..\Faces\alice\alice_teethLower.bmp"
  File "..\Faces\alice\alice_HairGraduatedBob.bmp"
  File "..\Faces\alice\alice_sock.bmp"
  File "..\Faces\alice\alice_skin_hi.bmp"
  File "..\Faces\alice\alice_eyeR_hi.bmp"
  File "..\Faces\alice\alice_eyeL_hi.bmp"
  File "..\Faces\alice\alice.fdp"
  File "..\Faces\alice\alice.dat"
  
  	;-----------------------
	; Start Menu
	; good place to add start menu stuff since this section is required.
	SetOutPath "$INSTDIR\Bin\"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    IfFileExists "$INSTDIR\Bin\XfacePlayer.exe" "" +2
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\XfacePlayer.lnk" "$INSTDIR\Bin\XfacePlayer.exe"
    IfFileExists "$INSTDIR\Bin\XfaceEd.exe" "" +2
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\XfaceEd.lnk" "$INSTDIR\Bin\XfaceEd.exe"
    IfFileExists "$INSTDIR\Bin\XfaceClient.exe" "" +2
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\XfaceClient.lnk" "$INSTDIR\Bin\XfaceClient.exe"
	WriteINIStr "$SMPROGRAMS\$STARTMENU_FOLDER\Xface on the web.url" "InternetShortcut" "URL" "http://xface.fbk.eu/"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

LangString DESC_XfacePlayer ${LANG_ENGLISH} "This option will install XfacePlayer,\
											 which can playback MPEG-4 FAP files."
LangString DESC_XfacePlayer ${LANG_TURKISH} "Bu seçenek ile MPEG-4 FAP dosyalarini \
											gösteren XfacePlayer'i sisteminize kuracaksiniz."
LangString DESC_XfacePlayer ${LANG_ITALIAN} "Questa opzione permette di installare XfacePlayer, che puo' eseguire MPEG-4 FAP file."

LangString DESC_Alice ${LANG_ENGLISH} "This option will copy Alice face to your hard disk. \
										Without a face model, you cannot do a lot in Xface. \
										This face model is created by FaceGen software from \
										Singular Inversions and distributed with permission."
LangString DESC_Alice ${LANG_ITALIAN} "Questa opzione permette di copiare la faccia Alice sul tuo hard disk. Senza un modello di faccia non si puo' fare molto con Xface. Questo modello di faccia e' distribuito sotto licenza Singular Inversions."
LangString DESC_Alice ${LANG_TURKISH} "Bu seçenek ile Alice modelini sabit diskinize kopyalayacaksiniz. Bir yüz modeli olmadan Xface ile fazla birsey yapamazsiniz. Alice modeli Singular Inversions'dan alinan izin ile dagitilmaktadir."

LangString DESC_XfaceClient ${LANG_ENGLISH} "This option will install XfaceClient, which controls XfacePlayer over a network. Note that, in order to create FAP from APML strings, you need an external, third party FAP generator tool as well."
LangString DESC_XfaceClient ${LANG_ITALIAN} "Questa opzione permette di installare XfaceClient, che controlla XfacePlayer attraverso la rete. Nota: per creare file FAP da stringhe APML, e' necessario un tool di generazione FAP esterno."
LangString DESC_XfaceClient ${LANG_TURKISH} "XfacePlayer'i ag üzerinden kontrol etmeye yarayan XfaceClient'i sisteminize kuracaksiniz. AMPL scriptlerinden FAP yaratabilmek için ayrica 3. parti bir FAP yaratan yazilima ihtiyaciniz olacak."

LangString DESC_XfaceEd ${LANG_ENGLISH} "This option will install XfaceEd, which help you create talking heads from static 3D mesh models."
LangString DESC_XfaceEd ${LANG_ITALIAN} "Questa opzione permette di installare XfaceEd, che aiuta a creare facce parlanti da modelli 3D statici."
LangString DESC_XfaceEd ${LANG_TURKISH} "Bu seçenek ile sabit 3B modellerden 3B konusan kafa modelleri gelistirmenize yarayan XfaceEd'i sisteminize kuracaksiniz."

LangString DESC_Faps ${LANG_ENGLISH} "Some sample FAP files are provided as example."
LangString DESC_Faps ${LANG_ITALIAN} "Sono forniti alcuni file FAP di esempio."
LangString DESC_Faps ${LANG_TURKISH} "Deneme amaçli örnek FAP dosyalari."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${XfacePlayer} $(DESC_XfacePlayer)
  !insertmacro MUI_DESCRIPTION_TEXT ${Alice} $(DESC_Alice)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfaceClient} $(DESC_XfaceClient)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfaceEd} $(DESC_XfaceEd)
  !insertmacro MUI_DESCRIPTION_TEXT ${Faps} $(DESC_Faps)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

; uninstall stuff

UninstallText "This will uninstall Xface (${VER_DISPLAY}) from your PC. Hit next to continue."

; special uninstall section.
Section "Uninstall"

  ; remove files
  Delete $INSTDIR\Bin\*.*"
  Delete $INSTDIR\Bin\lang\*.*"
  Delete $INSTDIR\Bin\res\*.*"
  Delete $INSTDIR\Faces\alice\*.*"
  Delete $INSTDIR\Faps\*.*"
  
  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe

  Delete "$SMPROGRAMS\Xface\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\Xface"
  RMDir "$INSTDIR\Faces\alice"
  RMDir "$INSTDIR\Faces"
  RMDir "$INSTDIR\Faps"
  RMDir "$INSTDIR\Bin\res"
  RMDir "$INSTDIR\Bin\lang"
  RMDir "$INSTDIR\Bin"
  RMDir "$INSTDIR"
  
  
	; delete all registry keys we have created
	DeleteRegKey HKCU "Software\FBK-irst Xface"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

; eof
