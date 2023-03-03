module;

#include <macro.hpp>

export module gal.utility:constexpr_string.template_string;

import std;
import :constexpr_string.base;

namespace gal::gui::utility
{
	namespace template_string_detail
	{
		template<typename T, T... Chars>
		struct basic_template_string : constexpr_string_base<basic_template_string<T, Chars...>, T>
		{
			using value_type								   = T;
			using size_type									   = std::size_t;

			using const_iterator							   = const value_type*;

			constexpr static size_type					  size = sizeof...(Chars);
			constexpr static value_type					  value[size]{Chars...};
			constexpr static size_type					  size_no_0 = size - (value[size - 1] == '\0');// include '\0' ?

			[[nodiscard]] constexpr static const_iterator begin() noexcept { return std::ranges::begin(value); }

			[[nodiscard]] constexpr static const_iterator end() noexcept { return std::ranges::end(value); }
		};

		template<typename, typename>
		struct basic_bilateral_template_string;

		template<typename T, T... Left, T... Right>
		struct basic_bilateral_template_string<basic_template_string<T, Left...>, basic_template_string<T, Right...>>
			: bilateral_constexpr_string_base<
					  basic_bilateral_template_string<basic_template_string<T, Left...>, basic_template_string<T, Right...>>,
					  basic_template_string<T, Left...>,
					  basic_template_string<T, Right...>,
					  typename basic_template_string<T, Left...>::value_type>
		{
			using left_type												  = basic_template_string<T, Left...>;
			using right_type											  = basic_template_string<T, Right...>;

			using value_type											  = typename left_type::value_type;
			using size_type												  = typename left_type::size_type;

			using const_iterator										  = typename left_type::const_iterator;

			constexpr static size_type					  left_size		  = left_type::template size;
			constexpr static size_type					  left_size_no_0  = left_type::template size_no_0;
			constexpr static size_type					  right_size	  = right_type::template size;
			constexpr static size_type					  right_size_no_0 = right_type::template size_no_0;

			[[nodiscard]] constexpr static const_iterator left_begin() noexcept { return left_type::template begin(); }

			[[nodiscard]] constexpr static const_iterator left_end() noexcept { return left_type::template end(); }

			[[nodiscard]] constexpr static const_iterator right_begin() noexcept { return right_type::template begin(); }

			[[nodiscard]] constexpr static const_iterator right_end() noexcept { return right_type::template end(); }
		};

		template<typename...>
		struct basic_multiple_template_string;

		template<typename BasicTemplateString, typename... BasicTemplateStrings>
			requires(std::is_same_v<typename BasicTemplateString::value_type, typename BasicTemplateStrings::value_type> && ...)
		struct basic_multiple_template_string<BasicTemplateString, BasicTemplateStrings...>
			: multiple_constexpr_string_base<
					  basic_multiple_template_string<BasicTemplateString, BasicTemplateStrings...>,
					  typename BasicTemplateString::value_type,
					  typename BasicTemplateString::size_type,
					  BasicTemplateString,
					  BasicTemplateStrings...>
		{
			using base_type		 = multiple_constexpr_string_base<basic_multiple_template_string<BasicTemplateString, BasicTemplateStrings...>, typename BasicTemplateString::value_type, typename BasicTemplateString::size_type, BasicTemplateString, BasicTemplateStrings...>;

			using value_type	 = typename BasicTemplateString::value_type;
			using size_type		 = typename BasicTemplateString::size_type;

			using const_iterator = typename BasicTemplateString::const_iterator;

			template<std::size_t Index>
			[[nodiscard]] constexpr static const_iterator begin() noexcept
			{
				return typename base_type::template subtype<Index>::begin();
			}

			template<std::size_t Index>
			[[nodiscard]] constexpr static const_iterator end() noexcept
			{
				return typename base_type::template subtype<Index>::end();
			}
		};
	}// namespace template_string_detail

