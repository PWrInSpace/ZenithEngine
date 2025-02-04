#pragma once
#include "GFX/Pipeline/RenderPass/Base/QueuePass.h"

namespace ZE::GFX::Pipeline::RenderPass
{
	class OutlineGenerationPass : public Base::QueuePass
	{
	public:
		OutlineGenerationPass(Graphics& gfx, std::string&& name);
		virtual ~OutlineGenerationPass() = default;
	};
}