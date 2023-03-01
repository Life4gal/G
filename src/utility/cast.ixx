module;

#include <macro.hpp>

export module gal.utility:cast;

import std;
import :compare;
import :type_traits;
import :breakpoint;
import :exception;

export namespace gal::gui::utility
{
	/**
	 * @brief Cast a pointer to a class to its base class or itself.
	 * @tparam TargetPointer The base class or itself pointer type.
	 * @tparam Type The derived class type.
	 * @param input The pointer to cast.
	 * @return A pointer of the base class or itself.
	 */
	template<typename TargetPointer, std::derived_from<std::remove_pointer_t<TargetPointer>> Type>
		requires std::is_pointer_v<TargetPointer> and
				 (std::is_const_v<std::remove_pointer_t<TargetPointer>> == std::is_const_v<Type> or
				  std::is_const_v<std::remove_pointer_t<TargetPointer>>)
	[[nodiscard]] constexpr auto up_cast(Type* input) noexcept -> TargetPointer
	{
		if constexpr (std::is_same_v<std::remove_const_t<Type>, std::remove_cv_t<std::remove_pointer_t<TargetPointer>>>)
		{
			return input;
		}
		else
		{
			return static_cast<TargetPointer>(input);
		}
	}

	/**
	 * @brief Cast a pointer to a class to its derived class or itself.
	 * @tparam TargetPointer The derived class or itself pointer type.
	 * @tparam Type The base class type.
	 * @param input The pointer to cast.
	 * @return A pointer of the derived class or itself.
	 *
	 * @note It is undefined behavior if the argument is not of type @b TargetPointer.
	 */
	template<typename TargetPointer, std::derived_from<std::remove_pointer_t<TargetPointer>> Type>
		requires std::is_pointer_v<TargetPointer> and
				 (std::is_const_v<std::remove_pointer_t<TargetPointer>> == std::is_const_v<Type> or
				  std::is_const_v<std::remove_pointer_t<TargetPointer>>)
	[[nodiscard]] constexpr auto down_cast(Type* input) noexcept -> TargetPointer
	{
		if constexpr (std::is_same_v<std::remove_const_t<Type>, std::remove_cv_t<std::remove_pointer_t<TargetPointer>>>)
		{
			return input;
		}
		else
		{
			GAL_GUI_ASSUME(input == nullptr or dynamic_cast<TargetPointer>(input) != nullptr);
			return static_cast<TargetPointer>(input);
		}
	}

	template<typename TargetPointer>
		requires std::is_pointer_v<TargetPointer>
	[[nodiscard]] constexpr auto up_cast(std::nullptr_t) noexcept -> TargetPointer
	{
		return nullptr;
	}

	template<typename TargetPointer>
		requires std::is_pointer_v<TargetPointer>
	[[nodiscard]] constexpr auto down_cast(std::nullptr_t) noexcept -> TargetPointer
	{
		return nullptr;
	}

	/**
	 * @brief Cast a reference to a class to its base class or itself.
	 * @tparam TargetReference The base class or itself reference type.
	 * @tparam Type The derived class type.
	 * @param input The reference to cast.
	 * @return A reference of the base class or itself.
	 */
	template<typename TargetReference, std::derived_from<std::remove_reference_t<TargetReference>> Type>
		requires std::is_reference_v<TargetReference> and
				 (std::is_const_v<std::remove_reference_t<TargetReference>> == std::is_const_v<TargetReference> or
				  std::is_const_v<std::remove_reference_t<TargetReference>>)
	[[nodiscard]] constexpr auto up_cast(Type& input) noexcept -> TargetReference
	{
		if constexpr (std::is_same_v<std::remove_const_t<Type>, std::remove_cvref_t<TargetReference>>)
		{
			return input;
		}
		else
		{
			return static_cast<TargetReference>(input);
		}
	}

