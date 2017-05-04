// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "ui.h"
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "lib/stb_image.h"

textureList TEXTURE;
fontList FONT;

int currentSelectedItem = 0;
int themeListOffset = 0;

int timer;

int loadingAnimTimer = 0;
u32 rainbow[360] = {0xffff8080, 0xffff8280, 0xffff8480, 0xffff8680, 0xffff8880, 0xffff8a80, 0xffff8c80, 0xffff8e80, 0xffff9180, 0xffff9380, 0xffff9580, 0xffff9780, 0xffff9980, 0xffff9b80, 0xffff9d80, 0xffff9f80, 0xffffa280, 0xffffa480, 0xffffa680, 0xffffa880, 0xffffaa80, 0xffffac80, 0xffffae80, 0xffffb080, 0xffffb380, 0xffffb580, 0xffffb780, 0xffffb980, 0xffffbb80, 0xffffbd80, 0xffffbf80, 0xffffc180, 0xffffc380, 0xffffc680, 0xffffc880, 0xffffca80, 0xffffcc80, 0xffffce80, 0xffffd080, 0xffffd280, 0xffffd480, 0xffffd780, 0xffffd980, 0xffffdb80, 0xffffdd80, 0xffffdf80, 0xffffe180, 0xffffe380, 0xffffe680, 0xffffe880, 0xffffea80, 0xffffec80, 0xffffee80, 0xfffff080, 0xfffff280, 0xfffff480, 0xfffff780, 0xfffff980, 0xfffffb80, 0xfffffd80, 0xffffff80, 0xfffdff80, 0xfffbff80, 0xfff9ff80, 0xfff7ff80, 0xfff4ff80, 0xfff2ff80, 0xfff0ff80, 0xffeeff80, 0xffecff80, 0xffeaff80, 0xffe8ff80, 0xffe5ff80, 0xffe3ff80, 0xffe1ff80, 0xffdfff80, 0xffddff80, 0xffdbff80, 0xffd9ff80, 0xffd7ff80, 0xffd4ff80, 0xffd2ff80, 0xffd0ff80, 0xffceff80, 0xffccff80, 0xffcaff80, 0xffc8ff80, 0xffc6ff80, 0xffc4ff80, 0xffc1ff80, 0xffbfff80, 0xffbdff80, 0xffbbff80, 0xffb9ff80, 0xffb7ff80, 0xffb5ff80, 0xffb3ff80, 0xffb0ff80, 0xffaeff80, 0xffacff80, 0xffaaff80, 0xffa8ff80, 0xffa6ff80, 0xffa4ff80, 0xffa1ff80, 0xff9fff80, 0xff9dff80, 0xff9bff80, 0xff99ff80, 0xff97ff80, 0xff95ff80, 0xff93ff80, 0xff91ff80, 0xff8eff80, 0xff8cff80, 0xff8aff80, 0xff88ff80, 0xff86ff80, 0xff84ff80, 0xff82ff80, 0xff80ff80, 0xff80ff82, 0xff80ff84, 0xff80ff86, 0xff80ff88, 0xff80ff8a, 0xff80ff8c, 0xff80ff8e, 0xff80ff91, 0xff80ff93, 0xff80ff95, 0xff80ff97, 0xff80ff99, 0xff80ff9b, 0xff80ff9d, 0xff80ff9f, 0xff80ffa2, 0xff80ffa4, 0xff80ffa6, 0xff80ffa8, 0xff80ffaa, 0xff80ffac, 0xff80ffae, 0xff80ffb0, 0xff80ffb3, 0xff80ffb5, 0xff80ffb7, 0xff80ffb9, 0xff80ffbb, 0xff80ffbd, 0xff80ffbf, 0xff80ffc1, 0xff80ffc4, 0xff80ffc6, 0xff80ffc8, 0xff80ffca, 0xff80ffcc, 0xff80ffce, 0xff80ffd0, 0xff80ffd2, 0xff80ffd4, 0xff80ffd7, 0xff80ffd9, 0xff80ffdb, 0xff80ffdd, 0xff80ffdf, 0xff80ffe1, 0xff80ffe3, 0xff80ffe6, 0xff80ffe8, 0xff80ffea, 0xff80ffec, 0xff80ffee, 0xff80fff0, 0xff80fff2, 0xff80fff4, 0xff80fff7, 0xff80fff9, 0xff80fffb, 0xff80fffd, 0xff80ffff, 0xff80fdff, 0xff80fbff, 0xff80f9ff, 0xff80f7ff, 0xff80f4ff, 0xff80f2ff, 0xff80f0ff, 0xff80eeff, 0xff80ecff, 0xff80eaff, 0xff80e8ff, 0xff80e5ff, 0xff80e3ff, 0xff80e1ff, 0xff80dfff, 0xff80ddff, 0xff80dbff, 0xff80d9ff, 0xff80d7ff, 0xff80d4ff, 0xff80d2ff, 0xff80d0ff, 0xff80ceff, 0xff80ccff, 0xff80caff, 0xff80c8ff, 0xff80c6ff, 0xff80c4ff, 0xff80c1ff, 0xff80bfff, 0xff80bdff, 0xff80bbff, 0xff80b9ff, 0xff80b7ff, 0xff80b5ff, 0xff80b3ff, 0xff80b0ff, 0xff80aeff, 0xff80acff, 0xff80aaff, 0xff80a8ff, 0xff80a6ff, 0xff80a4ff, 0xff80a1ff, 0xff809fff, 0xff809dff, 0xff809bff, 0xff8099ff, 0xff8097ff, 0xff8095ff, 0xff8093ff, 0xff8090ff, 0xff808eff, 0xff808cff, 0xff808aff, 0xff8088ff, 0xff8086ff, 0xff8084ff, 0xff8082ff, 0xff8080ff, 0xff8280ff, 0xff8480ff, 0xff8680ff, 0xff8880ff, 0xff8a80ff, 0xff8c80ff, 0xff8e80ff, 0xff9080ff, 0xff9380ff, 0xff9580ff, 0xff9780ff, 0xff9980ff, 0xff9b80ff, 0xff9d80ff, 0xff9f80ff, 0xffa280ff, 0xffa480ff, 0xffa680ff, 0xffa880ff, 0xffaa80ff, 0xffac80ff, 0xffae80ff, 0xffb080ff, 0xffb380ff, 0xffb580ff, 0xffb780ff, 0xffb980ff, 0xffbb80ff, 0xffbd80ff, 0xffbf80ff, 0xffc180ff, 0xffc380ff, 0xffc680ff, 0xffc880ff, 0xffca80ff, 0xffcc80ff, 0xffce80ff, 0xffd080ff, 0xffd280ff, 0xffd580ff, 0xffd780ff, 0xffd980ff, 0xffdb80ff, 0xffdd80ff, 0xffdf80ff, 0xffe180ff, 0xffe380ff, 0xffe580ff, 0xffe880ff, 0xffea80ff, 0xffec80ff, 0xffee80ff, 0xfff080ff, 0xfff280ff, 0xfff480ff, 0xfff680ff, 0xfff980ff, 0xfffb80ff, 0xfffd80ff, 0xffff80ff, 0xffff80fd, 0xffff80fb, 0xffff80f9, 0xffff80f7, 0xffff80f4, 0xffff80f2, 0xffff80f0, 0xffff80ee, 0xffff80ec, 0xffff80ea, 0xffff80e8, 0xffff80e5, 0xffff80e3, 0xffff80e1, 0xffff80df, 0xffff80dd, 0xffff80db, 0xffff80d9, 0xffff80d7, 0xffff80d4, 0xffff80d2, 0xffff80d0, 0xffff80ce, 0xffff80cc, 0xffff80ca, 0xffff80c8, 0xffff80c6, 0xffff80c4, 0xffff80c1, 0xffff80bf, 0xffff80bd, 0xffff80bb, 0xffff80b9, 0xffff80b7, 0xffff80b5, 0xffff80b2, 0xffff80b0, 0xffff80ae, 0xffff80ac, 0xffff80aa, 0xffff80a8, 0xffff80a6, 0xffff80a4, 0xffff80a1, 0xffff809f, 0xffff809d, 0xffff809b, 0xffff8099, 0xffff8097, 0xffff8095, 0xffff8093, 0xffff8091, 0xffff808e, 0xffff808c, 0xffff808a, 0xffff8088, 0xffff8086, 0xffff8084, 0xffff8082};

