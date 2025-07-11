// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../dai/dai.h"

extern LayoutEntity* D_8D2E2B0;
extern LayoutEntity* D_8D2E384;

extern Overlay OVL_EXPORT(Overlay);

extern char OVL_EXPORT(cutscene_script_en)[];
extern char OVL_EXPORT(cutscene_script_fr)[];
extern char OVL_EXPORT(cutscene_script_sp)[];
extern char OVL_EXPORT(cutscene_script_ge)[];
extern char OVL_EXPORT(cutscene_script_it)[];

s32 E_ID(BACKGROUND_BLOCK);
s32 E_ID(LOCK_CAMERA);
s32 E_ID(UNK_13);
s32 E_ID(EXPLOSION_VARIANTS);
s32 E_ID(GREY_PUFF);
s32 E_ID(UNK_16);
s32 E_ID(UNK_17);
s32 E_ID(UNK_18);
s32 E_ID(UNK_19);
s32 E_ID(UNK_1A);
s32 E_ID(UNK_1B);
s32 E_ID(UNK_1C);
s32 E_ID(UNK_1D);
s32 E_ID(UNK_1E);
s32 E_ID(CLOUDS);
s32 E_ID(UNK_20);
s32 E_ID(UNK_21);
s32 E_ID(UNK_22);
s32 E_ID(UNK_23);
s32 E_ID(UNK_24);
s32 E_ID(UNK_25);
s32 E_ID(UNK_26);
s32 E_ID(UNK_27);
s32 E_ID(UNK_28);
s32 E_ID(BONE_PILLAR_HEAD);
s32 E_ID(BONE_PILLAR_FIRE);
s32 E_ID(BONE_PILLAR_PIECES);
s32 E_ID(BONE_PILLAR_SPIKE_BALL);
s32 E_ID(UNK_2D);
s32 E_ID(UNK_2E);
s32 E_ID(UNK_2F);
s32 E_ID(UNK_30);
s32 E_ID(UNK_31);
s32 E_ID(UNK_32);
s32 E_ID(UNK_33);
s32 E_ID(SKELETON_PIECES);
s32 E_ID(UNK_35);
s32 E_ID(UNK_36);
s32 E_ID(BAT);
s32 E_ID(BLACK_CROW);
s32 E_ID(BLUE_CROW);
s32 E_ID(SKELERANG);
s32 E_ID(SKELERANG_BOOMERANG);
s32 E_ID(SKELERANG_UNKNOWN);
s32 E_ID(UNK_3D);
s32 E_ID(UNK_3E);
s32 E_ID(UNK_3F);
s32 E_ID(UNK_40);
s32 E_ID(UNK_41);
s32 E_ID(UNK_42);
s32 E_ID(SEALED_DOOR);
s32 E_ID(BREAKABLE_DEBRIS);
s32 E_ID(CUTSCENE);
s32 E_ID(UNK_46);
s32 E_ID(UNK_47);
s32 E_ID(MIST_DOOR);

u8* OVL_EXPORT(cutscene_script); // bss
u8* D_pspeu_092970B8;            // bss

#include "../get_lang_at.h"

void InitEntityIds(void) {
    E_ID(BACKGROUND_BLOCK) = 0x11;
    E_ID(LOCK_CAMERA) = 0x12;
    E_ID(UNK_13) = 0x13;
    E_ID(EXPLOSION_VARIANTS) = 0x14;
    E_ID(GREY_PUFF) = 0x15;
    E_ID(UNK_16) = 0x16;
    E_ID(UNK_17) = 0x17;
    E_ID(UNK_18) = 0x18;
    E_ID(UNK_19) = 0x19;
    E_ID(UNK_1A) = 0x1A;
    E_ID(UNK_1B) = 0x1B;
    E_ID(UNK_1C) = 0x1C;
    E_ID(UNK_1D) = 0x1D;
    E_ID(UNK_1E) = 0x1E;
    E_ID(CLOUDS) = 0x1F;
    E_ID(UNK_20) = 0x20;
    E_ID(UNK_21) = 0x21;
    E_ID(UNK_22) = 0x22;
    E_ID(UNK_23) = 0x23;
    E_ID(UNK_24) = 0x24;
    E_ID(UNK_25) = 0x25;
    E_ID(UNK_26) = 0x26;
    E_ID(UNK_27) = 0x27;
    E_ID(UNK_28) = 0x28;
    E_ID(BONE_PILLAR_HEAD) = 0x29;
    E_ID(BONE_PILLAR_FIRE) = 0x2A;
    E_ID(BONE_PILLAR_PIECES) = 0x2B;
    E_ID(BONE_PILLAR_SPIKE_BALL) = 0x2C;
    E_ID(UNK_2D) = 0x2D;
    E_ID(UNK_2E) = 0x2E;
    E_ID(UNK_2F) = 0x2F;
    E_ID(UNK_30) = 0x30;
    E_ID(UNK_31) = 0x31;
    E_ID(UNK_32) = 0x32;
    E_ID(UNK_33) = 0x33;
    E_ID(SKELETON_PIECES) = 0x34;
    E_ID(UNK_35) = 0x35;
    E_ID(UNK_36) = 0x36;
    E_ID(BAT) = 0x37;
    E_ID(BLACK_CROW) = 0x38;
    E_ID(BLUE_CROW) = 0x39;
    E_ID(SKELERANG) = 0x3A;
    E_ID(SKELERANG_BOOMERANG) = 0x3B;
    E_ID(SKELERANG_UNKNOWN) = 0x3C;
    E_ID(UNK_3D) = 0x3D;
    E_ID(UNK_3E) = 0x3E;
    E_ID(UNK_3F) = 0x3F;
    E_ID(UNK_40) = 0x40;
    E_ID(UNK_41) = 0x41;
    E_ID(UNK_42) = 0x42;
    E_ID(SEALED_DOOR) = 0x43;
    E_ID(BREAKABLE_DEBRIS) = 0x44;
    E_ID(CUTSCENE) = 0x45;
    E_ID(UNK_46) = 0x46;
    E_ID(UNK_47) = 0x47;
    E_ID(MIST_DOOR) = 0x48;
}

void OVL_EXPORT(Load)(void) {
    OVL_EXPORT(cutscene_script) = GetLangAt(
        0, (u8*)OVL_EXPORT(cutscene_script_en),
        (u8*)OVL_EXPORT(cutscene_script_fr),
        (u8*)OVL_EXPORT(cutscene_script_sp),
        (u8*)OVL_EXPORT(cutscene_script_ge),
        (u8*)OVL_EXPORT(cutscene_script_it));
    D_pspeu_092970B8 = GetLangAt(
        0, (u8*)OVL_EXPORT(cutscene_script_en),
        (u8*)OVL_EXPORT(cutscene_script_fr),
        (u8*)OVL_EXPORT(cutscene_script_sp),
        (u8*)OVL_EXPORT(cutscene_script_ge),
        (u8*)OVL_EXPORT(cutscene_script_it));
    InitEntityIds();
    PfnEntityUpdates = OVL_EXPORT(EntityUpdates);
    g_pStObjLayoutHorizontal = &D_8D2E2B0;
    g_pStObjLayoutVertical = &D_8D2E384;
    func_892A018();
    memcpy(&g_api, &OVL_EXPORT(Overlay), sizeof(Overlay));
}
