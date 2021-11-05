@echo off

echo 开始清除无用文件
del /s /q %~dp0*.log
del /s /q %~dp0*.scvd

pause