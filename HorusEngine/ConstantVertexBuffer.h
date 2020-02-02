#pragma once
#include "ConstantBuffer.h"

namespace GFX::Resource
{
	template<typename T>
	class ConstantVertexBuffer : public ConstantBuffer<T>
	{
		using ConstantBuffer<T>::ConstantBuffer;
		using ConstantBuffer<T>::GetContext;
		using ConstantBuffer<T>::constantBuffer;
		using ConstantBuffer<T>::slot;
		using ConstantBuffer<T>::name;

	public:
		ConstantVertexBuffer(const ConstantVertexBuffer&) = delete;
		ConstantVertexBuffer& operator=(const ConstantVertexBuffer&) = delete;
		~ConstantVertexBuffer() = default;

		static inline std::shared_ptr<ConstantVertexBuffer> Get(Graphics& gfx, const std::string& tag, const T& values, UINT slot = 0U);
		static inline std::shared_ptr<ConstantVertexBuffer> Get(Graphics& gfx, const std::string& tag, UINT slot = 0U);

		static inline std::string GenerateRID(const std::string& tag, const T& values, UINT slot = 0U) noexcept { return GenerateRID(tag, slot); }
		static inline std::string GenerateRID(const std::string& tag, UINT slot = 0U) noexcept;

		inline void Bind(Graphics& gfx) noexcept override { GetContext(gfx)->VSSetConstantBuffers(slot, 1U, constantBuffer.GetAddressOf()); }
		inline std::string GetRID() const noexcept override { return GenerateRID(name, slot); }
	};

	template<typename T>
	struct is_resolvable_by_codex<ConstantVertexBuffer<T>>
	{
		static constexpr bool value{ true };
	};

	template<typename T>
	inline std::shared_ptr<ConstantVertexBuffer<T>> ConstantVertexBuffer<T>::Get(Graphics& gfx, const std::string& tag, const T& values, UINT slot)
	{
		return Codex::Resolve<ConstantVertexBuffer<T>>(gfx, tag, values, slot);
	}

	template<typename T>
	inline std::shared_ptr<ConstantVertexBuffer<T>> ConstantVertexBuffer<T>::Get(Graphics& gfx, const std::string& tag, UINT slot)
	{
		return Codex::Resolve<ConstantVertexBuffer<T>>(gfx, tag, slot);
	}

	template<typename T>
	inline std::string ConstantVertexBuffer<T>::GenerateRID(const std::string& tag, UINT slot) noexcept
	{
		return "#" + std::string(typeid(ConstantVertexBuffer<T>).name()) + "#" + tag + "#" + std::to_string(slot) + "#";
	}
}
