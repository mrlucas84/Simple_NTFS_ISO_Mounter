# -----------------------------------------------------------------
# Simple NTFS ISO Mounter Makefile (CaptainCPS-X, 2013)
# -----------------------------------------------------------------
APP_NAME			?=	Simple_NTFS_ISO_Mounter
APP_VER				?=	1.02
DATE 				?=	$(shell date +"%Y%m%d")
CONTENTID			?=	CAPCPS-SISO00123_00-0000000000000000

# -----------------------------------------------------------------
# General Paths
# -----------------------------------------------------------------
CCORE_PATH			=	$(CURDIR)
OBJS_DIR			= 	objs

# -----------------------------------------------------------------
# Change this line with your specific Python installation path
# -----------------------------------------------------------------
PYTHON_PATH			=	/c/Python27

# -----------------------------------------------------------------
# Build tools path
# -----------------------------------------------------------------
MY_BIN_TOOLS		=	$(CURDIR)/bin

# -----------------------------------------------------------------
# Update environment PATH variable
# -----------------------------------------------------------------
export PATH 		:=	$(PATH):$(PYTHON_PATH):$(MY_BIN_TOOLS)

# -----------------------------------------------------------------
# Left here in case you have a different SDK setup
# -----------------------------------------------------------------
SDK_VERSION			?=	3_4_0
CELL_SDK			?=	/c/usr/local/cell
CELL_MK_DIR			?=	$(CELL_SDK)/samples/mk
export SN_PS3_PATH	:=	$(CELL_SDK)/host-win32/sn

# -----------------------------------------------------------------
# Compiled myself (properly sign SELF(s) with compression) 
# -----------------------------------------------------------------
MAKE_SELF			= make_self_wc.exe

# -----------------------------------------------------------------
# SCETOOL used to sign "EBOOT.BIN"
# -----------------------------------------------------------------
MAKE_SELF_NPDRM		=	scetool.exe \
						--sce-type=SELF \
						--compress-data=TRUE \
						--skip-sections=FALSE \
						--key-revision=04 \
						--self-ctrl-flags=4000000000000000000000000000000000000000000000000000000000000002 \
						--self-cap-flags=00000000000000000000000000000000000000000000007B0000000100000000 \
						--self-auth-id=1010000001000003 \
						--self-app-version=0001000000000000 \
						--self-add-shdrs=TRUE \
						--self-vendor-id=01000002 \
						--self-type=NPDRM \
						--self-fw-version=0003004000000000 \
						--np-license-type=FREE \
						--np-content-id=$(CONTENTID) \
						--np-app-type=EXEC \
						--np-real-fname="EBOOT.BIN" \
						--np-klicensee=72F990788F9CFF745725F08E4C128387 \
						--np-add-sig=FALSE \
						--encrypt

# -----------------------------------------------------------------
# "pkg.py" will create PKGs properly, "psn_package_npdrm.exe" only add 
# specific files and directories, but for example, will not add 
# "MANUAL" directory if present.
# -----------------------------------------------------------------
PKG			 		?=	pkg.py

# -----------------------------------------------------------------
# Uncomment this at your own risk, most warnings will not be displayed
# -----------------------------------------------------------------
#PPU_CWARNFLAGS		?=	$(CWARNFLAGS_L1)
#PPU_CXXWARNFLAGS	?=	$(CXXWARNFLAGS_L1)
# -----------------------------------------------------------------
include $(CELL_MK_DIR)/sdk.makedef.mk
# -----------------------------------------------------------------

PPU_LDLIBDIR		+=	-L$(PPU_PSGL_LIBDIR) \
						-L$(CELL_SDK)/target/ppu/lib/hash \
						-L$(CURDIR)\source\cobra

PPU_LDLIBS			:=  -lPSGLFX -lPSGL -lPSGLU \
						-lm \
						-lusbd_stub \
						-lfs_stub \
						-lio_stub \
						-lsysutil_stub \
						-ldbgfont \
						-lresc_stub \
						-lgcm_cmd \
						-lgcm_sys_stub \
						-lsysmodule_stub \
						-lperf \
						-lsysutil_screenshot_stub \
						-lnet_stub \
						-lnetctl_stub \
						-laudio_stub \
						-lmstreamThreadMP3 \
						-lspurs_stub \
						-lcobra -lmd5

