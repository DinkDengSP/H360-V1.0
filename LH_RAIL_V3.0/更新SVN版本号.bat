@ECHO OFF

REM 申请管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM 删除SVN版本号文件
del %cd%\Code\CommonFiles\SystemUtil\SvnVersion.h

REM 生成新的SVN版本号文件
subwcrev.exe ./ "./Code/CommonFiles/SystemUtil/SvnVersionTemplate.h" "./Code/CommonFiles/SystemUtil/SvnVersion.h

pause

@ECHO ON