	export
	{
		template<char... Chars>
		using template_string = template_string_detail::basic_template_string<char, Chars...>;
		template<wchar_t... Chars>
		using template_wstring = template_string_detail::basic_template_string<wchar_t, Chars...>;
		template<char8_t... Chars>
		// ReSharper disable once CppInconsistentNaming
		using template_u8string = template_string_detail::basic_template_string<char8_t, Chars...>;
		template<char16_t... Chars>
		// ReSharper disable once CppInconsistentNaming
		using template_u16string = template_string_detail::basic_template_string<char16_t, Chars...>;
		template<char32_t... Chars>
		// ReSharper disable once CppInconsistentNaming
		using template_u32string = template_string_detail::basic_template_string<char32_t, Chars...>;

		template<typename Left, typename Right>
			requires std::is_same_v<typename Left::value_type, typename Right::value_type> && std::is_same_v<typename Left::value_type, char>
		using template_bilateral_string = template_string_detail::basic_bilateral_template_string<Left, Right>;
		template<typename Left, typename Right>
			requires std::is_same_v<typename Left::value_type, typename Right::value_type> && std::is_same_v<typename Left::value_type, wchar_t>
		using template_bilateral_wstring = template_string_detail::basic_bilateral_template_string<Left, Right>;
		template<typename Left, typename Right>
			requires std::is_same_v<typename Left::value_type, typename Right::value_type> && std::is_same_v<typename Left::value_type, char8_t>
		// ReSharper disable once CppInconsistentNaming
		using template_bilateral_u8string = template_string_detail::basic_bilateral_template_string<Left, Right>;
		template<typename Left, typename Right>
			requires std::is_same_v<typename Left::value_type, typename Right::value_type> && std::is_same_v<typename Left::value_type, char16_t>
		// ReSharper disable once CppInconsistentNaming
		using template_bilateral_u16string = template_string_detail::basic_bilateral_template_string<Left, Right>;
		template<typename Left, typename Right>
			requires std::is_same_v<typename Left::value_type, typename Right::value_type> && std::is_same_v<typename Left::value_type, char32_t>
		// ReSharper disable once CppInconsistentNaming
		using template_bilateral_u32string = template_string_detail::basic_bilateral_template_string<Left, Right>;

		template<typename FirstString, typename... RemainingStrings>
			requires(std::is_same_v<typename FirstString::value_type, typename RemainingStrings::value_type> && ...) && std::is_same_v<typename FirstString::value_type, char>
		using template_multiple_string = template_string_detail::basic_multiple_template_string<FirstString, RemainingStrings...>;
		template<typename FirstString, typename... RemainingStrings>
			requires(std::is_same_v<typename FirstString::value_type, typename RemainingStrings::value_type> && ...) && std::is_same_v<typename FirstString::value_type, wchar_t>
		using template_multiple_wstring = template_string_detail::basic_multiple_template_string<FirstString, RemainingStrings...>;
		template<typename FirstString, typename... RemainingStrings>
			requires(std::is_same_v<typename FirstString::value_type, typename RemainingStrings::value_type> && ...) && std::is_same_v<typename FirstString::value_type, char8_t>
		// ReSharper disable once CppInconsistentNaming
		using template_multiple_u8string = template_string_detail::basic_multiple_template_string<FirstString, RemainingStrings...>;
		template<typename FirstString, typename... RemainingStrings>
			requires(std::is_same_v<typename FirstString::value_type, typename RemainingStrings::value_type> && ...) && std::is_same_v<typename FirstString::value_type, char16_t>
		// ReSharper disable once CppInconsistentNaming
		using template_multiple_u16string = template_string_detail::basic_multiple_template_string<FirstString, RemainingStrings...>;
		template<typename FirstString, typename... RemainingStrings>
			requires(std::is_same_v<typename FirstString::value_type, typename RemainingStrings::value_type> && ...) && std::is_same_v<typename FirstString::value_type, char32_t>
		// ReSharper disable once CppInconsistentNaming
		using template_multiple_u32string = template_string_detail::basic_multiple_template_string<FirstString, RemainingStrings...>;
	}

