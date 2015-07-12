#include "libretro.h"
#include "FlipRetro.h"

#include <string.h>
#include <stdlib.h>


#if 1
#include "gl/Render.cpp"
#endif

Render render;

void retro_set_environment(retro_environment_t cb) {
	environ_cb=cb;

	bool True=true;
	environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &True);
}

bool retro_load_game(const struct retro_game_info* game) {
	return render.init();
}

void retro_get_system_info(struct retro_system_info *info) {
	info->library_name = "Flip";
	info->library_version = "";
}

void retro_get_system_av_info(struct retro_system_av_info* info) {
	info->geometry.base_width = WIDTH;
	info->geometry.base_height = HEIGHT;
	info->geometry.max_width = WIDTH;
	info->geometry.max_height = HEIGHT;
	info->timing.fps = 60;
	info->timing.sample_rate = 44*1024;
}
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void* data, size_t size) { return false; }
bool retro_unserialize(const void* data, size_t size) { return false; }

void retro_run(void) {
	input_poll_cb();
	//TODO ticking
	
	render.render();
}
