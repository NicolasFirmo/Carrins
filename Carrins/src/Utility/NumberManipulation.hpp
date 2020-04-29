#pragma once
#include <type_traits>

namespace nic {

	template<typename T1, typename T2, typename T3>
	constexpr T1 Clamp(const T1 value, const T2 topLimit, const T3 bottomLimit = 0) noexcept
	{
		assert(topLimit >= bottomLimit);
		if (value >= topLimit)
			return topLimit;
		else if (value <= bottomLimit)
			return bottomLimit;
		else
			return value;
	}

	template<typename T1, typename T2, typename T3>
	constexpr T1 Wrap(const T1 value, const T2 topLimit, const T3 bottomLimit = 0) noexcept
	{
		assert(topLimit >= bottomLimit);
		const T1 span = topLimit - bottomLimit;
		if (value >= topLimit)
		{
			const size_t n = (value - topLimit) / span;
			return value - (1 + n) * span;
		}
		else if (value < bottomLimit)
		{
			const size_t n = (bottomLimit - value - (std::is_integral_v<T1> ? 1 : 0)) / span;
			return value + (1 + n) * span;
		}
		return value;
	}

} // namespace nic

