#pragma once
#include "Types.h"
#include <bitset>
#include <deque>
#include <optional>

namespace ZE::WinAPI
{
	class Keyboard final
	{
		friend class Window;

	public:
		class Event final
		{
		public:
			enum class Type : bool { Down, Up };

		private:
			Type type;
			U8 code;

		public:
			constexpr Event(Type type, U8 code) noexcept : type(type), code(code) {}
			Event(Event&&) = default;
			Event(const Event&) = default;
			Event& operator=(Event&&) = default;
			Event& operator=(const Event&) = default;
			~Event() = default;

			constexpr bool IsDown() const noexcept { return type == Type::Down; }
			constexpr bool IsUp() const noexcept { return type == Type::Up; }
			constexpr U8 GetCode() const noexcept { return code; }
		};

	private:
		static constexpr size_t NUMBER_OF_VKEYS = 256;
		static constexpr size_t BUFFER_SIZE = 32;

		bool autorepeatEnabled = false; // Accounting long key press
		std::bitset<NUMBER_OF_VKEYS> keystates; // States for all virtual keys from WinAPI
		std::deque<Event> keybuffer; // Buffer for KEY_UP/DOWN events
		std::deque<char> charbuffer; // Buffer for ON_CHAR events

		void ClearStates() noexcept { keystates.reset(); }

		template<typename T>
		static void TrimBuffer(std::deque<T>& buffer) noexcept;

		void OnKeyDown(U8 keycode) noexcept;
		void OnKeyUp(U8 keycode) noexcept;
		void OnChar(char character) noexcept;

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		~Keyboard() = default;

		constexpr bool IsAutorepeat() const noexcept { return autorepeatEnabled; }
		constexpr void SetAutorepeat(bool mode) noexcept { autorepeatEnabled = mode; }
		constexpr bool IsKeyDown(U8 keycode) const noexcept { return keystates[keycode]; }

		bool IsKeyReady() const noexcept { return keybuffer.size(); }
		bool IsCharReady() const noexcept { return charbuffer.size(); }
		void FlushKeys() noexcept { keybuffer = std::deque<Event>(); }
		void FlushChars() noexcept { charbuffer = std::deque<char>(); }

		void Flush() noexcept;
		std::optional<Event> ReadKey() noexcept;
		std::optional<char> ReadChar() noexcept;
	};
}