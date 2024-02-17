@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM 删除SVN版本号文件,生成新的SVN版本号文件 APP
del %cd%\..\CommonFiles\UtilFiles\SvnVersion.h
subwcrev.exe ./ "./../CommonFiles/UtilFiles/VersionSvnTemplate.h" "./../CommonFiles/UtilFiles/SvnVersion.h"

REM 删除以前的目标文件
del /a /f /q %cd%\..\..\Data\*.bin

REM BootLoader编译
"IarBuild.exe" %cd%\App\PorjectFiles\H360_MidControl1.0_PHY_App.ewp -build "Release" -log warnings
REM App编译
"IarBuild.exe" %cd%\Boot\PorjectFiles\H360_MidControl1.0_PHY_Boot.ewp -build "Release" -log warnings
REM 删除无用文件
call DeleteFile.bat

pause

@ECHO ON