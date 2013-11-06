@echo off

cd source/ntfs_isolist
@call cleanup.bat

cd ../libntfs_ext
@call cleanup.bat

cd ../rawseciso
@call cleanup.bat

cd ../ntfs_ext_iso
@call cleanup.bat

cd ../..

@set PATH=C:/msys/1.0/bin;C:/mingw/bin;%PATH%;

@make _cleanup
