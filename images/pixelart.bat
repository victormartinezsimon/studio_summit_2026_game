@echo off
REM ================================
REM Pixel Art Batch + Paleta Global
REM ImageMagick (Windows)
REM ================================

REM --- CONFIGURACION ---
set INPUT_DIR=input
set OUTPUT_DIR=output

REM Resolucion "pixel"
set PIXEL_W=64
set PIXEL_H=64

REM Resolucion final (escalada)
set FINAL_W=64
set FINAL_H=64

REM Numero de colores
set COLORS=8

REM --------------------

echo.
echo ================================
echo Generando paleta global (%COLORS% colores)
echo ================================

REM Crear paleta global a partir de TODAS las imagenes
magick %INPUT_DIR%\*.png ^
  -filter point -resize %PIXEL_W%x%PIXEL_H%! ^
  -append +dither -colors %COLORS% -unique-colors ^
  palette.png

IF ERRORLEVEL 1 (
  echo ERROR al generar la paleta.
  pause
  exit /b
)

echo Paleta creada: palette.png
echo.

REM 2. Extraer colores de la paleta como lista hex (un color por linea)
magick palette.png txt:- ^
  | findstr /r "[0-9]" ^
  | findstr /v "^#" ^
  > "%OUTPUT_DIR%\palette_colors.txt"

REM Crear carpeta de salida si no existe
if not exist %OUTPUT_DIR% (
  mkdir %OUTPUT_DIR%
)

echo ================================
echo Convirtiendo imagenes a pixel art
echo ================================

for %%F in (%INPUT_DIR%\*.png) do (
  echo Procesando %%~nxF

  REM --- Generar imagen pixel art ---
  magick "%%F" ^
    -filter point -resize %PIXEL_W%x%PIXEL_H%! ^
    -filter point -resize %FINAL_W%x%FINAL_H%! ^
    -dither none -remap palette.png ^
    "%OUTPUT_DIR%\%%~nxF"

  REM --- Reducir imagen al tamano pixel y remapear (sin escalar) ---
  magick "%%F" ^
    -filter point -resize %PIXEL_W%x%PIXEL_H%! ^
    -dither none -remap palette.png ^
    "%OUTPUT_DIR%\%%~nF_indexed.png"

  REM --- Generar CSV via script Python ---
  echo Generando CSV para %%~nF...
  python _csv_gen.py %PIXEL_W% %PIXEL_H% "%OUTPUT_DIR%\%%~nF_indexed.png" "%OUTPUT_DIR%\%%~nF.csv"

  REM --- Limpiar imagen temporal ---
  del "%OUTPUT_DIR%\%%~nF_indexed.png" 2>nul
)

REM --- Limpiar script temporal ---
rem del _csv_gen.py 2>nul

echo.
echo ================================
echo Proceso terminado
echo Resultados en: %OUTPUT_DIR%
echo  - Imagenes pixel art (.png)
echo  - Arrays de paleta (.csv)
echo ================================
pause