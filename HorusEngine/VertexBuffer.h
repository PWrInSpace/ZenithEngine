#pragma once
#include "Codex.h"
#include "VertexBufferData.h"

namespace GFX::Resource
{
	class VertexBuffer : public IBindable
	{
		UINT stride;
		std::string name;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	public:
		VertexBuffer(Graphics& gfx, const std::string& tag, const Data::VertexBufferData& buffer);
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		virtual ~VertexBuffer() = default;

		static inline bool NotStored(const std::string& tag) noexcept { return Codex::NotStored<VertexBuffer>(tag); }
		static inline std::shared_ptr<VertexBuffer> Get(Graphics& gfx, const std::string& tag, const Data::VertexBufferData& buffer);
		template<typename ...Ignore>
		static inline std::string GenerateRID(const std::string& tag, Ignore&& ...ignore) noexcept;

		inline void Bind(Graphics& gfx) noexcept override;
		inline std::string GetRID() const noexcept override { return GenerateRID(name); }
	};

	template<>
	struct is_resolvable_by_codex<VertexBuffer>
	{
		static constexpr bool generate{ true };
	};

	inline std::shared_ptr<VertexBuffer> VertexBuffer::Get(Graphics& gfx, const std::string& tag, const Data::VertexBufferData& buffer)
	{
		return Codex::Resolve<VertexBuffer>(gfx, tag, buffer);
	}

	template<typename ...Ignore>
	inline std::string VertexBuffer::GenerateRID(const std::string& tag, Ignore&& ...ignore) noexcept
	{
		return "#" + std::string(typeid(VertexBuffer).name()) + "#" + tag + "#";
	}

	inline void VertexBuffer::Bind(Graphics& gfx) noexcept
	{
		const UINT offset = 0U;
		GetContext(gfx)->IASetVertexBuffers(0U, 1U, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
}