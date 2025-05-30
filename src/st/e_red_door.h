// SPDX-License-Identifier: AGPL-3.0-or-later
#include "sfx.h"

extern u16 g_EInitCommon[];
extern u16 OVL_EXPORT(RedDoorTiles)[2][8];

static u8 g_eRedDoorUV[][8] = {
    {0xB1, 0xB7, 0xB1, 0xB7, 0x21, 0x21, 0x5F, 0x5F},
    {0x88, 0xA8, 0x88, 0xA8, 0x21, 0x21, 0x5F, 0x5F},
    {0xA8, 0x88, 0xA8, 0x88, 0x21, 0x21, 0x5F, 0x5F},
};

static bool EntityIsNearPlayer(Entity* self) {
    s16 distanceX;
    s16 diffX;
    s16 distanceY;
    s16 diffY;

    diffX = PLAYER.posX.i.hi - self->posX.i.hi;
    distanceX = abs(diffX);
#ifdef STAGE_IS_NO1
    if (distanceX > 24) {
#else
    if (distanceX > 16) {
#endif
        return false;
    }

    diffY = PLAYER.posY.i.hi - self->posY.i.hi;
    distanceY = abs(diffY);
    if (distanceY > 32) {
        return false;
    }

    return true;
}

// params: (& 0x100) Opens to the right
//         (& 0xFF) Index into map-specific tiles (g_eRedDoorTiles)
//                  to place behind the door
void OVL_EXPORT(EntityRedDoor)(Entity* self) {
    Primitive* prim;
    s32 i;
    s16 angle;
    u8* uv;
    s16 x;
    s16 y;
    u8 sp3F;
    s16 endX;
    s16 scrollX;
    s16 scrollY;
    s32 tileIdx;

    switch (self->step) {
    case 0:
#ifdef STAGE_IS_NO1
        self->ext.redDoor.unk88 = self->params & 0x10;
        self->params &= 0xFFEF;
#endif
        InitializeEntity(g_EInitCommon);
        self->animSet = 7;
        self->animCurFrame = 1;
#ifdef STAGE_IS_NO1
        if (self->ext.redDoor.unk88) {
            self->zPriority = 0x58;
        } else {
            self->zPriority = PLAYER.zPriority - 0x20;
        }
#else
        self->zPriority = PLAYER.zPriority - 0x20;
#endif
        self->facingLeft = 0;
        self->posY.i.hi += 0x1F;

        if (self->params & 0x100) {
            self->ext.redDoor.unk86 = -4;
        } else {
            self->ext.redDoor.unk86 = 4;
        }
        self->posX.i.hi += self->ext.redDoor.unk86;
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, LEN(g_eRedDoorUV));
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        uv = (u8*)g_eRedDoorUV;
        prim = &g_PrimBuf[self->primIndex];
        i = 0;
        y = self->posY.i.hi - 0x1F;
        while (prim != NULL) {
            prim->u0 = uv[0];
            prim->u1 = uv[1];
            prim->u2 = uv[2];
            prim->u3 = uv[3];
            prim->v0 = uv[4];
            prim->v1 = uv[5];
            prim->v2 = uv[6];
            prim->v3 = uv[7];
            prim->tpage = 0x1F;
            prim->clut = 0x198;
#ifdef STAGE_IS_NO1
            if (self->ext.redDoor.unk88) {
                prim->priority = 0x58;
            } else {
                prim->priority = PLAYER.zPriority - 0x20;
            }
#else
            prim->priority = PLAYER.zPriority - 0x20;
#endif
            prim->y0 = prim->y1 = y;
            prim->y2 = prim->y3 = y + 62;
            if (i == 0) {
                prim->y0 = prim->y1 = y;
                prim->y2 = prim->y3 = y + 62;
            }
            prim->drawMode = DRAW_COLORS | DRAW_UNK02;
            prim->r0 = prim->b0 = prim->g0 = 0x7F;
            prim->r1 = prim->b1 = prim->g1 = 0x7F;
            prim->r2 = prim->b2 = prim->g2 = 0x7F;
            prim->r3 = prim->b3 = prim->g3 = 0x7F;
            if (i == 2 && !(self->params & 0x100)) {
                prim->drawMode |= DRAW_HIDE;
            }
            if (i == 1 && (self->params & 0x100)) {
                prim->drawMode |= DRAW_HIDE;
            }
            i++;
            uv += 8;
            prim = prim->next;
        }
        if (EntityIsNearPlayer(self)) {
            if (!(self->params & 0x100)) {
                g_api.PlaySfxVolPan(SFX_DOOR_OPEN, 0x60, -6);
                self->ext.redDoor.angle = 0x1000;
            }
            if (self->params & 0x100) {
                g_api.PlaySfxVolPan(SFX_DOOR_OPEN, 0x60, 6);
                self->ext.redDoor.angle = 0x800;
            }
            self->animCurFrame = 0;
            self->step = 4;
            PLAYER.velocityY = 0;
            g_Player.padSim = 0;
            g_Player.demo_timer = 24;
#ifdef STAGE_IS_NO1
            if (self->ext.redDoor.unk88 && self->step == 3) {
                PLAYER.zPriority = 0x5C;
            }
#endif
        } else {
            self->ext.redDoor.angle = 0xC00;
            prim = &g_PrimBuf[self->primIndex];
            i = 0;
            while (prim != NULL) {
                prim->drawMode |= DRAW_HIDE;
                i++;
                if (i == 3) {
                    break;
                }
                prim = prim->next;
            }
        }
        break;
    case 1:
#if defined(VERSION_BETA)
        if (PLAYER.step == 1 && PLAYER.facingLeft != GetSideToPlayer() &&
            EntityIsNearPlayer(self))
#else
        if (!(((PLAYER.facingLeft != GetSideToPlayer()) & 1) ^ 1) &&
            ((PLAYER.step == 0x19 && g_PlayableCharacter) ||
             PLAYER.step == 1) &&
            EntityIsNearPlayer(self))
#endif
        {
            if (!(self->params & 0x100)) {
                g_api.PlaySfxVolPan(SFX_DOOR_OPEN, 0x60, -6);
            }
            if (self->params & 0x100) {
                g_api.PlaySfxVolPan(SFX_DOOR_OPEN, 0x60, 6);
            }
            prim = &g_PrimBuf[self->primIndex];
            i = 0;
            while (prim != NULL) {
                if (i == 1 && !(self->params & 0x100)) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
                if (i == 2 && (self->params & 0x100)) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
                if (i == 0) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
                i++;
                prim = prim->next;
            }
            self->animCurFrame = 0;
            g_Player.padSim = 0;
            g_Player.demo_timer = 2;
            self->step++;
        }
        break;
    case 2:
        g_Player.padSim = 0;
        g_Player.demo_timer = 24;
        if (!(self->params & 0x100)) {
            self->ext.redDoor.angle += 0x20;
            if (self->ext.redDoor.angle >= 0x1000) {
                self->ext.redDoor.angle = 0x1000;
            }
            if (self->ext.redDoor.angle == 0x1000) {
                self->step++;
            }
        } else {
            self->ext.redDoor.angle -= 0x20;
            if (self->ext.redDoor.angle <= 0x800) {
                self->ext.redDoor.angle = 0x800;
            }
            if (self->ext.redDoor.angle == 0x800) {
                self->step++;
            }
        }
#ifdef STAGE_IS_NO1
        if (self->ext.redDoor.unk88 && self->step == 3) {
            g_Tilemap.x = 0x98;
            PLAYER.zPriority = 0x5C;
        }
        g_unkGraphicsStruct.unk18 = 1;
#endif
        break;
    case 3:
        if (g_Player.demo_timer >= 4) {
            return;
        }
        if (!(self->params & 0x100)) {
            g_Player.padSim = PAD_LEFT;
        } else {
            g_Player.padSim = PAD_RIGHT;
        }
        g_Player.demo_timer = 3;
#ifdef STAGE_IS_NO1
        if (PLAYER.posX.i.hi < 0x64 && self->ext.redDoor.unk88) {
            g_Tilemap.left++;
            g_PlayerX -= 0x100;
            g_Tilemap.x = 0x100;
            g_unkGraphicsStruct.unk18 = 0;
        }
        if (PLAYER.posX.i.hi < 0 || PLAYER.posX.i.hi > 0x100) {
            g_unkGraphicsStruct.unk18 = 0;
        }
#endif
        break;
    case 4:
        if (!(self->params & 0x100)) {
            g_Player.padSim = PAD_RIGHT;
        } else {
            g_Player.padSim = PAD_LEFT;
        }
        g_Player.demo_timer = 4;
        if (EntityIsNearPlayer(self) == 0) {
#ifdef STAGE_IS_NO1
            if (self->ext.redDoor.unk88) {
                PLAYER.zPriority = g_unkGraphicsStruct.g_zEntityCenter;
            }
            self->step++;
#ifdef VERSION_PSP
            g_Player.demo_timer = 0;
#endif
#else
            self->step++;
#endif
        }
        break;
    case 5:
        g_Player.padSim = 0;
        g_Player.demo_timer = 4;
        if (!(self->params & 0x100)) {
            self->ext.redDoor.angle -= 0x20;
            if (self->ext.redDoor.angle < 0xC01) {
                self->ext.redDoor.angle = 0xC00;
            }
        } else {
            self->ext.redDoor.angle += 0x20;
            if (self->ext.redDoor.angle >= 0xC00) {
                self->ext.redDoor.angle = 0xC00;
            }
        }
        if (self->ext.redDoor.angle == 0xC00) {
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; prim != NULL; i++, prim = prim->next) {
                prim->drawMode |= DRAW_HIDE;
            }
            if (!(self->params & 0x100)) {
                g_api.PlaySfxVolPan(SFX_DOOR_CLOSE_A, 0x60, -6);
            }
            if (self->params & 0x100) {
                g_api.PlaySfxVolPan(SFX_DOOR_CLOSE_A, 0x60, 6);
            }
            self->animCurFrame = 1;
            self->step = 1;
#ifdef STAGE_IS_NO1
            g_unkGraphicsStruct.unk18 = 0;
#endif
        }
        break;
    }

    if (self->step != 1) {
        g_api.func_8010E168(1, 0x20);
        g_api.func_8010DFF0(1, 1);
    }

    x = self->posX.i.hi - self->ext.redDoor.unk86;
    if (self->params & 0x100) {
        x--;
    } else {
        x++;
    }

    i = 0;
    angle = self->ext.redDoor.angle;
    prim = &g_PrimBuf[self->primIndex];
    for (; prim != NULL; i++, prim = prim->next) {
        if (!(prim->drawMode & DRAW_HIDE)) {
            if (!(self->params & 0x100)) {
                if (i == 0) {
                    endX = prim->x0 = prim->x2 =
                        x + ((rcos(angle) >> 8) * 32 >> 4);
                    prim->x1 = prim->x3 =
                        prim->x0 - ((rsin(angle) >> 4) * 6 >> 8);
                    if (angle > 0xF80) {
                        prim->x1 = prim->x3 = prim->x0 + 1;
                    }
                    if (angle > 0xE00) {
                        prim->u0 = prim->u2 = 178;
                        prim->u1 = prim->u3 = 182;
                    }
                    if (angle <= 0xE00) {
                        prim->u0 = prim->u2 = 177;
                        prim->u1 = prim->u3 = 183;
                    }
                    if (angle == 0x1000) {
                        prim->r1 = prim->b1 = prim->g1 = 63;
                        prim->r3 = prim->b3 = prim->g3 = 63;
                    } else {
                        prim->r1 = prim->b1 = prim->g1 =
                            0x7F - ((angle & 0x3FF) >> 4);
                        prim->r3 = prim->b3 = prim->g3 =
                            0x7F - ((angle & 0x3FF) >> 4);
                    }
                } else {
                    prim->x0 = prim->x2 = x;
                    prim->x1 = prim->x3 = endX;
                    if (angle == 0x1000) {
                        prim->r0 = prim->b0 = prim->g0 = 63;
                        prim->r2 = prim->b2 = prim->g2 = 63;
                    } else {
                        prim->r0 = prim->b0 = prim->g0 = (angle & 0x3FF) >> 4;
                        prim->r2 = prim->b2 = prim->g2 = (angle & 0x3FF) >> 4;
                    }
                }
            } else {
                if (i == 0) {
                    endX = prim->x1 = prim->x3 =
                        x + ((rcos(angle) >> 8) * 32 >> 4);
                    prim->x0 = prim->x2 =
                        prim->x1 + (((rsin(angle) >> 4) * 6) >> 8);
                    if (angle < 0x880) {
                        prim->x0 = prim->x2 = prim->x1 - 1;
                    }
                    if (angle < 0xA00) {
                        prim->u0 = prim->u2 = 178;
                        prim->u1 = prim->u3 = 182;
                    }
                    if (angle > 0xA00) {
                        prim->u0 = prim->u2 = 177;
                        prim->u1 = prim->u3 = 183;
                    }
                    if (angle == 0x800) {
                        prim->r0 = prim->b0 = prim->g0 = 127;
                        prim->r2 = prim->b2 = prim->g2 = 127;
                    } else {
                        prim->r0 = prim->b0 = prim->g0 =
                            63 + ((angle & 0x3FF) >> 4);
                        prim->r2 = prim->b2 = prim->g2 =
                            63 + ((angle & 0x3FF) >> 4);
                    }
                } else {
                    prim->x0 = prim->x2 = endX - 1;
                    prim->x1 = prim->x3 = x;
                    if (angle == 0x800) {
                        prim->r1 = prim->b1 = prim->g1 = 63;
                        prim->r3 = prim->b3 = prim->g3 = 63;
                    } else {
                        prim->r1 = prim->b1 = prim->g1 =
                            63 - ((angle & 0x3FF) >> 4);
                        prim->r3 = prim->b3 = prim->g3 =
                            63 - ((angle & 0x3FF) >> 4);
                    }
                }
            }
        }
    }

    sp3F = self->params & 0xFF;
    if (self->animCurFrame) {
        for (i = 0; i < 4; i++) {
            x = self->posX.i.hi;
            y = self->posY.i.hi - 24 + i * 0x10;
            scrollX = x + g_Tilemap.scrollX.i.hi;
            scrollY = y + g_Tilemap.scrollY.i.hi;
            tileIdx = (scrollX >> 4) + (scrollY >> 4) * g_Tilemap.hSize * 0x10;
            g_Tilemap.fg[tileIdx] = OVL_EXPORT(RedDoorTiles)[sp3F][i];
        }
    } else {
        for (i = 0; i < 4; i++) {
            x = self->posX.i.hi;
            y = self->posY.i.hi - 24 + i * 0x10;
            scrollX = x + g_Tilemap.scrollX.i.hi;
            scrollY = y + g_Tilemap.scrollY.i.hi;
            tileIdx = (scrollX >> 4) + (scrollY >> 4) * g_Tilemap.hSize * 0x10;
            g_Tilemap.fg[tileIdx] = OVL_EXPORT(RedDoorTiles)[sp3F][i + 4];
        }
    }
}
