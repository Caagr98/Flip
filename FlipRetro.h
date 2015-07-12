#pragma once

#include "libretro.h"

#define WIDTH  600
#define HEIGHT 400

retro_environment_t        environ_cb     = NULL;
retro_video_refresh_t      video_cb       = NULL;
retro_audio_sample_t       audio_cb       = NULL;
retro_audio_sample_batch_t audio_batch_cb = NULL;
retro_input_poll_t         input_poll_cb  = NULL;
retro_input_state_t        input_state_cb = NULL;

void retro_set_video_refresh     (retro_video_refresh_t      cb) { video_cb       = cb; }
void retro_set_audio_sample      (retro_audio_sample_t       cb) { audio_cb       = cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll        (retro_input_poll_t         cb) { input_poll_cb  = cb; }
void retro_set_input_state       (retro_input_state_t        cb) { input_state_cb = cb; }

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

void retro_set_controller_port_device(unsigned port, unsigned device) {}

void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned index, bool enabled, const char* code) {}

void retro_init(void) {}
void retro_deinit(void) {}
void retro_reset(void) {}
bool retro_load_game_special(unsigned game_type, const struct retro_game_info* info, size_t num_info) { return false; }
void retro_unload_game(void) {}

unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
void* retro_get_memory_data(unsigned id) { return NULL; }
size_t retro_get_memory_size(unsigned id) { return 0; }

