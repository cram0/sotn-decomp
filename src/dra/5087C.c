// SPDX-License-Identifier: AGPL-3.0-or-later
#include "dra.h"
#include "dra_bss.h"
#include "menu.h"
#include "servant.h" // for InitializeMode enum

// fake struct for D_801375BC
typedef struct {
    RoomLoadDef* def;
    Point32 pos;
} RoomLoadDefHolder;

extern s32 D_801375C8;
extern bool D_8C630C8;
extern s32 D_psp_091CE578;
extern void (*D_8CE9C48)();
extern s32 D_psp_091CE570;

#if defined(VERSION_PSP)
extern s32* D_8D97C40;
#define SAVE_DATA_PTR &D_8D97C40
#define CAN_PAUSE g_PauseAllowed&& D_psp_091CE578
#else
#define SAVE_DATA_PTR 0x801EA000
#define CAN_PAUSE g_PauseAllowed
#endif

// BSS
static bool D_80137598;
static s32 D_8013759C;
static s32 D_801375A0;
static s32 D_801375A4;
static s32 D_801375A8;
static s32 D_801375AC;
static s32 D_801375B0;
static s32 D_801375B4;
static s32 D_801375B8;
static RoomLoadDefHolder D_801375BC;

RoomTeleport D_800A245C[] = {
    {496, 392, 0x00A0, STAGE_NO0, STAGE_NZ0},
    {240, 648, 0x0000, STAGE_NO0, STAGE_NO2},
    {16, 132, 0x0038, STAGE_NO0, STAGE_NO1},
    {496, 132, 0x0078, STAGE_NO0, STAGE_NP3},
    {240, 132, 0x0000, STAGE_NO0, STAGE_NO4},
    {128, 36, 0x0000, STAGE_NO0, STAGE_CEN},
    {16, 1672, 0x0068, STAGE_LIB, STAGE_NO1},
    {128, 212, 0x00A8, STAGE_CEN, STAGE_NO0},
    {128, 77, 16, STAGE_RCEN, STAGE_BO6},
    {240, 388, 0x0030, STAGE_NZ1, STAGE_TOP},
    {16, 132, 16, STAGE_NZ1, STAGE_NO1},
    {16, 644, 0x0000, STAGE_NO3, STAGE_NO0},
    {16, 132, 0x00A0, STAGE_NO3, STAGE_NO4},
    {752, 132, 0x0050, STAGE_NO3, STAGE_NZ0},
    {240, 132, 0x0018, STAGE_NO3, STAGE_WRP},
    {16, 180, 0x0000, STAGE_DRE, STAGE_DRE},
    {16, 388, 0x0050, STAGE_CAT, STAGE_CHI},
    {112, 644, 0x0000, STAGE_NO3, STAGE_NO3},
    {240, 132, 0x0088, STAGE_CHI, STAGE_CAT},
    {16, 132, 128, STAGE_CHI, STAGE_NO4},
    {16, 132, 0x0020, STAGE_CHI, STAGE_WRP},
    {16, 132, 0x0078, STAGE_NZ0, STAGE_NP3},
    {16, 132, 0x0030, STAGE_NZ0, STAGE_NO0},
    {240, 132, 0x0060, STAGE_NZ0, STAGE_DAI},
    {16, 132, 0x0068, STAGE_NO2, STAGE_NO0},
    {240, 132, 0x0030, STAGE_NO2, STAGE_DAI},
    {1264, 132, 0x0030, STAGE_NO2, STAGE_ARE},
    {16, 132, 0x0008, STAGE_NO2, STAGE_WRP},
    {240, 388, 0x0038, STAGE_WRP, STAGE_TOP},
    {272, 644, 0x0068, STAGE_WRP, STAGE_NO1},
    {240, 1412, 0x0030, STAGE_WRP, STAGE_NO2},
    {16, 132, 128, STAGE_WRP, STAGE_NP3},
    {752, 132, 0x0038, STAGE_WRP, STAGE_CHI},
    {16, 132, 0x0078, STAGE_DAI, STAGE_NO2},
    {16, 132, 0x0020, STAGE_DAI, STAGE_ARE},
    {16, 132, 0x0008, STAGE_DAI, STAGE_NZ0},
    {16, 1924, 16, STAGE_DAI, STAGE_TOP},
    {320, 696, 16, STAGE_TOP, STAGE_RTOP},
    {320, 847, 16, STAGE_RTOP, STAGE_TOP},
    {16, 132, 0x0028, 0x001F, STAGE_LIB},
    {3824, 132, 0x0040, STAGE_NO1, STAGE_NO0},
    {752, 132, 0x0060, STAGE_NO1, STAGE_LIB},
    {240, 132, 16, STAGE_NO1, STAGE_WRP},
    {240, 132, 0x0058, STAGE_NO1, STAGE_NZ1},
    {896, 952, 16, STAGE_TOP, STAGE_TOP},
    {16, 132, 0x0000, STAGE_TOP, STAGE_NZ1},
    {16, 132, 0x0000, STAGE_TOP, STAGE_WRP},
    {752, 648, 0x0088, STAGE_TOP, STAGE_DAI},
    {240, 132, 0x0050, STAGE_NO4, STAGE_NP3},
    {16, 388, 0x0088, STAGE_NO4, STAGE_NO0},
    {240, 132, 0x0000, STAGE_NO4, STAGE_CHI},
    {16, 180, 0x0000, STAGE_NO4, STAGE_DRE},
    {496, 388, 0x0048, STAGE_ARE, STAGE_DAI},
    {16, 388, 16, STAGE_ARE, STAGE_NO2},
    {16, 132, 0x0090, STAGE_NO3, STAGE_NO3}, // Entrance Forest Spawn Point
    {16, 132, 0x0000, STAGE_MAD, STAGE_MAD}, // Debug Stage
    {16, 644, 0x0000, STAGE_NP3, STAGE_NO0},
    {16, 132, 0x00A0, STAGE_NP3, STAGE_NO4},
    {752, 132, 0x0050, STAGE_NP3, STAGE_NZ0},
    {240, 132, 0x0018, STAGE_NP3, STAGE_WRP},
    {128, 164, 0x0128, STAGE_DRE, STAGE_NO4},
    {448, 1716, 0x0058, 0x0000, STAGE_DAI}, // Unused?
    {16, 132, 0x0028, 0x0000, STAGE_LIB},   // Library Card Destination
    {16, 132, 0x0038, 0x0000, STAGE_NO1},
    {16, 132, 128, 0x000C, STAGE_NZ0},
    {240, 132, 16, 0x0036, STAGE_CAT},
    {16, 132, 0x0020, 0x0036, STAGE_CAT},
    {496, 132, 0x0000, 0x0023, STAGE_BO7},
    {16, 132, 0x0058, 0x0037, STAGE_NO2},
    {752, 132, 0x0068, 0x0037, STAGE_NO2},
    {496, 132, 0x0000, 36, STAGE_MAR},
    {128, 388, 0x0000, 0x0038, STAGE_CEN},
#if defined(VERSION_US)
    {496, 132, 0x0000, 0x0001, STAGE_BO4},
#else
    {128, 77, 0x0010, 0x0028, STAGE_BO6},
#endif
    {240, 644, 0x0008, 0x0039, STAGE_NO4},
    {16, 132, 0x00E8, 0x0039, STAGE_NO4},
    {16, 132, 0x0000, 0x0029, STAGE_BO5},
    {240, 132, 0x0000, 0x0029, STAGE_BO5},
    {240, 132, 0x0050, 0x003A, STAGE_NO1},
    {16, 132, 0x0060, 0x003A, STAGE_NO1},
    {16, 132, 0x0000, 0x0021, STAGE_BO4},
    {496, 132, 0x0000, 0x0021, STAGE_BO4},
    {752, 132, 0x0018, 0x003B, STAGE_DAI},
    {16, 132, 0x0028, 0x003B, STAGE_DAI},
    {16, 132, 0x0000, 0x0026, STAGE_BO3},
    {496, 132, 0x0000, 0x0026, STAGE_BO3},
    {496, 132, 0x0008, 0x0016, STAGE_CHI},
    {16, 132, 0x0018, 0x0016, STAGE_CHI},
    {16, 132, 0x0000, 0x0005, STAGE_BO7},
    {496, 132, 0x0000, 0x0005, STAGE_BO7},
    {16, 132, 0x0018, 0x003C, STAGE_CHI},
    {496, 132, 0x0008, 0x003C, STAGE_CHI},
    {16, 132, 0x0000, 0x0025, STAGE_BO2},
    {496, 132, 0x0000, 0x0025, STAGE_BO2},
    {240, 132, 0x0058, 0x003D, STAGE_NZ0},
    {16, 644, 0x0098, 0x003D, STAGE_NZ0},
    {16, 132, 0x0000, 0x003D, STAGE_NZ0},
    {1008, 388, 0x0000, 0x002C, STAGE_BO1},
    {16, 132, 0x0000, 0x002C, STAGE_BO1},
    {752, 132, 0x0050, 0x0017, STAGE_NO0},
    {16, 132, 0x0098, 0x0017, STAGE_NO0},
    {16, 132, 0x0000, 0x0000, STAGE_MAR},
    {240, 179, 0x0000, 0x000B, STAGE_BO6},
    {752, 132, 0x0018, 0x0019, STAGE_DAI},
    {16, 132, 0x0028, 0x0019, STAGE_DAI},
    {16, 132, 0x0000, 0x0006, STAGE_BO5},
    {496, 132, 0x0000, 0x0006, STAGE_BO5},
    {240, 132, 0x0050, 0x001A, STAGE_NO1},
    {16, 132, 0x0060, 0x001A, STAGE_NO1},
    {16, 132, 0x0000, 0x0001, STAGE_BO4},
    {240, 132, 0x00C0, 0x001B, STAGE_NO4},
    {240, 644, 0x0008, 0x001B, STAGE_NO4},
    {16, 132, 0x0008, 0x0009, STAGE_BO3},
    {1264, 132, 0x0020, 0x001C, STAGE_ARE},
    {16, 132, 0x0030, 0x001C, STAGE_ARE},
    {16, 132, 0x0000, 0x000A, STAGE_BO2},
    {496, 132, 0x0000, 0x000A, STAGE_BO2},
    {240, 132, 16, 0x001D, STAGE_CAT},
    {16, 132, 0x0020, 0x001D, STAGE_CAT},
    {496, 132, 0x0000, 0x0003, STAGE_BO1},
    {16, 132, 0x0030, 0x003E, STAGE_ARE},
    {1264, 132, 0x0020, 0x003E, STAGE_ARE},
    {496, 132, 0x0000, 0x002A, STAGE_BO0},
    {16, 132, 0x0000, 0x002A, STAGE_BO0},
    {16, 132, 0x0058, 0x001E, STAGE_NO2},
    {752, 132, 0x0068, 0x001E, STAGE_NO2},
    {496, 132, 0x0000, 0x0004, STAGE_BO0},
    {16, 132, 0x0000, 0x0000, STAGE_TE1},
    {1248, 396, 0x0000, 0x0000, STAGE_ST0},
    {240, 132, 0x0000, 0x000D, STAGE_NZ1},
    {1264, 392, 0x0000, 0x0002, STAGE_LIB},
    {16, 132, 0x0000, 0x002D, STAGE_NZ1},
};

