; Xface Code
;
; Koray Balci 13.07.2007
;

!define VER_DISPLAY "1.0"
;--------------------------------
;Include Modern UI
!include "MUI.nsh"

;--------------------------------
;General
; The name of the installer
	SetCompressor /SOLID /FINAL lzma 
	Name "Xface Code"
	Caption "Xface Code ${VER_DISPLAY} Setup"

	XPStyle on
	; First is default
	;LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
	;LoadLanguageFile "${NSISDIR}\Contrib\Language files\Turkish.nlf"

	!define MUI_ICON "XfaceLogo.ico"
	!define MUI_UNICON "win-uninstall.ico"

	; The file to write
	OutFile "Xface-${VER_DISPLAY}-code.exe"

	; The default installation directory
	; InstallDir $PROGRAMFILES\XfaceCode
	
	; The text to prompt the user to enter a directory
	ComponentText "Xface ${VER_DISPLAY} source code will be installed in your system. This code is created by FBK-irst TCC division and protected by Mozilla 1.1 Open Source License."
	; The text to prompt the user to enter a directory
	DirText "Please select a destination folder:"
	
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
  !define MUI_LANGDLL_REGISTRY_KEY "Software\XfaceCode" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_COMPONENTS
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_UNPAGE_WELCOME
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
	
;--------------------------------
;Languages
  !insertmacro MUI_LANGUAGE "English"
  
; The stuff to install
Section "Xface Lib" Xface
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\XFace"
  CreateDirectory "$INSTDIR\XFace\lib"
  CreateDirectory "$INSTDIR\XFace\docs"
  CreateDirectory "$INSTDIR\XFace\include"
  CreateDirectory "$INSTDIR\XFace\include\XEngine"
  CreateDirectory "$INSTDIR\XFace\include\XFace"
  CreateDirectory "$INSTDIR\XFace\include\XFaceApp"
  CreateDirectory "$INSTDIR\XFace\include\XMath"
  CreateDirectory "$INSTDIR\XFace\src"
  CreateDirectory "$INSTDIR\XFace\src\XEngine"
  CreateDirectory "$INSTDIR\XFace\src\XFace"
  CreateDirectory "$INSTDIR\XFace\src\XFaceApp"
  CreateDirectory "$INSTDIR\XFace\src\XMath"
  SetOutPath $INSTDIR\XFace
  File "..\XFace\*.vcproj"
  File "..\XFace\*.txt"
  File "..\XFace\mpl-c"
  SetOutPath $INSTDIR\XFace\docs
  File "..\XFace\docs\*.html"
  File "..\XFace\docs\*.css"
  File "..\XFace\docs\Doxyfile"
  File "..\XFace\docs\*.chm"
  SetOutPath $INSTDIR\XFace\include\XEngine
  File "..\XFace\include\XEngine\*.h"
  SetOutPath $INSTDIR\XFace\include\XFace
  File "..\XFace\include\XFace\*.h"
  SetOutPath $INSTDIR\XFace\include\XFaceApp
  File "..\XFace\include\XFaceApp\*.h"
  SetOutPath $INSTDIR\XFace\include\XMath
  File "..\XFace\include\XMath\*.h"
  SetOutPath $INSTDIR\XFace\src\XEngine
  File "..\XFace\src\XEngine\*.cpp"
  SetOutPath $INSTDIR\XFace\src\XFace
  File "..\XFace\src\XFace\*.cpp"
  SetOutPath $INSTDIR\XFace\src\XFaceApp
  File "..\XFace\src\XFaceApp\*.cpp"
  SetOutPath $INSTDIR\XFace\src\XMath
  File "..\XFace\src\XMath\*.cpp"
SectionEnd

Section "XfaceEd" XfaceEd
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\wxFaceEd"
  CreateDirectory "$INSTDIR\wxFaceEd\res"
  CreateDirectory "$INSTDIR\wxFaceEd\lang"
  SetOutPath $INSTDIR\wxFaceEd
  File "..\wxFaceEd\*.cpp"
  File "..\wxFaceEd\*.h"
  File "..\wxFaceEd\*.sln"
  File "..\wxFaceEd\*.vcproj"
  File "..\wxFaceEd\*.rc"
  File "..\wxFaceEd\CMakeLists.txt"
  File "..\wxFaceEd\italianPhonemes.txt"
  File "..\wxFaceEd\test.smil"
  File "..\wxFaceEd\scriptProcs.xml"
  
  SetOutPath $INSTDIR\wxFaceEd\res
  File "..\wxFaceEd\res\*.*"
  SetOutPath $INSTDIR\wxFaceEd\lang
  File "..\wxFaceEd\lang\*.*"
SectionEnd

