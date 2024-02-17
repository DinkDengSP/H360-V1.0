@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

del %cd%\*.ninja_deps /f /s /q /a
del %cd%\*.ninja_log /f /s /q /a
del %cd%\*.o /f /s /q /a
del %cd%\*.pbi /f /s /q /a
del %cd%\*.dep /f /s /q /a
del %cd%\*.xcl /f /s /q /a
del %cd%\*.map /f /s /q /a
del %cd%\*.pbd /f /s /q /a
del %cd%\*.browse /f /s /q /a
del %cd%\*.pbw /f /s /q /a
del %cd%\*.ninja /f /s /q /a
del %cd%\*.jlink /f /s /q /a
del %cd%\*.wsdt /f /s /q /a
del %cd%\*.out /f /s /q /a
del %cd%\*.lst /f /s /q /a
del %cd%\*.s90 /f /s /q /a
del %cd%\*.r90 /f /s /q /a
del %cd%\*.d90 /f /s /q /a

pause

@ECHO ON