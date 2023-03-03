module;

#include <macro.hpp>

export module gal.utility:compare;

import std;

export namespace gal::gui::utility
{
	template<typename L, typename R>
	struct three_way_comparison;

	template<typename L, typename R>
		requires std::is_same_v<L, R> and
				 (std::unsigned_integral<L> or
				  std::signed_integral<L> or
				  std::floating_point<L>)
	struct three_way_comparison<L, R>
	{
		[[nodiscard]] constexpr auto operator()(const L l, const R r) const noexcept -> std::strong_ordering
		{
			return l <=> r;
		}
	};

	template<typename L, typename R>
		requires(std::signed_integral<L> and std::unsigned_integral<R>) or
				(std::unsigned_integral<L> and std::signed_integral<R>)
	struct three_way_comparison<L, R>
	{
		[[nodiscard]] constexpr auto operator()(const L l, const R r) const noexcept -> std::strong_ordering
		{
			// if (std::cmp_greater(l, r))
			// {
			// 	return std::strong_ordering::greater;
			// }

			if constexpr (std::signed_integral<L>)
			{
				if (l < 0)
				{
					return std::strong_ordering::less;
				}
				return static_cast<R>(l) <=> r;
			}
			else
			{
				if (r < 0)
				{
					return std::strong_ordering::greater;
				}
				return l <=> static_cast<L>(r);
			}
		}
	};

	template<typename L, typename R>
		requires(std::floating_point<L> and std::integral<R>) or
				(std::integral<L> and std::floating_point<R>)
	struct three_way_comparison<L, R>
	{
		[[nodiscard]] constexpr auto operator()(const L l, const R r) const noexcept -> std::partial_ordering
		{
			if constexpr (std::floating_point<L>)
			{
				if constexpr (sizeof(R) < sizeof(float))
				{
					return l <=> static_cast<float>(r);
				}
				else if constexpr (sizeof(R) < sizeof(double))
				{
					return l <=> static_cast<double>(r);
				}
				else if constexpr (sizeof(R) < sizeof(long double))
				{
					return l <=> static_cast<long double>(r);
				}
				else
				{
					GAL_UNREACHABLE("Invalid floating point type!");
				}
			}
			else
			{
				if constexpr (sizeof(L) < sizeof(float))
				{
					return static_cast<float>(l) <=> r;
				}
				else if constexpr (sizeof(L) < sizeof(double))
				{
					return static_cast<double>(l) <=> r;
				}
				else if constexpr (sizeof(L) < sizeof(long double))
				{
					return static_cast<long double>(l) <=> r;
				}
				else
				{
					GAL_UNREACHABLE("Invalid floating point type!");
				}
			}
		}
	};

	template<typename L, typename R>
		requires std::is_arithmetic_v<L> and std::is_arithmetic_v<R>
	[[nodiscard]] constexpr auto three_way_compare(const L l, const R r) noexcept -> auto
	{
		return three_way_comparison<L, R>{}(l, r);
	}
}// namespace gal::gui::utility
