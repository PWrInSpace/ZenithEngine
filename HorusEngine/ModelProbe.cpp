#include "ModelProbe.h"
#include "Model.h"

namespace GFX::Probe
{
	bool ModelProbe::PushNode(Shape::ModelNode& node) noexcept
	{
		const unsigned long long selectedID = selectedNode == nullptr ? 0 : selectedNode->GetID();
		const bool expanded = ImGui::TreeNodeEx((void*)node.GetID(),
			ImGuiTreeNodeFlags_OpenOnArrow |
			(node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf) |
			(node.GetID() == selectedID ? ImGuiTreeNodeFlags_Selected : 0), node.GetName().c_str());
		if (ImGui::IsItemClicked())
			selectedNode = &node;
		return expanded;
	}

	void ModelProbe::Visit(Graphics& gfx, Shape::ModelNode& node) const noexcept
	{
		bool meshOnly = node.IsMesh();
		if (ImGui::Checkbox("Node mesh-only", &meshOnly))
			node.SetMesh(gfx, meshOnly);
	}

	bool ModelProbe::Visit(Graphics& gfx, Shape::Model& model, Shape::ModelNode& root) noexcept
	{
		if (selectedNode == nullptr)
			selectedNode = &root;
		ImGui::Columns(2);
		ImGui::BeginChild("##NodeTree", ImVec2(0.0f, 231.5f), false, ImGuiWindowFlags_HorizontalScrollbar);
		bool change = root.Accept(gfx, *this);
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::NewLine();
		bool outline = model.IsOutline();
		if (ImGui::Checkbox("Model outline", &outline))
		{
			if (outline)
				model.SetOutline();
			else
				model.DisableOutline();
		}
		Visit(gfx, *selectedNode);
		change |= selectedNode->Accept(gfx, static_cast<BaseProbe&>(*this));
		ImGui::Columns(1);
		return change;
	}
}