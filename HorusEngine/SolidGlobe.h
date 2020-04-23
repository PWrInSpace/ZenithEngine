#pragma once
#include "BaseShape.h"
#include "Object.h"

namespace GFX::Shape
{
	class SolidGlobe : public BaseShape, public Object
	{
		DirectX::XMFLOAT3 sizes;
		Resource::ConstBufferPixel<Data::CBuffer::Solid>* materialBuffer = nullptr;

	public:
		SolidGlobe(Graphics& gfx, const DirectX::XMFLOAT3& position, const std::string& name, Data::ColorFloat4 material,
			unsigned int latitudeDensity, unsigned int longitudeDensity, float width = 1.0f, float height = 1.0f, float length = 1.0f);
		SolidGlobe(const SolidGlobe&) = delete;
		SolidGlobe& operator=(const SolidGlobe&) = delete;
		virtual ~SolidGlobe() = default;

		inline Resource::ConstBufferPixel<Data::CBuffer::Solid>& GetMaterial() noexcept { return *materialBuffer; }
		inline void SetTopologyMesh(Graphics& gfx) noexcept override { SetTopology(gfx, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ); }

		void ShowWindow(Graphics& gfx) noexcept override;
		void UpdateTransformMatrix() noexcept override;
	};
}