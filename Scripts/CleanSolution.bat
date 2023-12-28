@echo off
set directory="../Binaries"
if exist %directory% (
    rmdir /s /q %directory%
    echo Intermediate directory deleted successfully.
) else (
    echo Binaries directory does not exist.
)
set directory="../.vs"
if exist %directory% (
    rmdir /s /q %directory%
    echo .vs directory deleted successfully.
) else (
    echo .vs directory does not exist.
)
cd ../
del /S *.sln
del /S *.vcxproj
del /S *.vcxproj.filters
del /S *.vcxproj.user
echo Solution and project files deleted successfully.
pause