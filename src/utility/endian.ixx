module;

#include <macro.hpp>

export module gal.utility:endian;

import std;
import :memory;
import :type_traits;
import :exception;
import :breakpoint;

export namespace gal::gui::utility
{
	template<typename T>
	[[nodiscard]] constexpr auto byte_swap(const T input) noexcept -> T
		requires requires {
					 std::byteswap(input);
				 }
	{
		return std::byteswap(input);
	}

	template<std::endian Endian, integral T>
	[[nodiscard]] constexpr auto byte_to_native(const T input) noexcept -> T
	{
		if constexpr (std::endian::native == Endian)
		{
			return input;
		}
		else
		{
			return byte_swap(input);
		}
	}

	template<std::endian Endian, integral T>
	[[nodiscard]] constexpr auto byte_from_native(const T input) noexcept -> T
	{
		if constexpr (std::endian::native == Endian)
		{
			return input;
		}
		else
		{
			return byte_swap(input);
		}
	}

	template<integral T>
	[[nodiscard]] constexpr auto byte_little_to_native(const T input) noexcept -> T
	{
		return byte_to_native<std::endian::little>(input);
	}

	template<integral T>
	[[nodiscard]] constexpr auto byte_big_to_native(const T input) noexcept -> T
	{
		return byte_to_native<std::endian::big>(input);
	}

	template<integral T>
	[[nodiscard]] constexpr auto byte_native_to_litte(const T input) noexcept -> T
	{
		return byte_from_native<std::endian::little>(input);
	}

	template<integral T>
	[[nodiscard]] constexpr auto byte_native_to_big(const T input) noexcept -> T
	{
		return byte_from_native<std::endian::big>(input);
	}

	template<arithmetic T, std::endian Endian = std::endian::native>
	[[nodiscard]] constexpr auto byte_load(const T* source) noexcept -> T
	{
		GAL_NOT_NULL(source, "Cannot byte_load from null!");

		auto result = *source;
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		return result;
	}

	template<arithmetic T, std::endian Endian = std::endian::native, byte_like In>
	[[nodiscard]] constexpr auto byte_load(const In* source) noexcept -> T
	{
		GAL_NOT_NULL(source, "Cannot byte_load from null!");

		auto result = unaligned_load<T>(source);
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		return result;
	}

	template<arithmetic T, std::endian Endian = std::endian::native>
	[[nodiscard]] constexpr auto byte_load(const void* source) noexcept -> T
	{
		GAL_NOT_NULL(source, "Cannot byte_load from null!");

		auto result = unaligned_load<T>(source);
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		return result;
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_load_litte(const T* source) noexcept -> T
	{
		return byte_load<T, std::endian::little>(source);
	}

	template<arithmetic T, byte_like In>
	[[nodiscard]] constexpr auto byte_load_little(const In* source) noexcept -> T
	{
		return byte_load<T, std::endian::little>(source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_load_little(const void* source) noexcept -> T
	{
		return byte_load<T, std::endian::little>(source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_load_big(const T* source) noexcept -> T
	{
		return byte_load<T, std::endian::big>(source);
	}

	template<arithmetic T, byte_like In>
	[[nodiscard]] constexpr auto byte_load_big(const In* source) noexcept -> T
	{
		return byte_load<T, std::endian::big>(source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_load_big(const void* source) noexcept -> T
	{
		return byte_load<T, std::endian::big>(source);
	}

	template<arithmetic T, std::endian Endian = std::endian::native>
	constexpr auto byte_store(const T value, T* dest) noexcept -> void
	{
		GAL_NOT_NULL(dest, "Cannot byte_store to null!");

		auto result = value;
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		*dest = result;
	}

	template<arithmetic T, std::endian Endian = std::endian::native, byte_like In>
	constexpr auto byte_store(const T value, const In* dest) noexcept -> void
	{
		GAL_NOT_NULL(dest, "Cannot byte_store to null!");

		auto result = value;
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		unaligned_store(result, dest);
	}

	template<arithmetic T, std::endian Endian = std::endian::native>
	constexpr auto byte_store(const T value, const void* dest) noexcept -> void
	{
		GAL_NOT_NULL(dest, "Cannot byte_store to null!");

		auto result = value;
		if constexpr (std::endian::native != Endian)
		{
			result = byte_swap(result);
		}
		unaligned_store(result, dest);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_store_litte(const T value, T* source) noexcept -> T
	{
		return byte_store<T, std::endian::little>(value, source);
	}

	template<arithmetic T, byte_like In>
	[[nodiscard]] constexpr auto byte_store_little(const T value, In* source) noexcept -> T
	{
		return byte_store<T, std::endian::little>(value, source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_store_little(const T value, void* source) noexcept -> T
	{
		return byte_store<T, std::endian::little>(value, source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_store_big(const T value, T* source) noexcept -> T
	{
		return byte_store<T, std::endian::big>(value, source);
	}

	template<arithmetic T, byte_like In>
	[[nodiscard]] constexpr auto byte_store_big(const T value, In* source) noexcept -> T
	{
		return byte_store<T, std::endian::big>(value, source);
	}

	template<arithmetic T>
	[[nodiscard]] constexpr auto byte_store_big(const T value, void* source) noexcept -> T
	{
		return byte_store<T, std::endian::big>(value, source);
	}

	template<typename T, std::endian Endian, std::size_t Alignment = alignof(T)>
	class EndianBuffer
	{
	public:
		constexpr static std::endian endian	   = Endian;
		constexpr static std::size_t alignment = Alignment;

	private:
		using phantom_type						  = std::byte;
		constexpr static std::size_t phantom_size = sizeof(T);

		// using phantom_container_type			  = std::array<phantom_type, phantom_size>;

	public:
		using value_type = T;
		// using size_type				 = phantom_container_type::size_type;
		// using difference_type		 = phantom_container_type::difference_type;
		// using pointer				 = phantom_container_type::pointer;
		// using const_pointer			 = phantom_container_type::const_pointer;
		// using reference				 = phantom_container_type::reference;
		// using const_reference		 = phantom_container_type::const_reference;

		// using iterator				 = phantom_container_type::iterator;
		// using const_iterator		 = phantom_container_type::const_iterator;

		// using reverse_iterator		 = phantom_container_type::reverse_iterator;
		// using const_reverse_iterator = phantom_container_type::const_reverse_iterator;

	private:
		alignas(alignment) phantom_type data_[phantom_size];

	public:
		[[nodiscard]] constexpr auto operator*() const noexcept -> value_type
		{
			return byte_load<value_type, endian>(data_);
		}

		constexpr auto operator=(const value_type value) noexcept -> EndianBuffer&
		{
			byte_store<value_type, endian>(value, data_);
			return *this;
		}
	};
}// namespace gal::gui::utility
