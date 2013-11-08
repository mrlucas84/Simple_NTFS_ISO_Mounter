#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

#---------------------------------------------------------------------------------
#  TITLE, APPID, CONTENTID, ICON0 SFOXML before ppu_rules.
#---------------------------------------------------------------------------------

TITLE		:=	NTFS_ISO
APPID		:=	NISO56789
CONTENTID	:=	UP0001-$(APPID)_00-0000000000000000

include $(PSL1GHT)/ppu_rules

TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	datas
SHADERS		:=	shaders
INCLUDES	:=	include

LIBDIRS		:=	-L$(CURDIR) -L$(CURDIR)/../libntfs_ext

LIBS		:=	-lntfs_ext \
				-lcobra_psl1ght \
				\
				-lc -lsysfs -lfont -ltiny3d -lsimdmath -lgcm_sys -lio -lsysutil -lrt -llv2 -lsysmodule -lm

CFLAGS		=	-O2 -Wall -std=gnu99 -mcpu=cell $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)
LDFLAGS		=	$(MACHDEP) -Wl,-Map,$(notdir $@).map



ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SHADERS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export BUILDDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:= $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin)))
VCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.vcg)))
FCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.fcg)))

VPOFILES	:=	$(VCGFILES:.vcg=.vpo)
FPOFILES	:=	$(FCGFILES:.fcg=.fpo)

ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(addsuffix .o,$(VPOFILES)) \
					$(addsuffix .o,$(FPOFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(LIBPSL1GHT_INC) \
					-I$(CURDIR)/$(BUILD) -I$(PORTLIBS)/include

export LIBPATHS	:=	$(LIBPSL1GHT_LIB) -L$(PORTLIBS)/lib $(LIBDIRS)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).self  EBOOT.BIN

run:
	ps3load $(OUTPUT).self

# Compiled myself (properly sign SELF(s) with compression) 
MAKE_SELF		= 	make_self_wc.exe
MY_BIN_TOOLS	=	$(CURDIR)/../../bin

ntfs_ext_iso: $(BUILD)
	@rm -fr $(OUTPUT).elf
	@$(MY_BIN_TOOLS)/$(MAKE_SELF) $(CURDIR)/$(BUILD)/ntfs_ext_iso.elf ntfs_ext_iso.self
	@cp -fr ntfs_ext_iso.self ../../release/BASE/USRDIR/ntfs_ext_iso.self
	@cp -fr ntfs_ext_iso.self ../../release/UPD/USRDIR/ntfs_ext_iso.self

else

DEPENDS	:=	$(OFILES:.o=.d)

$(OUTPUT).self: $(OUTPUT).elf
$(OUTPUT).elf:	$(OFILES)

%.bin.o	:	%.bin
	@echo $(notdir $<)
	@$(bin2o)

%.vpo.o	:	%.vpo
	@echo $(notdir $<)
	@$(bin2o)

%.fpo.o	:	%.fpo

	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

endif