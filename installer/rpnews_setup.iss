; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "RepositoryNews"
#define MyAppVersion "1.0"
#define MyAppPublisher "Open Source"
#define MyAppURL "http://www.example.com/"
#define MyAppExeName "rpnews.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{6830C0B1-5B5B-4A09-B754-8D03FD364AD3}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=E:\RepositoryNews\RepositoryNews\installer
OutputBaseFilename=rpnews_setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "D:\helpbin\rpnews.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\git2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\MSVCP140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\VCRUNTIME140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\helpbin\plugins\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