float previewX = 8.f;
float previewY = 36.f;
float previewW = 0.0f;
float previewH = 0.0f;

u32 getLoadingAnim(){
	loadingAnimTimer += 2;

	if(loadingAnimTimer >= 360)
		loadingAnimTimer = 0;

	return rainbow[loadingAnimTimer];
}

Result load_png(string filename, sf2d_texture** texture, bool safe){
	// thx MarcusD
	vector<char> buffer;
	fileToVector(string(filename), buffer);

	void* ob = 0;
	int x = 0;
	int y = 0;

	if(safe){
		stbi_info_from_memory((const unsigned char*)&buffer[0], buffer.size(), &x, &y, 0);

		if(x == 0 || x > 412 || y == 0 || y > 482)
			return -1;
	}

	try {
		ob = stbi_load_from_memory((const unsigned char*)&buffer[0], buffer.size(), &x, &y, 0, 4);
	} catch (...){
		return -1;
	}

	if(!ob){
		printf("fail: %s\n", stbi_failure_reason());
		return -1;
	}

	*texture = sf2d_create_texture_mem_RGBA8((void*)ob, x, y, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	free(ob);

	if(!texture)
		return -1;

	return 0;
}

Result load_png_mem(vector<char>& data, sf2d_texture** texture, bool safe){
	void* ob = 0;
	int x = 0;
	int y = 0;

	if(safe){
		stbi_info_from_memory((const unsigned char*)&data[0], data.size(), &x, &y, 0);

		if(x == 0 || x > 412 || y == 0 || y > 482)
			return -1;
	}

	try {
		ob = stbi_load_from_memory((const unsigned char*)&data[0], data.size(), &x, &y, 0, 4);
	} catch (...){
		return -1;
	}

	if(!ob){
		printf("fail: %s\n", stbi_failure_reason());
		return -1;
	}

	*texture = sf2d_create_texture_mem_RGBA8((void*)ob, x, y, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	free(ob);

	if(!texture)
		return -1;

	return 0;
}

void drawMain(gfxScreen_t screen){
	if(screen == GFX_TOP){
		// bar
		sf2d_draw_rectangle(0, 0, 400, 30, 0xFFAB47BC);

		// logo
		sf2d_draw_texture_part(TEXTURE.ui.tx, 6, 6, 0, 0, 74, 19);

		// version
		sftd_draw_text(FONT.normal, 85, 8, 0xFFFFFFFF, 13, (string("v") + VERSION).c_str());

		// theme count
		string s = to_string(themes.size()) + " themes";
		sftd_draw_text(FONT.normal, 400 - sftd_get_text_width(FONT.normal, 13, s.c_str()) - 8, 5, 0xFFFFFFFF, 13, s.c_str());

		if(themes.size() != 0){
			if(LightLock_TryLock(&themes[currentSelectedItem].lock)){
				if(!themes[currentSelectedItem].infoIsFullyLoaded)
					sftd_draw_text(FONT.light, 178, 38, 0xFFFFFFFF, 24, themes[currentSelectedItem].fileName.c_str());
				else
					sftd_draw_text(FONT.light, 178, 38, 0xFFFFFFFF, 24, themes[currentSelectedItem].title.c_str());

				sftd_draw_text(FONT.normal, 178, 70, 0xFFFFFFFF, 13, "[description not available]");

				s = string("by Unknown");
				sftd_draw_text(FONT.normal, 400 - sftd_get_text_width(FONT.normal, 13, s.c_str()) - 8, 84, 0xFFFFFFFF, 13, s.c_str());

				sf2d_draw_rectangle(8, 54, 160, 160, getLoadingAnim());
				sf2d_draw_texture_part(TEXTURE.ui.tx, 8, 54, 0, 270, 160, 160);

				// install button
				sf2d_draw_texture_part(TEXTURE.ui.tx, 178, 219, 400, 0, 15, 15);
				sftd_draw_text(FONT.small, 196, 220, 0xFFFFFFFF, 11, "Install");

				// without bgm
				int w = sftd_get_text_width(FONT.small, 11, "w/o BGM");
				sf2d_draw_texture_part(TEXTURE.ui.tx, 400 - w - 8 - 30 - 3, 219, 490, 0, 30, 15);
				sf2d_draw_texture_part(TEXTURE.ui.tx, 400 - w - 8 - 30 - 3 - 10, 219, 520, 0, 15, 15);
				sftd_draw_text(FONT.small, 400 - w - 8, 220, 0xFFFFFFFF, 11, "w/o BGM");
			} else {
				// title
				sftd_draw_text(FONT.light, 178, 38, 0xFFFFFFFF, 24, themes[currentSelectedItem].title.c_str());

				// description
				istringstream iss(wrap(themes[currentSelectedItem].description, 30));
				s = "";
				for (size_t i = 0; i < 6; i++){
					string line;

					if(!getline(iss, line))
						break;

					s += (i ? "\n" : "") + line;
				}
				sftd_draw_text(FONT.normal, 178, 70, 0xFFFFFFFF, 13, s.c_str());

				// author
				int authorY = (count(s.begin(), s.end(), '\n') + 1) * 14;
				s = string("by ") + themes[currentSelectedItem].author;
				sftd_draw_text(FONT.normal, 400 - sftd_get_text_width(FONT.normal, 13, s.c_str()) - 8, 70 + authorY, 0xFFFFFFFF, 13, s.c_str());

				bool fullscreenBtn = true;
				bool BGMBtn = themes[currentSelectedItem].hasBGM;

				// install button
				sf2d_draw_texture_part(TEXTURE.ui.tx, 178, 219 - BGMBtn*17 - fullscreenBtn*17, 400, 0, 15, 15);
				sftd_draw_text(FONT.small, 196, 220 - BGMBtn*17 - fullscreenBtn*17, 0xFFFFFFFF, 11, "Install");

				// without bgm
				int w = sftd_get_text_width(FONT.small, 11, "w/o BGM");
				sf2d_draw_texture_part(TEXTURE.ui.tx, 400 - w - 8 - 30 - 3, 219 - BGMBtn*17 - fullscreenBtn*17, 490, 0, 30, 15);
				sf2d_draw_texture_part(TEXTURE.ui.tx, 400 - w - 8 - 30 - 3 - 10, 219 - BGMBtn*17 - fullscreenBtn*17, 520, 0, 15, 15);
				sftd_draw_text(FONT.small, 400 - w - 8, 220 - BGMBtn*17 - fullscreenBtn*17, 0xFFFFFFFF, 11, "w/o BGM");

				// fullscreen preview button & delete
				sf2d_draw_texture_part(TEXTURE.ui.tx, 178, 219 - BGMBtn*17, 430, 0, 15, 15);
				sftd_draw_text(FONT.small, 400 - sftd_get_text_width(FONT.small, 11, "[hold] delete") - 8, 220 - BGMBtn*17, 0xFFFFFFFF, 11, "[hold] delete");
				if(themes[currentSelectedItem].preview)
					sftd_draw_text(FONT.small, 196, 220 - BGMBtn*17, 0xFFFFFFFF, 11, "Fullscreen preview");

				// bgm preview button
				if(BGMBtn){
					sf2d_draw_texture_part(TEXTURE.ui.tx, 178, 219, 445, 0, 15, 15);
					sftd_draw_text(FONT.small, 196, 220, 0xFFFFFFFF, 11, "BGM preview");
				}

				// preview
				if(themes[currentSelectedItem].hasPreview && themes[currentSelectedItem].preview){
					if(previewX == 8.f){
						previewW = (float)(163.f/themes[currentSelectedItem].preview->width);
						previewH = (float)(196.f/themes[currentSelectedItem].preview->height);
					}

					sf2d_draw_texture_scale(themes[currentSelectedItem].preview, (int)previewX, (int)previewY, previewW, previewH);
				} else {
					if(themes[currentSelectedItem].hasPreview || !themes[currentSelectedItem].infoIsFullyLoaded){
						sf2d_draw_rectangle(8, 54, 160, 160, getLoadingAnim());
						sf2d_draw_texture_part(TEXTURE.ui.tx, 8, 54, 0, 270, 160, 160);
					} else {
						sf2d_draw_texture_part(TEXTURE.ui.tx, 8, 54, 160, 270, 160, 160);
					}
				}

				LightLock_Unlock(&themes[currentSelectedItem].lock);
			}

			if(isInstalling){
				sf2d_draw_rectangle(0, 0, 400, 240, 0xEE000000);
				sf2d_draw_texture_part(TEXTURE.ui.tx, 65, 91, 400, 270, 269, 58);
			} else if(update.size() != 0){
				sf2d_draw_rectangle(0, 0, 400, 240, 0xEE000000);
				sftd_draw_text(FONT.light, 28, 28, 0xFFFFFFFF, 24, "There's a new update!");
				sftd_draw_text(FONT.normal, 28, 52, 0xFFFFFFFF, 13, update.c_str());
			} else if(deletePrompt){
				sf2d_draw_rectangle(0, 0, 400, 240, 0xEE000000);
				sftd_draw_text(FONT.light, 200 - sftd_get_text_width(FONT.light, 24, "Are you sure you want to") / 2, 56, 0xFFFFFFFF, 24, "Are you sure you want to");
				sftd_draw_text(FONT.light, 200 - sftd_get_text_width(FONT.light, 24, "delete this theme?") / 2, 56 + 26, 0xFFFFFFFF, 24, "delete this theme?");
			} else if(dumpPrompt){
				sf2d_draw_rectangle(0, 0, 400, 240, 0xEE000000);
				sftd_draw_text(FONT.light, 200 - sftd_get_text_width(FONT.light, 24, "Dump installed theme?") / 2, 56 + 26, 0xFFFFFFFF, 24, "Dump installed theme?");
				sftd_draw_text(FONT.normal, 200 - sftd_get_text_width(FONT.normal, 13, "Please don't submit official themes to 3DSThem.es!") / 2, 56 + 26*2, 0x880000FF, 13, "Please don't submit official themes to 3DSThem.es!");
			} else if(currentPlayingAudio || audioIsPlaying){
				sf2d_draw_rectangle(0, 0, 400, 240, 0x88000000);
				sf2d_draw_texture_part(TEXTURE.ui.tx, 88, 91, 400, 328, 223, 92);
			}
		} else if(themes.size() == 0){
			string s = "No themes found! :(";
			sftd_draw_text(FONT.light, 200 - sftd_get_text_width(FONT.light, 24, s.c_str()) / 2, 38, 0xFFFFFFFF, 24, s.c_str());
			s = "Go to 3DSThem.es on your computer, download some themes,";
			sftd_draw_text(FONT.normal, 200 - sftd_get_text_width(FONT.normal, 13, s.c_str()) / 2, 38 + 26 + 13, 0xFFFFFFFF, 13, s.c_str());
			s = "and put them into the /Themes folder in your SD Card.";
			sftd_draw_text(FONT.normal, 200 - sftd_get_text_width(FONT.normal, 13, s.c_str()) / 2, 38 + 26 + 13*2, 0xFFFFFFFF, 13, s.c_str());
		}
	} else {
		// themes
		if(themes.size() != 0){
			for (int i = max(0, currentSelectedItem - 3); i < min((int)themes.size(), currentSelectedItem + 5); i++){
				if(i == currentSelectedItem)
					sf2d_draw_rectangle(0, 48 * i + 30 - themeListOffset, 320, 48, 0x19FFFFFF);

				if(LightLock_TryLock(&themes[i].lock)){
					sf2d_draw_texture_part(TEXTURE.ui.tx, 0, 48 * i + 30 - themeListOffset, 320, 270, 48, 48);

					if(!themes[i].infoIsFullyLoaded)
						sftd_draw_text(FONT.light, 56, 48 * i + 8 + 30 - themeListOffset, 0xFFFFFFFF, 24, themes[i].fileName.c_str());
					else
						sftd_draw_text(FONT.light, 56, 48 * i + 8 + 30 - themeListOffset, 0xFFFFFFFF, 24, themes[i].title.c_str());

					continue;
				}

				if(themes[i].icon){
					sf2d_draw_texture_scale(themes[i].icon, 0, 48 * i + 48 + 30 - themeListOffset, 1.0f, -1.0f);
				} else if(themes[i].hasPreview && themes[i].preview)
					sf2d_draw_texture_part_scale(themes[i].preview, 0, 48 * i + 30 - themeListOffset, 91, 21, 219, 219, 48.f/219, 48.f/219);
				else
					sf2d_draw_texture_part(TEXTURE.ui.tx, 0, 48 * i + 30 - themeListOffset, 320, 270, 48, 48);

				sftd_draw_text(FONT.light, 56, 48 * i + 8 + 30 - themeListOffset, 0xFFFFFFFF, 24, themes[i].title.c_str());

				LightLock_Unlock(&themes[i].lock);
			}
		} else {
			sftd_draw_text(FONT.light, 320 / 2 - sftd_get_text_width(FONT.light, 24, "Press START to quit.") / 2, 240 / 2 - 12, 0xFFFFFFFF, 24, "Press START to quit.");
		}

		// bar
		sf2d_draw_rectangle(0, 0, 320, 30, 0xFFAB47BC);

		// dump theme icon
		sf2d_draw_texture_part(TEXTURE.ui.tx, 320 - 3 - 24, 3, 146, 0, 24, 24);

		// preview (fullscreen)
		if(themes.size() != 0 && previewX != 8.f && themes[currentSelectedItem].preview && !LightLock_TryLock(&themes[currentSelectedItem].lock)){
			sf2d_draw_texture_scale(themes[currentSelectedItem].preview, (int)previewX - 40, (int)previewY - 240, previewW, previewH);
			LightLock_Unlock(&themes[currentSelectedItem].lock);
		}

		if(isInstalling){
			sf2d_draw_rectangle(0, 0, 320, 240, 0xEE000000);
			sftd_draw_text(FONT.light, 8, 8, 0xFFFFFFFF, 24, installProgress.c_str());
		} else if(update.size() != 0){
			sf2d_draw_rectangle(0, 0, 320, 240, 0xEE000000);
			sftd_draw_text(FONT.light, 320 / 2 - sftd_get_text_width(FONT.light, 24, "Install this update?") / 2, 42, 0xFFFFFFFF, 24, "Install this update?");

			sf2d_draw_texture_part(TEXTURE.ui.tx, 110, 98, 669, 270, 100, 40);
			sf2d_draw_texture_part(TEXTURE.ui.tx, 110 + 85 + 6, 98 + 25 + 6, 400, 0, 15, 15);
			sftd_draw_text(FONT.light, 110 + 100 / 2 - sftd_get_text_width(FONT.light, 24, "Yes") / 2, 103, 0xFFFFFFFF, 24, "Yes");

			sf2d_draw_texture_part(TEXTURE.ui.tx, 110, 142, 669, 270, 100, 40);
			sf2d_draw_texture_part(TEXTURE.ui.tx, 110 + 85 + 6, 142 + 25 + 6, 415, 0, 15, 15);
			sftd_draw_text(FONT.light, 110 + 100 / 2 - sftd_get_text_width(FONT.light, 24, "No") / 2, 147, 0xFFFFFFFF, 24, "No");
		} else if(deletePrompt || dumpPrompt){
			sf2d_draw_rectangle(0, 0, 320, 240, 0xEE000000);

			sf2d_draw_texture_part(TEXTURE.ui.tx, 110, 98 - 30, 669, 270, 100, 40);
			sf2d_draw_texture_part(TEXTURE.ui.tx, 110 + 85 + 6, 98 - 30 + 25 + 6, 400, 0, 15, 15);
			sftd_draw_text(FONT.light, 110 + 100 / 2 - sftd_get_text_width(FONT.light, 24, "Yes") / 2, 103 - 30, 0xFFFFFFFF, 24, "Yes");

			sf2d_draw_texture_part(TEXTURE.ui.tx, 110, 142 - 30, 669, 270, 100, 40);
			sf2d_draw_texture_part(TEXTURE.ui.tx, 110 + 85 + 6, 142 - 30 + 25 + 6, 415, 0, 15, 15);
			sftd_draw_text(FONT.light, 110 + 100 / 2 - sftd_get_text_width(FONT.light, 24, "No") / 2, 147 - 30, 0xFFFFFFFF, 24, "No");
		} else if(currentPlayingAudio || audioIsPlaying)
			sf2d_draw_rectangle(0, 0, 320, 240, 0x88000000);
	}
}

void drawLoading(gfxScreen_t screen){
	if(screen == GFX_TOP){
		sf2d_draw_rectangle(0, 0, 400, 240, 0xFFAB47BC);
		sf2d_draw_texture_part(TEXTURE.ui.tx, 0, 0, 400, 30, 400, 240);
		//sftd_draw_text(FONT.normal, 295, 73, 0xFFFFFFFF, 13, ("v" + VERSION).c_str());
	} else {
		sf2d_draw_rectangle(0, 0, 320, 240, getLoadingAnim());
		sf2d_draw_texture_part(TEXTURE.ui.tx, -40, 0, 0, 30, 400, 240);
	}
}

void drawScreen(gfxScreen_t screen){
	if(!themesScanned)
		drawLoading(screen);
	else
		drawMain(screen);
}

// ---

void UI_start(){
	FONT.normal = sftd_load_font_file("romfs:/RobotoCondensed-Regular.ttf");
	FONT.small = sftd_load_font_file("romfs:/RobotoCondensed-Regular.ttf");
	FONT.light = sftd_load_font_file("romfs:/RobotoCondensed-Light.ttf");
	TEXTURE.ui = {NULL, true};

	Result res;
	res = load_png("romfs:/ui.png", &TEXTURE.ui.tx, false);

	if(res)
		throwError("Failed to load textures.");

	// font cache thingy to make text render better
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sftd_draw_text_wrap(FONT.normal, 0, 0, 0x000000FF, 13, 300, "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789!?¡¿:()\"'<>\\/#%$-_&*+.,;{}[]@ ");
	sftd_draw_text_wrap(FONT.small, 0, 0, 0x000000FF, 11, 300, "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789!?¡¿:()\"'<>\\/#%$-_&*+.,;{}[]@ ");
	sftd_draw_text_wrap(FONT.light, 0, 0, 0x000000FF, 24, 300, "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789!?¡¿:()\"'<>\\/#%$-_&*+.,;{}[]@ ");
	sf2d_swapbuffers();

	UI_update();
}

void UI_cleanup(){
	sftd_free_font(FONT.normal);

	TEXTURE.ui.loaded = false;
	sf2d_free_texture(TEXTURE.ui.tx);
}

void UI_update(){
	if(isError && STATE.debug != GFX_TOP){
		sf2d_start_frame(GFX_TOP, GFX_LEFT);

		for (size_t i = 0; i < 240; i++){
			sf2d_draw_rectangle(0, i, 400, 1, RGBA8(i, 0, 0, 255));
		}

		sftd_draw_text(FONT.normal, 0, 0, 0xFFFFFFFF, 13, (string("An error has occurred. You have version ") + VERSION + ".").c_str());
		sftd_draw_text(FONT.normal, 0, 224, 0xFFFFFFFF, 13, "Press START to quit.");
		sftd_draw_text_wrap(FONT.normal, 0, 16, 0xFFFFFFFF, 13, 400, error.c_str());
		sf2d_end_frame();
		sf2d_swapbuffers();
		return;
	}

	timer++;
	runAnimations();

	if(STATE.debug != GFX_TOP){
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawScreen(GFX_TOP);
		sf2d_end_frame();
	}

	if(STATE.debug != GFX_BOTTOM){
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		drawScreen(GFX_BOTTOM);
		sf2d_end_frame();
	}

	sf2d_swapbuffers();
}

void selectTheme(int id){
	if(currentSelectedItem == id || id < 0 || id >= themes.size())
		return;

	currentSelectedItem = id;
	themeListOffset = min((int)themes.size() * 48 + 162, max(0, id * 48 - 81));

	checkInfosToBeLoaded(currentSelectedItem);
}

void toggleFullscreen(){
	if(previewX == 8.f){
		startAnimation(&previewX, 0.f, 50);
		startAnimation(&previewY, 0.f, 50);
		startAnimation(&previewW, 1.0001f, 50);
		startAnimation(&previewH, 1.f, 50);
	} else {
		startAnimation(&previewX, 8.f, 50);
		startAnimation(&previewY, 36.f, 50);
		startAnimation(&previewW, 0.4075f, 50);
		startAnimation(&previewH, 0.4066f, 50);
	}
}

int startAnimation(float* valueToChange, float end, float duration){
	Animation anim = {
		*valueToChange,
		end,
		duration,
		valueToChange,
		timer,
		true
	};
	animationList.push_back(anim);

	return animationList.size() - 1;
}

void runAnimations(){
	for (size_t i = 0; i < animationList.size(); i++){
		if(animationList[i].isActive){
			*animationList[i].valueToChange = easeOutQuint(timer - animationList[i].timeStarted, animationList[i].start, animationList[i].end - animationList[i].start, animationList[i].duration);

			if(timer > animationList[i].timeStarted + animationList[i].duration){
				animationList[i].isActive = false;
			}
		}
	}
}

vector<Animation> animationList;

float easeOutQuint(float t, float b, float c, float d){
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1) + b;
}
