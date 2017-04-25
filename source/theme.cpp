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

	while (true) {
		u32 entriesRead;
		FS_DirectoryEntry entry;
		FSDIR_Read(themeDir, &entriesRead, 1, &entry);
		if(entriesRead) {
			char fileName[0x106];
			utf2ascii(fileName, entry.name);

			if(entry.attributes & FS_ATTRIBUTE_DIRECTORY){
				if(fileExists("/Themes/" + string(fileName) + "/body_LZ.bin")){
					Theme theme = {
						string(fileName),
						string(fileName),
						"[description not available]",
						"Unknown",
						NULL,
						NULL,
						false,
						fileExists("/Themes/" + string(fileName) + "/bgm.ogg"),
						fileExists("/Themes/" + string(fileName) + "/Preview.png"),
						NULL
					};

					//LightLock_Init(&theme.lock_icon);
					LightLock_Init(&theme.preview_lock);

					ifstream smdhFile("/Themes/" + string(fileName) + "/info.smdh", ios::in | ios::binary);
					if(smdhFile.is_open()) {
						char* buffer = new char[0x520];
						smdhFile.read(buffer, 0x520);

						int offset = 0x8;

						string tmpTitle = "";
						string tmpDescription = "";
						string tmpAuthor = "";

						for (size_t i = 0; i < 128; i++) {
							if(buffer[offset + i] == '\00')
								break;

							tmpTitle += buffer[offset + i];
							offset++;
						}

						offset = 0x8 + 128;

						for (size_t i = 0; i < 256; i++) {
							if(buffer[offset + i] == '\00')
								break;

							tmpDescription += buffer[offset + i];
							offset++;
						}

						offset = 0x8 + 128 + 256;

						for (size_t i = 0; i < 128; i++) {
							if(buffer[offset + i] == '\00')
								break;

							tmpAuthor += buffer[offset + i];
							offset++;
						}

						//if(tmpTitle.find_first_not_of('\00') != string::npos)
						if(tmpTitle.size() != 0)
							theme.title = tmpTitle;

						if(tmpDescription.size() != 0)
							theme.description = tmpDescription;

						if(tmpAuthor.size() != 0)
							theme.author = tmpAuthor;

						smdhFile.seekg(0x24C0);
						char* iconBuf = new char[0x1200];
						smdhFile.read(iconBuf, 0x1200);

						// detects default icons
						if(iconBuf[0] != '\x9D' && iconBuf[1] != '\x04' && iconBuf[0] != '\xBF' && iconBuf[1] != '\x0D'){
							theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
							u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
							u16* src = (u16*)(iconBuf);
							for (u8 j = 0; j < 48; j += 8){
								memcpy(dst, src, 48 * 8 * sizeof(u16));
								src += 48 * 8;
								dst += 64 * 8;
							}
						}

						//memcpy(theme.icon->tex.data, iconBuf, 48*48*2);
						//theme.icon->tiled = 1;
					}

					smdhFile.close();

					themes.push_back(theme);
				}
			} else if(
				entry.shortExt[0] == 'Z' &&
				entry.shortExt[1] == 'I' &&
				entry.shortExt[2] == 'P') {
				unzFile zipFile = unzOpen(string("/Themes/" + string(fileName)).c_str());

				if(!zipFile || unzLocateFile(zipFile, "body_LZ.bin", 0))
					continue;

				Theme theme = {
					string(fileName),
					string(fileName),
					"[description not available]",
					"Unknown",
					NULL,
					NULL,
					true,
					false,
					false,
					NULL
				};

				LightLock_Init(&theme.preview_lock);

				// check if bgm exists
				if(!unzLocateFile(zipFile, "bgm.ogg", 0))
					theme.hasBGM = true;

				// check if preview exists
				if(!unzLocateFile(zipFile, "Preview.png", 0))
					theme.hasPreview = true;

				vector<char> smdhData;
				if(!unzLocateFile(zipFile, "info.smdh", 0) && !unzOpenCurrentFile(zipFile) && !zippedFileToVector(zipFile, smdhData)){
					if(smdhData.size() == 0x36C0){
						int offset = 0x8;

						string tmpTitle = "";
						string tmpDescription = "";
						string tmpAuthor = "";

						for (size_t i = 0; i < 128; i++) {
							if(smdhData[offset + i] == '\00')
								break;

							tmpTitle += smdhData[offset + i];
							offset++;
						}

						offset = 0x8 + 128;

						for (size_t i = 0; i < 256; i++) {
							if(smdhData[offset + i] == '\00')
								break;

							tmpDescription += smdhData[offset + i];
							offset++;
						}

						offset = 0x8 + 128 + 256;

						for (size_t i = 0; i < 128; i++) {
							if(smdhData[offset + i] == '\00')
								break;

							tmpAuthor += smdhData[offset + i];
							offset++;
						}

						if(tmpTitle.size() != 0)
							theme.title = tmpTitle;

						if(tmpDescription.size() != 0)
							theme.description = tmpDescription;

						if(tmpAuthor.size() != 0)
							theme.author = tmpAuthor;

						// detects default icons
						if(smdhData[0x24C0] != '\x9D' && smdhData[0x24C1] != '\x04' && smdhData[0x24C0] != '\xBF' && smdhData[0x24C1] != '\x0D'){
							theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
							u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
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

				themes.push_back(theme);

				unzClose(zipFile);

				// open zip
				/*
				long zipFileSize;
				char* zipFileBuffer;
				FILE* zipFileStream = fopen(string("/Themes/" + string(fileName)).c_str(), "rb");

				if(!zipFileStream)
					continue;

				fseek(zipFileStream, 0L, SEEK_END);
				zipFileSize = ftell(zipFileStream);
				rewind(zipFileStream);

				zipFileBuffer = new char[zipFileSize + 1];
				fread(zipFileBuffer, zipFileSize, 1, zipFileStream);
				fclose(zipFileStream);
				*/
				/*
				char* archive_zip = NULL;
				size_t archive_zip_size = read_file_to_mem(&archive_zip, string("/Themes/" + string(fileName)).c_str(), 0);

				mz_zip_archive zipFile;
				memset(&zipFile, 0, sizeof(zipFile));
				mz_bool status = mz_zip_reader_init_mem(&zipFile, archive_zip, archive_zip_size, 0);
				//mz_bool status = mz_zip_reader_init_file(&zipFile, string("/Themes/" + string(fileName)).c_str(), 0);

				free(archive_zip);

				if(!status){
					printf("!!failed to open zip file %s\n", string("/Themes/" + string(fileName)).c_str());
					continue;
				}

				// check if body exists
				if(mz_zip_reader_locate_file(&zipFile, "body_LZ.bin", "", 0) == -1){
					// doesn't exist
					mz_zip_reader_end(&zipFile);
					printf("no body found for %s\n", string("/Themes/" + string(fileName)).c_str());
					continue;
				}

				Theme theme = {
					string(fileName),
					string(fileName),
					"[description not available]",
					"Unknown",
					NULL,
					NULL,
					true,
					false,
					false,
					NULL
				};

				LightLock_Init(&theme.preview_lock);

				// check if bgm exists
				if(mz_zip_reader_locate_file(&zipFile, "bgm.bcstm", "", 0) != -1)
					theme.hasBGM = true;

				// check if preview exists
				if(mz_zip_reader_locate_file(&zipFile, "Preview.png", "", 0) != -1)
					theme.hasPreview = true;

				// read smdh
				size_t smdhSize;
				char* smdhData = (char*)mz_zip_reader_extract_file_to_heap(&zipFile, "info.smdh", &smdhSize, 0);

				if(smdhData && smdhSize == 0x36C0){
					int offset = 0x8;

					string tmpTitle = "";
					string tmpDescription = "";
					string tmpAuthor = "";

					for (size_t i = 0; i < 128; i++) {
						tmpTitle += smdhData[offset + i];
						offset++;
					}

					for (size_t i = 0; i < 256; i++) {
						tmpDescription += smdhData[offset + i];
						offset++;
					}

					for (size_t i = 0; i < 128; i++) {
						tmpAuthor += smdhData[offset + i];
						offset++;
					}

					if(tmpTitle.find_first_not_of('\00') != string::npos)
						theme.title = tmpTitle;

					if(tmpDescription.find_first_not_of('\00') != string::npos)
						theme.description = tmpDescription;

					if(tmpAuthor.find_first_not_of('\00') != string::npos)
						theme.author = tmpAuthor;

					// detects default icons
					if(smdhData[0x24C0] != '\x9D' && smdhData[0x24C1] != '\x04' && smdhData[0x24C0] != '\xBF' && smdhData[0x24C1] != '\x0D'){
						theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
						u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
						u16* src = (u16*)(&smdhData[0x24C0]);
						for (u8 j = 0; j < 48; j += 8){
							memcpy(dst, src, 48 * 8 * sizeof(u16));
							src += 48 * 8;
							dst += 64 * 8;
						}
					}

					themes.push_back(theme);
				}

				free(smdhData);

				mz_zip_reader_end(&zipFile);
				*/

				/*
				Zip* zipFile = ZipOpen(string("/Themes/" + string(fileName)).c_str());

				if(zipFile == NULL) {
					printf("!!failed to open zip file %s\n", string("/Themes/" + string(fileName)).c_str());
					continue;
				}

				try {
					ZipFile* file = ZipFileRead(zipFile, "body_LZ.bin", NULL);

					if(file == NULL) {
						ZipClose(zipFile);
						printf("no body found for %s\n", string("/Themes/" + string(fileName)).c_str());
						continue;
					}

					ZipFileFree(file);

					Theme theme = {
						string(fileName),
						string(fileName),
						"[description not available]",
						"Unknown",
						NULL,
						NULL,
						true,
						false,
						false
					};

					ZipFile* bgm = ZipFileRead(zipFile, "bgm.bcstm", NULL);

					if(bgm){
						ZipFileFree(bgm);
						theme.hasBGM = true;
					}

					//LightLock_Init(&theme.lock_icon);
					//LightLock_Init(&theme.preview_lock);

					ZipFile* smdhFile = ZipFileRead(zipFile, "info.smdh", NULL);

					if(smdhFile) {
						int offset = 0x8;

						string tmpTitle = "";
						string tmpDescription = "";
						string tmpAuthor = "";

						for (size_t i = 0; i < 128; i++) {
							tmpTitle += smdhFile->data[offset + i];
							offset++;
						}

						for (size_t i = 0; i < 256; i++) {
							tmpDescription += smdhFile->data[offset + i];
							offset++;
						}

						for (size_t i = 0; i < 128; i++) {
							tmpAuthor += smdhFile->data[offset + i];
							offset++;
						}

						if(tmpTitle.find_first_not_of(' ') != string::npos)
							theme.title = tmpTitle;

						if(tmpDescription.find_first_not_of(' ') != string::npos)
							theme.description = tmpDescription;

						if(tmpAuthor.find_first_not_of(' ') != string::npos)
							theme.author = tmpAuthor;

						theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
						u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
						u16* src = (u16*)(&smdhFile->data[0x24C0]);
						for (u8 j = 0; j < 48; j += 8){
							memcpy(dst, src, 48 * 8 * sizeof(u16));
							src += 48 * 8;
							dst += 64 * 8;
						}

						themes.push_back(theme);

						ZipFileFree(smdhFile);
					}

					ZipClose(zipFile);
				} catch(...) {
					printf("!!expection\n" );
					continue;
				}

				*/



				//unzFile zipFile = unzOpen(string("/Themes/" + string(fileName)).c_str());

				//if(zipFile == NULL){
				//	printf("!!failed to open zip file %s\n", string("/Themes/" + string(fileName)).c_str());
				//	continue;
				//}

				//try {
				//	if(unzLocateFile(zipFile, "body_LZ.bin", nullptr) == UNZ_END_OF_LIST_OF_FILE) {
				//		unzClose(zipFile);
				//		printf("no body found for %s\n", string("/Themes/" + string(fileName)).c_str());
				//		continue;
				//	}

				//	Theme theme = {
				//		string(fileName),
				//		string(fileName),
				//		"[description not available]",
				//		"Unknown author",
				//		NULL,
				//		NULL,
				//		false
				//	};

				//	LightLock_Init(&theme.lock_icon);
				//	LightLock_Init(&theme.preview_lock);

				//	if(unzLocateFile(zipFile, "info.smdh", nullptr) == UNZ_OK) {
				//		if(unzOpenCurrentFile(zipFile) != UNZ_OK)
				//			continue;

				//		char smdhFile[0x36C0];

				//		unzReadCurrentFile(zipFile, smdhFile, 0x36C0);

				//		int offset = 0x8;

				//		for (size_t i = 0; i < 128; i++) {
				//			theme.title += smdhFile[offset + i];
				//			offset++;
				//		}

				//		for (size_t i = 0; i < 256; i++) {
				//			theme.description += smdhFile[offset + i];
				//			offset++;
				//		}

				//		for (size_t i = 0; i < 128; i++) {
				//			theme.author += smdhFile[offset + i];
				//			offset++;
				//		}
				//	}
				//} catch (...) {
				//	unzClose(zipFile);
				//	printf("!!expection\n" );
				//	continue;
				//}
			}
		} else
			break;
	}

	FSDIR_Close(themeDir);

	themesScanned = true;

	for (size_t i = 0; i < themes.size(); i++) {
		printf("theme:%s by %s\n", themes[i].title.c_str(), themes[i].author.c_str());
		queueTask(loadPreview, (void*)i);
	}
}

void loadPreview(void* id){
	while (true) {
		if(!LightLock_TryLock(&themes[(int)id].preview_lock))
			break;

		svcSleepThread(1);
	}

	if(!themes[(int)id].isZip){
		sf2d_texture* tmp;
		Result res = load_png((string("/Themes/") + themes[(int)id].fileName + "/Preview.png").c_str(), &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		}

		//sf2d_free_texture(tmp);
	} else {
		// open zip
		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[(int)id].fileName)).c_str());

		vector<char> pngData;
		if(zipFile && !unzLocateFile(zipFile, "Preview.png", 0) && !unzOpenCurrentFile(zipFile) && !zippedFileToVector(zipFile, pngData)){
			unzCloseCurrentFile(zipFile);

			sf2d_texture* tmp;
			Result res = load_png_mem(pngData, &tmp);

			if(!res){
				themes[(int)id].preview = tmp;
				C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
			}
		}

		unzClose(zipFile);

		/*
		char* archive_zip = NULL;
		size_t archive_zip_size = read_file_to_mem(&archive_zip, string("/Themes/" + string(themes[(int)id].fileName)).c_str(), 0);

		mz_zip_archive zipFile;
		memset(&zipFile, 0, sizeof(zipFile));
		mz_bool status = mz_zip_reader_init_mem(&zipFile, archive_zip, archive_zip_size, 0);
		//mz_bool status = mz_zip_reader_init_file(&zipFile, string("/Themes/" + string(fileName)).c_str(), 0);

		free(archive_zip);

		if(status) {
			size_t pngSize;
			char* pngData = (char*)mz_zip_reader_extract_file_to_heap(&zipFile, "Preview.png", &pngSize, 0);

			if(pngData && pngSize != 0){
				sf2d_texture* tmp;
				Result res = load_png_mem((void*)pngData, pngSize, &tmp);

				if(!res){
					themes[(int)id].preview = tmp;
					C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
				}
			}

			free(pngData);
			mz_zip_reader_end(&zipFile);
		}
		*/
	}

	LightLock_Unlock(&themes[(int)id].preview_lock);
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
		return throwError("Failed to open SaveData.dat");

	FSFILE_GetSize(saveDataDat_handle, &saveDataDat_size);

	saveDataDat_buf = new u8[saveDataDat_size];
	if(FSFILE_Read(saveDataDat_handle, nullptr, 0, saveDataDat_buf, (u32)saveDataDat_size))
		return throwError("Failed to read SaveData.dat");

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
			return throwError("Failed to write to SaveData.dat");
	}

	free(saveDataDat_buf);
	FSFILE_Close(saveDataDat_handle);

	installProgress += "\nCreating BodyCache.bin...";

	// Inject body_lz.bin into BodyCache.bin
	Handle bodyCacheBin_handle;
	FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"));
	if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), 0, (u64)bodyData.size()))
		return throwError("Failed to create BodyCache.bin");

	if(FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open BodyCache.bin");

	installProgress += "\nWriting to BodyCache.bin...";

	if(FSFILE_Write(bodyCacheBin_handle, nullptr, 0, &bodyData[0], (u64)bodyData.size(), FS_WRITE_FLUSH))
		return throwError("Failed to write to BodyCache.bin");

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
		return throwError("Failed to create BgmCache.bin");

	if(FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open BgmCache.bin");

	installProgress += "\nWriting to BgmCache.bin...";

	if(BGMData.size() != 0)
		ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, &BGMData[0], BGMData.size(), FS_WRITE_FLUSH);
	else {
		char* empty = new char[3371008];
		ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, empty, (u64)3371008, FS_WRITE_FLUSH);
		free(empty);
	}
	if(ret)
		return throwError("Failed to write to BgmCache.bin");

	FSFILE_Close(bgmCacheBin_handle);

	installProgress += "\nCreating ThemeManage.bin...";

	// Update ThemeManage.bin
	u8* themeManageBin_buf = new u8[0x800];
	Handle themeManageBin_handle;


	FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"));
	if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), 0, 0x800))
		return throwError("Failed to create ThemeManage.bin");

	if(FSUSER_OpenFile(&themeManageBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), FS_OPEN_WRITE, 0))
		return throwError("Failed to open ThemeManage.bin");

	if(FSFILE_Read(themeManageBin_handle, nullptr, 0, themeManageBin_buf, (u32)0x800))
		return throwError("Failed to read ThemeManage.bin");

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
		return throwError("Failed to write to ThemeManage.bin");

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
