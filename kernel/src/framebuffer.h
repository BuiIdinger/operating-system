#pragma once

#include <stdint.h>

uint32_t packPixel(uint8_t r, uint8_t g, uint8_t b, const int32_t display);
int32_t framebufferInit(void);