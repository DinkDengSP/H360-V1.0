@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM 删除以前的目标文件
del /a /f /q %cd%\Output\*.bin

REM 删除SVN版本号文件
del %cd%\CommonFunc\SystemUtil\SvnVersion.h
REM 生成新的SVN版本号文件
subwcrev.exe ./ "./CommonFunc/SystemUtil/VersionSvnTemplate.h" "./CommonFunc/SystemUtil/SvnVersion.h"

REM 七号板流水线接口轨道
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_7_Online\ProjectFiles\H360_ActionBoard_ID_7_Online.ewp -build "H360S_Release" -log warnings
REM 八号板流水线本机轨道
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_8_Local\ProjectFiles\H360_ActionBoard_ID_8_Local.ewp -build "H360S_Release" -log warnings

REM 删除以前的目标文件
REM call DeleteFile.bat

pause

@ECHO ON