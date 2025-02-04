#pragma once
#include "GFX/Pipeline/RenderPass/Base/RenderPass.h"
#include "Camera/ICamera.h"

namespace ZE::GFX::Pipeline::RenderPass
{
	class SkyboxPass : public Base::RenderPass
	{
		U32 indexCount;
		Camera::ICamera* mainCamera = nullptr;

	public:
		SkyboxPass(Graphics& gfx, std::string&& name);
		virtual ~SkyboxPass() = default;

		constexpr void BindCamera(Camera::ICamera& camera) noexcept { mainCamera = &camera; }

		void Execute(Graphics& gfx) override;
	};
}