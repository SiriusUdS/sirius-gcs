@echo off

pushd %~dp0..
.\tools\premake5 vs2022
popd
exit /b 0
