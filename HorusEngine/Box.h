#pragma once
#include "BaseShape.h"
#include "Object.h"

namespace GFX::Shape
{
	class Box : public BaseShape, public Object
	{
		DirectX::XMFLOAT3 sizes;

	public:
		Box(Graphics& gfx, const DirectX::XMFLOAT3& position, const std::string& name, Data::ColorFloat4 material,
			float width = 1.0f, float height = 1.0f, float length = 1.0f);
		Box(const Box&) = delete;
		Box& operator=(const Box&) = delete;
		virtual ~Box() = default;

		inline void SetTopologyMesh(Graphics& gfx) noexcept override { SetTopology(gfx, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ); }

		void ShowWindow(Graphics& gfx) noexcept override;
		void UpdateTransformMatrix() noexcept override;
	};
}