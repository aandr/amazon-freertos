@echo off
rem ################################################################################
rem # This batch file executes some preprocess for build and then executes the make
rem ################################################################################

if not exist "%~dp0..\..\..\..\lib\third_party\mcu_vendor\renesas\tools\aws_tests.bat" (
    echo ERROR: Unable to find "%~dp0..\..\..\..\lib\third_party\mcu_vendor\renesas\tools\aws_tests.bat"
    exit 2
)

"%~dp0..\..\..\..\lib\third_party\mcu_vendor\renesas\tools\aws_tests.bat" %*
