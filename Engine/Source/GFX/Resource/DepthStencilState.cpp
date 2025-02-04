#include "GFX/Resource/DepthStencilState.h"
#include "GFX/Resource/GfxDebugName.h"
#include "GFX/Graphics.h"

namespace ZE::GFX::Resource
{
	DepthStencilState::DepthStencilState(Graphics& gfx, StencilMode mode) : mode(mode)
	{
		ZE_GFX_ENABLE_ALL(gfx);

		D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
		switch (mode)
		{
		case StencilMode::Write:
		{
			desc.DepthEnable = FALSE;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.StencilEnable = TRUE;
			desc.StencilWriteMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			break;
		}
		case StencilMode::Mask:
		{
			desc.DepthEnable = FALSE;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.StencilEnable = TRUE;
			desc.StencilReadMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		}
		case StencilMode::DepthOff:
		{
			desc.DepthEnable = FALSE;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		}
		case StencilMode::Reverse:
		{
			desc.DepthFunc = D3D11_COMPARISON_GREATER;
			break;
		}
		case StencilMode::DepthFirst:
		{
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		}
		}
		ZE_GFX_THROW_FAILED(GetDevice(gfx)->CreateDepthStencilState(&desc, &state));
		ZE_GFX_SET_RID(state.Get());
	}
}