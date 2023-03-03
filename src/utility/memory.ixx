module;

#include <macro.hpp>

export module gal.utility:memory;

import std;
import :exception;
import :breakpoint;
import :type_traits;
import :cast;
import :math;

namespace gal::gui::utility
{
	export
	{
		/**
		 * @brief Check if a pointer is properly aligned for the object it is pointing at.
		 * @tparam T The object type.
		 * @param pointer A pointer to an object.
		 * @param alignment The object alignment.
		 * @return Aligned or not.
		 */
		template<typename T>
		[[nodiscard]] constexpr auto is_aligned(const T* pointer, const std::size_t alignment = std::alignment_of_v<T>) noexcept -> bool
		{
			return reinterpret_cast<std::ptrdiff_t>(pointer) % alignment == 0;
		}

		template<typename T>
		[[nodiscard]] constexpr auto ceil_align(const T* pointer, const std::size_t alignment) noexcept -> T*
		{
			return reinterpret_cast<T*>(ceil(reinterpret_cast<std::uintptr_t>(pointer), wide_cast<std::uintptr_t>(alignment)));
		}

		template<typename T>
		[[nodiscard]] constexpr auto floor_align(const T* pointer, const std::size_t alignment) noexcept -> T*
		{
			return reinterpret_cast<T*>(floor(reinterpret_cast<std::uintptr_t>(pointer), wide_cast<std::uintptr_t>(alignment)));
		}

		/**
		 * @brief Make an unaligned load of an unsigned integer.
		 */
		template<arithmetic T, byte_like In>
		[[nodiscard]] constexpr auto unaligned_load(const In* source) noexcept -> T
		{
			GAL_NOT_NULL(source, "Cannot unaligned_load from null!");

			T result{};

#if defined(G_COMPILER_MSVC)
			if (not std::is_constant_evaluated())
#else
			if not consteval
#endif
			{
				std::memcpy(&result, source, sizeof(T));
				return result;
			}

			if constexpr (std::endian::native == std::endian::little)// NOLINT
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
			GAL_NOT_NULL(dest, "Cannot unaligned_store from null!");

			using unsigned_type		  = std::make_unsigned_t<T>;

			const auto unsigned_value = static_cast<unsigned_type>(value);

#if defined(G_COMPILER_MSVC)
			if (not std::is_constant_evaluated())
#else
			if not consteval
#endif
			{
				std::memcpy(dest, &unsigned_value, sizeof(T));
				return;
			}

			if constexpr (std::endian::native == std::endian::little)// NOLINT
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

		/**
		 * @brief Advance a pointer by a number of bytes.
		 * @tparam T The object type.
		 * @param pointer A pointer to the object.
		 * @param distance The number of bytes to advance the pointer, may be negative.
		 * @return A new point which advanced.
		 */
		template<typename T>
		[[nodiscard]] constexpr auto byte_advance(T * pointer, const std::ptrdiff_t distance) noexcept -> T*
		{
			GAL_NOT_NULL(pointer);
			return reinterpret_cast<T*>(reinterpret_cast<char*>(pointer) + distance);
		}

		/**
		 * @brief Advance a pointer by a number of bytes.
		 * @tparam T The object type.
		 * @param pointer A pointer to the object.
		 * @param distance The number of bytes to advance the pointer, may be negative.
		 * @return A new point which advanced.
		 */
		template<typename T>
		[[nodiscard]] constexpr auto byte_advance(const T* pointer, const std::ptrdiff_t distance) noexcept -> const T*
		{
			GAL_NOT_NULL(pointer);
			return reinterpret_cast<const T*>(reinterpret_cast<char*>(pointer) + distance);
		}
	}

	export
	{
		/**
		 * @brief Copy an object to another memory locations.
		 * @tparam Pointer The pointer type.
		 * @tparam T The object type.
		 * @param source_pointer A pointer to an object.
		 * @param dest A pointer to @b allocated memory.
		 * @return The dest pointer with the new object who's lifetime was started.
		 */
		template<typename Pointer, typename T>
		[[nodiscard]] constexpr auto copy_construct(Pointer source_pointer, T * dest) noexcept -> T*
			requires requires {
						 std::is_constructible_v<T, typename std::pointer_traits<Pointer>::element_type>;
					 }
		{
			GAL_NOT_NULL(source_pointer);
			GAL_NOT_NULL(dest);

			return std::construct_at(dest, *source_pointer);
		}

		/**
		 * @brief Copy an object to another memory locations.
		 * @tparam Iterator The iterator type.
		 * @tparam T The object type.
		 * @param iterator An iterator to an object.
		 * @param dest A pointer to @b allocated memory.
		 * @return The dest pointer with the new object who's lifetime was started.
		 */
		template<typename Iterator, typename T>
		[[nodiscard]] constexpr auto copy_construct(Iterator source_iterator, T * dest) noexcept -> T*
			requires requires {
						 std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>;
					 }
		{
			GAL_ASSUME(source_iterator != Iterator{});
			GAL_NOT_NULL(dest);

			return std::construct_at(dest, *source_iterator);
		}

		/**
		 * @brief Move an object to another memory locations.
		 * @tparam Pointer The pointer type.
		 * @tparam T The object type.
		 * @param source_pointer A pointer to an object.
		 * @param dest A pointer to @b allocated memory.
		 * @return The dest pointer with the new object who's lifetime was stored.
		 *
		 * @note It is undefined behavior when @b source_pointer and @b dest point to the same object.
		 * @note It is undefined behavior if either or both @b source_pointer and @b dest are nullptr.
		 */
		template<typename Pointer, typename T>
		[[nodiscard]] constexpr auto move_construct(Pointer source_pointer, T * dest) noexcept -> T*
			requires requires {
						 std::is_constructible_v<T, typename std::pointer_traits<Pointer>::element_type>;
					 }
		{
			GAL_NOT_NULL(source_pointer);
			GAL_NOT_NULL(dest);

			auto* result = std::construct_at(dest, std::move(*source_pointer));
			std::destroy_at(std::addressof(*source_pointer));
			return result;
		}

		/**
		 * @brief Move an object to another memory locations.
		 * @tparam Iterator The iterator type.
		 * @tparam T The object type.
		 * @param source_iterator An iterator to an object.
		 * @param dest A pointer to @b allocated memory.
		 * @return The dest pointer with the new object who's lifetime was stored.
		 *
		 * @note It is undefined behavior when @b source_pointer and @b dest point to the same object.
		 * @note It is undefined behavior if either or both @b source_pointer and @b dest are nullptr.
		 */
		template<typename Iterator, typename T>
		[[nodiscard]] constexpr auto move_construct(Iterator source_iterator, T * dest) noexcept -> T*
			requires requires {
						 std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>;
					 }
		{
			GAL_ASSUME(source_iterator != Iterator{});
			GAL_NOT_NULL(dest);

			auto* result = std::construct_at(dest, std::move(*source_iterator));
			std::destroy_at(std::addressof(*source_iterator));
			return result;
		}
	}

	template<typename PoI>
	struct is_valid_range;

	//	template<typename Pointer>
	//		requires requires {
	//					 typename std::pointer_traits<Pointer>::element_type;
	//				 }
	//	struct is_valid_range<Pointer>
	//	{
	//		using type = typename std::pointer_traits<Pointer>::element_type;
	//
	//		[[nodiscard]] constexpr auto operator()(const Pointer begin, const Pointer end) const noexcept -> bool
	//		{
	//			return end - begin >= 0;
	//		}
	//	};
	//
	//	template<typename Iterator>
	//		requires requires {
	//					 typename std::iterator_traits<Iterator>::value_type;
	//				 }
	//	struct is_valid_range<Iterator>
	//	{
	//		using type = typename std::iterator_traits<Iterator>::value_type;
	//
	//	private:
	//		template<typename I>
	//		struct compare;
	//
	//		template<typename I>
	//			requires requires(const I lhs, const I rhs) {
	//						 lhs > rhs;
	//					 }
	//		struct compare<I>
	//		{
	//			[[nodiscard]] constexpr auto operator()(const I begin, const I end) const noexcept -> bool
	//			{
	//				return end >= begin;
	//			}
	//		};
	//
	//		template<typename I>
	//			requires requires(const I lhs, const I rhs) {
	//						 lhs - rhs;
	//					 }
	//		struct compare<I>
	//		{
	//			[[nodiscard]] constexpr auto operator()(const I begin, const I end) const noexcept -> bool
	//			{
	//				return end - begin >= 0;
	//			}
	//		};
	//
	//	public:
	//		[[nodiscard]] constexpr auto operator()(const Iterator begin, const Iterator end) const noexcept -> bool
	//		{
	//			return compare<Iterator>{}(begin, end);
	//		}
	//	};

	template<typename PoI>
	struct is_valid_range
	{
		using type = std::remove_cvref_t<decltype(*std::declval<PoI>())>;

		[[nodiscard]] constexpr auto operator()(const PoI begin, const PoI end) const noexcept -> bool
		{
			return std::distance(begin, end) >= 0;
		}
	};

	export
	{
		/**
		 * @brief Copy objects into a memory location.
		 */
		template<typename PointerOrIterator, typename T>
		constexpr auto copy_construct(PointerOrIterator source_begin, PointerOrIterator source_end, T * dest_begin) noexcept -> void
			requires requires {
						 copy_construct(source_begin, dest_begin);
					 }
		{
			GAL_ASSUME(source_begin != source_end);
			GAL_ASSUME(is_valid_range<PointerOrIterator>{}(source_begin, source_end));

			auto source = source_begin;
			auto dest	= dest_begin;
			while (source != source_end)
			{
				copy_construct(source, dest);
				++source;
				++dest;
			}
		}

		/**
		 * @brief Move objects into a memory location.
		 */
		template<typename PointerOrIterator, typename T>
		constexpr auto move_construct(PointerOrIterator source_begin, PointerOrIterator source_end, T * dest_begin) noexcept -> void
			requires requires {
						 move_construct(source_begin, dest_begin);
					 }
		{
			GAL_ASSUME(source_begin != source_end);
			GAL_ASSUME(is_valid_range<PointerOrIterator>{}(source_begin, source_end));

			auto source = source_begin;
			auto dest	= dest_begin;
			while (source != source_end)
			{
				move_construct(source, dest);
				++source;
				++dest;
			}
		}

		/**
		 * @brief Construct a set of objects.
		 */
		template<typename PointerOrIterator, typename... Args>
		constexpr auto construct(PointerOrIterator dest_begin, PointerOrIterator dest_end, const Args&... args) noexcept(std::is_nothrow_constructible_v<typename is_valid_range<PointerOrIterator>::type, const Args&...>)->void
		{
			GAL_ASSUME(dest_begin != dest_end);
			GAL_ASSUME(is_valid_range<PointerOrIterator>{}(dest_begin, dest_end));

			for (auto it = dest_begin; it != dest_end; ++it)
			{
				std::construct_at(std::addressof(*it), args...);
			}
		}
	}
}// namespace gal::gui::utility
