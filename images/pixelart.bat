@echo off
setlocal enabledelayedexpansion

REM --- CONFIGURACION ---
set INPUT_DIR=input
set OUTPUT_DIR=output

REM Resolucion "pixel"
set PIXEL_W=64
set PIXEL_H=64

set PALETE_FILE=%OUTPUT_DIR%/Palette.png

REM Crear carpeta de salida si no existe
if not exist %OUTPUT_DIR% (
  mkdir %OUTPUT_DIR%
)

:MAIN_MENU
cls
echo ==========================================
echo              MAIN MENU
echo ==========================================
echo.
echo   1. Generate Palete
echo   2. Generate Image
echo   3. Generate CSV
echo   4. Copy pre-done images
echo   5. Generate CSV -2
echo   6. Exit
echo.
echo ==========================================
set /p CHOICE="  Select an option (1-5): "

if "%CHOICE%"=="1" goto FUNCTION_ONE
if "%CHOICE%"=="2" goto FUNCTION_TWO
if "%CHOICE%"=="3" goto FUNCTION_THREE
if "%CHOICE%"=="4" goto FUNCTION_FOUR
if "%CHOICE%"=="5" goto FUNCTION_FIVE
if "%CHOICE%"=="6" goto EXIT

echo.
echo   [!] Invalid option. Please try again.
timeout /t 2 >nul
goto MAIN_MENU


:FUNCTION_ONE
cls
echo ==========================================
echo              Generate palete
echo ==========================================
set /p NUMBER_COLORS="  Select number of colors: "

set "IMG_LIST="
for /r "%INPUT_DIR%" %%F in (*.png) do (
  set "IMG_LIST=!IMG_LIST! "%%F""
)

REM Crear paleta global a partir de TODAS las imagenes
magick !IMG_LIST! ^
  -alpha remove -alpha off ^
  -filter point -resize %PIXEL_W%x%PIXEL_H%! ^
  -append +dither -colors %NUMBER_COLORS% -unique-colors ^
  "%PALETE_FILE%"

IF ERRORLEVEL 1 (
  echo ERROR al generar la paleta.
  pause
  exit /b
)

echo Paleta creada: palette.png
echo.

REM 2. Extraer colores de la paleta como lista hex (un color por linea)
magick "%PALETE_FILE%" txt:- ^
  | findstr /r "[0-9]" ^
  | findstr /v "^#" ^
  > "%OUTPUT_DIR%\palette_colors.txt"

pause
goto MAIN_MENU


:FUNCTION_TWO
cls
echo ==========================================
echo              Generate images
echo ==========================================

for /d %%D in ("%INPUT_DIR%\*") do (
    set "FOLDER_NAME=%%~nxD"
    
    :: Split "WxH" into WIDTH and HEIGHT
    for /f "tokens=1,2 delims=x" %%A in ("!FOLDER_NAME!") do (
        set "WIDTH=%%A"
        set "HEIGHT=%%B"
    )
    
    echo Processing: !FOLDER_NAME! ^(WIDTH=!WIDTH! HEIGHT=!HEIGHT!^)
    set "FOLDER_IN=%INPUT_DIR%\!FOLDER_NAME!"
    set "FOLDER_OUT=%OUTPUT_DIR%\!FOLDER_NAME!"
    echo Input !FOLDER_IN!
    echo Output !FOLDER_OUT!

    if not exist !FOLDER_OUT! (
        mkdir !FOLDER_OUT!
    )

    for %%F in (!FOLDER_IN!\*.png) do (
        echo Processing %%~nxF

        magick "%%F" ^
            -alpha remove -alpha off ^
            -filter point -resize !WIDTH!x!HEIGHT!! ^
            -filter point -resize %PIXEL_W%x%PIXEL_H%! ^
            -dither none -remap "%PALETE_FILE%" ^
            "!FOLDER_OUT!\%%~nxF"
    )
)

pause
goto MAIN_MENU
       
:FUNCTION_THREE
cls
echo ==========================================
echo              Generate csv
echo ==========================================

