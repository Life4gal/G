module;

export module gal.utility:math;

import std;

namespace gal::gui::utility
{
	export
	{
		/**
		 * @brief The greatest multiple of alignment less than or equal to value.
		 * @tparam T The unsigned type.
		 * @param value The unsigned value to round.
		 * @param alignment The alignment.
		 * @return The greatest multiple of alignment less than or equal to value.
		 */
		template<std::unsigned_integral T>
		[[nodiscard]] constexpr auto floor(const T value, const std::type_identity_t<T> alignment) noexcept -> T
		{
			return (value / alignment) * alignment;
		}

		/**
		 * @brief The smallest multiple of alignment greater than or equal to value.
		 * @tparam T The unsigned type.
		 * @param value The unsigned value to round.
		 * @param alignment The alignment.
		 * @return The smallest multiple of alignment greater than or equal to value.
		 */
		template<std::unsigned_integral T>
		[[nodiscard]] constexpr auto ceil(const T value, const std::type_identity_t<T> alignment) noexcept -> T
		{
			return floor(value + (alignment - 1), alignment);
		}

		template<typename Iterator>
		[[nodiscard]] constexpr auto mean(Iterator first, Iterator last) noexcept -> typename std::iterator_traits<Iterator>::value_type
			requires requires {
						 std::is_floating_point_v<typename std::iterator_traits<Iterator>::value_type>;
					 }
		{
			using type		 = typename std::iterator_traits<Iterator>::value_type;
			const auto init	 = static_cast<type>(0);

			const auto sum	 = std::reduce(first, last, init);
			const auto count = static_cast<type>(std::distance(first, last));

			return count > static_cast<type>(0) ? sum / count : sum;
		}

		template<typename T = double, typename Iterator>
		[[nodiscard]] constexpr auto mean(Iterator first, Iterator last) noexcept -> T
			requires requires {
						 not std::is_floating_point_v<typename std::iterator_traits<Iterator>::value_type>;
					 }
		{
			using type		 = typename std::iterator_traits<Iterator>::value_type;
			const auto init	 = static_cast<type>(0);

			const auto sum	 = static_cast<T>(std::reduce(first, last, init));
			const auto count = static_cast<T>(static_cast<type>(std::distance(first, last)));

			return count > static_cast<T>(0) ? sum / count : sum;
		}
	}
}// namespace gal::gui::utility
