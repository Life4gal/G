#pragma once

#include <G/utility/assert.hpp>
#include <G/utility/type_traits.hpp>
#include <bit>
#include <cstring>
#include <memory>

namespace gal::gui::utility
{
	/**
	 * @brief Make an unaligned load of an unsigned integer.
	 */
	template<arithmetic T, byte_like In>
	[[nodiscard]] constexpr auto unaligned_load(const In* source) noexcept -> T
	{
		GAL_GUI_NOT_NULL(source, "Cannot unaligned_load from null!");

		T result{};

		if not consteval
		{
			std::memcpy(&result, source, sizeof(T));
			return result;
		}

		if constexpr (std::endian::native == std::endian::little)
		{
			for (auto i = sizeof(T); i != 0; --i)
			{
				if constexpr (sizeof(result) > 1)
				{
					result <<= 8;
				}
				result |= static_cast<std::uint8_t>(source[i - 1]);
			}
		}
		else
		{
			for (auto i = 0; i != sizeof(T); ++i)
			{
				if constexpr (sizeof(result) > 1)
				{
					result <<= 8;
				}
				result |= static_cast<std::uint8_t>(source[i]);
			}
		}

		return result;
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto unaligned_load(const void* source) noexcept -> T
	{
		return unaligned_load<T>(static_cast<const std::byte*>(source));
	}

	/**
	 * @brief Make an unaligned store of an unsigned integer.
	 */
	template<arithmetic T, byte_like In>
	constexpr auto unaligned_store(const T value, In* dest) noexcept -> void
	{
		GAL_GUI_NOT_NULL(dest, "Cannot unaligned_store from null!");

		using unsigned_type		  = std::make_unsigned_t<T>;

		const auto unsigned_value = static_cast<unsigned_type>(value);

		if not consteval
		{
			std::memcpy(dest, &unsigned_value, sizeof(T));
			return;
		}

		if constexpr (std::endian::native == std::endian::little)
		{
			for (auto i = 0; i != sizeof(T); ++i)
			{
				dest[i] = static_cast<In>(unsigned_value);
				unsigned_value >>= 8;
			}
		}
		else
		{
			for (auto i = sizeof(T); i != 0; --i)
			{
				dest[i - 1] = static_cast<In>(unsigned_value);
				unsigned_value >>= 8;
			}
		}
	}

	template<arithmetic T>
	constexpr auto unaligned_store(const T value, void* dest) noexcept -> void
	{
		unaligned_load<T>(value, static_cast<std::byte*>(dest));
	}
}// namespace gal::gui::utility
