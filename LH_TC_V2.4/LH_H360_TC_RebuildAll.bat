@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM 删除以前的目标文件
del /a /f /q %cd%\Data\*.bin

REM 删除SVN版本号文件
del %cd%\\Code\CommonFunc\CommonUtil\SvnVersion.h
REM 生成新的SVN版本号文件
subwcrev.exe ./ "./Code/CommonFunc/CommonUtil/VersionSvnTemplate.h" "./Code/CommonFunc/CommonUtil/SvnVersion.h"

REM Boot编译
"IarBuild.exe" %cd%\Code\ProjectCollection\LH_TC_BOOT\ProjectFiles\LH_TC_BOOT.ewp -build "H360_Release" -log warnings
REM APP编译
"IarBuild.exe" %cd%\Code\ProjectCollection\LH_TC_APP\ProjectFiles\LH_TC_APP.ewp -build "H360_Release" -log warnings
REM 删除以前的目标文件
call DeleteFile.bat

pause

@ECHO ON