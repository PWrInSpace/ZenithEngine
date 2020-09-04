#include "DepthStencilShaderInput.h"
#include "GfxExceptionMacros.h"

namespace GFX::Pipeline::Resource
{
	inline DXGI_FORMAT DepthStencilShaderInput::UsageShaderInput(Usage usage) noexcept
	{
		switch (usage)
		{
		default:
		case Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case Usage::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		}
	}

	DepthStencilShaderInput::DepthStencilShaderInput(Graphics& gfx, unsigned int width, unsigned int height, UINT slot, Usage usage)
		: DepthStencil(gfx, width, height, true, usage), slot(slot)
	{
		GFX_ENABLE_ALL(gfx);

		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		depthStencilView->GetResource(&resource);

		D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {};
		textureViewDesc.Format = UsageShaderInput(usage);
		textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		textureViewDesc.Texture2D.MipLevels = 1U;
		textureViewDesc.Texture2D.MostDetailedMip = 0U;
		GFX_THROW_FAILED(GetDevice(gfx)->CreateShaderResourceView(resource.Get(), &textureViewDesc, &textureView));
	}

	Surface DepthStencilShaderInput::ToSurface(Graphics& gfx, bool linearScale) const
	{
		GFX_ENABLE_ALL(gfx);

		// Create temporary texture to read from CPU side
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		textureView->GetResource(&resource);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		resource.As(&texture);
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		texture->GetDesc(&textureDesc);
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
		textureDesc.BindFlags = 0U;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> textureStaged;
		GFX_THROW_FAILED(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &textureStaged));
		GFX_THROW_FAILED_INFO(GetContext(gfx)->CopyResource(textureStaged.Get(), texture.Get()));

		Surface surface(GetWidth(), GetHeight());
		D3D11_MAPPED_SUBRESOURCE subResource = { 0 };
		GFX_THROW_FAILED_INFO(GetContext(gfx)->Map(textureStaged.Get(), 0U, D3D11_MAP::D3D11_MAP_READ, 0U, &subResource));
		const char* bytes = static_cast<const char*>(subResource.pData);
		for (unsigned int y = 0U; y < GetHeight(); ++y)
		{
			const Surface::Pixel* row = reinterpret_cast<const Surface::Pixel*>(bytes + subResource.RowPitch * static_cast<size_t>(y));
			for (unsigned int x = 0U; x < GetWidth(); ++x)
			{
				switch (textureDesc.Format)
				{
				case DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS:
				{
					const auto raw = 0xFFFFFF & (row + x)->GetValue();
					if (linearScale)
					{
						const float normalized = static_cast<float>(raw) / static_cast<float>(0xFFFFFF);
						const float linearized = 0.01f / (1.01f - normalized);
						const uint8_t channel = static_cast<uint8_t>(linearized * 255.0f);
						surface.PutPixel(x, y, { channel, channel, channel });
					}
					else
					{
						const uint8_t channel = raw >> 16;
						surface.PutPixel(x, y, { channel, channel, channel });
					}
					break;
				}
				case DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS:
				{
					const auto raw = *reinterpret_cast<const float*>(row + x);
					if (linearScale)
					{
						const float linearized = 0.01f / (1.01f - raw);
						const uint8_t channel = static_cast<uint8_t>(linearized * 255.0f);
						surface.PutPixel(x, y, { channel, channel, channel });
					}
					else
					{
						const uint8_t channel = static_cast<uint8_t>(raw * 255.0f);
						surface.PutPixel(x, y, { channel, channel, channel });
					}
					break;
				}
				default:
					throw Surface::ImageException(__LINE__, __FILE__, "Cannot convert DepthStencil to Surface, wrong DXGI_FORMAT!");
				}
			}
		}
		GFX_THROW_FAILED_INFO(GetContext(gfx)->Unmap(textureStaged.Get(), 0U));
		return surface;
	}
}