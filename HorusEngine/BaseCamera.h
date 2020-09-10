#pragma once
#include "ICamera.h"
#include "CameraFrustrum.h"
#include "CameraIndicator.h"

namespace Camera
{
	class BaseCamera : public ICamera
	{
	protected:
		mutable DirectX::XMFLOAT4X4 viewMatrix;
		mutable DirectX::XMFLOAT4X4 projectionMatrix;
		ProjectionData projection;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
		mutable bool viewUpdate = true;
		mutable bool projectionUpdate = true;
		std::shared_ptr<GFX::Shape::CameraIndicator> indicator = nullptr;
		std::shared_ptr<GFX::Shape::CameraFrustrum> frustrum = nullptr;
		bool enableIndicator = true;
		bool enableFrustrum = false;

		virtual DirectX::FXMMATRIX UpdateView() const noexcept = 0;
		DirectX::FXMMATRIX UpdateProjection() const noexcept;

	public:
		BaseCamera(GFX::Graphics& gfx, GFX::Pipeline::RenderGraph& graph, const std::string& name,
			float fov, float nearClip, float farClip, const DirectX::XMFLOAT3& position) noexcept;
		BaseCamera(const BaseCamera&) = default;
		BaseCamera& operator=(const BaseCamera&) = default;
		virtual ~BaseCamera() = default;

		constexpr void EnableIndicator() noexcept { enableIndicator = true; }
		constexpr void DisableIndicator() noexcept { enableIndicator = false; }
		constexpr void EnableFrustrumIndicator() noexcept { enableFrustrum = true; }
		constexpr void DisableFrustrumIndicator() noexcept { enableFrustrum = false; }

		inline void ResetView() const noexcept override { viewUpdate = true; }
		inline void ResetProjection() const noexcept override { projectionUpdate = true; }

		inline void SetPos(const DirectX::XMFLOAT3& pos) noexcept override { position = pos; viewUpdate = true; }
		inline const DirectX::XMFLOAT3& GetPos() const noexcept override { return position; }

		inline void SetOutline() noexcept override { indicator->SetOutline(); }
		inline void DisableOutline() noexcept override { indicator->DisableOutline(); }

		DirectX::FXMMATRIX GetProjection() const noexcept override;
		DirectX::FXMMATRIX GetView() const noexcept override;

		void Roll(float delta) noexcept override;

		void Bind(GFX::Graphics& gfx) const noexcept override;
		void Accept(GFX::Graphics& gfx, GFX::Probe::BaseProbe& probe) noexcept override;
		void Submit(uint64_t channelFilter) noexcept override;
	};
}