	/**
	 * @brief Cast a reference to a class to its derived class or itself.
	 * @tparam TargetReference The derived class or itself reference type.
	 * @tparam Type The base class type.
	 * @param input The reference to cast.
	 * @return A reference of the derived class or itself.
	 */
	template<typename TargetReference, std::derived_from<std::remove_reference_t<TargetReference>> Type>
		requires std::is_reference_v<TargetReference> and
				 (std::is_const_v<std::remove_reference_t<TargetReference>> == std::is_const_v<TargetReference> or
				  std::is_const_v<std::remove_reference_t<TargetReference>>)
	[[nodiscard]] constexpr auto down_cast(Type& input) noexcept -> TargetReference
	{
		if constexpr (std::is_same_v<std::remove_const_t<Type>, std::remove_cvref_t<TargetReference>>)
		{
			return input;
		}
		else
		{
			GAL_GUI_ASSUME(dynamic_cast<std::add_pointer_t<std::remove_reference_t<TargetReference>>>(std::addressof(input)) != nullptr);
			return static_cast<TargetReference>(input);
		}
	}

	/**
	 * @brief Cast a number to a type that will be able to represent all values without loss of precision.
	 * @tparam Out The numeric type to cast to.
	 * @tparam In The numeric type to cast from.
	 * @param input The input value.
	 * @return The ouput value.
	 */
	template<arithmetic Out, arithmetic In>
		requires type_in_range<Out, In>
	[[nodiscard]] constexpr auto wide_cast(const In input) noexcept -> Out
	{
		return static_cast<Out>(input);
	}

	/**
	 * @brief Cast a numeric value to an integer saturating on overflow.
	 * @tparam Out The signed-integer or unsigned-integer type to cast to.
	 * @tparam In The numeric type to cast from.
	 * @param input The input value.
	 * @return The converted value, which is saturated if @b input is overflowing or underflowing.
	 */
	template<integral Out, arithmetic In>
	[[nodiscard]] constexpr auto saturate_cast(const In input) noexcept -> Out
	{
		if constexpr (std::is_floating_point_v<In>)
		{
			if (std::isnan(input))
			{
				return Out{0};
			}
		}

		if (three_way_compare(input, std::numeric_limits<Out>::lowest()) != std::strong_ordering::greater)
		{
			return std::numeric_limits<Out>::lowest();
		}
		else if (three_way_compare(input, std::numeric_limits<Out>::max()) != std::strong_ordering::less)
		{
			return std::numeric_limits<Out>::max();
		}
		else
		{
			return static_cast<Out>(input);
		}
	}

	namespace cast_detail
	{
		template<arithmetic Out, arithmetic In>
		[[nodiscard]] constexpr auto narrow_validate(const Out out, const In in) noexcept -> bool
		{
			// in-value and out-value compares the same, after converting out-value back to in-type.
			auto result = (in == static_cast<In>(out));

			// If the types have different signs we need to do an extra test to make sure the actual sign
			// of the values are the same as well.
			if constexpr (std::numeric_limits<Out>::is_signed != std::numeric_limits<In>::is_signed)
			{
				result &= (in < In{0}) == (out < Out{0});
			}

			return result;
		}
	}// namespace cast_detail

	/**
	 * @brief Cast numeric values without loss of precision.
	 * @tparam Out The numeric type to cast to.
	 * @tparam In The numeric type to cast from
	 * @param input The input value.
	 * @return The value casted to a different type without loss of precision.
	 *
	 * @note It is undefined behavior to cast a value which will cause a loss of precision.
	 */
	template<arithmetic Out, arithmetic In>
	[[nodiscard]] constexpr auto narrow_cast(const In input) noexcept -> Out
	{
		if constexpr (type_in_range<Out, In>)
		{
			return static_cast<Out>(input);
		}
		else
		{
			const auto out = static_cast<Out>(input);
			GAL_GUI_ASSERT(cast_detail::narrow_validate(out, input), "Invalid narrow cast!");
			return out;
		}
	}

	template<arithmetic Out, arithmetic In>
	[[nodiscard]] constexpr auto round_cast(const In input) noexcept -> Out
	{
		if constexpr (std::is_floating_point_v<In>)
		{
			return narrow_cast<Out>(std::round(input));
		}
		else
		{
			return narrow_cast<Out>(input);
		}
	}