RoomBossTeleport D_800A297C[] = {
    {0x20, 0x1A, STAGE_NO0, TIMEATTACK_EVENT_FIRST_MARIA_MEET, 0x64},
    {0x00, 0x01, STAGE_DRE, 0xFF, 0x3C},
    {0x01, 0x01, STAGE_DRE, 0xFF, 0x3C},
    {0x14, 0x10, STAGE_NO2, TIMEATTACK_EVENT_OLROX_DEFEAT, 0x7D},
    {0x13, 0x32, STAGE_CAT, TIMEATTACK_EVENT_GRANFALOON_DEFEAT, 0x76},
    {0x13, 0x16, STAGE_ARE, TIMEATTACK_EVENT_MINOTAUR_WEREWOLF_DEFEAT, 0x72},
    {0x14, 0x16, STAGE_ARE, TIMEATTACK_EVENT_MINOTAUR_WEREWOLF_DEFEAT, 0x73},
    {0x28, 0x27, STAGE_NO4, TIMEATTACK_EVENT_SCYLLA_DEFEAT, 0x6F},
    {0x39, 0x17, STAGE_NO1, TIMEATTACK_EVENT_DOPPLEGANGER_10_DEFEAT, 0x6C},
#if defined(VERSION_US)
    {0x3A, 0x17, STAGE_NO1, TIMEATTACK_EVENT_DOPPLEGANGER_10_DEFEAT, 0x48},
#endif
    {0x17, 0x0D, STAGE_DAI, TIMEATTACK_EVENT_HYPPOGRYPH_DEFEAT, 0x68},
    {0x18, 0x0D, STAGE_DAI, TIMEATTACK_EVENT_HYPPOGRYPH_DEFEAT, 0x69},
    {0x21, 0x08, STAGE_TOP, TIMEATTACK_EVENT_SAVE_RICHTER, 0x65},
    {0x1E, 0x28, STAGE_CHI, TIMEATTACK_EVENT_CERBERUS_DEFEAT, 0x57},
    {0x1F, 0x28, STAGE_CHI, TIMEATTACK_EVENT_CERBERUS_DEFEAT, 0x58},
    {0x2B, 0x29, STAGE_RARE, TIMEATTACK_EVENT_RALPH_GRANT_SYPHA_DEFEAT, 0x79},
    {0x2C, 0x29, STAGE_RARE, TIMEATTACK_EVENT_RALPH_GRANT_SYPHA_DEFEAT, 0x7A},
    {0x33, 0x23, STAGE_RNZ0, TIMEATTACK_EVENT_BEELZEBUB_DEFEAT, 0x60},
    {0x36, 0x24, STAGE_RNZ0, TIMEATTACK_EVENT_BEELZEBUB_DEFEAT, 0x61},
    {0x21, 0x17, STAGE_RCHI, TIMEATTACK_EVENT_DEATH_DEFEAT, 0x5B},
    {0x20, 0x17, STAGE_RCHI, TIMEATTACK_EVENT_DEATH_DEFEAT, 0x5C},
    {0x28, 0x32, STAGE_RDAI, TIMEATTACK_EVENT_MEDUSA_DEFEAT, 0x53},
    {0x27, 0x32, STAGE_RDAI, TIMEATTACK_EVENT_MEDUSA_DEFEAT, 0x54},
    {0x06, 0x28, STAGE_RNO1, TIMEATTACK_EVENT_THE_CREATURE_DEFEAT, 0x4F},
    {0x05, 0x28, STAGE_RNO1, TIMEATTACK_EVENT_THE_CREATURE_DEFEAT, 0x50},
    {0x17, 0x18, STAGE_RNO4, TIMEATTACK_EVENT_DOPPLEGANGER_40_DEFEAT, 0x4B},
    {0x2B, 0x2F, STAGE_RNO2, TIMEATTACK_EVENT_AKMODAN_II_DEFEAT, 0x46},
    {0x2C, 0x0D, STAGE_RCAT, TIMEATTACK_EVENT_GALAMOTH_DEFEAT, 0x43},
    {0x80, 0x00, 0x00, 0x00, 0x00},
};

s32 func_800F087C(u32 chunkX, u32 chunkY) {
    RoomBossTeleport* phi_s1;

    for (phi_s1 = &D_800A297C[0]; true; phi_s1++) {
        if (phi_s1->x == 0x80) {
            return 0;
        }
        // All must match, otherwise we jump out.
        if (phi_s1->x != chunkX || phi_s1->y != chunkY ||
            phi_s1->stageId != g_StageId) {
            continue;
        }

        if (phi_s1->eventId == TIMEATTACK_EVENT_INVALID) {
            return phi_s1->unk10 + 2;
        }
        if (TimeAttackController(phi_s1->eventId, TIMEATTACK_GET_RECORD) == 0) {
            return phi_s1->unk10 + 2;
        }
    }
}

// Performs calculations for background parallax
void func_800F0940(void) {
    switch (g_BgLayers[0].scrollKind) {
    case 1:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi;
        g_BgLayers[0].scrollY.i.hi = g_Tilemap.scrollY.i.hi;
        break;
    case 2:
        g_BgLayers[0].scrollX.i.hi = (g_Tilemap.scrollX.i.hi / 2);
        g_BgLayers[0].scrollY.i.hi = (g_Tilemap.scrollY.i.hi / 2) + 0x76;
        break;
    case 3:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi / 2;
        g_BgLayers[0].scrollY.i.hi = g_Tilemap.scrollY.i.hi;
        break;
    case 4:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi;
        g_BgLayers[0].scrollY.i.hi = g_Tilemap.scrollY.i.hi / 2;
        if (g_StageId == STAGE_RCHI) {
            g_BgLayers[0].scrollY.i.hi += 0x80;
        }
        break;
    case 5:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi / 2;
        g_BgLayers[0].scrollY.i.hi = g_Tilemap.scrollY.i.hi / 2;
        if (g_StageId == STAGE_RDAI) {
            g_BgLayers[0].scrollX.i.hi += 0x80;
        }
        break;
    case 6:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi / 2;
        g_BgLayers[0].scrollY.i.hi =
            (g_Tilemap.scrollY.i.hi / 2 - ((g_Tilemap.vSize - 1) * 128)) +
            (g_BgLayers[0].h * 128);
        if (g_StageId == STAGE_RDAI) {
            g_BgLayers[0].scrollX.i.hi += 0x80;
            g_BgLayers[0].scrollY.i.hi = g_Tilemap.scrollY.i.hi / 2;
        }
        break;
    case 7:
        g_BgLayers[0].scrollX.i.hi = g_Tilemap.scrollX.i.hi / 2;
        g_BgLayers[0].scrollY.i.hi = 4;
        break;
    default:
        g_BgLayers[0].scrollX.i.hi = 0;
        g_BgLayers[0].scrollY.i.hi = 4;
        break;
    }
}

s32 SetNextRoomToLoad(u32 x, u32 y) {
    s32 res;
    RoomHeader* room;
    // We only ever use the first one of these. Could be a fake array.
    RoomLoadDef* loader[4];
    RoomTeleport* tele;

    if (g_Player.status & PLAYER_STATUS_DEAD) {
        return 0;
    }
    res = func_800F087C(x, y);
    if (res) {
        return res;
    }
    // Look for the proper room at the xy coordinates.
    for (room = &g_api.o.rooms[0]; true; room++) {
        // Perhaps the final room gets a hard-coded value of 0x40?
        // Indicates no room found, return 0
        if (room->left == 0x40) {
            return 0;
        }
        // Now check the 4 coordinates. If x,y are beyond the room's
        // bounds, then this isn't the room we're looking for.
        if (x < room->left) {
            continue;
        }
        if (y < room->top) {
            continue;
        }
        if (x > room->right) {
            continue;
        }
        if (y > room->bottom) {
            continue;
        }
        // All 4 bounds passed. We found our room.

        // Don't know what this is testing for.
        loader[0] = &room->load;
        if (loader[0]->tilesetId == 0xFF) {
            tele = &D_800A245C[loader[0]->tileLayoutId];
            if (tele->stageId == STAGE_ST0) {
                return 0;
            }
        }
        // Finally set this to the room we found, and return a success.
        D_801375BC.def = &room->load;
        return 1;
    }
}

