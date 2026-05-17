@echo off
set "targetDir=C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin"
echo ======================================================
echo    SETTING UP MINGW32-MAKE FOR YOUR SYSTEM
echo ======================================================
echo.
echo Target Directory: %targetDir%

if exist "%targetDir%\mingw32-make.exe" (
    echo [FOUND] mingw32-make.exe exists.
) else (
    echo [ERROR] Could not find mingw32-make.exe at the expected location.
    pause
    exit /b
)

echo.
echo Adding to User PATH...
setx PATH "%PATH%;%targetDir%"

echo.
echo ======================================================
echo SUCCESS! Please RESTART your terminal (or computer).
echo ======================================================
echo.
echo Current test (running from full path):
"%targetDir%\mingw32-make.exe" -v
echo.
pause
