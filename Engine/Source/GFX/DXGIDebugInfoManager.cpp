#include "GFX/DXGIDebugInfoManager.h"
#include "Exception/WinApiException.h"
#include "Exception/GfxExceptionMacros.h"
#include <combaseapi.h>

namespace ZE::GFX
{
	DXGIDebugInfoManager::DXGIDebugInfoManager()
	{
		ZE_GFX_ENABLE_NOINFO();
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		const auto dxgiDebugModule = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (dxgiDebugModule == nullptr)
			throw ZE_WIN_EXCEPT_LAST();

		const auto dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(dxgiDebugModule, "DXGIGetDebugInterface")));
		if (dxgiGetDebugInterface == nullptr)
			throw ZE_WIN_EXCEPT_LAST();

		ZE_GFX_THROW_FAILED_NOINFO(dxgiGetDebugInterface(IID_PPV_ARGS(&infoQueue)));
		ZE_GFX_THROW_FAILED_NOINFO(dxgiGetDebugInterface(IID_PPV_ARGS(&debug)));
	}

	std::vector<std::string> DXGIDebugInfoManager::GetMessages() const
	{
		ZE_GFX_ENABLE_NOINFO();

		std::vector<std::string> messages;
		const U64 end = infoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (U64 i = offset; i < end; ++i)
		{
			SIZE_T msgLen = 0;
			ZE_GFX_THROW_FAILED_NOINFO(infoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLen)); // Get length of msg

			std::unique_ptr<U8[]> bytes = std::make_unique<U8[]>(msgLen);
			DXGI_INFO_QUEUE_MESSAGE* msg = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			ZE_GFX_THROW_FAILED_NOINFO(infoQueue->GetMessage(DXGI_DEBUG_ALL, i, msg, &msgLen)); // Get msg

			messages.emplace_back(msg->pDescription);
		}
		return messages;
	}
}