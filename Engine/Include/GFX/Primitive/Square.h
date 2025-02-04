#pragma once
#include "IndexedTriangleList.h"

namespace ZE::GFX::Primitive
{
	class Square
	{
	public:
		Square() = delete;

		static std::string GetName(const std::vector<VertexAttribute>& attributes = {}) noexcept;
		static std::string GetNameNDC2D() noexcept { return "N_"; }

		static std::shared_ptr<Data::VertexLayout> GetLayout(const std::vector<VertexAttribute>& attributes = {}) noexcept;
		static std::shared_ptr<Data::VertexLayout> GetLayoutNDC2D() noexcept;

		static IndexedTriangleList Make(const std::vector<VertexAttribute>& attributes = {}) noexcept;
		static IndexedTriangleList MakeNDC2D() noexcept;
	};
}