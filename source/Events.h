#pragma once
#include "debug.h"

namespace RUIN
{
	enum class EventType
	{
		MouseMoved,
		MouseDown,
		MouseUp,
		MouseScroll
	};

	struct MouseScrollData
	{
		float scrollAmount;
	};

	class Event
	{
	public:
		static Event ScrollEvent(float scrollAmount);
		static Event MouseMovedEvent();
		static Event MouseDownEvent();
		static Event MouseUpEvent();

		bool Is(EventType type) const { return m_Type == type; }
		EventType GetType() const { return m_Type; }
		const MouseScrollData& GetMouseScrollData() const { RASSERT(m_Type == EventType::MouseScroll, "Incorrect event type!"); return m_MouseScrollData; }

	private:
		EventType m_Type;

		union
		{
			MouseScrollData m_MouseScrollData;
		};
	};
}