	template<arithmetic Out, arithmetic In>
	[[nodiscard]] constexpr auto floor_cast(const In input) noexcept -> Out
	{
		if constexpr (std::is_floating_point_v<In>)
		{
			return narrow_cast<Out>(std::floor(input));
		}
		else
		{
			return narrow_cast<Out>(input);
		}
	}

	template<arithmetic Out, arithmetic In>
	[[nodiscard]] constexpr auto ceil_cast(const In input) noexcept -> Out
	{
		if constexpr (std::is_floating_point_v<In>)
		{
			return narrow_cast<Out>(std::ceil(input));
		}
		else
		{
			return narrow_cast<Out>(input);
		}
	}

	/**
	 * @brief Cast a character.
	 * @tparam Out The value type of the result.
	 * @tparam In The value type of the character.
	 * @param input The value of the character.
	 * @return The casted value.
	 *
	 * @note Both the input and output types are interpreted as unsigned values, even if
	 * 	they are signed values. For example `char` may be either signed or unsigned,
	 * 	but you have to treat those as unsigned values.
	 * @note @b input value after casting, must fit in the output type.
	 */
	template<integral Out, integral In>
	[[nodiscard]] constexpr auto char_cast(const In input) noexcept -> Out
	{
		using unsigned_in_type	= std::make_unsigned_t<In>;
		using unsigned_out_type = std::make_unsigned_t<Out>;

		// We cast to unsigned of the same type, so that we don't accidentally sign extent 'char'.
		auto unsigned_in		= static_cast<unsigned_in_type>(input);
		auto unsigned_out		= narrow_cast<unsigned_out_type>(unsigned_in);
		return static_cast<Out>(unsigned_out);
	}

	template<integral Out>
	[[nodiscard]] constexpr auto char_cast(const std::byte input) noexcept -> Out
	{
		static_assert(sizeof(std::byte) == sizeof(std::uint8_t));
		return char_cast<Out>(static_cast<std::uint8_t>(input));
	}

	/**
	 * @brief Return the low half of the input value.
	 */
	template<unsigned_integral Out, unsigned_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of low_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto low_bit_cast(const In input) noexcept -> Out
	{
		return static_cast<Out>(input);
	}

	/**
	 * @brief Return the high half of the input value.
	 */
	template<unsigned_integral Out, unsigned_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of high_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto high_bit_cast(const In input) noexcept -> Out
	{
		return static_cast<Out>(input >> sizeof(Out) * std::numeric_limits<unsigned char>::digits);
	}

	/**
	 * @brief Return the upper half of the input value.
	 */
	template<unsigned_integral Out, unsigned_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of merge_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto merge_bit_cast(const In high, const In low) noexcept -> Out
	{
		auto result = static_cast<Out>(high);
		result <<= sizeof(In) * std::numeric_limits<unsigned char>::digits;
		result |= static_cast<Out>(low);
		return result;
	}

	/**
	 * @brief Return the low half of the input value.
	 */
	template<signed_integral Out, signed_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of low_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto low_bit_cast(const In input) noexcept -> Out
	{
		using unsigned_in_type	= std::make_unsigned_t<In>;
		using unsigned_out_type = std::make_unsigned_t<Out>;

		return static_cast<Out>(low_bit_cast<unsigned_out_type>(static_cast<unsigned_in_type>(input)));
	}

	/**
	 * @brief Return the high half of the input value.
	 */
	template<signed_integral Out, signed_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of high_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto high_bit_cast(const In input) noexcept -> Out
	{
		using unsigned_in_type	= std::make_unsigned_t<In>;
		using unsigned_out_type = std::make_unsigned_t<Out>;

		return static_cast<Out>(high_bit_cast<unsigned_out_type>(static_cast<unsigned_in_type>(input)));
	}

