# PSP specific targets
build_pspeu: $(addsuffix _psp,$(PSP_EU_EXTRACT_TARGETS))

extract_pspeu: $(addprefix $(BUILD_DIR)/,$(addsuffix .ld,$(PSP_EU_EXTRACT_TARGETS)))

$(WIBO):
	wget -O $@ https://github.com/decompals/wibo/releases/download/0.6.13/wibo
	sha256sum --check $(WIBO).sha256
	chmod +x $(WIBO)
$(MWCCPSP): $(WIBO) $(BIN_DIR)/mwccpsp_219

$(MWCCGAP_APP):
	git submodule init $(MWCCGAP_DIR)
	git submodule update $(MWCCGAP_DIR)

dra_psp: $(BUILD_DIR)/dra.bin
stlib_psp: $(BUILD_DIR)/lib.bin
stno4_psp: $(BUILD_DIR)/no4.bin
stst0_psp: $(BUILD_DIR)/st0.bin
stwrp_psp: $(BUILD_DIR)/wrp.bin
tt_000_psp: $(BUILD_DIR)/tt_000.bin

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@
$(BUILD_DIR)/lib.bin: $(BUILD_DIR)/stlib.elf
	$(OBJCOPY) -O binary $< $@
$(BUILD_DIR)/no4.bin: $(BUILD_DIR)/stno4.elf
	$(OBJCOPY) -O binary $< $@
$(BUILD_DIR)/st0.bin: $(BUILD_DIR)/stst0.elf
	$(OBJCOPY) -O binary $< $@
$(BUILD_DIR)/wrp.bin: $(BUILD_DIR)/stwrp.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/dra.ld: $(CONFIG_DIR)/splat.pspeu.dra.yaml $(BASE_SYMBOLS) $(CONFIG_DIR)/symbols.pspeu.dra.txt
	$(SPLAT) $<
$(BUILD_DIR)/st%.ld: $(CONFIG_DIR)/splat.pspeu.st%.yaml $(BASE_SYMBOLS) $(CONFIG_DIR)/symbols.pspeu.st%.txt
	$(SPLAT) $<
$(BUILD_DIR)/tt_%.ld: $(CONFIG_DIR)/splat.pspeu.tt_%.yaml $(BASE_SYMBOLS) $(CONFIG_DIR)/symbols.pspeu.tt_%.txt
	$(SPLAT) $<

ST_DRA_MERGE = 624DC 628AC 6BF64 6D584 6E42C 6FDF8 704D0 84B88 8A0A4
$(BUILD_DIR)/dra.elf: $(BUILD_DIR)/dra.ld $(addprefix $(BUILD_DIR)/src/dra/,$(addsuffix .c.o,$(ST_DRA_MERGE))) $$(call list_o_files_psp,dra_psp)
	$(call link_with_deadstrip,dra,$@)
$(BUILD_DIR)/tt_%.elf: $(BUILD_DIR)/tt_%.ld $$(call list_o_files_psp,servant/tt_$$*) $(BUILD_DIR)/assets/servant/tt_%/mwo_header.bin.o
	$(call link_with_deadstrip,tt_$*,$@)

ST_LIB_MERGE = collision e_chair st_update create_entity e_red_door e_room_fg st_common prim_helpers e_bloody_zombie e_misc en_thornweed_corpseweed e_skeleton e_life_up e_flea_man e_mist_door e_flea_armor e_candle_table
$(BUILD_DIR)/stlib.elf: $(BUILD_DIR)/stlib.ld $(addprefix $(BUILD_DIR)/src/st/lib/,$(addsuffix .c.o,$(ST_LIB_MERGE))) $$(call list_o_files_psp,st/lib_psp) $(BUILD_DIR)/assets/st/lib/mwo_header.bin.o
	$(call link_with_deadstrip,stlib,$@)
ST_NO4_MERGE =
$(BUILD_DIR)/stno4.elf: $(BUILD_DIR)/stno4.ld $(addprefix $(BUILD_DIR)/src/st/no4/,$(addsuffix .c.o,$(ST_NO4_MERGE))) $$(call list_o_files_psp,st/no4_psp) $(BUILD_DIR)/assets/st/no4/mwo_header.bin.o
	$(call link_with_deadstrip,stno4,$@)
ST_ST0_MERGE = prologue_scroll title_card popup e_room_fg st_common collision e_lock_camera st_update e_red_door create_entity st_debug 2A218 e_particles e_collect prim_helpers e_bg_vortex e_misc 2805C 2A8DC
$(BUILD_DIR)/stst0.elf: $(BUILD_DIR)/stst0.ld $(addprefix $(BUILD_DIR)/src/st/st0/,$(addsuffix .c.o,$(ST_ST0_MERGE))) $$(call list_o_files_psp,st/st0_psp) $(BUILD_DIR)/assets/st/st0/mwo_header.bin.o
	$(call link_with_deadstrip,stst0,$@)
ST_WRP_MERGE = st_update e_particles e_room_fg st_common st_debug e_breakable popup warp e_red_door create_entity prim_helpers collision e_stage_name
$(BUILD_DIR)/stwrp.elf: $(BUILD_DIR)/stwrp.ld $(addprefix $(BUILD_DIR)/src/st/wrp/,$(addsuffix .c.o,$(ST_WRP_MERGE))) $$(call list_o_files_psp,st/wrp_psp) $(BUILD_DIR)/assets/st/wrp/mwo_header.bin.o
	$(call link_with_deadstrip,stwrp,$@)

# Recipes
$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -o $@ $<

# Most of PSP is compiled with -O0, except part of DRA. This block selects the proper flag.
OPT_HIGH = -O4,p #need this because otherwise the comma breaks the if-statement
# Allow override. Any file in this list will get O4.
OPT_HI_OVERRIDES = 33F0.c.o A710.c.o C0B0.c.o EC60.c.o 186E8.c.o 61F30.c.o 624DC.c.o 628AC.c.o 63C90.c.o 64EE0.c.o
OPTIMIZATION = $(if $(filter $(notdir $@),$(OPT_HI_OVERRIDES)), $(OPT_HIGH), -Op)

$(BUILD_DIR)/%.c.o: %.c $(MWCCPSP) $(MWCCGAP_APP)
	@mkdir -p $(dir $@)
	$(MWCCGAP) $< $@ --mwcc-path $(MWCCPSP) --use-wibo --wibo-path $(WIBO) --as-path $(AS) --asm-dir-prefix asm/pspeu --macro-inc-path include/macro.inc $(MWCCPSP_FLAGS) $(OPTIMIZATION)

$(BUILD_DIR)/assets/%/mwo_header.bin.o: assets/%/mwo_header.bin
	@mkdir -p $(dir $@)
	$(LD) -r -b binary -o $@ $<