rem "remove result file"
del "%OUTPUT_DIR%\sprites.txt"

for /d %%D in ("%INPUT_DIR%\*") do (
    set "FOLDER_NAME=%%~nxD"
    
    :: Split "WxH" into WIDTH and HEIGHT
    for /f "tokens=1,2 delims=x" %%A in ("!FOLDER_NAME!") do (
        set "WIDTH=%%A"
        set "HEIGHT=%%B"
    )
    
    echo Processing: !FOLDER_NAME! ^(WIDTH=!WIDTH! HEIGHT=!HEIGHT!^)
    set "FOLDER_IN=%INPUT_DIR%\!FOLDER_NAME!"
    set "FOLDER_OUT=%OUTPUT_DIR%\!FOLDER_NAME!"
    echo Input !FOLDER_IN!
    echo Output !FOLDER_OUT!

    for %%F in (!FOLDER_IN!\*.png) do (
        echo Processing %%~nxF

        echo Generando CSV para %%~nF...
        python _csv_gen.py !WIDTH! !HEIGHT!  "!FOLDER_OUT!\%%~nxF" "%OUTPUT_DIR%\sprites.txt" "%PALETE_FILE%" %%~nF

    )
)

pause
goto MAIN_MENU

:FUNCTION_FOUR
cls
echo ==========================================
echo              Copy pre generated images
echo ==========================================

for /d %%D in ("%INPUT_DIR%\*") do (
    set "FOLDER_NAME=%%~nxD"
    
    :: Split "WxH" into WIDTH and HEIGHT
    for /f "tokens=1,2 delims=x" %%A in ("!FOLDER_NAME!") do (
        set "WIDTH=%%A"
        set "HEIGHT=%%B"
    )
    
    echo Processing: !FOLDER_NAME! ^(WIDTH=!WIDTH! HEIGHT=!HEIGHT!^)
    set "FOLDER_IN=%INPUT_DIR%\!FOLDER_NAME!"
    set "FOLDER_OUT=%OUTPUT_DIR%\!FOLDER_NAME!"
    echo Input !FOLDER_IN!
    echo Output !FOLDER_OUT!

    if not exist !FOLDER_OUT! (
        mkdir !FOLDER_OUT!
    )

    for %%F in (!FOLDER_IN!\*.png) do (
        echo Processing %%~nxF

        magick "%%F" ^
            -alpha remove -alpha off ^
            -dither none -remap "%PALETE_FILE%" ^
            "!FOLDER_OUT!\%%~nxF"
    )
)

pause
goto MAIN_MENU

:FUNCTION_FIVE
cls
echo ==========================================
echo              Generate csv -2(now its personal)
echo ==========================================

rem "remove result file"
del "%OUTPUT_DIR%\sprites.txt"

for /d %%D in ("%INPUT_DIR%\*") do (
    set "FOLDER_NAME=%%~nxD"
    
    :: Split "WxH" into WIDTH and HEIGHT
    for /f "tokens=1,2 delims=x" %%A in ("!FOLDER_NAME!") do (
        set "WIDTH=%%A"
        set "HEIGHT=%%B"
    )
    
    echo Processing: !FOLDER_NAME! ^(WIDTH=!WIDTH! HEIGHT=!HEIGHT!^)
    set "FOLDER_IN=%INPUT_DIR%\!FOLDER_NAME!"
    set "FOLDER_OUT=%OUTPUT_DIR%\!FOLDER_NAME!"
    echo Input !FOLDER_IN!
    echo Output !FOLDER_OUT!

    for %%F in (!FOLDER_IN!\*.png) do (
        echo Processing %%~nxF

        echo Generando CSV para %%~nF...
        python image_to_palette_magick.py "!FOLDER_IN!\%%~nxF" "%PALETE_FILE%" "%OUTPUT_DIR%\sprites.txt"
    )
)

pause
goto MAIN_MENU

:EXIT
cls
echo.
echo   Goodbye!
echo.
timeout /t 2 >nul
endlocal
exit /b 0