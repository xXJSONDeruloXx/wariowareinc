#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

BASEROM_SHA1 := 3f556448d290fa5406d6ed367fee16cc02387ad3

ifeq ($(strip $(DEVKITARM)),)
    $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq (,$(wildcard baserom.gba))
    $(error No ROM provided. Please place an unmodified ROM named "baserom.gba" in the root folder)
endif

ifneq ($(shell sha1sum -t baserom.gba), $(BASEROM_SHA1)  baserom.gba)
    $(error Provided ROM is not correct. Expected SHA1: $(BASEROM_SHA1))
endif

SHELL := /bin/bash

CPP := $(CC) -E

include $(DEVKITARM)/base_rules

CROSS   := arm-none-eabi-
OBJCOPY := $(CROSS)objcopy
LD      := $(CROSS)gcc
AS      := $(CROSS)as
CC1     := tools/agbcc/bin/agbcc

# Verbose toggle
V := @
ifeq ($(VERBOSE), 1)
    V =
endif

# Colors
NO_COL  := \033[0m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m

define print
  $(V)echo -e "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)"
endef

# Whether to build a byte-for-byte matching ROM
NONMATCHING ?= 0

TARGET      := wariowareinc
TARGET_SHA1 := $(BASEROM_SHA1)

# Preprocessor defines
CFLAGS   := -mthumb-interwork -Wparentheses -O2 -fhex-asm
CPPFLAGS := -I tools/agbcc -I tools/agbcc/include -I . -iquote include -nostdinc -undef

#---------------------------------------------------------------------------------

BUILD      := build
SOURCES    := src $(shell find src -type d)
ASM        := asm
INCLUDES   := include
BIN        := bin
DATA	   := data
SCENE_DATA := $(shell find $(DATA)/scenes -type d)
GRAPHICS   := $(shell find graphics -type d)
AUDIO      := audio
MUSIC	:= $(AUDIO)/sequences
SFX        := $(AUDIO)/samples

C_DIRS     := $(sort $(SOURCES) $(GRAPHICS) $(AUDIO) $(DATA) $(SCENE_DATA))
ASM_DIRS   := $(sort $(ASM) $(DATA))
BS_DIRS    := $(SCENE_DATA)
GFX_DIRS   := $(GRAPHICS)

ALL_DIRS   := $(BIN) $(ASM_DIRS) $(C_DIRS) $(SFX) $(MUSIC)
ALL_DIRS   := $(sort $(ALL_DIRS))
BUILD_DIRS := $(BUILD) $(addprefix $(BUILD)/,$(ALL_DIRS))

ifeq ($(NONMATCHING), 0)
    LD_SCRIPT := wariowareinc.ld
else
    LD_SCRIPT := wariowareinc_modern.ld
endif

UNDEFINED_SYMS := undefined_syms.ld

#---------------------------------------------------------------------------------

export OUTPUT := $(BUILD)/$(TARGET)

CFILES   := $(foreach dir,$(C_DIRS),$(wildcard $(dir)/*.c))
SFILES   := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s)) $(foreach dir,$(BS_DIRS),$(wildcard $(dir)/*.bs))
SFILES   := $(filter-out asm/asm_08008130.s asm/asm_0800daec.s asm/asm_0800daf8.s asm/asm_0800e760.s asm/asm_0800fb9c.s asm/asm_0801684c.s asm/asm_08016d7c.s asm/asm_08016f58.s asm/asm_08016f5c.s asm/asm_080171f4.s asm/asm_08017200.s asm/asm_0801749c.s asm/asm_080174a0.s asm/asm_08017910.s asm/asm_0801792c.s asm/asm_080179dc.s asm/asm_080179e0.s asm/asm_08017a2c.s asm/asm_08017a48.s asm/asm_08017a54.s asm/asm_080180c8.s asm/asm_080194d4.s asm/asm_08019500.s asm/asm_08019f00.s asm/asm_08019f04.s asm/asm_0801a0b0.s asm/asm_0801a0cc.s asm/asm_0801a5b0.s asm/asm_0801ad60.s asm/asm_0801ad9c.s asm/asm_0801b3d8.s asm/asm_0801b77c.s asm/asm_0801b79c.s asm/asm_0801c540.s asm/asm_0801ccbc.s asm/asm_0801cce0.s asm/asm_0801cd78.s asm/asm_0801cd84.s asm/asm_0801cd88.s asm/asm_0801cdd8.s asm/asm_0801cdfc.s asm/asm_0801cfcc.s asm/asm_0801dafc.s asm/asm_0801e4e8.s asm/asm_0801e510.s asm/asm_0801e6e8.s \
    asm/asm_0801e6f4.s asm/asm_0801e914.s asm/asm_0801e938.s asm/asm_080202dc.s asm/asm_080202f8.s asm/asm_080203ac.s asm/asm_080203b8.s asm/asm_080203bc.s asm/asm_080203f4.s asm/asm_08020414.s asm/asm_08020b60.s asm/asm_08020b64.s asm/asm_08020fac.s asm/asm_08020fd4.s asm/asm_08021070.s asm/asm_0802107c.s asm/asm_08021080.s asm/asm_080210d0.s asm/asm_080210fc.s asm/asm_080218d0.s asm/asm_080222d0.s asm/asm_080222d4.s asm/asm_0802264c.s asm/asm_08022674.s asm/asm_0802278c.s asm/asm_08022798.s asm/asm_0802279c.s asm/asm_080227ec.s asm/asm_0802280c.s asm/asm_08022ce4.s asm/asm_08022ce8.s asm/asm_08022ec4.s asm/asm_08022ee4.s asm/asm_08023120.s asm/asm_08023124.s asm/asm_08023240.s asm/asm_0802325c.s asm/asm_08023ca0.s asm/asm_08023cbc.s asm/asm_08023de0.s asm/asm_08023de4.s asm/asm_08023efc.s asm/asm_08023f30.s asm/asm_0802476c.s asm/asm_08024770.s asm/asm_08024b54.s asm/asm_08025550.s \
    asm/asm_08025600.s asm/asm_08025604.s asm/asm_080257ac.s asm/asm_08025898.s asm/asm_08026408.s asm/asm_0802640c.s asm/asm_08026458.s asm/asm_08026474.s asm/asm_08026480.s asm/asm_08026b18.s asm/asm_080272f0.s asm/asm_08027494.s asm/asm_080278c8.s asm/asm_08027ae4.s asm/asm_08027e38.s asm/asm_08028448.s asm/asm_080285d8.s asm/asm_08028930.s asm/asm_08028e70.s asm/asm_08028e7c.s asm/asm_08029444.s asm/asm_08029450.s asm/asm_080299ec.s asm/asm_0802b098.s asm/asm_0802b4e8.s asm/asm_0802b4f4.s asm/asm_0802ba94.s asm/asm_0802ba98.s asm/asm_0802c0d0.s asm/asm_0802c0dc.s asm/asm_0802c264.s asm/asm_0802c4ec.s asm/asm_0802ca78.s asm/asm_0802ca84.s asm/asm_0802cdf0.s asm/asm_0802cdfc.s asm/asm_0802d318.s asm/asm_0802da54.s asm/asm_0802da60.s asm/asm_0802e4c8.s asm/asm_0802ecb0.s asm/asm_0802ecbc.s asm/asm_0802f4b4.s asm/asm_0802fbb8.s asm/asm_0802fbc4.s asm/asm_080307cc.s asm/asm_080307d8.s \
    asm/asm_08030c94.s asm/asm_08030ca0.s asm/asm_080315e8.s asm/asm_080315ec.s asm/asm_08031c58.s asm/asm_08031c64.s asm/asm_08032034.s asm/asm_08032040.s asm/asm_0803294c.s asm/asm_08032e1c.s asm/asm_08032e28.s asm/asm_08032e34.s asm/asm_08033624.s asm/asm_08033630.s asm/asm_08033d2c.s asm/asm_08033d38.s asm/asm_080342bc.s asm/asm_080342c0.s asm/asm_0803572c.s asm/asm_08035738.s asm/asm_080359a8.s asm/asm_0803679c.s asm/asm_08037188.s asm/asm_08037acc.s asm/asm_08037ad0.s asm/asm_08037de4.s asm/asm_0803a090.s asm/asm_0803a758.s asm/asm_0803a75c.s asm/asm_0803c150.s asm/asm_0803c6e4.s asm/asm_0803cdc4.s asm/asm_0803f32c.s asm/asm_0803fb88.s asm/asm_0803fe70.s asm/asm_0803fe74.s asm/asm_08041554.s asm/asm_08042364.s asm/asm_080434bc.s asm/asm_08043c74.s asm/asm_08044480.s asm/asm_08044c78.s asm/asm_08045514.s asm/asm_08045c20.s asm/asm_08046adc.s asm/asm_08046afc.s asm/asm_08046ff8.s \
    asm/asm_080477e0.s asm/asm_080477e4.s asm/asm_0804861c.s asm/asm_08048a04.s asm/asm_08048eec.s asm/asm_08049734.s asm/asm_08049738.s asm/asm_08049e18.s asm/asm_0804a738.s asm/asm_0804b18c.s asm/asm_0804d884.s asm/asm_0804e6c0.s asm/asm_080508d8.s asm/asm_08051728.s asm/asm_08051e48.s asm/asm_08051e4c.s asm/asm_080521b0.s asm/asm_08053538.s asm/asm_0805353c.s asm/asm_08053e38.s asm/asm_08053e3c.s asm/asm_08054474.s asm/asm_08054478.s asm/asm_08054bf0.s asm/asm_080550e4.s asm/asm_080550e8.s asm/asm_08055824.s asm/asm_08055828.s asm/asm_08055f98.s asm/asm_08055f9c.s asm/asm_080564e0.s asm/asm_080564e4.s asm/asm_08056784.s asm/asm_080580ac.s asm/asm_080580b0.s asm/asm_080597a4.s asm/asm_08059a0c.s asm/asm_08059a10.s asm/asm_0805ab28.s asm/asm_0805c2ec.s asm/asm_0805c54c.s asm/asm_0805d390.s asm/asm_0805e6ec.s asm/asm_0805e6f0.s asm/asm_0805f0f8.s asm/asm_0805f0fc.s asm/asm_0805f434.s \
    asm/asm_08061030.s asm/asm_0806707c.s asm/asm_08067080.s asm/asm_080676ec.s asm/asm_080676f0.s asm/asm_0806827c.s asm/asm_08068280.s asm/asm_08068438.s asm/asm_0806911c.s asm/asm_08069120.s asm/asm_0806983c.s asm/asm_08069840.s asm/asm_0806a924.s asm/asm_0806b6ec.s asm/asm_0806b6f0.s asm/asm_0806b998.s asm/asm_0806e0f0.s asm/asm_0806e0f4.s asm/asm_0806e5e0.s asm/asm_0806e5e4.s asm/asm_0806f3a4.s asm/asm_0806f3a8.s asm/asm_0806fc28.s asm/asm_0806fc2c.s asm/asm_0806fe1c.s asm/asm_0807584c.s asm/asm_08075850.s asm/asm_08076fac.s asm/asm_08077170.s asm/asm_08079554.s asm/asm_08079558.s asm/asm_0807a2b8.s asm/asm_0807ab40.s asm/asm_0807ab44.s asm/asm_0807b9d8.s asm/asm_0807be28.s asm/asm_0807be2c.s asm/asm_0807d010.s asm/asm_0807d508.s asm/asm_0807dec8.s asm/asm_0807decc.s asm/asm_0807e684.s asm/asm_0807ea04.s asm/asm_0807ea08.s asm/asm_0807f074.s asm/asm_080815e8.s asm/asm_08081fd0.s asm/asm_08081fd4.s \
    asm/asm_080825e0.s asm/asm_080825e4.s asm/asm_08082f14.s asm/asm_08082f18.s asm/asm_08083e30.s asm/asm_08083e34.s asm/asm_080840b0.s asm/asm_08085e4c.s asm/asm_08085e50.s asm/asm_080862c8.s asm/asm_08086544.s asm/asm_08086548.s asm/asm_08087d58.s asm/asm_08088484.s asm/asm_08088488.s asm/asm_08088560.s asm/asm_08088604.s asm/asm_08088bb4.s asm/asm_0808933c.s asm/asm_080894b4.s asm/asm_0808ebf4.s asm/asm_080903e4.s asm/asm_080903e8.s asm/asm_08091a68.s asm/asm_08091a6c.s asm/asm_08097e40.s asm/asm_08097e44.s asm/asm_08097fc8.s asm/asm_08099d34.s asm/asm_08099d38.s asm/asm_0809a8b0.s asm/asm_0809a8b4.s asm/asm_0809b27c.s asm/asm_0809ce60.s asm/asm_0809dd54.s asm/asm_0809dd58.s asm/asm_0809e4d0.s asm/asm_0809e4d4.s asm/asm_0809eb98.s asm/asm_0809eb9c.s asm/asm_0809ffa8.s asm/asm_080a0d48.s asm/asm_080a0d4c.s asm/asm_080a17bc.s asm/asm_080a17c0.s asm/asm_080a1cb4.s asm/asm_080a1cb8.s \
    asm/asm_080a2520.s asm/asm_080a2fa0.s asm/asm_080a2fa4.s asm/asm_080a430c.s asm/asm_080a4310.s asm/asm_080a4420.s asm/asm_080a550c.s asm/asm_080a5510.s asm/asm_080a5864.s asm/asm_080a5868.s asm/asm_080a6130.s asm/asm_080a6134.s asm/asm_080a7720.s asm/asm_080a7724.s asm/asm_080a7b7c.s asm/asm_080a7b80.s asm/asm_080aab5c.s asm/asm_080aac70.s asm/asm_080ab8e8.s asm/asm_080ab8ec.s asm/asm_080ac3e0.s asm/asm_080ac3e4.s asm/asm_080ac520.s asm/asm_080adb34.s asm/asm_080adb38.s asm/asm_080aeab8.s asm/asm_080aeabc.s asm/asm_080b0604.s asm/asm_080b2ba8.s asm/asm_080b37a0.s asm/asm_080b46a8.s asm/asm_080b46ac.s asm/asm_080b5890.s asm/asm_080b5894.s asm/asm_080b7798.s asm/asm_080b7b08.s asm/asm_080b7b0c.s asm/asm_080b8c48.s asm/asm_080b8c4c.s asm/asm_080b9798.s asm/asm_080b979c.s asm/asm_080ba00c.s asm/asm_080ba010.s asm/asm_080ba8dc.s asm/asm_080ba8e0.s asm/asm_080ba9d0.s asm/asm_080babd4.s \
    asm/asm_080bf184.s asm/asm_080bf88c.s asm/asm_080bf890.s asm/asm_080bfac4.s asm/asm_080bfac8.s asm/asm_080c081c.s asm/asm_080c0820.s asm/asm_080c09b8.s asm/asm_080c09bc.s asm/asm_080c1b94.s asm/asm_080c1b98.s asm/asm_080c2b1c.s asm/asm_080c2b20.s asm/asm_080c2bb8.s asm/asm_080c2bbc.s asm/asm_080c450c.s asm/asm_080c4510.s asm/asm_080c4750.s asm/asm_080c6718.s asm/asm_080c7898.s asm/asm_080c789c.s asm/asm_080c7e38.s asm/asm_080c7e3c.s asm/asm_080c8f24.s asm/asm_080c8f28.s asm/asm_080c904c.s asm/asm_080ca304.s asm/asm_080ca308.s asm/asm_080cc79c.s asm/asm_080cc7a0.s asm/asm_080cc880.s asm/asm_080cd110.s asm/asm_080ceba4.s asm/asm_080ceba8.s asm/asm_080cfa80.s asm/asm_080cfa84.s asm/asm_080d0078.s asm/asm_080d007c.s asm/asm_080d05fc.s asm/asm_080d0600.s asm/asm_080d0828.s asm/asm_080d2f60.s asm/asm_080d8e84.s asm/asm_080d8e88.s asm/asm_080d9340.s asm/asm_080d9344.s asm/asm_080d9790.s \
    asm/asm_080d9794.s asm/asm_080d99d0.s asm/asm_080d99d4.s asm/asm_080d9b08.s asm/asm_080dcd50.s asm/asm_080dd3a8.s asm/asm_080dd3ac.s asm/asm_080dd4c4.s asm/asm_080dd4c8.s asm/asm_080de584.s asm/asm_080de588.s asm/asm_080df41c.s asm/asm_080e0ed4.s asm/asm_080e0ed8.s asm/asm_080e0fa4.s asm/asm_080e26a8.s asm/asm_080e26ac.s asm/asm_080e4450.s asm/asm_080e4d0c.s asm/asm_080e4d10.s asm/asm_080e4e08.s asm/asm_080e6388.s asm/asm_080e638c.s asm/asm_080e6a48.s asm/asm_080e6a4c.s asm/asm_080e9b5c.s asm/asm_080eabfc.s asm/asm_080eac00.s asm/asm_080eb024.s asm/asm_080eb028.s asm/asm_080eb678.s asm/asm_080eb67c.s asm/asm_080ec2a0.s asm/asm_080ec2a4.s asm/asm_080ee604.s asm/asm_080f26d0.s asm/asm_080f3c78.s,$(SFILES))
BINFILES := $(foreach dir,$(BIN),$(wildcard $(dir)/*.bin)) \
			$(foreach dir,$(MUSIC),$(wildcard $(dir)/*.mid)) 
WAVFILES    :=  $(foreach dir,$(SFX),$(wildcard $(dir)/*.wav))

4BPPFILES   :=  $(filter-out $(BINFILES),$(foreach dir,$(GRAPHICS),$(wildcard $(dir)/*.4bpp)))
TILEMAPS	:=  $(foreach dir,$(GFX_DIRS),$(wildcard $(dir)/*.tilemap))
JSONFILES   :=  $(foreach dir,$(AUDIO),$(wildcard $(dir)/*.json))

CFILES := $(filter-out %.inc.c, $(CFILES))

PCMFILES       := $(addprefix $(BUILD)/,$(WAVFILES:.wav=.pcm))
OFILES_GENERATED := $(addprefix $(BUILD)/,$(addsuffix .s.o,$(JSONFILES))) \
					$(addprefix $(BUILD)/,$(addsuffix .s.o,$(4BPPFILES))) \
				    $(addprefix $(BUILD)/,$(addsuffix .s.o,$(TILEMAPS)))

OFILES_SOURCES   := $(addprefix $(BUILD)/,$(addsuffix .o,$(SFILES)))  \
                    $(addprefix $(BUILD)/,$(addsuffix .o,$(CFILES)))  \
                    $(addprefix $(BUILD)/,$(addsuffix .o,$(BINFILES)))

OFILES := $(OFILES_SOURCES) $(OFILES_GENERATED)

#---------------------------------------------------------------------------------
.PHONY: default clean distclean rebuild sha1 report
.SECONDARY:
#---------------------------------------------------------------------------------

default: $(OUTPUT).gba
	$(V)if [ "$(NONMATCHING)" = "1" ]; then \
		echo "Build succeeded!"; \
	else \
		if [ "$(shell sha1sum -t $(OUTPUT).gba)" = "$(TARGET_SHA1)  $(OUTPUT).gba" ]; then \
			echo "$(TARGET).gba: OK"; \
		else \
			echo "Build succeeded, but did not match the official ROM."; \
		fi; \
	fi

sha1:
	@sha1sum baserom.gba

#---------------------------------------------------------------------------------

clean:
	$(V)echo clean ...
	$(V)rm -fr $(BUILD)/*.o $(BUILD)/*.s $(BUILD)/*.h $(BUILD)/*.d $(OUTPUT).elf $(OUTPUT).gba

distclean:
	$(V)echo distclean ...
	$(V)rm -fr $(BUILD)

rebuild: clean default

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(BUILD_DIRS):
	$(V)echo -e "$(GREEN)Creating build directory: $(YELLOW)$@$(NO_COL)"
	$(V)mkdir -p $@

$(OUTPUT).gba: $(OUTPUT).elf
	$(V)$(OBJCOPY) --pad-to=0x800000 --gap-fill=0x00 -O binary $< $@
	$(V)echo "ROM assembled!"

$(OUTPUT).elf: $(OFILES) $(BUILD)/$(LD_SCRIPT)
	$(V)echo "Linking..."
	$(V)$(file > $(BUILD)/objlist.rsp, $(OFILES))
	$(V)$(LD) @$(BUILD)/objlist.rsp tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a \
		-T $(BUILD)/$(LD_SCRIPT) -T $(UNDEFINED_SYMS) \
		-Wl,--no-warn-rwx-segments,-z,noexecstack,--no-warn-execstack,-Map $(@:.elf=.map) \
		-nostartfiles -o $@

#---------------------------------------------------------------------------------
# Binary blobs via bin2s
# Exports: <stem>_bin[]  and  <stem>_bin_size
#---------------------------------------------------------------------------------

$(BUILD)/%.bin.o $(BUILD)/%.bin.h: %.bin | $(BUILD_DIRS)
	$(call print,Bin2s:,$<,$@)
	$(V){ bin2s -a 4 -H $(BUILD)/$<.h $<; printf '\n'; } | $(AS) -o $(BUILD)/$<.o

$(BUILD)/%.mid.o	$(BUILD)/%.mid.h :	%.mid | $(BUILD_DIRS)
	$(call print,Copying MIDI file:,$<,$@)
	$(V){ bin2s -a 4 -H $(BUILD)/$<.h $<; printf '\n'; } | $(AS) -o $(BUILD)/$<.o


# WAV files
$(BUILD)/%.pcm : %.wav | $(BUILD_DIRS)
	$(call print,Converting WAV file to raw PCM audio:,$<,$@)
	$(V)python3 tools/wav_to_pcm.py $< $@

#---------------------------------------------------------------------------------
# C files (agbcc pipeline: cpp -> agbcc -> as)
#---------------------------------------------------------------------------------

define build_c_file
	$(call print,Compiling:,$<,$@)
	$(V)$(CPP) -MMD -MF $(BUILD)/$*.d -MT $@ $(CPPFLAGS) $< -o $(BUILD)/$*.i
	$(V)$(CC1) $(CFLAGS) $(BUILD)/$*.i -o $(BUILD)/$*.s
	$(V)printf ".text\n\t.align\t2, 0\n" >> $(BUILD)/$*.s
	$(V)printf ".section .note.GNU-stack,\"\",%%progbits\n" >> $(BUILD)/$*.s
	$(V)$(AS) -march=armv4t -o $@ $(BUILD)/$*.s
endef

$(BUILD)/%.c.o: %.c | $(BUILD_DIRS)
	$(call build_c_file)

#---------------------------------------------------------------------------------
# ASM
#---------------------------------------------------------------------------------

$(BUILD)/%.s.o: %.s | $(BUILD_DIRS)
	$(call print,Assembling:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -x assembler-with-cpp $< -o $(BUILD)/$*.s
	$(V)$(AS) -MD $(BUILD)/$*.d -march=armv4t -o $@ $(BUILD)/$*.s

# Beatscript
$(BUILD)/%.bs.o : %.bs | $(BUILD_DIRS)
	$(call print,Assembling Beatscript:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -x assembler-with-cpp $< -o $(BUILD)/$*.bs
	$(V)$(AS) -MD $(BUILD)/$*.d -march=armv4t -o $@ $(BUILD)/$*.bs

$(BUILD)/%.json.s : %.json $(PCMFILES) tools/sample_parser.py | $(BUILD_DIRS)
	$(call print,Generating data table from JSON:,$<,$@)
	$(V)python3 tools/sample_parser.py $< $@

$(BUILD)/%.4bpp.s : %.4bpp | $(BUILD_DIRS)
	$(call print,Compressing graphics:,$<,$@)
	$(V)python3 tools/compression.py $< $@

$(BUILD)/%.tilemap.s : %.tilemap | $(BUILD_DIRS)
	$(call print,Compressing tilemap:,$<,$@)
	$(V)python3 tools/compression.py $< $@

$(OFILES_GENERATED): $(BUILD)/%.s.o : $(BUILD)/%.s | $(BUILD_DIRS)
	$(call print,Assembling:,$<,$@)
	$(V)$(AS) -MD $(BUILD)/$*.d -march=armv4t -o $@ $(BUILD)/$*.s

#---------------------------------------------------------------------------------
# Preprocessed linker script
#---------------------------------------------------------------------------------

$(BUILD)/$(LD_SCRIPT): $(LD_SCRIPT)
	$(call print,Preprocessing linker script:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -x c $< -o $@

-include $(addprefix $(BUILD)/,$(CFILES:.c=.d))

print-%: ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true


OBJDIFF_VERSION := 3.7.1
OBJDIFF_OS := $(shell uname -s)
OBJDIFF_ARCH := $(shell uname -m)

ifeq ($(OBJDIFF_OS),Darwin)
ifeq ($(OBJDIFF_ARCH),arm64)
OBJDIFF_ASSET := objdiff-cli-macos-arm64
else ifeq ($(OBJDIFF_ARCH),x86_64)
OBJDIFF_ASSET := objdiff-cli-macos-x86_64
endif
else ifeq ($(OBJDIFF_OS),Linux)
ifeq ($(OBJDIFF_ARCH),aarch64)
OBJDIFF_ASSET := objdiff-cli-linux-aarch64
else ifeq ($(OBJDIFF_ARCH),x86_64)
OBJDIFF_ASSET := objdiff-cli-linux-x86_64
endif
endif

ifndef OBJDIFF_ASSET
$(error Unsupported objdiff-cli platform: $(OBJDIFF_OS)/$(OBJDIFF_ARCH))
endif

OBJDIFF_CLI := tools/$(OBJDIFF_ASSET)

$(OBJDIFF_CLI):
	curl -L -o $@ https://github.com/encounter/objdiff/releases/download/v$(OBJDIFF_VERSION)/$(OBJDIFF_ASSET)
	chmod +x $@

report: $(OFILES) $(OBJDIFF_CLI)
	python3 tools/gen_objdiff.py
	$(OBJDIFF_CLI) report generate -o build/report.json