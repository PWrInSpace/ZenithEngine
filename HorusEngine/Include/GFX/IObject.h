#pragma once
#include "Pipeline/IRenderable.h"

namespace GFX
{
	class IObject : public virtual Pipeline::IRenderable
	{
	public:
		virtual ~IObject() = default;

		virtual const Float4& GetAngle() const noexcept = 0;
		virtual void SetAngle(const Vector& rotor) noexcept = 0;
		virtual void SetAngle(const Float4& rotor) noexcept = 0;

		virtual float GetScale() const noexcept = 0;
		virtual void SetScale(float newScale) noexcept = 0;

		virtual const Float3& GetPos() const noexcept = 0;
		virtual void SetPos(const Float3& position) noexcept = 0;

		virtual const std::string& GetName() const noexcept = 0;
		virtual void SetName(const std::string& newName) noexcept = 0;

		virtual void Update(const Float3& delta, const Vector& rotor) noexcept { UpdatePos(delta); UpdateAngle(rotor); }
		virtual void UpdatePos(const Float3& delta) noexcept = 0;
		virtual void UpdateAngle(const Vector& rotor) noexcept = 0;
	};
}

// Assertions to use before storing quaternion rotations
#define ASSERT_QTRN_UNIT_V(rotor) assert(Math::Internal::XMQuaternionIsUnit(rotor))
#define ASSERT_QTRN_UNIT(rotor) ASSERT_QTRN_UNIT_V(Math::XMLoadFloat4(&rotor))