	/**
	 * @brief Return the upper half of the input value.
	 */
	template<signed_integral Out, signed_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of merge_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto merge_bit_cast(const In high, const In low) noexcept -> Out
	{
		using unsigned_in_type	= std::make_unsigned_t<In>;
		using unsigned_out_type = std::make_unsigned_t<Out>;

		return static_cast<Out>(merge_bit_cast<unsigned_out_type>(static_cast<unsigned_in_type>(high), static_cast<unsigned_in_type>(low)));
	}

	/**
	 * @brief Return the low half of the input value.
	 */
	template<unsigned_integral Out, signed_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of low_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto low_bit_cast(const In input) noexcept -> Out
	{
		using unsigned_in_type = std::make_unsigned_t<In>;

		return static_cast<Out>(low_bit_cast<Out>(static_cast<unsigned_in_type>(input)));
	}

	/**
	 * @brief Return the high half of the input value.
	 */
	template<unsigned_integral Out, signed_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of high_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto high_bit_cast(const In input) noexcept -> Out
	{
		using unsigned_in_type = std::make_unsigned_t<In>;

		return static_cast<Out>(high_bit_cast<Out>(static_cast<unsigned_in_type>(input)));
	}

	/**
	 * @brief Return the upper half of the input value.
	 */
	template<signed_integral Out, unsigned_integral In>
		requires(sizeof(Out) * 2 == sizeof(In))// Return value of merge_bit_cast must be half the size of the input
	[[nodiscard]] constexpr auto merge_bit_cast(const In high, const In low) noexcept -> Out
	{
		using unsigned_out_type = std::make_unsigned_t<Out>;

		return narrow_cast<Out>(merge_bit_cast<unsigned_out_type>(high, low));
	}

	/**
	 * @brief Cast an integral to an unsigned integral of the same size.
	 */
	template<integral In>
	[[nodiscard]] constexpr auto to_unsigned(const In input) noexcept -> std::make_unsigned_t<In>
	{
		return static_cast<std::make_unsigned_t<In>>(input);
	}

	/**
	 * @briefc Cast an integral to an signed integral of the same size.
	 */
	template<integral In>
	[[nodiscard]] constexpr auto to_signed(const In input) noexcept -> std::make_signed_t<In>
	{
		return static_cast<std::make_signed_t<In>>(input);
	}

	/**
	 * @brief Cast between integral types truncating or zero-extending the result.
	 */
	template<integral Out, integral In>
	[[nodiscard]] constexpr auto truncate(const In input) noexcept -> Out
	{
		return static_cast<Out>(to_unsigned(input));
	}

	template<typename Enum>
		requires std::is_enum_v<Enum> or std::is_scoped_enum_v<Enum>
	[[nodiscard]] constexpr auto to_underlying(const Enum e) noexcept -> std::underlying_type_t<Enum>
	{
		// return std::to_underlying(e);
		return static_cast<std::underlying_type_t<Enum>>(e);
	}

	template<typename T>
	[[nodiscard]] constexpr auto to_boolean(T&& input) noexcept -> bool
		requires requires {
					 static_cast<bool>(std::forward<T>(input));
				 }
	{
		return static_cast<bool>(std::forward<T>(input));
	}

	template<typename OutPointer>
		requires std::is_pointer_v<OutPointer>
	[[nodiscard]] constexpr auto to_pointer(const std::intptr_t address) noexcept -> OutPointer
	{
		return reinterpret_cast<OutPointer>(address);
	}

	template<typename OutPointer>
		requires std::is_pointer_v<OutPointer>
	[[nodiscard]] constexpr auto to_pointer(const std::uintptr_t address) noexcept -> OutPointer
	{
		return reinterpret_cast<OutPointer>(address);
	}

	template<typename OutInteger = std::intptr_t>
		requires(std::is_same_v<OutInteger, std::intptr_t> or std::is_same_v<OutInteger, std::uintptr_t>)
	[[nodiscard]] constexpr auto to_address(const void* const pointer) noexcept -> OutInteger
	{
		return reinterpret_cast<OutInteger>(pointer);
	}

