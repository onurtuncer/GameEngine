#include "Hazel/Core/Base.h"

#include "Hazel/Core/Log.h"

#include <Windows.h>

#define HAZEL_BUILD_ID "v0.1a"

namespace Hazel {

	void InitializeCore()
	{
		Hazel::Log::Init();

		HZ_CORE_TRACE("Hazel Engine {}", HAZEL_BUILD_ID);
		HZ_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		HZ_CORE_TRACE("Shutting down...");
	}

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hazel::InitializeCore();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Hazel::ShutdownCore();
		break;
	}
	return TRUE;
}