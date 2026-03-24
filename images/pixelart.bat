@echo off
setlocal enabledelayedexpansion

REM --- CONFIGURACION ---
set INPUT_DIR=input
set OUTPUT_DIR=output
set INPUT_PALETE_DIR=input_palette

REM Resolucion "pixel"
set PIXEL_W=64
set PIXEL_H=64

set PALETE_DIR="%OUTPUT_DIR%/Palette/"
set PALETE_FILE=%PALETE_DIR%/Palette.png

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
echo   3. Generate Output file
echo   4. Exit
echo.
echo ==========================================
set /p CHOICE="  Select an option (1-4): "

if "%CHOICE%"=="1" goto FUNCTION_ONE
if "%CHOICE%"=="2" goto FUNCTION_TWO
if "%CHOICE%"=="3" goto FUNCTION_THREE
if "%CHOICE%"=="4" goto EXIT

echo.
echo   [!] Invalid option. Please try again.
timeout /t 2 >nul
goto MAIN_MENU


:FUNCTION_ONE

if not exist %PALETE_DIR% (
  mkdir %PALETE_DIR%
)

cls
echo ==========================================
echo              Generate palete
echo ==========================================
set /p NUMBER_COLORS="  Select number of colors: "

set "IMG_LIST="
for /r "%INPUT_PALETE_DIR%" %%F in (*.png) do (
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

for %%F in (%INPUT_DIR%\*.png) do (
    echo Processing %%~nxF

    echo Generando img para %%~nF...
    python image_to_palette_png.py "%INPUT_DIR%\%%~nxF" "%PALETE_FILE%" "%OUTPUT_DIR%\%%~nxF"
)

pause
goto MAIN_MENU
       
:FUNCTION_THREE
cls
echo ==========================================
echo              Generate output file
echo ==========================================

rem "remove result file"
del "%OUTPUT_DIR%\sprites.txt"

for %%F in (%OUTPUT_DIR%\*.png) do (
  echo Processing %%~nxF
      echo Processing %%~nxF

      echo Generando CSV para %%~nF...
      python image_to_palette_magick.py "%OUTPUT_DIR%\%%~nxF" "%PALETE_FILE%" "%OUTPUT_DIR%\sprites.txt"
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