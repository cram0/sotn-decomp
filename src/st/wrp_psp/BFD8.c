#include "common.h"
#include "game.h"

INCLUDE_ASM("st/wrp_psp/psp/wrp_psp/BFD8", AnimateEntity);

u8 GetSideToPlayer() {
    u8 side = 0;
    Entity* player = &PLAYER;

    if (g_CurrentEntity->posX.i.hi > player->posX.i.hi) {
        side |= 1;
    }

    if (g_CurrentEntity->posY.i.hi > player->posY.i.hi) {
        side |= 2;
    }
    return side;
}

void MoveEntity() {
    g_CurrentEntity->posX.val += g_CurrentEntity->velocityX;
    g_CurrentEntity->posY.val += g_CurrentEntity->velocityY;
}

void FallEntity(void) {
    if (g_CurrentEntity->velocityY < FALL_TERMINAL_VELOCITY) {
        g_CurrentEntity->velocityY += FALL_GRAVITY;
    }
}

Entity* AllocEntity(Entity* start, Entity* end) {
    Entity* current = start;

    while (current < end) {
        if (!current->entityId) {
            DestroyEntity(current);
            return current;
        }
        current++;
    }
    return NULL;
}

INCLUDE_ASM("st/wrp_psp/psp/wrp_psp/BFD8", UnkEntityFunc0);

u16 func_8018C160(Entity* a, Entity* b) {
    s32 diffX = b->posX.i.hi - a->posX.i.hi;
    s32 diffY = b->posY.i.hi - a->posY.i.hi;
    return ratan2(diffY, diffX);
}
