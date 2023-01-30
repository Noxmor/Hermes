@echo off
SetLocal EnableDelayedExpansion

SET assembly=Hermes

cd ../

if exist bin @RD /S /Q bin
if exist bin-int @RD /S /Q bin-int

if exist %assembly%.exe DEL %assembly%.exe

mkdir bin
mkdir bin-int

SET FILES= 
for /R %%f in (*.c) do (
	SET FILES=!FILES! %%f
)

SET FLAGS=-Wall -Wextra -O3
SET INCLUDES=-I../hermes/src
SET DEFINES=-DHM_ENABLE_ASSERTS

echo Building %assembly%...

PUSHD bin-int
gcc %FILES% %FLAGS% -c %INCLUDES% %DEFINES%

if %ERRORLEVEL% NEQ 0 (exit /b)

SET OBJ_FILES= 
for /R %%f in (*.o) do (
	SET OBJ_FILES=!OBJ_FILES! %%f
)
POPD

gcc %OBJ_FILES% -o bin/%assembly%.exe %INCLUDES% %DEFINES% %LINKS%

if %ERRORLEVEL% NEQ 0 (exit /b) else (echo Successfully built %assembly%^^!)

COPY bin\%assembly%.exe %assembly%.exe 1>NUL