s32 func_800F0CD8(s32 arg0) {
    s32 var_s0;
    s32 ret;
    // Remaining vars might be fake. Not needed on PSP.
    s16 temp_a3;
    s32 temp_a0;
    s16 temp_a2;

    if (!g_unkGraphicsStruct.unk18) {
        if (D_80097C98 == 2) {
            ret = SetNextRoomToLoad(g_Tilemap.left + (PLAYER.posX.i.hi >> 8),
                                    g_Tilemap.top + (PLAYER.posY.i.hi >> 8));
            D_801375BC.pos.x = PLAYER.posX.i.hi & 0xFF;
            D_801375BC.pos.y = PLAYER.posY.i.hi & 0xFF;
            return ret;
        }
        if (arg0) {
            if (g_PlayerX < g_Tilemap.x) {
                ret = SetNextRoomToLoad(
                    g_Tilemap.left - 1, g_Tilemap.top + (g_PlayerY >> 8));
                if (ret) {
#if defined(VERSION_PSP)
                    if (PLAYER.posX.i.hi < 4) {
                        PLAYER.posX.i.hi = -1;
                        PLAYER.posX.i.lo = 0;
                    }
#endif
                    D_801375BC.pos.x = PLAYER.posX.i.hi + 256;
                    D_801375BC.pos.y = PLAYER.posY.i.hi;
                    g_Player.unk78 = 1;
                    return ret;
                } else {
                    g_PlayerX = g_Tilemap.x;
                    PLAYER.posX.i.hi = 0;
                }
            }
            if (g_PlayerX >= g_Tilemap.width) {
                ret = SetNextRoomToLoad(
                    g_Tilemap.right + 1, g_Tilemap.top + (g_PlayerY >> 8));
                if (ret) {
#if defined(VERSION_PSP)
                    if (PLAYER.posX.i.hi > 252) {
                        PLAYER.posX.i.hi = 256;
                        PLAYER.posX.i.lo = 0;
                    }
#endif
                    D_801375BC.pos.x = PLAYER.posX.i.hi - 256;
                    D_801375BC.pos.y = PLAYER.posY.i.hi;
                    g_Player.unk78 = 1;
                    return ret;
                } else {
                    g_PlayerX = g_Tilemap.width - 1;
                    PLAYER.posX.i.hi = 255;
                }
            }
        } else {
            goto block_25;
        }
    }
    if (!g_unkGraphicsStruct.unk24) {
        if (g_PlayerY < g_Tilemap.y + 4) {
            ret = SetNextRoomToLoad(
                g_Tilemap.left + (g_PlayerX >> 8), g_Tilemap.top - 1);
            if (ret) {
                D_801375BC.pos.x = PLAYER.posX.i.hi;
                D_801375BC.pos.y = PLAYER.posY.i.hi + 208;
                g_PlayerY -= 128;
                g_Player.unk78 = 2;
                return ret;
            } else {
                g_PlayerY = g_Tilemap.y + 4;
                PLAYER.posY.i.hi = 0;
            }
        }
        var_s0 = 48;
        if (!(g_Player.vram_flag & 1) &&
            !(g_Player.status &
              (PLAYER_STATUS_BAT_FORM | PLAYER_STATUS_MIST_FORM))) {
            var_s0 = 24;
        }
        if (g_PlayerY >= g_Tilemap.height - var_s0 + 20) {
            ret = SetNextRoomToLoad(
                g_Tilemap.left + (g_PlayerX >> 8), g_Tilemap.bottom + 1);
            if (ret) {
                D_801375BC.pos.x = PLAYER.posX.i.hi;
                D_801375BC.pos.y = PLAYER.posY.i.hi - (256 - var_s0);
                g_PlayerY += 0x80;
                g_Player.unk78 = 2;
                return ret;
            } else {
                g_PlayerY = g_Tilemap.height - var_s0 + 0x13;
                PLAYER.posY.i.hi = 271 - var_s0;
            }
        }
    }
block_25:
    if (g_PlayerX < g_Tilemap.x + g_unkGraphicsStruct.unkC) {
        if (arg0 && g_Tilemap.hSize != 1) {
            if (g_Tilemap.x + g_unkGraphicsStruct.unkC <
                g_PlayerX + D_801375A4) {
                PLAYER.posX.i.hi += g_PlayerX + D_801375A4 -
                                    (g_Tilemap.x + g_unkGraphicsStruct.unkC);
            }
        }
        g_Tilemap.scrollX.i.hi = g_Tilemap.x;
    } else if (g_Tilemap.width + g_unkGraphicsStruct.unkC - 256 < g_PlayerX) {
        if (arg0 && g_Tilemap.hSize != 1) {
            if (g_PlayerX + D_801375A4 <
                g_Tilemap.width + g_unkGraphicsStruct.unkC - 256) {
                PLAYER.posX.i.hi +=
                    g_PlayerX + D_801375A4 -
                    (g_Tilemap.width + g_unkGraphicsStruct.unkC - 256);
            }
        }
        g_Tilemap.scrollX.i.hi = g_Tilemap.width - 256;
    } else {
        g_Tilemap.scrollX.i.hi = g_PlayerX - g_unkGraphicsStruct.unkC;
        PLAYER.posX.i.hi = g_unkGraphicsStruct.unkC;
    }
    if (g_unkGraphicsStruct.unk1C != 0) {
        if (g_PlayerY < g_Tilemap.y + 140) {
            g_Tilemap.scrollY.i.hi = g_Tilemap.y + 4;
            PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
        } else if (g_Tilemap.height - 116 < g_PlayerY) {
            g_Tilemap.scrollY.i.hi = g_Tilemap.height - 252;
            PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
        } else {
            g_Tilemap.scrollY.i.hi = g_PlayerY - 136;
            PLAYER.posY.i.hi = 136;
        }
    } else if (g_PlayerY < g_Tilemap.y + 140) {
        if (g_Tilemap.scrollY.i.hi - (g_PlayerY - 136) >= 4 &&
            g_Tilemap.y + 8 < g_Tilemap.scrollY.i.hi) {
            g_Tilemap.scrollY.i.hi -= 4;
            PLAYER.posY.i.hi += 4;
        } else if (g_Tilemap.scrollY.i.hi < g_Tilemap.y && g_Tilemap.y != 0) {
            g_Tilemap.scrollY.i.hi += 4;
            PLAYER.posY.i.hi -= 4;
        } else {
// From here down, things get funky. There is no difference between
// what is done in these two versions, but PSP is straightforward while
// the non-psp relies on temp variables, meaningless self-assigns, and
// do-while(0). There might be some trick to unify these, but it's not
// evident so far. It's possible the real code is somewhere in between.
// Perhaps Saturn or X360 version could someday reveal the truth.
// Or just someone REALLY clever coming along ;). Good luck!
#if defined(VERSION_PSP)
            g_Tilemap.scrollY.i.hi = g_Tilemap.y + 4;
            PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
        }
    } else {
        if (g_Tilemap.height - 116 < g_PlayerY) {
            g_Tilemap.scrollY.i.hi = g_Tilemap.height - 252;
            PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
        } else if (g_Tilemap.scrollY.i.hi - (g_PlayerY - 136) >= 4) {
            g_Tilemap.scrollY.i.hi = g_Tilemap.scrollY.i.hi - 4;
            PLAYER.posY.i.hi = PLAYER.posY.i.hi + 4;
        } else {
            g_Tilemap.scrollY.i.hi = g_PlayerY - 136;
            PLAYER.posY.i.hi = 136;
        }
    }
#else
            temp_a2 = g_Tilemap.y + 4;
            g_Tilemap.scrollY.i.hi = temp_a2;
            PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
        }
    } else {
        temp_a2 = g_Tilemap.scrollY.i.hi;
        temp_a3 = PLAYER.posY.i.hi;
        g_Tilemap.scrollY.i = g_Tilemap.scrollY.i;
        temp_a0 = temp_a2 << 16;
        if (g_Tilemap.height - 116 < g_PlayerY) {
            do {
                g_Tilemap.scrollY.i.hi = g_Tilemap.height - 252;
                PLAYER.posY.i.hi = g_PlayerY - g_Tilemap.scrollY.i.hi;
            } while (0);
        } else if ((temp_a0 >> 16) - (g_PlayerY - 136) >= 4) {
            g_Tilemap.scrollY.i.hi = temp_a2 - 4;
            PLAYER.posY.i.hi = temp_a3 + 4;
        } else {
            g_Tilemap.scrollY.i.hi = g_PlayerY - 136;
            PLAYER.posY.i.hi = 136;
        }
    }
#endif
    return 0;
}

// Stripped on PSP; only called in HD.
void func_800F1424(void) {
    if (g_pads[1].tapped & PAD_R1) {
        g_Tilemap.flags ^= 2;
    }
    if (g_pads[1].tapped & PAD_L1) {
        g_Tilemap.flags ^= 1;
    }
    if ((g_pads[1].tapped & PAD_L2) && (g_BgLayers[0].tileDef != 0)) {
        g_BgLayers[0].flags ^= 1;
    }
}

void func_800F14CC(void) {
    RoomTeleport* temp_a2;
    s32 temp_a1;
    s32 newY;

    if (D_8003C730 == 1) {
        PLAYER.posX.i.hi = 0x80;
        PLAYER.posY.i.hi = 0xA4;
        if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
            PLAYER.posY.i.hi += 0x10;
        }
        SetNextRoomToLoad(g_Tilemap.left, g_Tilemap.top);
        return;
    }
    if (D_8003C730 == 2) {
        PLAYER.posX.i.hi = 0x74;
        PLAYER.posY.i.hi = 0xA4;
        SetNextRoomToLoad(g_Tilemap.left, g_Tilemap.top);
        return;
    }
    temp_a2 = &D_800A245C[D_8006C374];

    // TODO: !FAKE Ugly casts, need to work this out.
    D_801375BC.def =
        &((RoomHeader*)((u8*)g_api.o.rooms + temp_a2->roomId))->load;
    PLAYER.posX.i.hi = temp_a2->x;
    PLAYER.posY.i.hi = temp_a2->y;

    if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
        temp_a1 =
            (D_801375BC.def - 1)->objLayoutId - (D_801375BC.def - 1)->tilesetId;
        PLAYER.posX.i.hi =
            (((D_801375BC.def - 1)->objGfxId -
              (D_801375BC.def - 1)->tileLayoutId + 1)
             << 8) -
            temp_a2->x;
        newY = (temp_a1 << 8) - (temp_a2->y & 0xFF00);
        if (D_80097C98 == 4) {
            newY |= 0x47;
        } else if (g_StageId == STAGE_RCEN) {
            newY |= 0xD0;
        } else if ((g_StageId == STAGE_RNO0) && (D_8006C374 == 7)) {
            newY |= 0x30;
        } else if (D_8006C374 == 8) {
            newY = 0xB3;
        } else if (newY == 0) {
            if (temp_a1 != 0) {
                newY = 0x88;
            } else {
                newY = 0x84;
            }
        } else if (newY == temp_a1) {
            newY |= 0x84;
        } else {
            newY |= 0x88;
        }
        PLAYER.posY.i.hi = newY;
    }
}

s32 func_800F16D0(void) {
    s32 stageId;
    RoomTeleport* tele;

    if (D_8003C730 != 0)
        return g_StageId;
    else if (D_80097C98 == 4)
        return STAGE_TOP | STAGE_INVERTEDCASTLE_FLAG;
    else if (D_80097C98 == 5)
        return STAGE_TOP;
    else if (D_80097C98 == 6)
        return STAGE_LIB;
    else {
        tele = &D_800A245C[D_8006C374];
        stageId = tele->stageId;
        if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
            stageId ^= STAGE_INVERTEDCASTLE_FLAG;
        }
        return stageId;
    }
}

static void writeColorToBuffer(
    u8* buffer, s32 offsetX, s32 offsetY, s32 colorIndex) {
    // Pixels are stored 2 per byte
    s32 bufferIndex = (offsetX / PIXELS_PER_BYTE) + (offsetY * 4);

    if (!(offsetX & 1)) {
        buffer[bufferIndex] = (buffer[bufferIndex] & 0xF0) + colorIndex;
    } else {
        buffer[bufferIndex] = (buffer[bufferIndex] & 0xF) + (colorIndex << 4);
    }
}

static s32 readColorFromBuffer(u8* buffer, s32 offsetX, s32 offsetY) {
    s32 bufferIndex = (offsetX / PIXELS_PER_BYTE) + (offsetY * 4);

    if (!(offsetX & 1)) {
        return buffer[bufferIndex] & 0xF;
    } else {
        return (buffer[bufferIndex] & 0xF0) >> 4;
    }
}

