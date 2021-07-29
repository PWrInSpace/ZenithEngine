#pragma once
#include "D3D11.h"

namespace ZE::GFX::API::DX11
{
	class CommandList final
	{
		DX::ComPtr<ID3D11CommandList> commands;

	public:
		CommandList() = default;
		CommandList(CommandList&&) = default;
		CommandList(const CommandList&) = delete;
		CommandList& operator=(CommandList&&) = default;
		CommandList& operator=(const CommandList&) = delete;
		~CommandList() = default;

		ID3D11CommandList** GetList() noexcept { return commands.GetAddressOf(); }
	};
}