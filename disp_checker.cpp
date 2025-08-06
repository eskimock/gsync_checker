#include <iostream>
#include "nvapi.h"
#pragma comment(lib, "E:\\project\\repos\\getGsyncInfo\\getGsyncInfo\\R560-developer\\x86\\nvapi.lib")

int main() {
	// 初始化 NVAPI
	NvAPI_Status status = NvAPI_Initialize();
	if (status != NVAPI_OK) {
		std::cerr << "NVAPI 初始化失败" << std::endl;
		return 1;
	}

	// 获取物理 GPU 句柄
	NvPhysicalGpuHandle gpuHandle[NVAPI_MAX_PHYSICAL_GPUS];
	NvU32 gpuCount = 0;
	status = NvAPI_EnumPhysicalGPUs(gpuHandle, &gpuCount);
	if (status != NVAPI_OK || gpuCount == 0) {
		std::cerr << "无法枚举 GPU" << std::endl;
		NvAPI_Unload();
		return 1;
	}

	// 获取显示器句柄
	NvDisplayHandle displayHandle;
	status = NvAPI_EnumNvidiaDisplayHandle(0, &displayHandle); // 假设检查第一个显示器
	if (status != NVAPI_OK) {
		std::cerr << "无法获取显示器句柄" << std::endl;
		NvAPI_Unload();
		return 1;
	}

	// 获取主显示器ID
	NvU32 displayId;
	status = NvAPI_DISP_GetGDIPrimaryDisplayId(&displayId);
	if (status != NVAPI_OK) {
		std::cerr << "无法获取主显示器ID" << std::endl;
		NvAPI_Unload();
		return 1;
	}

	NV_MONITOR_CAPABILITIES caps;
	memset(&caps, 0, sizeof(caps));
	caps.version = NV_MONITOR_CAPABILITIES_VER;
	caps.size = sizeof(NV_MONITOR_CAPABILITIES);
	caps.infoType = NV_MONITOR_CAPS_TYPE_GENERIC;
	status = NvAPI_DISP_GetMonitorCapabilities(displayId, &caps);
	if (status == NVAPI_OK) {
		if (caps.data.caps.isTrueGsync) {
			std::cout << "显示器支持 G-SYNC" << std::endl;
		}
		else {
			std::cout << "显示器不支持 G-SYNC" << std::endl;
		}
	}
	else {
		std::cerr << "无法查询 G-SYNC 支持状态" << std::endl;
	}

	// 卸载 NVAPI
	NvAPI_Unload();

	system("pause");
	return 0;
}