static void copyMapTileColorsToBuffer(s32 tileX, s32 tileY, u8* buffer) {
    s32 offsetY, offsetX;
    u8* start;

    start = CASTLE_MAP_PTR;
    start += tileX * WIDTH_OF_MAP_TILE_IN_BYTES;
    start += tileY * (HEIGHT_OF_MAP_TILE_IN_PIXELS * WIDTH_OF_MAP_ROW_IN_BYTES);
    for (offsetY = 0; offsetY < 5; offsetY++) {
        for (offsetX = 0; offsetX < 4; offsetX++) {
            buffer[4 * offsetY + offsetX] =
                (start + offsetY * WIDTH_OF_MAP_ROW_IN_BYTES)[offsetX];
        }
    }
}

static void copyBufferToMapTile(s32 tileX, s32 tileY, u8* buffer) {
    s32 offsetY, offsetX;
    u8* start;

    start = CASTLE_MAP_PTR;
    start += tileX * WIDTH_OF_MAP_TILE_IN_BYTES;
    start += tileY * (HEIGHT_OF_MAP_TILE_IN_PIXELS * WIDTH_OF_MAP_ROW_IN_BYTES);
    for (offsetY = 0; offsetY < 5; offsetY++) {
        for (offsetX = 0; offsetX < 4; offsetX++) {
            (start + offsetY * WIDTH_OF_MAP_ROW_IN_BYTES)[offsetX] =
                buffer[(4 * offsetY) + offsetX];
        }
    }
}

void resetMapTileOnCastleBlueprint(s32 tileX, s32 tileY) {
    u8 buffer[4 * 5];
    s32 offsetY, offsetX;

    copyMapTileColorsToBuffer(tileX, tileY, buffer);
    for (offsetY = 0; offsetY < 4; offsetY++) {
        for (offsetX = 0; offsetX < 5; offsetX++) {
            writeColorToBuffer(buffer, offsetX, offsetY, 0);
        }
    }
    copyBufferToMapTile(tileX, tileY, buffer);
}

void paintMapTileOnCastleBlueprint(s32 tileX, s32 tileY, s32 colorIndex) {
    u8 buffer[4 * 5];
    s32 offsetY, offsetX;

    copyMapTileColorsToBuffer(tileX, tileY, buffer);
    for (offsetY = 0; offsetY < 4; offsetY++) {
        for (offsetX = 0; offsetX < 5; offsetX++) {
            if (colorIndex == 1 && offsetX == 0) {
                writeColorToBuffer(buffer, offsetX, offsetY, 0);
            } else if (colorIndex != 2 && offsetX == 4) {
                writeColorToBuffer(buffer, offsetX, offsetY, 0);
            } else {
                writeColorToBuffer(buffer, offsetX, offsetY, 1);
            }
        }
    }
    copyBufferToMapTile(tileX, tileY, buffer);
}

void repaintMapTilesOnCastleBlueprint(s32 invertedCastleIndicator) {
    // In the Underground Caverns, there is a vertical asymmetry in what tiles
    // are expected to be revealed by normal player movement in certain rooms.
    // Because of this asymmetry, the contours of the revealed map won't exactly
    // match between both castles. To rectify this, certain map tiles are
    // repainted on the castle blueprint to match the expected reveals for that
    // version of the castle.
    //
    // This function is called whenever a player loads a new stage, but will
    // only result in a noticeable change to the castle blueprint when the
    // player changes castles.
    if (invertedCastleIndicator == 0) {
        // Underground Caverns, Left Ferryman Route
        resetMapTileOnCastleBlueprint(14, 43);
        resetMapTileOnCastleBlueprint(16, 43);
        // Underground Caverns, Right Ferryman Route
        resetMapTileOnCastleBlueprint(43, 42);
        resetMapTileOnCastleBlueprint(44, 42);
        resetMapTileOnCastleBlueprint(45, 42);
        resetMapTileOnCastleBlueprint(48, 42);
    } else {
        // Reverse Caverns
        paintMapTileOnCastleBlueprint(14, 43, 0);
        paintMapTileOnCastleBlueprint(16, 43, 1);
        paintMapTileOnCastleBlueprint(43, 42, 2);
        paintMapTileOnCastleBlueprint(44, 42, 2);
        paintMapTileOnCastleBlueprint(45, 42, 2);
        paintMapTileOnCastleBlueprint(48, 42, 2);
    }
}

void func_800F1B08(s32 tileX, s32 tileY, s32 arg2) {
#define VramPosX 0x340
#define VramPosY 0x100
    RECT rect;
    u8 buffer1[20];
    u8 buffer2[20];
    s32 x;
    s32 y;
    s32 colorIndex;
    u8* src;
    u8* dst;
    u8* buffer;

    if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
        tileX = 0x3F - tileX;
        tileY = 0x3F - tileY;
    }
    buffer = buffer1;
    rect.x = VramPosX + tileX;
    rect.y = VramPosY + tileY * 4;
    rect.w = 2;
    rect.h = 5;
    StoreImage(&rect, (u_long*)buffer);
    DrawSync(0);
    src = CASTLE_MAP_PTR;
    src += tileX * WIDTH_OF_MAP_TILE_IN_BYTES;
    src += tileY * HEIGHT_OF_MAP_TILE_IN_PIXELS * WIDTH_OF_MAP_ROW_IN_BYTES;

    for (y = 0; y < 5; y++) {
        for (x = 0; x < 4; x++) {
            buffer2[4 * y + x] = (src + y * WIDTH_OF_MAP_ROW_IN_BYTES)[x];
        }
    }

    if (arg2 == 0) {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                colorIndex = readColorFromBuffer(buffer, x, y);
                if (colorIndex == 0 || colorIndex == 3 || colorIndex == 13) {
                    colorIndex = readColorFromBuffer(buffer2, x, y);
                    if (colorIndex == 2) {
                        colorIndex = 1;
                    }
                    if (colorIndex == 0) {
                        writeColorToBuffer(buffer, x, y, 14);
                    } else {
                        writeColorToBuffer(buffer, x, y, colorIndex);
                    }
                }
            }
        }
    } else {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                if (readColorFromBuffer(buffer, x, y) == 0) {
                    if (readColorFromBuffer(buffer2, x, y) == 0) {
                        writeColorToBuffer(buffer, x, y, 13);
                    } else {
                        writeColorToBuffer(buffer, x, y, 3);
                    }
                }
            }
        }
    }
    LoadTPage(
        (u_long*)buffer, 0, 0, VramPosX + tileX, VramPosY + tileY * 4, 8, 5);
}

void DrawSecretPassageOnMap(s32 x, s32 y, s32 direction) {
#define VramPosX 0x340
#define VramPosY 0x100
    RECT rect;
    u8 buf[20];
    u8* bitmap = buf;

    rect.x = x + VramPosX;
    rect.y = y * 4 + VramPosY;
    rect.w = 2;
    rect.h = 5;
    StoreImage(&rect, (u_long*)bitmap);
    DrawSync(0);
    if (direction == WALL_TOP) {
        writeColorToBuffer(bitmap, 2, 0, readColorFromBuffer(bitmap, 2, 1));
    }
    if (direction == WALL_LEFT) {
        writeColorToBuffer(bitmap, 0, 2, readColorFromBuffer(bitmap, 1, 2));
    }
    if (direction == WALL_BOTTOM) {
        writeColorToBuffer(bitmap, 2, 4, readColorFromBuffer(bitmap, 2, 3));
    }
    if (direction == WALL_RIGHT) {
        writeColorToBuffer(bitmap, 4, 2, readColorFromBuffer(bitmap, 3, 2));
    }
    LoadTPage((u_long*)bitmap, 0, 0, x + VramPosX, y * 4 + VramPosY, 8, 5);
}

// clang-format off
u8 D_800A2BC0[] = {
    12, 34, WALL_LEFT,   NZ0_SECRET_WALL_OPEN,    RNZ0_SECRET_WALL_OPEN,
    12, 34, WALL_BOTTOM, NZ0_SECRET_FLOOR_OPEN,   RNZ0_SECRET_CEILING_OPEN,
    32, 40, WALL_RIGHT,  CHI_DEMON_SWITCH,        RCHI_DEMON_SWITCH,
    37, 41, WALL_LEFT,   CHI_SECRET_WALL_OPEN,    RCHI_SECRET_WALL_OPEN,
    43, 11, WALL_LEFT,   NZ1_LOWER_WALL_OPEN,     RNZ1_UPPER_WALL_OPEN,
    50, 11, WALL_RIGHT,  NZ1_UPPER_WALL_OPEN,     RNZ1_LOWER_WALL_OPEN,
    11, 41, WALL_LEFT,   JEWEL_SWORD_ROOM_OPEN,   JEWEL_ROOM_OPEN,
    21, 22, WALL_BOTTOM, ARE_ELEVATOR_ACTIVATED,  RARE_ELEVATOR_ACTIVATED,
    20, 21, WALL_TOP,    ARE_SECRET_CEILING_OPEN, RARE_SECRET_FLOOR_OPEN,
    29, 22, WALL_RIGHT,  NO2_SECRET_WALL_OPEN,    RNO2_SECRET_WALL_OPEN,
    19, 19, WALL_TOP,    NO2_SECRET_CEILING_OPEN, RNO2_SECRET_FLOOR_OPEN,
    35, 8,  WALL_TOP,    TOP_SECRET_STAIRS,       RTOP_SECRET_STAIRS,
    39, 39, WALL_BOTTOM, NO4_SECRET_FLOOR_OPEN,   RNO4_SECRET_CEILING_OPEN,
    36, 27, WALL_LEFT,   NO4_SECRET_WALL_OPEN,    RNO4_SECRET_WALL_OPEN,
    32, 26, WALL_BOTTOM, CEN_OPEN,                RCEN_OPEN,
    0x00, // terminator
};
// clang-format on

void RevealSecretPassageOnMap(s32 playerTileX, s32 playerTileY, s32 flagId) {
    s32 currentTileX;
    s32 currentTileY;
    s32 passageDirection;
    s32 castleFlagId;
    s32 reverseCastleFlagId;
    u8* secretMapWallEntry;

    if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
        playerTileX = 63 - playerTileX;
        playerTileY = 63 - playerTileY;
    }
    secretMapWallEntry = D_800A2BC0;
    while (*secretMapWallEntry) {
        currentTileX = *secretMapWallEntry++;
        currentTileY = *secretMapWallEntry++;
        passageDirection = *secretMapWallEntry++;
        castleFlagId = *secretMapWallEntry++;
        reverseCastleFlagId = *secretMapWallEntry++;
        if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
            // Use the equivalent flag in Reverse Castle instead
            castleFlagId = reverseCastleFlagId;
        }
        if (castleFlagId != 0xFF) {
            if (flagId != 0xFFFF) {
                if (flagId == castleFlagId) {
                    DrawSecretPassageOnMap(
                        currentTileX, currentTileY, passageDirection);
                }
            } else {
                if (currentTileX == playerTileX &&
                    currentTileY == playerTileY &&
                    g_CastleFlags[castleFlagId]) {
                    DrawSecretPassageOnMap(
                        currentTileX, currentTileY, passageDirection);
                }
            }
        }
    }
}

