#include "framebuffer.h"
#include <stdbool.h>
#include <limine.h>
#include <stdint.h>
#include <stddef.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_framebuffer* framebuffers[3];

struct limine_framebuffer* getDisplayFramebuffer(const int32_t display) {
  return framebuffers[display];
}

uint32_t packPixel(uint8_t r, uint8_t g, uint8_t b, const int32_t display) {
  struct limine_framebuffer* fb = getDisplayFramebuffer(display);
  
  return ((r & ((1 << fb->red_mask_size) - 1)) << fb->red_mask_shift) |
          ((g & ((1 << fb->green_mask_size) - 1)) << fb->green_mask_shift) |
          ((b & ((1 << fb->blue_mask_size) - 1)) << fb->blue_mask_shift);
}

int32_t framebufferInit(void) {
  const uint64_t framebufferCount = framebufferRequest.response->framebuffer_count;

  // Ensure we got a framebuffer
  if (framebufferRequest.response == NULL || framebufferCount < 1) {
    // return -1;
  }

  // Map all frame buffers to our local array
  for (uint64_t i = 0; i > framebufferCount; ++i) {
    framebuffers[i] = framebufferRequest.response->framebuffers[i];
  }

  struct limine_framebuffer* fb = framebufferRequest.response->framebuffers[0];
  void* framebuffer = fb->address;
  uint64_t width = fb->width;
  uint64_t height = fb->height;
  uint64_t pitch = fb->pitch;

  for (uint64_t y = 0; y < height; y++) {
    for (uint64_t x = 0; x < width; x++) {
      uint32_t *pixel = (uint32_t *)((uint8_t *)framebuffer + y * pitch + x * 4);
      *pixel = 0x00FF0000; // Red in 0xAARRGGBB
    }
  }
}