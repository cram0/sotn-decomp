// SPDX-License-Identifier: AGPL-3.0-or-later
#include "wrp.h"

static u8 anim_1[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_2[] = {4, 0, 4, 0, 0, 0, 0, 0};
static u8* g_eBreakableAnimations[] = {
    anim_1, anim_2, NULL, NULL, NULL, NULL, NULL, NULL};
static u8 g_eBreakableHitboxes[] = {8, 8, 0, 0, 0, 0, 0, 0};

#ifndef VERSION_PSP // on PSP this is in the BSS section, not data
static u8 g_eBreakableExplosionTypes[] = {0, 0, 0, 0, 0, 0, 0, 0};
#else
static u8 g_eBreakableExplosionTypes[] = {0};
#endif

static u16 g_eBreakableanimSets[] = {
    ANIMSET_DRA(3), ANIMSET_DRA(3), 0, 0, 0, 0, 0, 0};
static u8 g_eBreakableDrawModes[] = {
    DRAW_TPAGE | DRAW_TPAGE2 | DRAW_UNK_40,
    DRAW_TPAGE | DRAW_TPAGE2,
    DRAW_DEFAULT,
    DRAW_DEFAULT,
    DRAW_DEFAULT,
    DRAW_DEFAULT,
    DRAW_DEFAULT,
    DRAW_DEFAULT};

// stripped on PSP
static u8 unused[] = {0, 0, 0, 0, 0, 0, 0, 0};

#include "../e_breakable.h"
