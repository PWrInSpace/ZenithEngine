#pragma once
#include "GfxResPtr.h"

namespace ZE::GFX::Resource
{
	class Viewport : public IBindable
	{
		D3D11_VIEWPORT viewport;

	public:
		constexpr Viewport(Graphics& gfx, U32 width, U32 height) noexcept;
		Viewport(Viewport&&) = default;
		Viewport& operator=(Viewport&&) = default;
		virtual ~Viewport() = default;

		static GfxResPtr<Viewport> Get(Graphics& gfx, U32 width, U32 height) { return Codex::Resolve<Viewport>(gfx, width, height); }
		static std::string GenerateRID(U32 width, U32 height) noexcept { return "V" + std::to_string(width) + "x" + std::to_string(height); }

		constexpr U32 GetWidth() const noexcept { return static_cast<U32>(viewport.Width); }
		constexpr U32 GetHeight() const noexcept { return static_cast<U32>(viewport.Height); }

		void Bind(Graphics& gfx) const override { GetContext(gfx)->RSSetViewports(1, &viewport); }
		std::string GetRID() const noexcept override { return GenerateRID(GetWidth(), GetHeight()); }
	};

	template<>
	struct is_resolvable_by_codex<Viewport>
	{
		static constexpr bool GENERATE_ID{ true };
	};

#pragma region Functions
	constexpr Viewport::Viewport(Graphics& gfx, U32 width, U32 height) noexcept
	{
		viewport.Width = static_cast<FLOAT>(width);
		viewport.Height = static_cast<FLOAT>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
	}
#pragma endregion
}