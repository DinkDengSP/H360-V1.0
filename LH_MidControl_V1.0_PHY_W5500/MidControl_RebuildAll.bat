@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM 删除以前的目标文件
del /a /f /q %cd%\Data\*.bin

REM 删除SVN版本号文件
del %cd%\Code\CommonFiles\SystemUtil\SvnVersion.h
REM 生成新的SVN版本号文件
subwcrev.exe ./ "./Code/CommonFiles/SystemUtil/SvnVersionTemplate.h" "./Code/CommonFiles/SystemUtil/SvnVersion.h"

REM BootLoader编译
"IarBuild.exe" %cd%\Code\ProjectCollection\MidControlBoot\ProjectFiles\MidControlBoot_PHY_W5500.ewp -build "CI" -log warnings
REM BootLoader编译
"IarBuild.exe" %cd%\Code\ProjectCollection\MidControlBoot\ProjectFiles\MidControlBoot_PHY_W5500.ewp -build "CS" -log warnings
REM App编译
"IarBuild.exe" %cd%\Code\ProjectCollection\MidControlApp\ProjectFiles\MidControlApp_PHY_W5500.ewp -build "CI" -log warnings
REM App编译
"IarBuild.exe" %cd%\Code\ProjectCollection\MidControlApp\ProjectFiles\MidControlApp_PHY_W5500.ewp -build "CS" -log warnings
REM 删除无用文件
call DeleteFile.bat

pause

@ECHO ON