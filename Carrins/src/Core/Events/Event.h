#pragma once

class Event
{
	template <typename E>
	using EventFn = bool (*)(E &);

public:
	virtual ~Event() = default;

	bool IsHandled() const { return m_Handled; }

	template <typename E>
	bool Dispatch(EventFn<E> func)
	{
		if (auto evt = dynamic_cast<E *>(this))
		{
			m_Handled = func(*evt);
			return true;
		}
		else
			return false;
	}

private:
	bool m_Handled = false;
};