; LinePerpendicularToCurveThroughPoint.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 
;
; example2.nsi expands on this by adding a uninstaller and start menu shortcuts.

;--------------------------------

; The name of the installer
!include LogicLib.nsh

;!define OFFLINE 1

Var /GLOBAL offline
Var /GLOBAL dotNetRequired
Var /GLOBAL cppRedistRequired

!ifdef OFFLINE
  Name "LinePerpendicularToCurveAtPoint x64 Offline Installer.exe"
  OutFile "LinePerpendicularToCurveAtPoint x64 Offline Installer.exe"
!else
  Name "LinePerpendicularToCurveAtPoint x64.exe"
  OutFile "LinePerpendicularToCurveAtPoint x64.exe"
!endif

Function Setup

!ifdef OFFLINE
  StrCpy $offline "1"
!else
  StrCpy $offline "0"
!endif

FunctionEnd

Function CheckCPPRedist

ReadRegDWORD $0 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
ReadRegDWORD $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Major"
ReadRegDWORD $2 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Minor"
${If} ${Errors}
	DetailPrint "VC++ installation required"
	StrCpy $cppRedistRequired 1
${Else}
	${If} $0 <> 0
		DetailPrint "Found Version $1.$2"
		${If} $1 >= 14
			${Andif} $2 >= 29
			DetailPrint "VC++ version is ok"
			StrCpy $cppRedistRequired 0
		${Else}
			DetailPrint "VC++ installation required"
			StrCpy $cppRedistRequired 1
		${EndIf}
	${EndIf}
${EndIf}

FunctionEnd

Function CheckDotNet

ClearErrors
ReadRegDWORD $0 HKLM "SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full" "Release"

${If} ${Errors}
	DetailPrint ".net not installed"
	StrCpy $dotNetRequired 1
${Else}
	${If} $0 >= 528040
		DetailPrint ".net build $0 found"
		StrCpy $dotNetRequired 0
	${Else}
		DetailPrint ".net build $0 found - install of .net 4.8 required."
		StrCpy $dotNetRequired 1
	${EndIf}
${EndIf}

FunctionEnd



Function .onInit
  Call Setup
  
FunctionEnd


InstallDir $PROGRAMFILES64\LinePerpendicularToCurveAtPoint




Section -Prerequisits
SetOutPath $TEMP
Call CheckCPPRedist
Call CheckDotNet


!ifndef OFFLINE
    ${If} $cppRedistRequired == 1
		NSISdl::download https://aka.ms/vs/16/release/vc_redist.x64.exe vc_redist.x64.exe
		ExecWait vc_redist.x64.exe
		Delete vc_redist.x64.exe
    ${EndIf}

    ${If} $dotNetRequired == 1
		NSISdl::download https://download.visualstudio.microsoft.com/download/pr/2d6bb6b2-226a-4baa-bdec-798822606ff1/8494001c276a4b96804cde7829c04d7f/ndp48-x86-x64-allos-enu.exe ndp48-x86-x64-allos-enu.exe
		ExecWait ndp48-x86-x64-allos-enu.exe
		Delete ndp48-x86-x64-allos-enu.exe
	${EndIf}
!else
    File "..\..\PreReqs\vc_redist.x64.exe"
	File "..\..\PreReqs\ndp48-x86-x64-allos-enu.exe"
	${If} $cppRedistRequired == 1
		ExecWait vc_redist.x64.exe
	${EndIf}
	Delete vc_redist.x64.exe
	${If} $dotNetRequired == 1
	    ExecWait ndp48-x86-x64-allos-enu.exe
	${EndIf}
	Delete ndp48-x86-x64-allos-enu.exe
!endif
		
SectionEnd


	
Section LinePerpendicularToCurveThroughPoint
	SetOutPath $INSTDIR

	File "..\x64\Release\*.exe"
	File "..\x64\Release\*.dll"
    File "..\x64\Release\*.txt"
	
	
SectionEnd
	
	