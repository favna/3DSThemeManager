// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "network.h"

string update;

Result HTTPGet(vector<char>& returnedVec, string url){
	printf("URL:%s\n", url.c_str());

	// thx https://github.com/devkitPro/3ds-examples/blob/master/network/http/source/main.c
	Result ret = 0;
	httpcContext context;
	u8 *buf, *lastbuf;
	u32 statuscode = 0,
		contentsize = 0,
		readsize = 0,
		size = 0;

	ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 1);
	ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
	ret = httpcAddRequestHeaderField(&context, "User-Agent", (string("Themely/") + VERSION).c_str());
	ret = httpcAddRequestHeaderField(&context, "Accept-Language", "en");
	ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");

	if(closing) return -1;

	ret = httpcBeginRequest(&context);
	if(ret != 0){
		/*printf("35-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		return ret;
	}

	if(closing) return -1;

	ret = httpcGetResponseStatusCode(&context, &statuscode);
	if(ret != 0){
		/*printf("42-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		return ret;
	}

	if(closing) return -1;

	if(statuscode != 200){
		/*printf("48-STATUSCODE:%i\n", statuscode);*/

		httpcCloseContext(&context);
		return -1;
	}

	if(closing) return -1;

	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if(ret != 0){
		/*printf("55-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		return ret;
	}

	if(closing) return -1;

	buf = (u8*)malloc(0x1000);
	if(buf == NULL){
		/*printf("62-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		return ret;
	}

	do {
		if(closing) return -1;

		ret = httpcDownloadData(&context, buf + size, 0x1000, &readsize);
		size += readsize;
		if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING){
				lastbuf = buf;
				buf = (u8*)realloc(buf, size + 0x1000);
				if(buf == NULL){
					/*printf("74-RET:%lX\n", ret);*/

					httpcCloseContext(&context);
					free(lastbuf);
					return ret;
				}
			}
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

	if(closing) return -1;

	if(ret != 0){
		/*printf("83-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		free(buf);
		return ret;
	}

	if(closing) return -1;

	lastbuf = buf;
	buf = (u8*)realloc(buf, size);
	if(buf == NULL){
		/*printf("92-RET:%lX\n", ret);*/

		httpcCloseContext(&context);
		free(lastbuf);
		return ret;
	}

	if(closing) return -1;

	httpcCloseContext(&context);

	//if(buf[0] == '!'){
	//	string err = reinterpret_cast<char*>(buf);
	//	return ret;
	//}

	for (int i = 0; i < size; i++)
		returnedVec.push_back(buf[i]);

	return 0;
}

void checkForUpdate(void*){
	vector<char> httpData;
	Result ret = HTTPGet(httpData, "http://app.3dsthem.es/updateCheck.php");
	if(ret){
		printf("Failed to check for an update.\n");
		return;
	}

	if(httpData.size() != 0 && httpData[0] == '!'){
		httpData.erase(httpData.begin());

		update = string(httpData.begin(), httpData.end());
	}
}

void installUpdate(){
	if(envIsHomebrew()){
		vector<char> threedsxData;
		Result ret = HTTPGet(threedsxData, "http://app.3dsthem.es/release/latest/Themely.3dsx");
		if(ret)
			return throwError("Failed to download update 3DSX. If you're still having problems,\nupdate manually.");

		Handle threedsxHandle;
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/Themely.3dsx"));
		if(FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/Themely.3dsx"), 0, (u64)threedsxData.size()))
			return throwError("Failed to create Themely.3dsx");

		if(FSUSER_OpenFile(&threedsxHandle, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_WRITE, 0))
			return throwError("Failed to open Themely.3dsx");

		if(FSFILE_Write(threedsxHandle, nullptr, 0, &threedsxData[0], (u64)threedsxData.size(), FS_WRITE_FLUSH))
			return throwError("Failed to write to Themely.3dsx.");

		FSFILE_Close(threedsxHandle);
	} else {
		vector<char> ciaData;
		Result ret = HTTPGet(ciaData, "http://app.3dsthem.es/release/latest/Themely.cia");
		if(ret)
			return throwError("Failed to download update CIA. If you're still having problems, update through FBI -> TitleDB.");

		amInit();
		Handle handle;
		AM_QueryAvailableExternalTitleDatabase(NULL);
		AM_StartCiaInstall(MEDIATYPE_SD, &handle);
		FSFILE_Write(handle, nullptr, 0, &ciaData[0], (u64)ciaData.size(), 0);
		AM_FinishCiaInstall(handle);
		amExit();
	}

	closing = true;
}
