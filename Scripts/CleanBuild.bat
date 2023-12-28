@echo off
set directory="../Build"
if exist %directory% (
    rmdir /s /q %directory%
    echo Build directory deleted successfully.
) else (
    echo Build directory does not exist.
)
pause