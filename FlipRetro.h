#pragma once

#include "libretro.h"

#define WIDTH  600
#define HEIGHT 400

extern retro_environment_t        environ_cb;
extern retro_video_refresh_t      video_cb;
extern retro_audio_sample_t       audio_cb;
extern retro_audio_sample_batch_t audio_batch_cb;
extern retro_input_poll_t         input_poll_cb;
extern retro_input_state_t        input_state_cb;

