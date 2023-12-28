@echo off

echo "Creating Debug build..."
set hasBeenCopied=0
if exist "..\Binaries\windows-x86_64\Debug\App\App.exe" (
	if exist "..\Binaries\windows-x86_64\Debug\App\Core.dll" (
		echo copying exe
		copy "..\Binaries\windows-x86_64\Debug\App\App.exe" "..\Build\Debug\" 
		echo copying dll
		copy "..\Binaries\windows-x86_64\Debug\App\Core.dll" "..\Build\Debug\"
		echo copying resources
		xcopy "..\Resources\" "..\Build\Debug\" /E /I /Y
		set hasBeenCopied=1
	)
)
if %hasBeenCopied%==1 (
	echo "Debug build created successfully"
) else (
	echo "Debug build creation failed"
)

echo.
echo.
echo Creating Release build...
set hasBeenCopied=0
if exist "..\Binaries\windows-x86_64\Release\App\App.exe" (
	if exist "..\Binaries\windows-x86_64\Release\App\Core.dll" (
		echo copying exe
		copy "..\Binaries\windows-x86_64\Release\App\App.exe" "..\Build\Release\"
		echo copying dll
		copy "..\Binaries\windows-x86_64\Release\App\Core.dll" "..\Build\Release\"
		echo copying resources
		xcopy "..\Resources\" "..\Build\Release\" /E /I /Y
		set hasBeenCopied=1
	)
)
if %hasBeenCopied%==1 (
	echo "Release build created successfully"
) else (
	echo "Release build creation failed"
)

echo.
echo.
echo "Creating Dist build..."
set hasBeenCopied=0
if exist "..\Binaries\windows-x86_64\Dist\App\App.exe" (
	if exist "..\Binaries\windows-x86_64\Dist\App\Core.dll" (
		echo copying exe
		copy "..\Binaries\windows-x86_64\Dist\App\App.exe" "..\Build\Release\"
		echo copying dll
		copy "..\Binaries\windows-x86_64\Dist\App\Core.dll" "..\Build\Release\"
		echo copying resources
		xcopy "..\Resources\" "..\Build\Release\" /E /I /Y
		set hasBeenCopied=1
	)
)
if %hasBeenCopied%==1 (
	echo "Dist build created successfully"
) else (
	echo "Dist build creation failed"
)
echo.
echo.
pause