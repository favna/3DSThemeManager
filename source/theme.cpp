// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "theme.h"

vector<Theme> themes;
bool themesScanned = false;
bool isInstalling = false;
string installProgress = "";

void scanThemes(void*){
	Result ret;

	Handle themeDir;
	ret = FSUSER_OpenDirectory(&themeDir, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/Themes/"));
	printf("%lX\n", ret);

	while (true){
		u32 entriesRead;
		FS_DirectoryEntry entry;
		FSDIR_Read(themeDir, &entriesRead, 1, &entry);
		if(entriesRead){
			if(entry.attributes & FS_ATTRIBUTE_DIRECTORY){
				if(fileExists(u"/Themes/" + u16string((char16_t*)entry.name) + u"/body_LZ.bin")){
					Theme theme = {
						u16tstr(entry.name, 0x106),
						u16tstr(entry.name, 0x106),
						"[description not available]",
						"Unknown",
						NULL,
						NULL,
						false,
						false,
						false,
						false,
						fileExists(u"/Themes/" + u16string((char16_t*)entry.name) + u"/bgm.ogg"),
						fileExists(u"/Themes/" + u16string((char16_t*)entry.name) + u"/Preview.png")
					};

					LightLock_Init(&theme.lock);

					themes.push_back(theme);

					if(themes.size() <= 4){
						themes[themes.size()-1].infoIsloaded = true;
						themes[themes.size()-1].previewIsLoaded = true;
						loadThemeInfo((void*)(themes.size()-1));
					}
				}
			} else if(
				entry.shortExt[0] == 'Z' &&
				entry.shortExt[1] == 'I' &&
				entry.shortExt[2] == 'P'){
				unzFile zipFile = unzOpen((string("/Themes/") + u16tstr(entry.name, 0x106)).c_str());

				if(!zipFile){
					printf("INVALID ZIP:%s\n", u16tstr(entry.name, 0x106).c_str());
				 	continue;
				}

				if(unzLocateFile(zipFile, "body_LZ.bin", 0)){
					unzClose(zipFile);
					printf("NO BODY:%s\n", u16tstr(entry.name, 0x106).c_str());
					continue;
				}

				Theme theme = {
					u16tstr(entry.name, 0x106),
					u16tstr(entry.name, 0x106),
					"[description not available]",
					"Unknown",
					NULL,
					NULL,
					true,
					false,
					false,
					false,
					false,
					false
				};

				LightLock_Init(&theme.lock);

				themes.push_back(theme);

				unzClose(zipFile);

				if(themes.size() <= 4){
					themes[themes.size()-1].infoIsloaded = true;
					themes[themes.size()-1].previewIsLoaded = true;
					loadThemeInfo((void*)(themes.size()-1));
				}
			}
		} else
			break;
	}

	FSDIR_Close(themeDir);

	themesScanned = true;

	for (size_t i = 0; i < themes.size(); i++){
		printf("theme:%s by %s\n", themes[i].title.c_str(), themes[i].author.c_str());
	}
}

void loadPreview(void* id){
	if(!themes[(int)id].isZip){
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(1);
		}

		sf2d_texture* tmp;
		Result res = load_png((string("/Themes/") + themes[(int)id].fileName + "/Preview.png").c_str(), &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		} else
			themes[(int)id].hasPreview = false;

		LightLock_Unlock(&themes[(int)id].lock);
	} else {
		// open zip
		// NOTE: this part is also on the loadThemeInfo function
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(1);
		}

		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[(int)id].fileName)).c_str());

		if(!zipFile){
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		vector<char> pngData;

		if(unzLocateFile(zipFile, "Preview.png", 0)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(unzOpenCurrentFile(zipFile)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(zippedFileToVector(zipFile, pngData)){
			unzCloseCurrentFile(zipFile);
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		sf2d_texture* tmp;
		Result res = load_png_mem(pngData, &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		} else
			themes[(int)id].hasPreview = false;

		LightLock_Unlock(&themes[(int)id].lock);
	}
}

void loadThemeInfo(void* id){
	themes[(int)id].infoIsloaded = true;

	if(!themes[(int)id].isZip){
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(1);
		}

		ifstream smdhFile("/Themes/" + themes[(int)id].fileName + "/info.smdh", ios::in | ios::binary);
		if(smdhFile.is_open()){
			char* buffer = new char[0x520];
			smdhFile.read(buffer, 0x520);

			int offset = 0x8;

			string tmpTitle = "";
			string tmpDescription = "";
			string tmpAuthor = "";

			for (size_t i = 0; i < 128; i++){
				if(buffer[offset + i] == '\00')
					break;

				tmpTitle += buffer[offset + i];
				offset++;
			}

			offset = 0x8 + 128;

			for (size_t i = 0; i < 256; i++){
				if(buffer[offset + i] == '\00')
					break;

				tmpDescription += buffer[offset + i];
				offset++;
			}

			offset = 0x8 + 128 + 256;

			for (size_t i = 0; i < 128; i++){
				if(buffer[offset + i] == '\00')
					break;

				tmpAuthor += buffer[offset + i];
				offset++;
			}

			//if(tmpTitle.find_first_not_of('\00') != string::npos)
			if(tmpTitle.size() != 0)
				themes[(int)id].title = tmpTitle;

			if(tmpDescription.size() != 0)
				themes[(int)id].description = tmpDescription;

			if(tmpAuthor.size() != 0 && tmpAuthor.size() < 20)
				themes[(int)id].author = tmpAuthor;

			smdhFile.seekg(0x24C0);
			char* iconBuf = new char[0x1200];
			smdhFile.read(iconBuf, 0x1200);

			// detects default icons
			if(iconBuf[0] != '\x9D' && iconBuf[1] != '\x04' && iconBuf[0] != '\xBF' && iconBuf[1] != '\x0D'){
				themes[(int)id].icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
				u16* dst = (u16*)(themes[(int)id].icon->tex.data + 64 * 8 * 2 * sizeof(u16));
				u16* src = (u16*)(iconBuf);
				for (u8 j = 0; j < 48; j += 8){
					memcpy(dst, src, 48 * 8 * sizeof(u16));
					src += 48 * 8;
					dst += 64 * 8;
				}
			}

			delete[] buffer;
			delete[] iconBuf;
		}

		smdhFile.close();

		LightLock_Unlock(&themes[(int)id].lock);

		loadPreview(id);

		themes[(int)id].infoIsFullyLoaded = true;
	} else {
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(1);
		}

		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[(int)id].fileName)).c_str());

		if(!zipFile){
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		// check if bgm exists
		if(!unzLocateFile(zipFile, "bgm.ogg", 0))
			themes[(int)id].hasBGM = true;

		// check if preview exists
		if(!unzLocateFile(zipFile, "Preview.png", 0))
			themes[(int)id].hasPreview = true;

		if(!unzLocateFile(zipFile, "info.smdh", 0)){
			if(!unzOpenCurrentFile(zipFile)){
				vector<char> smdhData;
				if(zippedFileToVector(zipFile, smdhData)){
					unzCloseCurrentFile(zipFile);
				} else {
					if(smdhData.size() == 0x36C0){
						int offset = 0x8;

						string tmpTitle = "";
						string tmpDescription = "";
						string tmpAuthor = "";

						for (size_t i = 0; i < 128; i++){
							if(smdhData[offset + i] == '\00')
								break;

							tmpTitle += smdhData[offset + i];
							offset++;
						}

						offset = 0x8 + 128;

						for (size_t i = 0; i < 256; i++){
							if(smdhData[offset + i] == '\00')
								break;

							tmpDescription += smdhData[offset + i];
							offset++;
						}

						offset = 0x8 + 128 + 256;

						for (size_t i = 0; i < 128; i++){
							if(smdhData[offset + i] == '\00')
								break;

							tmpAuthor += smdhData[offset + i];
							offset++;
						}

						if(tmpTitle.size() != 0)
							themes[(int)id].title = tmpTitle;

						if(tmpDescription.size() != 0)
							themes[(int)id].description = tmpDescription;

						if(tmpAuthor.size() != 0 && tmpAuthor.size() < 20)
							themes[(int)id].author = tmpAuthor;

						// detects default icons
						if(smdhData[0x24C0] != '\x9D' && smdhData[0x24C1] != '\x04' && smdhData[0x24C0] != '\xBF' && smdhData[0x24C1] != '\x0D'){
							themes[(int)id].icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
							u16* dst = (u16*)(themes[(int)id].icon->tex.data + 64 * 8 * 2 * sizeof(u16));
							u16* src = (u16*)(&smdhData[0x24C0]);
							for (u8 j = 0; j < 48; j += 8){
								memcpy(dst, src, 48 * 8 * sizeof(u16));
								src += 48 * 8;
								dst += 64 * 8;
							}
						}
					}

					unzCloseCurrentFile(zipFile);
				}
			}
		}

		vector<char> pngData;

		if(unzLocateFile(zipFile, "Preview.png", 0)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(unzOpenCurrentFile(zipFile)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(zippedFileToVector(zipFile, pngData)){
			unzCloseCurrentFile(zipFile);
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		sf2d_texture* tmp;
		printf("%i,%s\n", (int)id, themes[(int)id].fileName.c_str());
		Result res = load_png_mem(pngData, &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		} else
			themes[(int)id].hasPreview = false;

		LightLock_Unlock(&themes[(int)id].lock);
	}

	themes[(int)id].infoIsFullyLoaded = true;
}

void checkInfosToBeLoaded(int id){
	for (int i = 0; i < themes.size(); i++){
		if(i < (id - 4) || i > (id + 4)){
			// just in case the user is scrolling too fast
			while (true){
				if(!LightLock_TryLock(&taskQueueLock))
					break;

				svcSleepThread(1);
			}

			for (size_t j = 0; j < taskQueue.size(); j++){
				if(taskQueue[j].arg == (void*)i){
					if(taskQueue[j].entrypoint == loadThemeInfo){
						printf("DELETING TASK FOR THEME %i\n", i);
						themes[i].infoIsloaded = false;
						themes[i].previewIsLoaded = false;
						taskQueue.erase(taskQueue.begin() + j);
					}

					if(taskQueue[j].entrypoint == loadPreview){
						printf("DELETING TASK FOR THEME %i\n", i);
						themes[i].previewIsLoaded = false;
						taskQueue.erase(taskQueue.begin() + j);
					}
				}
			}

			LightLock_Unlock(&taskQueueLock);

			if(themes[i].hasPreview && themes[i].preview != NULL && !LightLock_TryLock(&themes[i].lock)){
				try {
					themes[i].previewIsLoaded = false;
					sf2d_free_texture(themes[i].preview);
					themes[i].preview = NULL;
				} catch (...){
					LightLock_Unlock(&themes[i].lock);
					printf("error\n");
				}

				LightLock_Unlock(&themes[i].lock);
			}
		} else {
			if(!LightLock_TryLock(&themes[i].lock)){
				if(!themes[i].infoIsloaded){
					//printf("loadThemeInfo 4 %i\n", i);
					themes[i].infoIsloaded = true;
					themes[i].previewIsLoaded = true;
					queueTask(loadThemeInfo, (void*)i);
				} else if(!themes[i].previewIsLoaded){
					//printf("loadPreview   4 %i\n", i);
					themes[i].previewIsLoaded = true;
					queueTask(loadPreview, (void*)i);
				}

				LightLock_Unlock(&themes[i].lock);
			}
		}
	}
}

void installTheme(void* noBGM){
	installProgress = "Reading body_LZ.bin...";

	Result ret = 0;
	vector<char> bodyData;
	vector<char> BGMData;

	// Load data
	if(!themes[currentSelectedItem].isZip){
		if(fileToVector(string("/Themes/") + themes[currentSelectedItem].fileName + "/body_LZ.bin", bodyData))
			return throwError("Failed to open body_LZ.bin file");

		installProgress += "\nReading BGM.bcstm...";

		if(!(bool)noBGM)
			fileToVector(string("/Themes/") + themes[currentSelectedItem].fileName + "/bgm.bcstm", BGMData);
	} else {
		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[currentSelectedItem].fileName)).c_str());

		if(!zipFile)
			return throwError("Failed to open ZIP file");

		if(unzLocateFile(zipFile, "body_LZ.bin", 0))
			return throwError("Can't find body_LZ.bin file in ZIP");

		if(unzOpenCurrentFile(zipFile))
			return throwError("Can't open body_LZ.bin file in ZIP");

		ret = zippedFileToVector(zipFile, bodyData);
		if(ret)
			return throwError(string(string("Can't read body_LZ.bin from ZIP -- error code ") + to_string(ret)).c_str());

		unzCloseCurrentFile(zipFile);

		installProgress += "\nReading BGM.bcstm...";

		if(!(bool)noBGM && !unzLocateFile(zipFile, "bgm.bcstm", 0)){
			if(unzOpenCurrentFile(zipFile))
				return throwError("Can't open bgm.bcstm file in ZIP");

			ret = zippedFileToVector(zipFile, BGMData);
			if(ret)
				return throwError(string(string("Can't read bgm.bcstm from ZIP -- error code ") + to_string(ret)).c_str());

			unzCloseCurrentFile(zipFile);
		}

		installProgress += "\nReading SaveData.dat...";

		unzClose(zipFile);
	}

	// Update SaveData.dat
	u8* saveDataDat_buf;
	u64 saveDataDat_size;
	Handle saveDataDat_handle;

	if(FSUSER_OpenFile(&saveDataDat_handle, ARCHIVE_HomeExt, fsMakePath(PATH_ASCII, "/SaveData.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0))
		return throwError("Failed to open SaveData.dat. Try selecting one of the default color themes on the home menu settings");

	FSFILE_GetSize(saveDataDat_handle, &saveDataDat_size);

	saveDataDat_buf = new u8[saveDataDat_size];
	if(FSFILE_Read(saveDataDat_handle, nullptr, 0, saveDataDat_buf, (u32)saveDataDat_size))
		return throwError("Failed to read SaveData.dat. Try selecting one of the default color themes on the home menu settings");

	installProgress += "\nWriting to SaveData.dat...";

	if(
		!(
			saveDataDat_buf[0x141b] == 0 &&
			saveDataDat_buf[0x13b8] != 0 &&
			saveDataDat_buf[0x13bc] == 0 &&
			saveDataDat_buf[0x13bd] == 3
		)
	){
		saveDataDat_buf[0x141b] = 0; // turn off shuffle
		memset(&saveDataDat_buf[0x13b8], 0, 8); // clear the regular theme structure
		saveDataDat_buf[0x13bd] = 3; // make it persistent
		saveDataDat_buf[0x13b8] = 0xff; // theme index

		if(FSFILE_Write(saveDataDat_handle, nullptr, 0, saveDataDat_buf, saveDataDat_size, FS_WRITE_FLUSH))
			return throwError("Failed to write to SaveData.dat. Try selecting one of the default color themes on the home menu settings");
	}

	free(saveDataDat_buf);
	FSFILE_Close(saveDataDat_handle);

	installProgress += "\nCreating BodyCache.bin...";

	// Inject body_lz.bin into BodyCache.bin
	Handle bodyCacheBin_handle;
	FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"));
	if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), 0, (u64)bodyData.size()))
		return throwError("Failed to create BodyCache.bin. Try selecting one of the default color themes on the home menu settings");

	if(FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open BodyCache.bin. Try selecting one of the default color themes on the home menu settings");

	installProgress += "\nWriting to BodyCache.bin...";

	if(FSFILE_Write(bodyCacheBin_handle, nullptr, 0, &bodyData[0], (u64)bodyData.size(), FS_WRITE_FLUSH))
		return throwError("Failed to write to BodyCache.bin. Try selecting one of the default color themes on the home menu settings");

	FSFILE_Close(bodyCacheBin_handle);

	installProgress += "\nCreating BgmCache.bin...";

	// Inject bgm.bcstm into BgmCache.bin
	Handle bgmCacheBin_handle;
	FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"));
	if(BGMData.size() != 0)
		ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), 0, BGMData.size());
	else
		ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), 0, (u64)3371008);
	if(ret)
		return throwError("Failed to create BgmCache.bin. Try selecting one of the default color themes on the home menu settings");

	if(FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open BgmCache.bin. Try selecting one of the default color themes on the home menu settings");

	installProgress += "\nWriting to BgmCache.bin...";

	if(BGMData.size() != 0)
		ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, &BGMData[0], BGMData.size(), FS_WRITE_FLUSH);
	else {
		char* empty = new char[3371008];
		ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, empty, (u64)3371008, FS_WRITE_FLUSH);
		free(empty);
	}
	if(ret)
		return throwError("Failed to write to BgmCache.bin. Try selecting one of the default color themes on the home menu settings");

	FSFILE_Close(bgmCacheBin_handle);

	installProgress += "\nCreating ThemeManage.bin...";

	// Update ThemeManage.bin
	u8* themeManageBin_buf = new u8[0x800];
	Handle themeManageBin_handle;


	FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"));
	if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), 0, 0x800))
		return throwError("Failed to create ThemeManage.bin. Try selecting one of the default color themes on the home menu settings");

	if(FSUSER_OpenFile(&themeManageBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open ThemeManage.bin. Try selecting one of the default color themes on the home menu settings");

	if(FSFILE_Read(themeManageBin_handle, nullptr, 0, themeManageBin_buf, (u32)0x800))
		return throwError("Failed to read ThemeManage.bin. Try selecting one of the default color themes on the home menu settings");

	installProgress += "\nWriting to ThemeManage.bin...";

	themeManageBin_buf[0x00] = 1;
	themeManageBin_buf[0x01] = 0;
	themeManageBin_buf[0x02] = 0;
	themeManageBin_buf[0x03] = 0;
	themeManageBin_buf[0x04] = 0;
	themeManageBin_buf[0x05] = 0;
	themeManageBin_buf[0x06] = 0;
	themeManageBin_buf[0x07] = 0;
	u32* bodySize_loc = (u32*)(&themeManageBin_buf[0x08]);
	u32* BGMSize_loc = (u32*)(&themeManageBin_buf[0x0C]);
	*bodySize_loc = (u32)bodyData.size();
	*BGMSize_loc = (u32)BGMData.size();
	themeManageBin_buf[0x10] = 0xFF;
	themeManageBin_buf[0x14] = 0x01;
	themeManageBin_buf[0x18] = 0xFF;
	themeManageBin_buf[0x1D] = 0x02;
	memset(&themeManageBin_buf[0x338], 0, 4);
	memset(&themeManageBin_buf[0x340], 0, 4);
	memset(&themeManageBin_buf[0x360], 0, 4);
	memset(&themeManageBin_buf[0x368], 0, 4);

	//u32 thememanage[0x20>>2];

	//memset(thememanage, 0, 0x20);
	//thememanage[0x0>>2] = 1;
	//thememanage[0x8>>2] = (u32)bodyData.size();
	//thememanage[0xC>>2] = (u32)BGMSize;
	//thememanage[0x10>>2] = 0xff;
	//thememanage[0x14>>2] = 1;
	//thememanage[0x18>>2] = 0xff;
	////thememanage[0x1c>>2] = 0x200;
	//thememanage[0x1D>>2] = 0x02;
	//memset(&thememanage[0x338>>2], 0, 4);
	//memset(&thememanage[0x340>>2], 0, 4);
	//memset(&thememanage[0x360>>2], 0, 4);
	//memset(&thememanage[0x368>>2], 0, 4);

	//memset(themeManageBin_buf, 0, 0x800);
	//memcpy(themeManageBin_buf, thememanage, 0x20);

	if(FSFILE_Write(themeManageBin_handle, nullptr, 0, themeManageBin_buf, 0x800, FS_WRITE_FLUSH))
		return throwError("Failed to write to ThemeManage.bin. Try selecting one of the default color themes on the home menu settings");

	FSFILE_Close(themeManageBin_handle);

	isInstalling = false;
	installProgress = "";
}

void toggleBGM(){
	// TODO: zip support
	if(!themes[currentSelectedItem].hasBGM)
		return;

	string path;

	if(themes[currentSelectedItem].isZip){
		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[currentSelectedItem].fileName)).c_str());

		if(!zipFile)
			return throwError("Failed to open ZIP file");

		if(unzLocateFile(zipFile, "bgm.ogg", 0))
			return throwError("Can't find bgm.ogg file in ZIP");

		if(unzOpenCurrentFile(zipFile))
			return throwError("Can't open bgm.ogg file in ZIP");

		std::vector<char> oggData;
		int ret = zippedFileToVector(zipFile, oggData);
		if(ret)
			return throwError(string(string("Can't read bgm.ogg from ZIP -- error code ") + to_string(ret)).c_str());

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		remove("/3ds/Themely/tmp/bgm.ogg");
		ofstream tmpOggFile("/3ds/Themely/tmp/bgm.ogg", ios::out | ios::binary);
		tmpOggFile.write(&oggData[0], oggData.size());
		tmpOggFile.close();

		path = "/3ds/Themely/tmp/bgm.ogg";
	} else
		path = string("/Themes/") + themes[currentSelectedItem].fileName + "/bgm.ogg";

	if(!audioIsPlaying && currentPlayingAudio == NULL){
		audioIsPlaying = true;
		AUDIO_load(path);
	} else
		AUDIO_stop();
}
