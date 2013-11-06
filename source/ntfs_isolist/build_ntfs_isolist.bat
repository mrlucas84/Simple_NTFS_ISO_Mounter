@echo off

@cd %~dp0

set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=c:/PSDK3v2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%WIN_PS3SDK%/ps3dev/bin;%WIN_PS3SDK%/ps3dev/ppu/bin;%WIN_PS3SDK%/ps3dev/spu/bin;%WIN_PS3SDK%/mingw/Python27;%PATH%;
set PSL1GHT=%PS3SDK%/psl1ght
set PS3DEV=%PS3SDK%/ps3dev

@echo "|"
@echo "---------------------------------------------------------------------------"
@echo "Building... libntfs_ext.a (w/PSL1GHT)"
@echo "---------------------------------------------------------------------------"
@echo "|"

@cd ../libntfs_ext

@echo " "

make

@echo "|"
@echo "---------------------------------------------------------------------------"
@echo "Building... ntfs_ext_iso.self (w/PSL1GHT)"
@echo "---------------------------------------------------------------------------"
@echo "|"

@cd ../ntfs_ext_iso

@echo " "

make ntfs_ext_iso

@echo "|"
@echo "---------------------------------------------------------------------------"
@echo "Building... ntfs_isolist.self (w/PSL1GHT)"
@echo "---------------------------------------------------------------------------"
@echo "|"

@cd ../ntfs_isolist

@echo " "

make isolist

pause