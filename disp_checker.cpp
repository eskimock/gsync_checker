#include <iostream>
#include "nvapi.h"
#pragma comment(lib, "E:\\project\\repos\\getGsyncInfo\\getGsyncInfo\\R560-developer\\x86\\nvapi.lib")

int main() {
	// ��ʼ�� NVAPI
	NvAPI_Status status = NvAPI_Initialize();
	if (status != NVAPI_OK) {
		std::cerr << "NVAPI ��ʼ��ʧ��" << std::endl;
		return 1;
	}

	// ��ȡ���� GPU ���
	NvPhysicalGpuHandle gpuHandle[NVAPI_MAX_PHYSICAL_GPUS];
	NvU32 gpuCount = 0;
	status = NvAPI_EnumPhysicalGPUs(gpuHandle, &gpuCount);
	if (status != NVAPI_OK || gpuCount == 0) {
		std::cerr << "�޷�ö�� GPU" << std::endl;
		NvAPI_Unload();
		return 1;
	}

	// ��ȡ��ʾ�����
	NvDisplayHandle displayHandle;
	status = NvAPI_EnumNvidiaDisplayHandle(0, &displayHandle); // �������һ����ʾ��
	if (status != NVAPI_OK) {
		std::cerr << "�޷���ȡ��ʾ�����" << std::endl;
		NvAPI_Unload();
		return 1;
	}

	// ��ȡ����ʾ��ID
	NvU32 displayId;
	status = NvAPI_DISP_GetGDIPrimaryDisplayId(&displayId);
	if (status != NVAPI_OK) {
		std::cerr << "�޷���ȡ����ʾ��ID" << std::endl;
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
			std::cout << "��ʾ��֧�� G-SYNC" << std::endl;
		}
		else {
			std::cout << "��ʾ����֧�� G-SYNC" << std::endl;
		}
	}
	else {
		std::cerr << "�޷���ѯ G-SYNC ֧��״̬" << std::endl;
	}

	// ж�� NVAPI
	NvAPI_Unload();

	system("pause");
	return 0;
}