// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "PathBuilder.h"
#include "PathDestroyer.h"
#include "ImageConvert.h"
#include "PathData.h"

IDebugLog glog;
PluginHandle g_pluginHandle = kPluginHandle_Invalid;

__declspec(dllexport) PathDataParent* PathDataCreate()
{
	return new PathData();
}
__declspec(dllexport) PathBuilderParent* PathBuilderCreate(PathDataParent& _pathData)
{
	return new PathBuilder(_pathData);
}
__declspec(dllexport) PathDestoryerParent* PathDestoryerCreate(PathDataParent& _pathData)
{
	return new PathDestoryer(_pathData);
}
__declspec(dllexport) ImageConvertParent* ImageConvertCreate(PathDataParent& _pathData)
{
	return new ImageConvert(_pathData);
}
PathDataParent* pathData = PathDataCreate();

extern "C"
{
	__declspec(dllexport) F4SEPluginVersionData F4SEPlugin_Version =
	{
		F4SEPluginVersionData::kVersion,

		PLUGIN_VERSION,
		"ALR",
		"Hudd",

		1,	// not version independent
		1,	// not version independent (extended field)
		{ CURRENT_RELEASE_RUNTIME, 0 },	// compatible with 1.10.980

		1,	// works with any version of the script extender. you probably do not need to put anything here
	};

	__declspec(dllexport) bool F4SEPlugin_Load(const F4SEInterface* f4se)
	{
		_MESSAGE("%s loaded", PLUGIN_NAME);

		PathBuilderParent* PBinst = PathBuilderCreate(*pathData);
		_MESSAGE("%s created path data", PLUGIN_NAME);

		ImageConvertParent* IMinst = ImageConvertCreate(*pathData);
		_MESSAGE("%s converted all images", PLUGIN_NAME);

		delete PBinst;
		delete IMinst;
		return true;
	}
};

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:

		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:

		PathDestoryerParent* PDinst = PathDestoryerCreate(*pathData);
		delete PDinst;
		delete& pathData;
		_MESSAGE("%s destroyed all images/paths", PLUGIN_NAME);
		// Perform any necessary cleanup.
		break;
	}

	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
