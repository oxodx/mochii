@echo off
call premake5 vs2026
IF %ERRORLEVEL% NEQ 0 (
	PAUSE
)
