module;

#include <macro.hpp>

export module gal.utility:charconv;

import std;
import :type_traits;
import :exception;
import :breakpoint;

[[maybe_unused]] constexpr auto						 sso_string_size			= std::string{}.capacity();
[[maybe_unused]] constexpr decltype(sso_string_size) integral_string_size		= 21;
[[maybe_unused]] constexpr decltype(sso_string_size) floating_point_string_size = 128;

export namespace gal::gui::utility
{
	/**
	 * @brief Convert integer to string.
	 * @tparam T The integer type.
	 * @param value The signed or unsigned integer value.
	 * @return The integer converted to a decimal string.
	 */
	template<std::integral T>
	[[nodiscard]] constexpr auto to_string(const T value) noexcept(integral_string_size <= sso_string_size) -> std::string
	{
		//		std::array<std::string::value_type, integral_string_size> buffer{};
		//
		//		const auto												  first = buffer.data();
		//		const auto												  last	= first + buffer.size();
		//
		//		const auto [end, err_code]										= std::to_chars(first, last, value);
		//		GAL_GUI_ASSUME(err_code == std::errc{});
		//
		//		std::string result{};
		//		result.reserve(buffer.size());
		//		std::ranges::copy(buffer, std::back_inserter(result));
		//		return result;
		return std::format("{:d}", value);
	}

	/**
	 * @brief Convert a string to an integer.
	 * @tparam T The integer type.
	 * @param string The string is an integer.
	 * @param base The base radix of the string encoded integer.
	 * @return The integer converted from a string.
	 */
	template<std::integral T>
	[[nodiscard]] constexpr auto from_string(const std::string_view string, int base = 10) noexcept -> T
	{
		const auto first = string.data();
		const auto last	 = first + string.size();

		T		   value{};
		const auto [end, err_code] = std::from_chars(first, last, value, base);
		if (end != last or err_code != std::errc{})
		{
			throw make_exception("Cannot convert string to integer.");
		}

		return value;
	}

	/**
	 * @brief Convert floating point to string.
	 * @tparam T The floating point type.
	 * @param value The floating point value.
	 * @return The floating point converted to a decimal string.
	 */
	template<std::floating_point T>
	[[nodiscard]] constexpr auto to_string(const T value) noexcept(false) -> std::string
	{
		//		std::array<std::string::value_type, floating_point_string_size> buffer{};
		//
		//		const auto												  first = buffer.data();
		//		const auto												  last	= first + buffer.size();
		//
		//		const auto [end, err_code]										= std::to_chars(first, last, value, std::chars_format::general);
		//		GAL_GUI_ASSUME(err_code == std::errc{});
		//
		//		std::string result{};
		//		result.reserve(buffer.size());
		//		std::ranges::copy(buffer, std::back_inserter(result));
		//		return result;
		return std::format("{:g}", value);
	}

	/**
	 * @brief Convert a string to an floating point.
	 * @tparam T The floating point type.
	 * @param string The string is a floating point.
	 * @return The floating point converted from a string.
	 */
	template<std::floating_point T>
	[[nodiscard]] constexpr auto from_string(const std::string_view string) noexcept(false) -> T
	{
		const auto first = string.data();
		const auto last	 = first + string.size();

		T		   value{};
		const auto [end, err_code] = std::from_chars(first, last, value);
		if (end != last or err_code != std::errc{})
		{
			throw make_exception("Cannot convert string to floating point.");
		}

		return value;
	}
}// namespace gal::gui::utility
