@echo off
SetLocal EnableDelayedExpansion

cd ../

if not exist bin mkdir bin
if not exist bin-int mkdir bin-int

SET FILES= 
for /R %%f in (*.c) do (
	SET FILES=!FILES! %%f
)

SET FLAGS=-Wall -Wextra -O3
SET INCLUDES=-I../hermes/src
SET DEFINES=-DHM_ENABLE_ASSERTS

SET assembly=Hermes
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