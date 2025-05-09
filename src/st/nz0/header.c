// SPDX-License-Identifier: AGPL-3.0-or-later
#include "nz0.h"

void Update();
void HitDetection();
void UpdateRoomPosition();
void InitRoomEntities();
void UpdateStageEntities();

extern s16** OVL_EXPORT(spriteBanks)[];
extern u_long* OVL_EXPORT(cluts)[];
extern LayoutEntity* OVL_EXPORT(pStObjLayoutHorizontal)[];
extern u_long* OVL_EXPORT(gfxBanks)[];
extern RoomDef OVL_EXPORT(rooms_layers)[];
extern RoomHeader OVL_EXPORT(rooms)[];

AbbreviatedOverlay OVL_EXPORT(Overlay) = {
    .Update = Update,
    .HitDetection = HitDetection,
    .UpdateRoomPosition = UpdateRoomPosition,
    .InitRoomEntities = InitRoomEntities,
    .rooms = OVL_EXPORT(rooms),
    .spriteBanks = OVL_EXPORT(spriteBanks),
    .cluts = OVL_EXPORT(cluts),
    .objLayoutHorizontal = OVL_EXPORT(pStObjLayoutHorizontal),
    .tileLayers = OVL_EXPORT(rooms_layers),
    .gfxBanks = OVL_EXPORT(gfxBanks),
    .UpdateStageEntities = UpdateStageEntities,
};

#include "gen_sprite_banks.h"
#include "gen_palette_def.h"
#include "gen_layers.h"
#include "gen_graphics_banks.h"