void RevealSecretPassageAtPlayerPositionOnMap(s32 castleFlagId) {
    RevealSecretPassageOnMap(g_Tilemap.left + (g_PlayerX >> 8),
                             g_Tilemap.top + (g_PlayerY >> 8), castleFlagId);
}

void func_800F2014(void) {
    s32 tileX;
    s32 tileY;
    s32 subMap;
    s32 idx;

    if (g_canRevealMap && (g_StageId != STAGE_ST0)) {
        tileX = g_Tilemap.left + (g_PlayerX >> 8);
        tileY = g_Tilemap.top + (g_PlayerY >> 8);
        subMap = 1 << ((3 - (tileX & 3)) * 2);
        idx = (tileX >> 2) + (tileY * 16);
        if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
            idx += 2 * 4 * WIDTH_OF_MAP_ROW_IN_BYTES;
        }
        if (!(g_CastleMap[idx] & subMap)) {
            g_CastleMap[idx] |= subMap;
            g_RoomCount++;
            func_800F1B08(tileX, tileY, 0);
            RevealSecretPassageOnMap(tileX, tileY, 0xFFFF);
        }
    }
}

void func_800F2120(void) {
    s32 tileX;
    s32 tileY;
    s32 subMap;
    s32 idx;

    repaintMapTilesOnCastleBlueprint(g_StageId & STAGE_INVERTEDCASTLE_FLAG);
    ClearImage(&g_Vram.D_800ACDE8, 0, 0, 0);
    DrawSync(0);

    for (tileY = 0; tileY < 64; tileY++) {
        for (tileX = 0; tileX < 64; tileX++) {
            // sequence of 2 bit masks: 0xC0, 0x30, 0x0C, 0x03
            // 0b11000000, 0b110000, 0b1100, 0b11
            subMap = 3 << ((3 - (tileX & 3)) * 2);
            idx = (tileX >> 2) + (tileY * 16);

            if (g_StageId & STAGE_INVERTEDCASTLE_FLAG) {
                idx += 0x400;
            }
            // 0x55 and 0xAA are masks for even and odd bits.
            // 0x55 is 0b1010101
            if (g_CastleMap[idx] & (subMap & 0x55)) {
                func_800F1B08(tileX, tileY, 0);
                RevealSecretPassageOnMap(tileX, tileY, 0xFFFF);
                // 0xAA is 0b10101010
            } else if (g_CastleMap[idx] & (subMap & 0xAA)) {
                func_800F1B08(tileX, tileY, 1);
            }
        }
    }
}

void func_800F223C(void) {
    g_StageId ^= STAGE_INVERTEDCASTLE_FLAG;
    func_800F2120();
    g_StageId ^= STAGE_INVERTEDCASTLE_FLAG;
}

u8 D_800A2C0C[] = {
    0x00, 0x06, 0x08, 0x26, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x98, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x18, 0x98,
    0xFF, 0xF8, 0xE7, 0x07, 0x00, 0xC0, 0x18, 0xF8, 0xFF, 0x01, 0xFC, 0x07,
    0x00, 0xC0, 0xF8, 0x3F, 0x00, 0x00, 0xFC, 0x37, 0x00, 0xC0, 0x1F, 0x00,
    0x00, 0x00, 0xFC, 0x3F, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F,
    0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F,
    0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F, 0x00, 0xC4, 0x00, 0x00,
    0x00, 0x00, 0xFE, 0x3F, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x00, 0xFE, 0x3F,
    0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC0, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x7F, 0x80, 0x3F,
    0x70, 0x00, 0x18, 0x00, 0x60, 0xFF, 0xFF, 0x3F, 0x38, 0xC0, 0xFF, 0xFF,
    0xFF, 0x03, 0x00, 0x38, 0x1C, 0xDF, 0x18, 0x38, 0x80, 0x01, 0x00, 0x30,
    0xFC, 0xFF, 0x03, 0x0E, 0x80, 0x00, 0x00, 0x30, 0x00, 0x01, 0x02, 0x06,
    0x80, 0x00, 0x00, 0x30, 0x80, 0x07, 0x02, 0x06, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x0F, 0x03, 0x06, 0x80, 0x00, 0x00, 0x00, 0x00, 0x18, 0x02, 0x0E,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x02, 0x08, 0x80, 0x00, 0x00, 0x00,
    0x00, 0xF0, 0xE3, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE1, 0x07,
    0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0x07, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x8E, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x0C, 0x80, 0xBF, 0xC1,
    0xFE, 0x3F, 0x00, 0x00, 0x0C, 0x00, 0xB0, 0x01, 0x80, 0x3F, 0x00, 0x00,
    0xFC, 0xFF, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x80, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
};

u8* D_800A2D44[] = {D_800A2C0C};

// called when castle map is bought from the librarian
void func_800F2288(s32 arg0) {
    s32 width;
    s32 height;
    s32 startx;
    s32 starty;
    u8* ptrMap;
    s32 j;
    s32 bit;
    s32 x;
    s32 y;
    s32 n;

    s32 idx;
    s32 subMap;

    ptrMap = D_800A2D44[arg0];
    while (true) {

        if ((startx = *ptrMap++) == 0xFF) {
            return;
        }
        starty = *ptrMap++;
        width = *ptrMap++;
        height = *ptrMap++;

        for (y = starty; height != 0; height--, y++) {
            for (j = width, x = startx; j != 0; j--) {
                n = *ptrMap++;
                for (bit = 0; bit < 8; bit++, x++) {
                    if (n & (1 << bit)) {
                        subMap = 3 << ((3 - (x & 3)) * 2);
                        idx = (x >> 2) + (y * 16);
                        if (g_StageId & 0x20) {
                            idx += 0x400;
                        }
                        if (!(g_CastleMap[idx] & (subMap & 0xAA))) {
                            g_CastleMap[idx] |= (subMap & 0xAA);
                            func_800F1B08(x, y, 1);
                        }
                    }
                }
            }
        }
    }
}

void func_800F2404(s32 arg0) {
    s32 i;

    if (arg0 == 0) {
        g_unkGraphicsStruct.BottomCornerTextTimer = 0;
        g_unkGraphicsStruct.D_800973F8 = 0;
        g_unkGraphicsStruct.D_800973FC = 0;
    }
    g_CutsceneHasControl = 0;

    g_unkGraphicsStruct.pauseEnemies = 0;
    g_unkGraphicsStruct.unk18 = 0;
    g_unkGraphicsStruct.unk1C = 0;
    g_unkGraphicsStruct.unkC = 0x80;

    if (g_unkGraphicsStruct.BottomCornerTextTimer != 0) {
        FreePrimitives(g_unkGraphicsStruct.BottomCornerTextPrims);
    }

    g_unkGraphicsStruct.BottomCornerTextTimer = 0;
    g_unkGraphicsStruct.BottomCornerTextPrims = 0;
    g_unkGraphicsStruct.g_zEntityCenter = 148;
    for (i = 0; i < LEN(g_unkGraphicsStruct.D_80097428); i++) {
        g_unkGraphicsStruct.D_80097428[i] = 0;
    }

    g_unkGraphicsStruct.unk20 = 0;
    g_unkGraphicsStruct.unk24 = 0;
    D_80097448[0] = 0;
    D_80097448[1] = 0;
    D_80097450 = 0;
    SetGPUBuffRGBZero();
}

void func_800F24F4(void) {
    s32 x;
    s32 y;
    s32 var_a0;

    x = g_Tilemap.left + (g_PlayerX >> 8);
    y = g_Tilemap.top + (g_PlayerY >> 8);
    if (D_8003C708.flags & STAGE_INVERTEDCASTLE_FLAG) {
        if (g_StageId == STAGE_RNO0 && x == 32 && y == 36) {
            if (TimeAttackController(TIMEATTACK_EVENT_FINAL_SAVEPOINT,
                                     TIMEATTACK_GET_RECORD) == 0) {
                TimeAttackController(
                    TIMEATTACK_EVENT_FINAL_SAVEPOINT, TIMEATTACK_SET_RECORD);
            }
        }

        var_a0 = 0;
        if (g_StageId == STAGE_RNO4 && x == 18 && y == 30) {
            D_80137598 = false;
            return;
        }
        if (g_StageId == STAGE_NO4 && x == 45 && y == 33) {
            if (PLAYER.posX.i.hi == 128) {
                D_8003C730 = 1;
            } else {
                if (TimeAttackController(TIMEATTACK_EVENT_SUCCUBUS_DEFEAT,
                                         TIMEATTACK_GET_RECORD)) {
                    D_80137598 = false;
                    return;
                }
            }
            var_a0 = 1;
        }
        func_801042C4(var_a0);
        D_80137598 = true;
        func_80105428();
    } else {
        D_80137598 = false;
    }
}

void DrawMapCursor(void) {
    s32 x;
    s32 y;
    s32 tick;
    s32 cursorSize;
    OT_TYPE* gpu_ot = &g_CurrentBuffer->ot[0];
    POLY_GT4* poly = &g_CurrentBuffer->polyGT4[g_GpuUsage.gt4];

    x = g_Tilemap.left + (g_PlayerX >> 8);
    y = g_Tilemap.top + (g_PlayerY >> 8);
    x = x * 4 - 14;
    y = y * 4 - 13;
    if (g_StageId & 0x20) {
        x -= 1;
        y -= 17;
    }

    tick = g_MapCursorTimer & 0x3F;
    tick = 0x3F - tick;
    tick = SQ(tick) >> 8;
    tick = 15 - tick;
    if (tick == 0) {
        tick = 15;
    }

    x += tick;
    y += tick;
    cursorSize = 32 - (tick * 2);
    setSemiTrans(poly, true);
    setShadeTex(poly, false);
    poly->x0 = x;
    poly->y0 = y;
    poly->x1 = x + cursorSize;
    poly->y1 = y;
    poly->x2 = x;
    poly->y2 = y + cursorSize;
    poly->x3 = x + cursorSize;
    poly->y3 = y + cursorSize;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 32;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 32;
    poly->u3 = 32;
    poly->v3 = 32;
    poly->tpage = 0x1A;

    poly->clut = g_ClutIds[0x170];

    func_80107250((Primitive*)poly, tick * 0x10);
    AddPrim(&gpu_ot[OTSIZE - 1], poly);
    g_GpuUsage.gt4++;
}

