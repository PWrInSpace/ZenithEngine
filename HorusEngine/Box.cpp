#include "Box.h"
#include "Primitives.h"
#include "TechniqueFactory.h"

namespace GFX::Shape
{
	Box::Box(Graphics& gfx, Pipeline::RenderGraph& graph, const DirectX::XMFLOAT3& position,
		const std::string& name, Data::ColorFloat4 color, float width, float height, float length)
		: BaseShape(gfx), Object(position, name), sizes(width, height, length)
	{
		std::string typeName = Primitive::Cube::GetName();
		if (Resource::VertexBuffer::NotStored(typeName) && Resource::IndexBuffer::NotStored(typeName))
		{
			auto list = Primitive::Cube::Make();
			SetVertexBuffer(Resource::VertexBuffer::Get(gfx, typeName, list.vertices));
			SetIndexBuffer(Resource::IndexBuffer::Get(gfx, typeName, list.indices));
		}
		else
		{
			Primitive::IndexedTriangleList list;
			SetVertexBuffer(Resource::VertexBuffer::Get(gfx, typeName, list.vertices));
			SetIndexBuffer(Resource::IndexBuffer::Get(gfx, typeName, list.indices));
		}

		std::vector<std::shared_ptr<Pipeline::Technique>> techniques;
		techniques.reserve(3);
		auto material = std::make_shared<Visual::Material>(gfx, color, name);
		auto vertexLayout = material->GerVertexLayout();

		techniques.emplace_back(Pipeline::TechniqueFactory::MakeShadowMap(gfx, graph, vertexLayout));
		techniques.emplace_back(Pipeline::TechniqueFactory::MakeLambertian(gfx, graph, std::move(material)));
		techniques.emplace_back(Pipeline::TechniqueFactory::MakeOutlineScale(gfx, graph, name, std::move(vertexLayout)));
		SetTechniques(gfx, std::move(techniques), *this);

		UpdateTransformMatrix();
	}

	void Box::Accept(Graphics& gfx, Probe::BaseProbe& probe) noexcept
	{
		Object::Accept(gfx, probe);
		BaseShape::Accept(gfx, probe);
	}

	void Box::UpdateTransformMatrix() noexcept
	{
		DirectX::XMStoreFloat4x4(transform.get(),
			DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&sizes)) *
			CreateTransformMatrix());
	}
}