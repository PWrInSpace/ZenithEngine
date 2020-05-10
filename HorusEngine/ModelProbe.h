#pragma once
#include "Graphics.h"
#include "BaseProbe.h"
#include "ImGui\imgui.h"

namespace GFX::Shape
{
	class ModelNode;
	class Model;
}

namespace GFX::Probe
{
	class ModelProbe : public BaseProbe
	{
		Shape::ModelNode* selectedNode = nullptr;

	public:
		ModelProbe() = default;
		ModelProbe(const ModelProbe&) = default;
		ModelProbe& operator=(const ModelProbe&) = default;
		virtual ~ModelProbe() = default;

		inline void PopNode() const noexcept { ImGui::TreePop(); }

		bool PushNode(Shape::ModelNode& node) noexcept;
		void Visit(Graphics& gfx, Shape::ModelNode& node) noexcept;
		void Visit(Graphics& gfx, Shape::Model& model, Shape::ModelNode& root) noexcept;
	};
}