	namespace template_string_test
	{
		using namespace std::string_view_literals;

		static_assert(GAL_TEMPLATE_STRING_TYPE("hello world")::match("hello world"));
		static_assert(GAL_TEMPLATE_STRING_TYPE("hello world")::match("hello world"sv));
		static_assert(GAL_TEMPLATE_STRING_TYPE("hello world")::as_string_view() == "hello world");

		static_assert(GAL_TEMPLATE_WSTRING_TYPE("hello world")::match(L"hello world"));
		static_assert(GAL_TEMPLATE_WSTRING_TYPE("hello world")::match(L"hello world"sv));
		static_assert(GAL_TEMPLATE_WSTRING_TYPE("hello world")::as_string_view() == L"hello world");

		static_assert(GAL_TEMPLATE_U8STRING_TYPE("hello world")::match(u8"hello world"));
		static_assert(GAL_TEMPLATE_U8STRING_TYPE("hello world")::match(u8"hello world"sv));
		static_assert(GAL_TEMPLATE_U8STRING_TYPE("hello world")::as_string_view() == u8"hello world");

		static_assert(GAL_TEMPLATE_U16STRING_TYPE("hello world")::match(u"hello world"));
		static_assert(GAL_TEMPLATE_U16STRING_TYPE("hello world")::match(u"hello world"sv));
		static_assert(GAL_TEMPLATE_U16STRING_TYPE("hello world")::as_string_view() == u"hello world");

		static_assert(GAL_TEMPLATE_U32STRING_TYPE("hello world")::match(U"hello world"));
		static_assert(GAL_TEMPLATE_U32STRING_TYPE("hello world")::match(U"hello world"sv));
		static_assert(GAL_TEMPLATE_U32STRING_TYPE("hello world")::as_string_view() == U"hello world");

		using bilateral_hello_world_type = GAL_TEMPLATE_BILATERAL_STRING_TYPE("hello", "world");
		static_assert(bilateral_hello_world_type::match_left("hello"));
		static_assert(bilateral_hello_world_type::match_left("hello"sv));
		static_assert(bilateral_hello_world_type::match_right("world"));
		static_assert(bilateral_hello_world_type::match_right("world"sv));

		using symmetry_hello_world_type = GAL_TEMPLATE_SYMMETRY_STRING_TYPE(
				"({[{("
				")}]})");
		static_assert(symmetry_hello_world_type::match_left("({[{("));
		static_assert(symmetry_hello_world_type::match_left("({[{("sv));
		static_assert(symmetry_hello_world_type::match_right(")}]})"));
		static_assert(symmetry_hello_world_type::match_right(")}]})"sv));

		using multiple_hello_world_type = GAL_TEMPLATE_MULTIPLE_STRING_TYPE_8("h", "e", "l", "l", "o", " ", "world", "!");
		static_assert(multiple_hello_world_type::match<0>("h"));
		static_assert(multiple_hello_world_type::match<0>("h"sv));
		static_assert(multiple_hello_world_type::match<1>("e"));
		static_assert(multiple_hello_world_type::match<1>("e"sv));
		static_assert(multiple_hello_world_type::match<2>("l"));
		static_assert(multiple_hello_world_type::match<2>("l"sv));
		static_assert(multiple_hello_world_type::match<3>("l"));
		static_assert(multiple_hello_world_type::match<3>("l"sv));
		static_assert(multiple_hello_world_type::match<4>("o"));
		static_assert(multiple_hello_world_type::match<4>("o"sv));
		static_assert(multiple_hello_world_type::match<5>(" "));
		static_assert(multiple_hello_world_type::match<5>(" "sv));
		static_assert(multiple_hello_world_type::match<6>("world"));
		static_assert(multiple_hello_world_type::match<6>("world"sv));
		static_assert(multiple_hello_world_type::match<7>("!"));
		static_assert(multiple_hello_world_type::match<7>("!"sv));
	}// namespace template_string_test
}// namespace gal::gui::utility
