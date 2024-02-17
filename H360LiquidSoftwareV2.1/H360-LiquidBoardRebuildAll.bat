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

REM AVR_IAR编译文件路径
set AVRIARPATH=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.0\common\bin\

REM H360版本
REM 磁珠针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleBead\H360LiquidSoftwareNeedleBeadV2.1.ewp -build "Release" -log warnings

REM 试剂针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleReagent\H360LiquidSoftwareNeedleReagentV2.1.ewp -build "Release" -log warnings

REM 样本针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleSample\H360LiquidSoftwareNeedleSampleV2.1.ewp -build "Release" -log warnings

REM H360S版本
REM 磁珠针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleBead\H360LiquidSoftwareNeedleBeadV2.1.ewp -build "H360S_Release" -log warnings

REM 试剂针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleReagent\H360LiquidSoftwareNeedleReagentV2.1.ewp -build "H360S_Release" -log warnings

REM 样本针 编译
"%AVRIARPATH%IarBuild.exe" %cd%\Code\ProjectCollection\ProjectNeedleSample\H360LiquidSoftwareNeedleSampleV2.1.ewp -build "H360S_Release" -log warnings

REM 删除以前的目标文件
call DeleteFile.bat

pause

@ECHO ON