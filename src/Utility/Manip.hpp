#pragma once
#include <type_traits>

#pragma warning(push)
#pragma warning( disable : 4244 )

namespace nic {

	template<typename T>
	constexpr auto Clamp(const T& value, const T& topLimit, const T& bottomLimit = 0) noexcept
	{
		assert(topLimit >= bottomLimit);
		if (value >= topLimit)
			return topLimit;
		else if (value <= bottomLimit)
			return bottomLimit;
		else
			return value;
	}

	template<typename T>
	constexpr auto Wrap(const T& value, const T& topLimit, const T& bottomLimit = 0) noexcept
	{
		assert(topLimit >= bottomLimit);
		const T span = topLimit - bottomLimit;
		if (value >= topLimit)
		{
			const size_t n = (value - topLimit) / span;
			return value - (1 + n) * span;
		}
		else if (value < bottomLimit)
		{
			const size_t n = (bottomLimit - value - (std::is_integral_v<T> ? 1 : 0)) / span;
			return value + (1 + n) * span;
		}
		return value;
	}

	template <class T>
	constexpr auto Max(const T& last) noexcept {
		return last;
	}
	template <class T, class... Args>
	constexpr auto Max(const T& first, const Args& ... args) noexcept {
		return ((first > args) && ...) ? first : Max(args ...);
	}

	template <class T>
	constexpr auto Min(const T& last) noexcept {
		return last;
	}
	template <class T, class... Args>
	constexpr auto Min(const T& first, const Args& ... args) noexcept {
		return ((first < args) && ...) ? first : Min(args ...);
	}

	template <class T, class... Args>
	constexpr auto Sum(const T& first, const Args& ... args) noexcept {
		return (first + (args + ...));
	}

	template <class T, class... Args>
	constexpr auto Mean(const T& first, const Args& ... args) noexcept {
		return Sum(first, args ...) / (1 + sizeof...(args));
	}

} // namespace nic

#pragma warning(pop)