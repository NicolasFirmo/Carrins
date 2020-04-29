#pragma once

namespace nic {

	template <typename T, size_t N>
	class Accumulator
	{
	public:
		constexpr Accumulator() noexcept = default;
		constexpr Accumulator(const T value) noexcept : m_Values()
		{
			for (size_t i = 0; i < N; i++)
				m_Values[i] = value;
		}
		constexpr void Push(T value) noexcept
		{
			if (++m_NextIdx >= N)
				m_NextIdx = 0;
			m_Values[m_NextIdx] = value;
		}

		constexpr T Sum() const noexcept
		{
			T acc(0);
			for (const auto &value : m_Values)
				acc += value;
			return acc;
		}
		constexpr T Average() const noexcept
		{
			return Sum() / N;
		}

	private:
		size_t m_NextIdx = 0;
		T m_Values[N];
	};

} // namespace nic