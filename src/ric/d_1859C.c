// SPDX-License-Identifier: AGPL-3.0-or-later
#include "ric.h"

#if defined(VERSION_PSP)
extern s32 D_8015459C;
extern s16 g_DeadPrologueTimer;
extern s16 D_801545AA;
extern s16 D_801545AC;
#else
s32 D_8015459C = 0;
RECT D_801545A0 = {512, 256, 32, 80};
s16 g_DeadPrologueTimer = 0;
s16 D_801545AA = 0;
s16 D_801545AC = 0;
#endif
u8 D_801545B0[] = {10, 8, 8, 6, 6, 4, 4,  4,  4, 4,
                   4,  4, 4, 4, 4, 4, -1, -1, 0, 0};
u8 D_801545C4[] = {4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 16, 16, 16, 16, 16, 16};
u8 D_801545D4[] = {8,  12, 16, 20, 24, 28, 32, 32,
                   32, 32, 32, 32, 32, 32, 32, 32};
Point16 g_RicSensorsCeiling[NUM_HORIZONTAL_SENSORS] = {
    {0, -16},
    {0, -16},
    {4, -16},
    {-4, -16},
};
Point16 g_RicSensorsFloor[NUM_HORIZONTAL_SENSORS] = {
    {0, 28},
    {0, 24},
    {4, 24},
    {-4, 24},
};
Point16 g_RicSensorsWall[NUM_VERTICAL_SENSORS * 2] = {
    {7, 23},  {7, 16},  {7, 8},  {7, 0},  {7, -8},  {7, -8},  {7, -15},
    {-7, 23}, {-7, 16}, {-7, 8}, {-7, 0}, {-7, -8}, {-7, -8}, {-7, -15},
};
s16 g_RicSensorsCeilingDefault[NUM_HORIZONTAL_SENSORS] = {-16, -16, -16, -16};
s16 g_RicSensorsFloorDefault[NUM_HORIZONTAL_SENSORS] = {28, 24, 24, 24};
s16 g_RicSensorsWallDefault[NUM_VERTICAL_SENSORS] = {23, 16, 8, 0, -8, -8, -15};
#if defined(VERSION_PSP)
extern s16 g_RicSensorsCeilingCrouch[NUM_HORIZONTAL_SENSORS]; // on BSS
#else
s16 g_RicSensorsCeilingCrouch[NUM_HORIZONTAL_SENSORS] = {0, 0, 0, 0};
#endif
s16 g_RicSensorsWallCrouch[NUM_VERTICAL_SENSORS] = {23, 16, 8, 4, 4, 1, 1};