bool func_800F27F4(s32 arg0) {
    if (arg0 == 0) {
        if (g_unkGraphicsStruct.D_800973FC != 0 || D_8006BB00 != 0) {
            return false;
        }
        if (D_8003C708.flags & (FLAG_UNK_40 | FLAG_UNK_20)) {
            return false;
        }
        D_801375C8 = 1;
        return true;
    }
    D_801375C8 = 8;
}

// Needs PlaySfx to have a signature of
// void PlaySfx(s32 sfxId); but changing it causes other parts
// of binary to mismatch. Removing it from the header seems
// to make it match everywhere
void func_800F2860(void) {
    s32 var_v0;

    switch (D_801375C8) {
    case 0:
        break;
    case 1:
        PlaySfx(0x80);
        D_801375C8++;
        break;
    case 2:
        if (func_80131F68() == false) {
            D_801375C8++;
            break;
        }
        break;
    case 3:
#if defined(VERSION_US)
        PlaySfx(0x33F);
#else
        PlaySfx(0x33C);
#endif
        D_801375C8++;
        break;
    case 4:
        if (func_80131F68() != false) {
            D_801375C8++;
            break;
        }
        break;
    case 5:
        if (func_80131F68() == false) {
            D_801375C8++;
            break;
        }
        break;
    case 6:
        PlaySfx(D_80097910);
        D_801375C8 = 0;
        return;
    case 7:
        if (func_80131F68() != 0) {
            return;
        }
        D_801375C8--;
        break;
    case 8:
        PlaySfx(0x80);
        D_801375C8--;
        break;
    }
}