GCC_PPU_CXXSTDFLAGS	+=	-fno-exceptions -O3 -Wall -mbig-endian
GCC_PPU_LDFLAGS		+=	-fno-exceptions -O3 -Wall -mbig-endian

PPU_CSTDFLAGS 		+= 	-Wno-sign-compare -fno-exceptions -O3 -Wall -std=gnu99 -mbig-endian
PPU_CXXSTDFLAGS		+= 	-Wno-sign-compare -fno-exceptions -O3 -Wall -mbig-endian

PPU_CPPFLAGS		+=	-DPSGL \
						-DHAVE_ZLIB_DEFLATE -DMINIZ_NO_ZLIB_COMPATIBLE_NAMES \
						-DAUDIO_CHANNELS=2 \
						-DHAVE_CONFIG_H \
						-DFPM_PPC \
						-DNO_GETENV

# --------------------------------------------------------------

PPU_TARGET			:=	EBOOT.ELF

PPU_INCDIRS			+= 	-Iinclude \
						-Isource \
						-Isource/miniz \
						-Isource/zlib \
						-Isource/ftp \
						-Isource/rarch \
						-Isource/history \
						-Isource/cobra

PPU_SRCS			= 	source/main.cpp \
						source/video.cpp \
						source/utils.cpp \
						source/font.cpp \
						source/input.cpp \
						source/cfw_modules.cpp \
						source/iso_mount.cpp \
						source/graphic.cpp \
						source/menu.cpp
						
					
PPU_SRCS			+=	source/rarch/rpng.cpp
					
PPU_SRCS			+=	source/ftp/ftp.cpp \
						source/ftp/ftpcmd.cpp \
						source/ftp/functions.cpp

PPU_SRCS			+=	source/zlib/adler32.c \
						source/zlib/compress.c \
						source/zlib/crc32.c \
						source/zlib/deflate.c \
						source/zlib/gzclose.c \
						source/zlib/gzlib.c \
						source/zlib/gzread.c \
						source/zlib/gzwrite.c \
						source/zlib/infback.c \
						source/zlib/inffast.c \
						source/zlib/inflate.c \
						source/zlib/inftrees.c \
						source/zlib/trees.c \
						source/zlib/uncompr.c \
						source/zlib/zutil.c
					
PPU_SRCS			+=	source/history/history.c

# -----------------------------------------------------------------
# SHADERS STUFF
# -----------------------------------------------------------------
#VPSHADER_SOURCES	:=	shaders/vs_basicSample.cg
#FPSHADER_SOURCES	:=	shaders/fs_basicSample.cg

FPSHADER_ELFS		=	$(patsubst %.cg, %.cgelf, $(FPSHADER_SRCS))
VPSHADER_ELFS		=	$(patsubst %.cg, %.cgelf, $(VPSHADER_SRCS))

PSGL_CGNV2ELF		=	$(PSGL_TOOLS_DIR)/cgnv2elf

# piggy back deletion of our .cgelf files into the cleanup system
# by using the user-clean rule instead of the clean-cg rule
CLEANFILES			+= $(FPSHADER_ELFS) $(VPSHADER_ELFS)
TARGET				+= $(FPSHADER_ELFS) $(VPSHADER_ELFS)
# -----------------------------------------------------------------

# -----------------------------------------------------------------
include $(CELL_MK_DIR)/sdk.target.mk
# -----------------------------------------------------------------

# -----------------------------------------------------------------
# SHADERS
# -----------------------------------------------------------------

$(FPSHADER_ELFS): %.cgelf: %.fpo
	$(PSGL_CGNV2ELF) $< $@

$(VPSHADER_ELFS): %.cgelf: %.vpo
	$(PSGL_CGNV2ELF) $< $@

# -----------------------------------------------------------------

SM_RELEASE = SOFTWARE_MANUAL_R1

RLS_DIR		= release
BASE_DIR	= $(RLS_DIR)/BASE
UPD_DIR		= $(RLS_DIR)/UPD
MAN_DIR		= $(RLS_DIR)/MANUAL_PKG