Section "XfacePlayer" XfacePlayer
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\wxFacePlayer"
  CreateDirectory "$INSTDIR\wxFacePlayer\res"
  CreateDirectory "$INSTDIR\wxFacePlayer\lang"
  SetOutPath $INSTDIR\wxFacePlayer
  File "..\wxFacePlayer\*.cpp"
  File "..\wxFacePlayer\*.h"
  File "..\wxFacePlayer\*.vcproj"
  File "..\wxFacePlayer\*.rc"
  File "..\wxFacePlayer\CMakeLists.txt"
  File "..\wxFacePlayer\xfaceplayer.ini"
  File "..\wxFacePlayer\scriptProcs.xml"
  
  SetOutPath $INSTDIR\wxFacePlayer\res
  File "..\wxFacePlayer\res\*.*"
  SetOutPath $INSTDIR\wxFacePlayer\lang
  File "..\wxFacePlayer\lang\*.*"
SectionEnd

Section "XfaceClient" XfaceClient
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\wxFaceClient"
  CreateDirectory "$INSTDIR\wxFaceClient\res"
  CreateDirectory "$INSTDIR\wxFaceClient\lang"
  SetOutPath $INSTDIR\wxFaceClient
  File "..\wxFaceClient\*.cpp"
  File "..\wxFaceClient\*.h"
  File "..\wxFaceClient\*.vcproj"
  File "..\wxFaceClient\*.rc"
  File "..\wxFaceClient\CMakeLists.txt"
  File "..\wxFaceClient\test.smil"
  File "..\wxFaceClient\scriptProcs.xml"
  
  SetOutPath $INSTDIR\wxFaceClient\res
  File "..\wxFaceClient\res\*.*"
  SetOutPath $INSTDIR\wxFaceClient\lang
  File "..\wxFaceClient\lang\*.*"
SectionEnd

Section "XfaceClientSimple" XfaceClientSimple
	SectionIn "RO"
  ; Set output path to the installation directory.
  CreateDirectory "$INSTDIR\wxFaceClientSimple"
  CreateDirectory "$INSTDIR\wxFaceClientSimple\resource"
  SetOutPath $INSTDIR\wxFaceClientSimple
  File "..\wxFaceClientSimple\*.cpp"
  File "..\wxFaceClientSimple\*.h"
  File "..\wxFaceClientSimple\*.vcproj"
  File "..\wxFaceClientSimple\*.rc"
  File "..\wxFaceClientSimple\sample.smil"
  
  SetOutPath $INSTDIR\wxFaceClientSimple\resource
  File "..\wxFaceClientSimple\resource\*.*"
SectionEnd

Section "XSmilAgent" XSmilAgent
	SectionIn "RO"
  CreateDirectory "$INSTDIR\XSmilAgent"
  SetOutPath $INSTDIR\XSmilAgent
  File "..\XSmilAgent\*.cpp"
  File "..\XSmilAgent\*.h"
  File "..\XSmilAgent\*.chm"
  File "..\XSmilAgent\*.vcproj"
  File "..\XSmilAgent\Doxyfile"
  File "..\XSmilAgent\CMakeLists.txt"
SectionEnd

Section "Sample Scripts & FAPs" samples
  ; Put some faps
  CreateDirectory "$INSTDIR\Faps\"
  SetOutPath $INSTDIR\Faps
  File "..\Faps\*.smil"
  File "..\Faps\*.fap"
  File "..\Faps\*.wav"
SectionEnd

LangString DESC_Xface ${LANG_ENGLISH} "This option will install Xface library source code.,"
LangString DESC_XfaceEd ${LANG_ENGLISH} "This option will install XfaceEd source code.,"
LangString DESC_XfacePlayer ${LANG_ENGLISH} "This option will install XfacePlayer source code.,"
LangString DESC_XfaceClient ${LANG_ENGLISH} "This option will install XfaceClient source code.,"
LangString DESC_XfaceClientSimple ${LANG_ENGLISH} "This option will install XfaceClientSimple source code.,"
LangString DESC_XSmilAgent ${LANG_ENGLISH} "This option will install XSmilAgent Library source code.,"
LangString DESC_Samples ${LANG_ENGLISH} "This option will install sample FAP and SMIL-Agent files."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Xface} $(DESC_Xface)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfaceEd} $(DESC_XfaceEd)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfacePlayer} $(DESC_XfacePlayer)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfaceClient} $(DESC_XfaceClient)
  !insertmacro MUI_DESCRIPTION_TEXT ${XfaceClientSimple} $(DESC_XfaceClientSimple)
  !insertmacro MUI_DESCRIPTION_TEXT ${XSmilAgent} $(DESC_XSmilAgent)
  !insertmacro MUI_DESCRIPTION_TEXT ${samples} $(DESC_Samples)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Installer Functions

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
	System::Call "kernel32::GetUserDefaultLangID()i.a"
FunctionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

; eof