void RunMainEngine(void) {
    Entity* ent;
    Primitive* prim;
    LayerDef* layer;
    s32 i;
    u16 ent_unk68;
    void (*playerInit)(s32 isPrologue);
    void (*RichterUpdater)(void);
    u32 tempX;
    u32 tempY;

#if defined(VERSION_PSP)
    if (D_8C630C8) {
        g_Status.hearts = 99;
    }
#endif
    // This function is a state machine, this variable is some kind of
    // overall state of the game engine
    switch (g_GameEngineStep) {
    case Engine_Init:
        if (g_IsUsingCd) {
            return;
        }
        D_8006BB00 = 0;
        D_801375C8 = 0;
        g_PauseAllowed = true;
#if defined(VERSION_PSP)
        D_psp_091CE578 = 1;
#endif
        g_StageId = func_800F16D0();
        DestroyEntitiesFromIndex(0);
        DestroyAllPrimitives();
        func_800EDAE4();
        func_800EAD7C();
        DrawHud();
        func_800F2404(0);
        if (g_StageId == STAGE_ST0 || g_PlayableCharacter != PLAYER_ALUCARD) {
            playerInit = (void*)g_PlOvl.D_8013C004;
            if (g_StageId == STAGE_ST0) {
                playerInit(1);
            } else {
                playerInit(0);
            }
        } else {
            AluInit();
        }
        if (g_StageId == STAGE_MAD) {
            g_api.o.StageEndCutScene();
        }
        g_backbufferX = 0;
        g_backbufferY = 0;
        func_800F14CC();
        LoadRoomLayer(D_801375BC.def->tileLayoutId);
        if (D_8003C708.flags & FLAG_UNK_20) {
            LoadGfxAsync(ANIMSET_DRA(3));
        }
        if (D_8003C708.flags & FLAG_UNK_40) {
            LoadGfxAsync(ANIMSET_DRA(4));
        }
        D_80097910 = g_StagesLba[g_StageId].unk18;
        if (g_StageId == STAGE_NO3 && D_8003C730 == 0) {
            D_80097910 = SE_INTRO_WIND;
        }
        if (
#if defined(VERSION_PSP)
            (D_8003C730 == 4) ||
#endif
            (D_8003C730 == 0) && !(D_8003C708.flags & FLAG_UNK_20)) {
            PlaySfx(D_80097910);
        }
        D_80097928 = 0;
        func_800EA538(2);
        if (D_801375BC.def->tilesetId != 0) {
            func_800EA5E4((D_801375BC.def->tilesetId & 0xFF) + 0x7fff | 0x4000);
        }
        if (D_801375BC.def->objGfxId != 0) {
            LoadGfxAsync((D_801375BC.def->objGfxId & 0xFF) + 0x7fff);
            D_80097904 = D_801375BC.def->objGfxId + 0x7fff;
        } else {
            D_80097904 = 0;
        }
        tempX = PLAYER.posX.i.hi;
        tempY = PLAYER.posY.i.hi;
        g_PlayerX = tempX;
        g_PlayerY = tempY;
        PLAYER.posX.i.hi = (u8)tempX;
        PLAYER.posY.i.hi = (u8)tempY;

        if (PLAYER.posX.i.hi < 0x80) {
            PLAYER.facingLeft = 0;
        }
        if (PLAYER.posX.i.hi >= 0x80) {
            PLAYER.facingLeft = 1;
        }
        g_Tilemap.x =
            ((D_801375BC.def - 1)->tileLayoutId - g_Tilemap.left) << 8;
        g_Tilemap.y = ((D_801375BC.def - 1)->tilesetId - g_Tilemap.top) << 8;
        g_Tilemap.width =
            (((D_801375BC.def - 1)->objGfxId - g_Tilemap.left) + 1) << 8;
        g_Tilemap.height =
            (((D_801375BC.def - 1)->objLayoutId - g_Tilemap.top) + 1) << 8;

        D_8009791C.x = g_Tilemap.left + (g_PlayerX >> 8);
        D_8009791C.y = g_Tilemap.top + (g_PlayerY >> 8);

        g_Tilemap.scrollX.i.hi = (D_8009791C.x - g_Tilemap.left) << 8;
        g_Tilemap.scrollY.i.hi = (D_8009791C.y - g_Tilemap.top) << 8;

        if (D_8003C730 == 2) {
            g_Tilemap.bottom -= 1;
            g_Tilemap.height -= 0x100;
            PLAYER.facingLeft = 1;
        }
        if (D_8006C374 == 0x2C) {
            g_Tilemap.y = 0x2FB;
            g_Tilemap.height = 0x3FB;
            g_StageId = STAGE_TOP;
        }
        if (D_80097C98 == 6) {
            PLAYER.facingLeft = 1;
        }
        if ((D_80097C98 == 4) || (D_80097C98 == 5) || (D_80097C98 == 6)) {
            func_800EA5AC(2, 0xFF, 0xFF, 0xFF);
        }
        func_800F0CD8(0);
        func_800F0CD8(0);
        g_PrevScrollX = (s32)g_Tilemap.scrollX.i.hi;
        g_PrevScrollY = (s32)g_Tilemap.scrollY.i.hi;
        g_api.o.InitRoomEntities(D_801375BC.def->objLayoutId);
        g_api.o.Update();
        g_api.o.Update();
        func_800F0940();
        func_801024DC();
        if ((D_80097C98 != 4) && (D_80097C98 != 5) && (D_80097C98 != 6)) {
            func_801027C4(4);
            func_801027C4(2);
        }
        D_80097C98 = 0;
        if (D_8003C730 == 1) {
            InitializeServant(ENGINE_INIT_SERVANT_2);
        } else {
            InitializeServant(ENGINE_INIT_SERVANT_0);
        }
        if (D_8003C730 == 3) {
            D_8003C730 = 0;
        }
#if defined(VERSION_PSP)
        if (D_8003C730 == 4) {
            D_8003C730 = 0;
            D_80097928 = 1;
            if (D_8006C374 == 0x25) {
                g_Tilemap.scrollX.i.hi = 0x640;
                g_Tilemap.scrollY.i.hi = 0x4BF;
                PLAYER.posX.i.hi = 0x80;
                PLAYER.posY.i.hi = 0x88;
                g_PlayerX = 0x6C0;
                g_PlayerY = 0x547;
                g_PrevScrollX = 0x640;
                g_PrevScrollY = 0x4BF;
            }
        }
#endif
        func_800F24F4();
#if defined(VERSION_PSP)
        g_unkGraphicsStruct.D_800973F8 = (s16)AllocPrimitives(PRIM_GT4, 16);
#else
        g_unkGraphicsStruct.D_800973F8 = AllocPrimitives(PRIM_GT4, 16);
#endif
        if (g_unkGraphicsStruct.D_800973F8 != 0) {
            prim = &g_PrimBuf[g_unkGraphicsStruct.D_800973F8];
            while (prim != NULL) {
                prim->tpage = 0x1A;
                prim->clut = 0x120;
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        }
        RefreshCapePalette();
        g_GameEngineStep++; // Move to Engine_Normal
#if defined(VERSION_PSP)
        func_psp_090DFBD0();
#endif
        return;
    case Engine_Normal:
        g_GameTimer++;
#if defined(VERSION_HD)
        func_800F1424();
#endif
        func_800F2014();
        g_ScrollDeltaX = g_Tilemap.scrollX.i.hi - g_PrevScrollX;
        g_ScrollDeltaY = g_Tilemap.scrollY.i.hi - g_PrevScrollY;
        g_PrevScrollX = g_Tilemap.scrollX.i.hi;
        g_PrevScrollY = g_Tilemap.scrollY.i.hi;

        g_api.o.HitDetection();
        D_8013759C = PLAYER.posX.i.hi;
        g_Player.unk7C = PLAYER.posX.i.hi;
        D_801375A0 = PLAYER.posY.i.hi;
        g_Player.unk7E = PLAYER.posY.i.hi;
        D_80097488.x.val = 0;
        D_80097488.y.val = 0;
        if (g_StageId == STAGE_ST0 || g_PlayableCharacter != PLAYER_ALUCARD) {
            g_PlOvl.D_8013C000();
            g_PlOvl.D_8013C008();
        } else {
            EntityAlucard();
            UpdatePlayerEntities();
        }
        g_api.o.UpdateRoomPosition();
        g_api.o.Update();

#if !defined(VERSION_HD)
        if (g_GameState == Game_Ending) {
            if (D_80097C98 == 6) {
                D_80097C98 = 0;
            }
            break;
        }
#endif
        func_800F2860();
        if (g_DemoMode == Demo_End) {
            g_DemoMode = Demo_None;
            D_80097C98 = 0x08000000;
            LoadSaveData(SAVE_DATA_PTR);
            D_8003C730 = 2;
            g_GameStep = Play_PrepareNextStage;
            return;
        }
        if (D_80097C98 & 0x80000000) {
            PlaySfx(SET_STOP_SEQ);
            StoreSaveData(SAVE_DATA_PTR, 0, 0);
            g_GameStep = Play_PrepareNextStage;
            return;
        }
        if ((D_80097C98 == 4) || (D_80097C98 == 5) || (D_80097C98 == 6)) {
            PlaySfx(SET_STOP_SEQ);
            func_800EA5AC(0xFFFF, 0xFF, 0xFF, 0xFF);
        }
        if (D_80097C98 == 4) {
            func_800F223C();
            D_8006C374 = 0x25;
            g_GameStep = Play_PrepareNextStage;
            return;
        } else if (D_80097C98 == 5) {
            func_800F223C();
            D_8006C374 = 0x26;
            g_GameStep = Play_PrepareNextStage;
            return;
        } else if (D_80097C98 == 6) {
            g_StageId = 0x22;
            func_800F223C();
            D_8006C374 = 0x27;
            g_GameStep = Play_PrepareNextStage;
            return;
        } else if (D_80097C98 == 1) {
            PLAYER.posX.i.hi += 0x100;
        }
        D_801375A4 = D_8013759C - PLAYER.posX.i.hi;
        D_801375A8 = D_801375A0 - PLAYER.posY.i.hi;
        g_PlayerX -= D_801375A4;
        g_PlayerY -= D_801375A8;
        D_8013759C = PLAYER.posX.val;
        D_801375A0 = PLAYER.posY.val;
        D_801375AC = g_Tilemap.scrollX.i.hi;
        D_801375B0 = g_Tilemap.scrollY.i.hi;

        if (g_unkGraphicsStruct.unk20 != 0) {
            func_8010E0D0(g_unkGraphicsStruct.unk20);
            PlaySfx(SET_PAUSE_SFX_SCRIPTS);
            g_GameEngineStep = Engine_5;
            return;
        }
        i = func_800F0CD8(1);
        if (i != 0) {
            func_801027A4();
            if (i > 1) {
                D_8006C374 = i - 2;
                g_GameStep = Play_PrepareNextStage;
                return;
            }
            if (D_801375BC.def->tilesetId == 0xFF) {
                D_8006C374 = D_801375BC.def->tileLayoutId;
                g_GameStep = Play_PrepareNextStage;
                return;
            }
#if defined(VERSION_PSP)
            func_8932B50();
#endif
            g_GameEngineStep = Engine_3;
            g_MenuStep = MENU_STEP_INIT;
            return;
        }
        D_801375B4 = D_801375AC - g_Tilemap.scrollX.i.hi;
        D_801375B8 = D_801375B0 - g_Tilemap.scrollY.i.hi;
        D_801375A4 = D_8013759C - PLAYER.posX.val;
        D_801375A8 = D_801375A0 - PLAYER.posY.val;
        D_801375A4 -= D_80097488.x.val;
        D_801375A8 -= D_80097488.y.val;
        func_800F0940();

        for (i = 0, ent = g_Entities; i < TOTAL_ENTITY_COUNT; i++, ent++) {
            ent_unk68 = ent->unk68;
            if (ent_unk68) {
                ent->posX.val += D_801375B4 * ent_unk68 * 0x100;
                ent->posY.val += D_801375B8 * ent_unk68 * 0x100;
            } else {
                if (ent->flags & FLAG_POS_CAMERA_LOCKED) {
                    ent->posX.i.hi += D_801375B4;
                    ent->posY.i.hi += D_801375B8;
                }
                if (ent->flags & FLAG_POS_PLAYER_LOCKED) {
                    ent->posX.val -= D_801375A4;
                    ent->posY.val -= D_801375A8;
                }
            }
        }
        func_80121F14(D_801375B4 << 0x10, D_801375B8 << 0x10);
        D_801375A4 = D_801375A4 >> 16;
        D_801375A8 = D_801375A8 >> 16;
        // Note: g_PrimBuf is MAX_PRIM_COUNT=1280 total in size.
        for (i = 0, prim = &g_PrimBuf[0]; i < 1024; i++, prim++) {
            if (prim->drawMode & DRAW_UNK02) {
                switch (prim->type & 0xf) {
                case PRIM_G4:
                case PRIM_GT4:
                    prim->x3 += D_801375B4;
                    prim->y3 += D_801375B8;
                case PRIM_GT3:
                    prim->x2 += D_801375B4;
                    prim->y2 += D_801375B8;
                case PRIM_LINE_G2:
                    prim->x1 += D_801375B4;
                    prim->y1 += D_801375B8;
                case PRIM_TILE:
                case PRIM_SPRT:
                    prim->x0 += D_801375B4;
                    prim->y0 += D_801375B8;
                case PRIM_NONE:
                case PRIM_ENV:
                    break;
                }
            } else if (prim->drawMode & DRAW_UNK_200) {
                switch (prim->type & 0xf) {
                case PRIM_G4:
                case PRIM_GT4:
                    prim->x3 -= D_801375A4;
                    prim->y3 -= D_801375A8;
                case PRIM_GT3:
                    prim->x2 -= D_801375A4;
                    prim->y2 -= D_801375A8;
                case PRIM_LINE_G2:
                    prim->x1 -= D_801375A4;
                    prim->y1 -= D_801375A8;
                case PRIM_TILE:
                case PRIM_SPRT:
                    prim->x0 -= D_801375A4;
                    prim->y0 -= D_801375A8;
                case PRIM_NONE:
                case PRIM_ENV:
                    break;
                }
            }
        }
        func_80102D70();
        func_801028AC(0);
        DrawHudSubweapon();
        func_800E414C();
        if (D_80137598) {
            func_80105428();
        }
        if (g_Player.status & PLAYER_STATUS_UNK80000) {
            g_GameEngineStep = Engine_10;
            g_MenuStep = MENU_STEP_INIT;
        }
        if (g_unkGraphicsStruct.D_800973FC != 0 && D_8006BB00 == 0) {
            D_8006BB00 = 1;
            MuteCd();
        } else if (g_unkGraphicsStruct.D_800973FC == 0 && D_8006BB00 != 0) {
            if (g_unkGraphicsStruct.pauseEnemies != 0) {
                D_80097928 = 0;
                D_8006BB00 = 0;
                UnMuteCd();
            } else if (D_80097928 == 0) {
                D_8006BB00 = 0;
                UnMuteCd();
            } else {
                UnMuteCd();
                if (!(D_8003C708.flags & (FLAG_UNK_40 | FLAG_UNK_20))) {
                    PlaySfx(D_80097910);
                    if (D_80097910 != 0) {
                        D_80097928 = 0;
                    }
                }
                D_8006BB00 = 0;
            }
        }
#if defined(VERSION_PSP)
        D_psp_091CE570 = 0;
#endif
        if (g_Player.status & PLAYER_STATUS_DEAD) {
            break;
        }
        if ((g_pads[0].tapped & PAD_START) && CAN_PAUSE) {
            func_801027A4();
            if ((g_StageId == STAGE_ST0) ||
                (g_PlayableCharacter != PLAYER_ALUCARD)) {
#if !defined(VERSION_PSP)
                if (!func_8010183C(0)) {
                    return;
                }
#endif
                D_800974A4 = 1;
                g_GameEngineStep = Engine_0x70;
#if defined(VERSION_PSP)
                g_MenuStep = MENU_STEP_INIT;
#endif
                PlaySfx(SET_RELEASE_RATE_LOW_22_23);
                PlaySfx(SET_RELEASE_RATE_LOW_20_21);
                PlaySfx(SET_PAUSE_SFX_SCRIPTS);
#if defined(VERSION_US)
                if (g_StageId == STAGE_ST0) {
                    MuteCd();
                }
#endif
            } else {
                PlaySfx(SET_RELEASE_RATE_LOW_22_23);
                PlaySfx(SET_RELEASE_RATE_LOW_20_21);
                PlaySfx(SET_PAUSE_SFX_SCRIPTS);
                func_801027C4(1);
                g_GameEngineStep++; // Goes from 1 to 2, into Engine_Menu
                g_MenuStep = MENU_STEP_INIT;
#if defined(VERSION_PSP)
                D_psp_091CE570 = 1;
#endif
            }
            break;
        }
        if ((g_pads[0].tapped & PAD_SELECT) && (g_StageId != STAGE_ST0) &&
            CAN_PAUSE) {
            func_801027C4(6);
            D_800974A4 = 1;
            g_GameEngineStep = Engine_Map;
        }
        break;
    case Engine_0x70:
#if defined(VERSION_PSP)
        DrawHudSubweapon2();
#else
        DrawHudSubweapon();
        if ((g_pads[0].tapped & PAD_START) && func_8010183C(1)) {
            D_800974A4 = 0;
            g_GameEngineStep = Engine_Normal;
            PlaySfx(SET_UNPAUSE_SFX_SCRIPTS);
            PlaySfx(SET_KEY_ON_20_21);
            PlaySfx(SET_KEY_ON_22_23);
#if defined(VERSION_US)
            if (g_StageId == STAGE_ST0) {
                UnMuteCd();
            }
#endif
        }
#endif
        func_801028AC(1);
        break;
    case Engine_Menu:
        MenuHandle();
        func_801028AC(1);
        break;
    case Engine_Map:
        if (g_canRevealMap) {
            DrawMapCursor();
        }
        if (g_pads[0].tapped & (PAD_START | PAD_SELECT)) {
            func_801027C4(7);
            D_800974A4 = 0;
            g_GameEngineStep = Engine_Normal;
        }
        g_api.o.UpdateStageEntities();
        func_80102D70();
        func_801028AC(1);
        break;
    case Engine_3:
        switch (g_MenuStep) {
        case MENU_STEP_INIT:
            if (D_80097C98 == 2) {
                func_800EA5AC(0xFF, 0xFF, 0xFF, 0xFF);
                g_MenuStep = MENU_STEP_EXIT_BEGIN;
            } else {
                func_801027C4(1);
                g_MenuStep++;
            }
            break;
        case MENU_STEP_FADE_FROM_GAME:
            if (func_801025F4()) {
                g_MenuStep++;
            }
            break;
        case MENU_STEP_OPENING:
            g_MenuStep++;
            break;
        case MENU_STEP_EXIT_BEGIN:
            if (g_IsUsingCd) {
                if (D_80097C98 == 2) {
                    func_800EA5AC(0xFF, 0xFF, 0xFF, 0xFF);
                }
                break;
            } else if (D_80097928 != 0) {
                D_80097910 = g_StagesLba[g_StageId].unk18;
                if (g_unkGraphicsStruct.D_800973FC != 1) {
                    PlaySfx(SET_STOP_MUSIC);
                    if (func_80131F68()) {
                        break;
                    }
                    PlaySfx(D_80097910);
                    D_80097928 = 0;
                }
            }
            if (D_80137598) {
                func_8010427C();
                D_80137598 = false;
            }
            layer = g_api.o.tileLayers[D_801375BC.def->tileLayoutId].fg;
            if (layer->rect.params & 0x80) {
                D_801375BC.def = &g_api.o.rooms[layer->rect.params & 0x7F].load;
                D_8009791C.x = layer->zPriority;
                D_8009791C.y = layer->flags;
            } else {
                D_8009791C.x = g_Tilemap.left + (g_PlayerX >> 8);
                D_8009791C.y = g_Tilemap.top + (g_PlayerY >> 8);
            }
            D_8013759C = PLAYER.posX.val;
            D_801375A0 = PLAYER.posY.val;
            PLAYER.posX.i.hi = D_801375BC.pos.x + g_Tilemap.scrollX.i.hi;
            PLAYER.posY.i.hi = D_801375BC.pos.y + g_Tilemap.scrollY.i.hi;
            if (D_8003C708.flags & (FLAG_UNK_40 | FLAG_UNK_20)) {
                LoadGfxAsync(ANIMSET_DRA(1));
            }
            func_800EA538(2);
            if (D_801375BC.def->tilesetId != 0) {
                func_800EA5E4(
                    ((D_801375BC.def->tilesetId & 0xFF) + 0x7FFF) | 0x4000);
            }
            if (D_801375BC.def->objGfxId != 0) {
                LoadGfxAsync((D_801375BC.def->objGfxId & 0xFF) + 0x7FFF);
                D_80097904 = D_801375BC.def->objGfxId + 0x7FFF;
            } else {
                D_80097904 = 0;
            }
            func_800F2404(1);
            PLAYER.posY.i.hi = (u8)PLAYER.posY.i.hi;
            PLAYER.posX.i.hi = (u8)PLAYER.posX.i.hi;
            LoadRoomLayer(D_801375BC.def->tileLayoutId);
            if (D_8003C708.flags & FLAG_UNK_20) {
                LoadGfxAsync(ANIMSET_DRA(3));
            }
            if (D_8003C708.flags & FLAG_UNK_40) {
                LoadGfxAsync(ANIMSET_DRA(4));
            }
            g_Tilemap.x =
                ((D_801375BC.def - 1)->tileLayoutId - g_Tilemap.left) << 8;
            g_Tilemap.y =
                ((D_801375BC.def - 1)->tilesetId - g_Tilemap.top) << 8;
            g_Tilemap.width =
                (((D_801375BC.def - 1)->objGfxId - g_Tilemap.left) + 1) << 8;
            g_Tilemap.height =
                (((D_801375BC.def - 1)->objLayoutId - g_Tilemap.top) + 1) << 8;
            g_Tilemap.scrollX.i.hi = (D_8009791C.x - g_Tilemap.left) << 8;
            g_Tilemap.scrollY.i.hi = (D_8009791C.y - g_Tilemap.top) << 8;
            g_PlayerX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
            g_PlayerY = PLAYER.posY.i.hi + g_Tilemap.scrollY.i.hi;
            func_8011A9D8();
            PLAYER.zPriority = g_unkGraphicsStruct.g_zEntityCenter;
            func_800F0CD8(0);
            func_8010BFFC();
            g_PlayerX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
            g_PlayerY = PLAYER.posY.i.hi + g_Tilemap.scrollY.i.hi;
            func_800F0CD8(0);
            if (g_StageId == STAGE_RTOP) {
                DestroyEntitiesFromIndex(0x40);
                for (i = 0; i < LEN(g_unkGraphicsStruct.D_80097428); i++) {
                    g_unkGraphicsStruct.D_80097428[i] = 0;
                }
                g_PrevScrollX = (s32)g_Tilemap.scrollX.i.hi;
                g_PrevScrollY = (s32)g_Tilemap.scrollY.i.hi;
                g_api.o.InitRoomEntities(D_801375BC.def->objLayoutId);
                g_api.o.Update();
                func_800F0CD8(0);
                func_800F0CD8(0);
                DestroyEntitiesFromIndex(0x40);
                for (i = 0; i < LEN(g_unkGraphicsStruct.D_80097428); i++) {
                    g_unkGraphicsStruct.D_80097428[i] = 0;
                }
            }
            D_801375A4 = D_8013759C - PLAYER.posX.val;
            D_801375A8 = D_801375A0 - PLAYER.posY.val;
            for (i = 0, ent = g_Entities; i < TOTAL_ENTITY_COUNT; i++, ent++) {
                if (ent->flags & FLAG_UNK_20000) {
                    ent->posX.val -= D_801375A4;
                    ent->posY.val -= D_801375A8;
                }
            }
            i = 0;
            func_80121F14(-D_801375A4, -D_801375A8);
            D_801375A4 = D_801375A4 >> 16;
            D_801375A8 = D_801375A8 >> 16;
            // Note: g_PrimBuf is MAX_PRIM_COUNT=1280 total in size.
            for (i = 0, prim = &g_PrimBuf[0]; i < 1024; i++, prim++) {
                if (prim->drawMode & DRAW_UNK_100) {
                    switch (prim->type & 0xf) {
                    case PRIM_G4:
                    case PRIM_GT4:
                        prim->x3 -= D_801375A4;
                        prim->y3 -= D_801375A8;
                    case PRIM_GT3:
                        prim->x2 -= D_801375A4;
                        prim->y2 -= D_801375A8;
                    case PRIM_LINE_G2:
                        prim->x1 -= D_801375A4;
                        prim->y1 -= D_801375A8;
                    case PRIM_TILE:
                    case PRIM_SPRT:
                        prim->x0 -= D_801375A4;
                        prim->y0 -= D_801375A8;
                    case PRIM_NONE:
                    case PRIM_ENV:
                        break;
                    }
                }
            }
            if (g_StageId != STAGE_RTOP) {
                DestroyEntitiesFromIndex(0x40);
                for (i = 0; i < LEN(g_unkGraphicsStruct.D_80097428); i++) {
                    g_unkGraphicsStruct.D_80097428[i] = 0;
                }
                g_PrevScrollX = g_Tilemap.scrollX.i.hi;
                g_PrevScrollY = g_Tilemap.scrollY.i.hi;
                g_api.o.InitRoomEntities(D_801375BC.def->objLayoutId);
                g_api.o.Update();
                func_800F0CD8(0);
                func_800F0CD8(0);
                DestroyEntitiesFromIndex(0x40);
                for (i = 0; i < LEN(g_unkGraphicsStruct.D_80097428); i++) {
                    g_unkGraphicsStruct.D_80097428[i] = 0;
                }
            }
            g_PrevScrollX = g_Tilemap.scrollX.i.hi;
            g_PrevScrollY = g_Tilemap.scrollY.i.hi;
            if (D_80097C98 == 2) {
                D_80097C98 = 3;
                func_800EA5AC(0x40, 0xFF, 0xFF, 0xFF);
            } else {
                D_80097C98 = 0;
            }
            g_api.o.InitRoomEntities(D_801375BC.def->objLayoutId);
            g_api.o.Update();
            g_api.o.Update();
            func_800F0940();
            func_800E414C();
            func_800F24F4();
            if (D_80097C98 == 3) {
                g_GameEngineStep = Engine_Normal;
            }
#if defined(VERSION_US)
            func_8011A9D8();
#endif
            g_MenuStep++;

            break;
        case MENU_STEP_EXIT_4:
            if (!func_800EB720()) {
                func_801027C4(2);
                g_MenuStep++;
            }
            break;
        case MENU_STEP_EXIT_5:
            if (func_801025F4()) {
                g_GameEngineStep = Engine_Normal;
            }
            break;
        }
        func_801028AC(1);
        func_801028AC(1);
        break;
    case 0x5:
        if (g_unkGraphicsStruct.unk20 != 0) {
            if (g_StageId != STAGE_ST0) {
                if (g_PlayableCharacter == PLAYER_ALUCARD) {
                    if (g_unkGraphicsStruct.unk20 == 0xFFF) {
                        EntityAlucard();
                        func_8011A870();
                        g_api.o.UpdateStageEntities();
                        if (g_pads[1].pressed & PAD_DOWN) {
                            g_unkGraphicsStruct.unk20 = 0;
                        }
                    } else {
                        if (g_unkGraphicsStruct.unk20 != 0xFF) {
                            func_8010DF70(g_unkGraphicsStruct.unk20);
                            UpdatePlayerEntities();
                        }
                        g_api.o.UpdateStageEntities();
                        func_80102D70();
                    }
#if defined(VERSION_PSP)
                } else if (g_PlayableCharacter == PLAYER_RICHTER) {
                    g_api.o.UpdateStageEntities();
                    func_80102D70();
                } else if (g_unkGraphicsStruct.unk20 == 0x100) {
                    RichterUpdater = g_PlOvl.D_8013C000;
                    RichterUpdater();
                    RichterUpdater = D_8CE9C48;
                    RichterUpdater();
                    g_api.o.UpdateStageEntities();
                    func_80102D70();
#endif
                } else {
                    g_api.o.UpdateStageEntities();
                    func_80102D70();
                }
            } else {
                D_8013759C = PLAYER.posX.i.hi;
                D_801375A0 = PLAYER.posY.i.hi;
                RichterUpdater = g_PlOvl.D_8013C000;
                RichterUpdater();
                RichterUpdater = g_PlOvl.D_8013C008;
                RichterUpdater();
                D_801375A4 = D_8013759C - PLAYER.posX.i.hi;
                D_801375A8 = D_801375A0 - PLAYER.posY.i.hi;
                g_PlayerX -= D_801375A4;
                g_PlayerY -= D_801375A8;
            }
        } else {
            func_8010E168(1, 0x30);
            g_GameEngineStep = Engine_Normal;
            PlaySfx(SET_UNPAUSE_SFX_SCRIPTS);
        }
        DrawHudSubweapon();
        return;
    case Engine_10:
        switch (g_MenuStep) {
        case MENU_STEP_INIT:
            if (g_IsUsingCd) {
                break;
            }
            PlaySfx(0x81);
            D_80097910 = 0;
            if (g_StageId == STAGE_ST0) {
                SetGameState(Game_GameOver);
                ClearImage(&g_Vram.D_800ACDA0, 0xFF, 0xFF, 0xFF);
                SetGPUBuffRGB(-1);
                DestroyEntitiesFromIndex(0);
                DestroyAllPrimitives();
                func_800EDAE4();
                return;
            }
            SetGameState(Game_GameOver);
            func_800E5498();
            return;
        case MENU_STEP_FADE_FROM_GAME:
            if (func_801025F4() != 0) {
                g_MenuStep++;
            }
            break;
        case MENU_STEP_OPENING:
            SetGameState(Game_GameOver);
            SetGPUBuffRGBZero();
            break;
        }
        func_801028AC(0);
        break;
    }
}
