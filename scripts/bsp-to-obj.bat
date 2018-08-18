@echo off
set parent=%~dp0
cd %parent%

:: ================== SETTINGS ==================

:: You can pre set BSPName, but it's not required.
:: You can pass in filename using command line, type help for usage examples.
:: The bsp file will be searched in the script(this one) and in the maps directories.
:: You can set this if you want to launch the script by double click.
set BSPName=

:: !IMPORTANT! set Q3MAP path here
set Q3MapPath=

:: !IMPORTANT! set game directory path here
set GameDirectory=

:: !IMPORTANT! correct game name here 
:: For instance: q3, et, rtcw; should match the q3map2 -game argument
set GameName=q3

:: No further edits required

:: =================== END ======================

:: Type help to get usage information
if /I "%1" == "help" goto printHelp
if /I "%1" == "-help" goto printHelp
if /I "%1" == "" (
    if "%BSPName%" == "" goto printHelp
)

:: Validate vars
if "%Q3MapPath%" == "" (
    call :printMissingQ3MapPath
    goto atExit
)
if "%GameDirectory%" == "" (
    call :printMissingGameDirectoryPath
    goto atExit                         
)

set BaseDirectoryName=baseq3
call :setBaseDirectoryName
set BasePath=%GameDirectory%/%BaseDirectoryName%

:: Find the right map/bsp file
set MapsDirectory=%BasePath%/maps/
if "%~1" == "" (
    set MapName=%BSPName%
) else (
    set MapName=%~1
)
set MapPath=""
call :findCorrectMapPath
if %MapPath% == "" (
    call :printMapFileNotFound
    goto atExit
)

:: Generate output path 
if "%~2" == "" (
    set OutputDirectory=%parent%
) else (
    set OutputDirectory=%~f2\
)

call :getFileBaseName %MapName%

set OutputName=%baseName%.obj
set OutputPath=%OutputDirectory%%OutputName%
if not exist "%OutputDirectory%" mkdir "%OutputDirectory%"

:: Additional switches
set ExtraSwitches=-meta -patchmeta

:: Compose compiler argument string, excluding mapname
set CompilerArguments=-convert -game %GameName% -fs_basepath "%GameDirectory%" -v -format obj %ExtraSwitches%

:: Print info
echo. & echo.[Settings]
echo    Q3MAP:              %Q3MapPath%
echo    GAME NAME:          %GameName%
echo    GAME DIRECTORY:     %GameDirectory%
echo    BASE DIRECTORY:     %BaseDirectoryName%
echo    MAP PATH:           %MapPath%
echo    EXTRA SWITCHES:     %ExtraSwitches%
echo    COMPILER ARGUMENTS: %CompilerArguments%

:: Do work
echo. & echo [Convert BSP to OBJ]
%Q3MapPath% %CompilerArguments% -outfile "%OutputPath%" "%MapPath%"

:atExit
    echo. & pause
    exit 0

:printHelp
    echo Basic usage:
    echo    bsp-to-obj.bat mapname [output relative path]
    echo    bsp-to-obj.bat castle.bsp
    echo    bsp-to-obj.bat castle.bsp models/castle
    goto atExit

:printMissingQ3MapPath
    echo Error: please set Q3Map2 path by editing this file (find Q3MapPath variable)!
    exit /B 0

:printMissingGameDirectoryPath
    echo Error: Please set game directory path by editing this file (find GameDirectory variable)!
    exit /B 0

:printMapFileNotFound
    echo Error: %MapName% was not found!
    exit /B 0

:setBaseDirectoryName
    if "%GameName%" == "q3" set BaseDirectoryName=baseq3
    if "%GameName%" == "et" set BaseDirectoryName=etmain
    if "%GameName%" == "wolf" set BaseDirectoryName=main
    if "%GameName%" == "darkplaces" set BaseDirectoryName=id1
    if "%GameName%" == "dq" set BaseDirectoryName=basedq
    if "%GameName%" == "ef" set BaseDirectoryName=basee
    if "%GameName%" == "etut" set BaseDirectoryName=etut
    if "%GameName%" == "ja" set BaseDirectoryName=base
    if "%GameName%" == "jk2" set BaseDirectoryName=base
    if "%GameName%" == "nexuiz" set BaseDirectoryName=data
    if "%GameName%" == "prophecy" set BaseDirectoryName=base
    if "%GameName%" == "qfusion" set BaseDirectoryName=base
    if "%GameName%" == "quakelive" set BaseDirectoryName=baseq3
    if "%GameName%" == "reaction" set BaseDirectoryName=Boomstick
    if "%GameName%" == "sof2" set BaseDirectoryName=base
    if "%GameName%" == "tenebrae" set BaseDirectoryName=base
    if "%GameName%" == "tremulous" set BaseDirectoryName=base
    if "%GameName%" == "unvanquished" set BaseDirectoryName=pkg
    if "%GameName%" == "xonotic" set BaseDirectoryName=data
    exit /B 0

:: search map in cwd
:: search map in game directory
:findCorrectMapPath
    if exist "%MapName%" ( 
        set MapPath=%parent%%MapName%
    ) else (
        if exist "%MapsDirectory%%MapName%" set MapPath=%MapsDirectory%%MapName%
    )
    exit /B 0

:getFileBaseName
    set baseName=%~n1
