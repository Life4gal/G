module;

#include <macro.hpp>

export module gal.utility:hash;

import std;
import :exception;
import :breakpoint;
import :cast;

export namespace gal::gui::utility
{
	[[nodiscard]] constexpr auto hash_combine(const std::size_t hash1, const std::size_t hash2) noexcept -> std::size_t
	{
		if constexpr (sizeof(std::size_t) == 4)// NOLINT
		{
			return hash1 + 0x9e3779b9_uz + (hash2 << 6) + (hash2 >> 2);
		}
		else if constexpr (sizeof(std::size_t) == 8)// NOLINT
		{
			return hash1 + 0x9e3779b97f681800_uz + (hash2 << 6) + (hash2 >> 2);
		}
		else
		{
			GAL_GUI_IMPLEMENTED("Currently only 32/64 bit hash combinations are implemented");
		}
	}

	template<typename First, typename Second, typename... Reset>
	[[nodiscard]] constexpr auto hash_combine(First&& first, Second&& second, Reset&&... reset) noexcept -> std::size_t
		requires requires {
					 std::hash<std::remove_cvref_t<First>>{};
					 std::hash<std::remove_cvref_t<Second>>{};
				 }
	{
		if constexpr (sizeof...(reset) == 0)
		{
			return hash_combine(
					std::hash<std::remove_cvref_t<First>>{}(std::forward<First>(first)),
					std::hash<std::remove_cvref_t<Second>>{}(std::forward<Second>(second)));
		}
		else
		{
			return hash_combine(
					std::hash<std::remove_cvref_t<First>>{}(std::forward<First>(first)),
					hash_combine(std::forward<Second>(second), reset...));
		}
	}
}// namespace gal::gui::utility