BASE_PKG	= $(APP_NAME)_$(APP_VER)_[$(DATE)]_[COBRA_7_446_CFW]_[BASE].pkg
UPD_PKG		= $(APP_NAME)_$(APP_VER)_[$(DATE)]_[COBRA_7_446_CFW]_[UPD].pkg
MAN_PKG		= $(APP_NAME)_$(SM_RELEASE).pkg

LIBCOBRA_SRC = cobra.c
LIBCOBRA_OBJ = cobra.o
LIBCOBRA_LIB = libcobra.a

_cobralib:
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Compiling... libcobra.a"
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	@cd source/cobra; rm -fr *.a *.o; cd ../..
	@cd source/cobra; $(SN_PS3_PATH)/../ppu/bin/ppu-lv2-gcc --std=c99 -O2 -Wall -c $(LIBCOBRA_SRC); cd ../..
	@cd source/cobra; $(SN_PS3_PATH)/../ppu/bin/ppu-lv2-ar rcs $(LIBCOBRA_LIB) $(LIBCOBRA_OBJ); cd ../..
	@cd source/cobra; $(SN_PS3_PATH)/../ppu/bin/ppu-lv2-strip -x $(LIBCOBRA_LIB); cd ../..
	@echo "Done!..."
	@echo "|"
	@echo "---------------------------------------------------------------------------"	
	@echo "Compiling... rawseciso.sprx"
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	@cd source/rawseciso; rm -fr objs *.prx *.sprx; cd ../..
	@cd source/rawseciso; make all; cd ../..
	@sleep 0.5
	@cp -fr source/rawseciso/rawseciso.sprx $(BASE_DIR)/USRDIR/rawseciso.sprx
	@cp -fr source/rawseciso/rawseciso.sprx $(UPD_DIR)/USRDIR/rawseciso.sprx
	@echo "Done!..."
	@echo "|"
	@echo "---------------------------------------------------------------------------"	
	@echo "Compiling Simple NTFS ISO Mounter..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	
_ccore: _cobralib $(PPU_TARGET)
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Signing / Copying / Cleaning ..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	@$(PPU_STRIP) -s $(PPU_TARGET) -o $(OBJS_DIR)/EBOOT.ELF
	@rm -fr EBOOT.ELF EBOOT.self
	@rm -fr $(BASE_DIR)/USRDIR/RELOAD.SELF $(UPD_DIR)/USRDIR/RELOAD.SELF
	@$(MY_BIN_TOOLS)/$(MAKE_SELF) $(OBJS_DIR)/EBOOT.ELF $(BASE_DIR)/USRDIR/RELOAD.SELF
	@rm -fr EBOOT.ELF EBOOT.self
	@cp -fr $(BASE_DIR)/USRDIR/RELOAD.SELF $(UPD_DIR)/USRDIR/RELOAD.SELF
	@cd bin; $(MAKE_SELF_NPDRM) ../$(OBJS_DIR)/EBOOT.ELF ../$(BASE_DIR)/USRDIR/EBOOT.BIN; cd ..
	@cp -fr $(BASE_DIR)/USRDIR/EBOOT.BIN $(UPD_DIR)/USRDIR/EBOOT.BIN

pkg:
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Preparing BASE PKG..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	$(PKG) --contentid $(CONTENTID) $(CURDIR)/$(BASE_DIR)/ $(CURDIR)/$(RLS_DIR)/$(BASE_PKG)
	@echo "Done!"

pkg_upd:
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Preparing PKG..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	$(PKG) --contentid $(CONTENTID) $(CURDIR)/$(UPD_DIR)/ $(CURDIR)/$(RLS_DIR)/$(UPD_PKG)
	@echo "Done!"

pkg_manual:
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Preparing SOFTWARE MANUAL PKG..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	$(PKG) --contentid $(CONTENTID) $(CURDIR)/$(MAN_DIR)/ $(CURDIR)/$(RLS_DIR)/$(MAN_PKG)
	@echo "Done!"

_cleanup: clean
	@echo "|"
	@echo "---------------------------------------------------------------------------"
	@echo "Cleaning all cores (please wait)..."
	@echo "---------------------------------------------------------------------------"
	@echo "|"
	@rm -fr $(OBJS_DIR) source/cobra/*.a source/cobra/*.o
