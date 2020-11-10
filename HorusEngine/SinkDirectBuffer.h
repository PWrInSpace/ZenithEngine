#pragma once
#include "Sink.h"

namespace GFX::Pipeline::RenderPass::Base
{
	template<typename T>
	class SinkDirectBuffer : public Sink
	{
		static_assert(std::is_base_of_v<Resource::IBufferResource, T>, "SinkDirectBuffer target type must be a IBufferResource type!");

		std::shared_ptr<T>& target;

	public:
		inline SinkDirectBuffer(const std::string& registeredName, std::shared_ptr<T>& buffer) : Sink(registeredName), target(buffer) {}
		virtual ~SinkDirectBuffer() = default;

		inline static std::unique_ptr<Sink> Make(const std::string& registeredName, std::shared_ptr<T>& resource) { return std::make_unique<SinkDirectBuffer>(registeredName, resource); }

		void Bind(Source& source) override;
	};

	template<typename T>
	void SinkDirectBuffer<T>::Bind(Source& source)
	{
		auto basePtr = source.LinkBuffer();
		auto ptr = std::dynamic_pointer_cast<T>(basePtr);
		if (ptr == nullptr)
			throw RGC_EXCEPT("Binding Sink \"" + GetRegisteredName() + "\" of type {" + typeid(T).name() +
				"} to Source \"" + GetPassPathString() + "." + GetSourceName() + "\" of incompatible type {" + typeid(basePtr).name());
		target = std::move(ptr);
		linked = true;
	}
}