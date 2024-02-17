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

REM H360工程编译
REM BootLoader编译
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_Bootloader\ProjectFiles\H360_ActionBoard_Bootloader.ewp -build "H360_Release" -log warnings
REM 一号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_1\ProjectFiles\H360_ActionBoard_ID_1.ewp -build "H360_Release" -log warnings
REM 二号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_2\ProjectFiles\H360_ActionBoard_ID_2.ewp -build "H360_Release" -log warnings
REM 三号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_3\ProjectFiles\H360_ActionBoard_ID_3.ewp -build "H360_Release" -log warnings
REM 四号
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_4\ProjectFiles\H360_ActionBoard_ID_4.ewp -build "H360_Release" -log warnings
REM 五号板单机
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_5\ProjectFiles\H360_ActionBoard_ID_5.ewp -build "H360_Release" -log warnings
REM 八号板单机轨道
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_8\ProjectFiles\H360_ActionBoard_ID_8.ewp -build "H360_Release" -log warnings

REM H360S工程编译
REM BootLoader编译
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_Bootloader\ProjectFiles\H360_ActionBoard_Bootloader.ewp -build "H360S_Release" -log warnings
REM 一号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_1\ProjectFiles\H360_ActionBoard_ID_1.ewp -build "H360S_Release" -log warnings
REM 二号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_2\ProjectFiles\H360_ActionBoard_ID_2.ewp -build "H360S_Release" -log warnings
REM 三号板
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_3\ProjectFiles\H360_ActionBoard_ID_3.ewp -build "H360S_Release" -log warnings
REM 四号
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_4\ProjectFiles\H360_ActionBoard_ID_4.ewp -build "H360S_Release" -log warnings
REM 五号板流水线
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_5\ProjectFiles\H360_ActionBoard_ID_5.ewp -build "H360S_Release" -log warnings
REM 七号板流水线接口轨道
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_7_Online\ProjectFiles\H360_ActionBoard_ID_7_Online.ewp -build "H360S_Release" -log warnings
REM 八号板流水线本机轨道
"IarBuild.exe" %cd%\ProjectCollection\H360_ActionBoard_ID_8_Local\ProjectFiles\H360_ActionBoard_ID_8_Local.ewp -build "H360S_Release" -log warnings

REM 删除以前的目标文件
call DeleteFile.bat

pause

@ECHO ON