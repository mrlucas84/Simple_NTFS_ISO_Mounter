@echo off

@set PATH=C:/msys/1.0/bin;C:/mingw/bin;%PATH%;

@make _ccore
rem make _ccore 2>&1 | tee makelog.txt 

@call source/ntfs_isolist/build_ntfs_isolist.bat

