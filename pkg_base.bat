@echo off
@cd %~dp0
@set PATH=C:/msys/1.0/bin;C:/mingw/bin;%PATH%;

make pkg

pause