	template<typename Out, byte_like In>
		requires std::is_trivially_default_constructible_v<keep_cv_t<Out, In>> and std::is_trivially_destructible_v<keep_cv_t<Out, In>>
	[[nodiscard]] constexpr auto implicit_cast(const std::span<In> bytes) noexcept(false) -> std::add_lvalue_reference<keep_cv_t<Out, In>>
	{
		using value_type = keep_cv_t<Out, In>;

		if (sizeof(value_type) > bytes.size())
		{
			throw std::bad_cast{};
		}

		GAL_GUI_NOT_NULL(bytes.data(), "Cannot implicit_cast null data!");

		if constexpr (alignof(value_type) != 1)
		{
			if (std::bit_cast<std::uintptr_t>(bytes.data()) % alignof(value_type) != 0)
			{
				throw std::bad_cast{};
			}
		}

		return *reinterpret_cast<value_type*>(bytes.data());
	}

	template<typename Out, byte_like In>
		requires std::is_trivially_default_constructible_v<keep_cv_t<Out, In>> and std::is_trivially_destructible_v<keep_cv_t<Out, In>>
	[[nodiscard]] constexpr auto implicit_cast(const std::span<In> bytes, std::size_t& offset) noexcept(false) -> std::add_lvalue_reference<keep_cv_t<Out, In>>
	{
		using value_type = keep_cv_t<Out, In>;

		if (sizeof(value_type) + offset > bytes.size())
		{
			throw std::bad_cast{};
		}

		GAL_GUI_NOT_NULL(bytes.data(), "Cannot implicit_cast null data!");

		const auto data = bytes.data() + offset;
		if constexpr (alignof(value_type) != 1)
		{
			if (std::bit_cast<std::uintptr_t>(data) % alignof(value_type) != 0)
			{
				throw std::bad_cast{};
			}
		}

		offset += sizeof(value_type);
		return *reinterpret_cast<value_type*>(data);
	}

	template<typename Out, byte_like In>
		requires std::is_trivially_default_constructible_v<keep_cv_t<Out, In>> and std::is_trivially_destructible_v<keep_cv_t<Out, In>>
	[[nodiscard]] constexpr auto implicit_cast(const std::span<In> bytes, const std::size_t n) noexcept(false) -> std::span<keep_cv_t<Out, In>>
	{
		using value_type = keep_cv_t<Out, In>;

		if (sizeof(value_type) * n > bytes.size())
		{
			throw std::bad_cast{};
		}
		GAL_GUI_NOT_NULL(bytes.data(), "Cannot implicit_cast null data!");

		if constexpr (alignof(value_type) != 1)
		{
			if (std::bit_cast<std::uintptr_t>(bytes.data()) % alignof(value_type) != 0)
			{
				throw std::bad_cast{};
			}
		}

		return std::span<keep_cv_t<Out, In>>{reinterpret_cast<value_type*>(bytes.data()), n};
	}

	template<typename Out, byte_like In>
		requires std::is_trivially_default_constructible_v<keep_cv_t<Out, In>> and std::is_trivially_destructible_v<keep_cv_t<Out, In>>
	[[nodiscard]] constexpr auto implicit_cast(const std::span<In> bytes, const std::size_t n, std::size_t& offset) noexcept(false) -> std::span<keep_cv_t<Out, In>>
	{
		using value_type = keep_cv_t<Out, In>;

		if (sizeof(value_type) * n + offset > bytes.size())
		{
			throw std::bad_cast{};
		}

		GAL_GUI_NOT_NULL(bytes.data(), "Cannot implicit_cast null data!");

		const auto data = bytes.data() + offset;
		if constexpr (alignof(value_type) != 1)
		{
			if (std::bit_cast<std::uintptr_t>(data) % alignof(value_type) != 0)
			{
				throw std::bad_cast{};
			}
		}

		offset += sizeof(value_type) * n;
		return std::span<keep_cv_t<Out, In>>{reinterpret_cast<value_type*>(bytes.data()), n};
	}

	constexpr auto operator""_uz(const std::size_t value) noexcept -> std::size_t
	{
		// Hello MSVC?
		return value;
	}
}// namespace gal::gui::utility
