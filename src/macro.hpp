#pragma once

#if defined(G_COMPILER_MSVC)
	#define GAL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
	#define GAL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#if defined(G_COMPILER_MSVC)
	#define GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT operator(
	#define GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT )
#else
	#define GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT operator[
	#define GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT ]
#endif

#define GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT GAL_PRIVATE_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT

#if defined(G_COMPILER_APPLE_CLANG) || defined(G_COMPILER_CLANG_CL) || defined(G_COMPILER_CLANG)
	#define GAL_NO_DESTROY [[clang::no_destroy]]
#else
	#define GAL_NO_DESTROY
#endif

#define GAL_PRIVATE_TO_STRING(s) #s
#define GAL_TO_STRING(s) GAL_PRIVATE_TO_STRING(s)

/**
 * @brief Set the message to display when the application terminates.
 * @see gal.utility.exception
 */
#define GAL_SET_TERMINATE_REASON(...) \
	::gal::gui::utility::terminate_reason.store("[" __FILE__ ":" GAL_TO_STRING(__LINE__) "] -> " __VA_ARGS__, std::memory_order::relaxed)

/**
 * @brief Prepare for breaking in the debugger.
 * @see gal.utility.breakpoint
 */
#define GAL_THE_WORLD(...)                 \
	GAL_SET_TERMINATE_REASON(__VA_ARGS__); \
	::gal::gui::utility::the_world()


#define GAL_ASSERT(expression, ...)                                                                    \
	do {                                                                                               \
		if (not static_cast<bool>(expression))                                                         \
		{                                                                                              \
			GAL_THE_WORLD("[ASSERT FAILED]: \"" __VA_ARGS__ "\" --> {" GAL_TO_STRING(expression) "}"); \
		}                                                                                              \
	} while (false)

#define GAL_ASSUME(expression, ...) GAL_ASSERT(expression __VA_OPT__(, ) __VA_ARGS__)

#define GAL_NOT_NULL(pointer, ...)                                                                    \
	do {                                                                                              \
		if (pointer == nullptr)                                                                       \
		{                                                                                             \
			GAL_THE_WORLD("[NOT-NULL FAILED]: \"" __VA_ARGS__ "\" --> {" GAL_TO_STRING(pointer) "}"); \
		}                                                                                             \
	} while (false)

#define GAL_UNREACHABLE(...)                                                      \
	[]<bool always_false = false>                                                 \
	{                                                                             \
		static_assert(always_false, "[UNREACHABLE BRANCH]: \"" __VA_ARGS__ "\""); \
	}                                                                             \
	();                                                                           \
	std::unreachable()

#define GAL_GUI_IMPLEMENTED(...) [[unlikely]] GAL_THE_WORLD("[NOT IMPLEMENTED]: \"" __VA_ARGS__ "\"");

#define GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(string_type, string, string_length, begin_index) \
	decltype([]<std::size_t... Index>(std::index_sequence<Index...>) constexpr noexcept          \
			 { return ::gal::gui::utility::string_type<                                          \
					   [](std::size_t index) constexpr noexcept                                  \
					   {                                                                         \
						   return (string)[(begin_index) + index];                               \
					   }(Index)...>{}; }(std::make_index_sequence<string_length>{}))

#define GAL_PRIVATE_GENERATE_TEMPLATE_STRING(string_type, string) \
	GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(string_type, string, sizeof(string) / sizeof((string)[0]), 0)

/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_STRING_TYPE(string) GAL_PRIVATE_GENERATE_TEMPLATE_STRING(template_string, string)
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_WSTRING_TYPE(string) GAL_PRIVATE_GENERATE_TEMPLATE_STRING(template_wstring, string)
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_U8STRING_TYPE(string) GAL_PRIVATE_GENERATE_TEMPLATE_STRING(template_u8string, string)
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_U16STRING_TYPE(string) GAL_PRIVATE_GENERATE_TEMPLATE_STRING(template_u16string, string)
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_U32STRING_TYPE(string) GAL_PRIVATE_GENERATE_TEMPLATE_STRING(template_u32string, string)

/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_BILATERAL_STRING_TYPE(left_string, right_string) ::gal::gui::utility::template_bilateral_string<GAL_TEMPLATE_STRING_TYPE(left_string), GAL_TEMPLATE_STRING_TYPE(right_string)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_BILATERAL_WSTRING_TYPE(left_string, right_string) ::gal::gui::utility::template_bilateral_wstring<GAL_TEMPLATE_WSTRING_TYPE(left_string), GAL_TEMPLATE_WSTRING_TYPE(right_string)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_BILATERAL_U8STRING_TYPE(left_string, right_string) ::gal::gui::utility::template_bilateral_u8string<GAL_TEMPLATE_U8STRING_TYPE(left_string), GAL_TEMPLATE_U8STRING_TYPE(right_string)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_BILATERAL_U16STRING_TYPE(left_string, right_string) ::gal::gui::utility::template_bilateral_u16string<GAL_TEMPLATE_U16STRING_TYPE(left_string), GAL_TEMPLATE_U16STRING_TYPE(right_string)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_BILATERAL_U32STRING_TYPE(left_string, right_string) ::gal::gui::utility::template_bilateral_u32string<GAL_TEMPLATE_U32STRING_TYPE(left_string), GAL_TEMPLATE_U32STRING_TYPE(right_string)>

/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_SYMMETRY_STRING_TYPE(string)                                                                          \
	::gal::gui::utility::template_bilateral_string<                                                                        \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_string, string, sizeof(string) / sizeof((string)[0]) / 2, 0), \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_string, string, sizeof(string) / sizeof((string)[0]) / 2, sizeof(string) / sizeof((string)[0]) / 2)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_SYMMETRY_WSTRING_TYPE(string)                                                                          \
	::gal::gui::utility::template_bilateral_wstring<                                                                        \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_wstring, string, sizeof(string) / sizeof((string)[0]) / 2, 0), \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_wstring, string, sizeof(string) / sizeof((string)[0]) / 2, sizeof(string) / sizeof((string)[0]) / 2)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_SYMMETRY_U8STRING_TYPE(string)                                                                          \
	::gal::gui::utility::template_bilateral_u8string<                                                                        \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u8string, string, sizeof(string) / sizeof((string)[0]) / 2, 0), \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u8string, string, sizeof(string) / sizeof((string)[0]) / 2, sizeof(string) / sizeof((string)[0]) / 2)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_SYMMETRY_U16STRING_TYPE(string)                                                                          \
	::gal::gui::utility::template_bilateral_u16string<                                                                        \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u16string, string, sizeof(string) / sizeof((string)[0]) / 2, 0), \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u16string, string, sizeof(string) / sizeof((string)[0]) / 2, sizeof(string) / sizeof((string)[0]) / 2)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
// ReSharper disable once CppInconsistentNaming
#define GAL_TEMPLATE_SYMMETRY_U32STRING_TYPE(string)                                                                          \
	::gal::gui::utility::template_bilateral_u32string<                                                                        \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u32string, string, sizeof(string) / sizeof((string)[0]) / 2, 0), \
			GAL_PRIVATE_DO_GENERATE_TEMPLATE_STRING(template_u32string, string, sizeof(string) / sizeof((string)[0]) / 2, sizeof(string) / sizeof((string)[0]) / 2)>

// todo: we need a way to get the number of parameters and expand to the corresponding parameter list exactly.
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_1(string) \
	::gal::gui::utility::template_multiple_string<  \
			GAL_TEMPLATE_STRING_TYPE(string)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_2(string1, string2) \
	::gal::gui::utility::template_multiple_string<            \
			GAL_TEMPLATE_STRING_TYPE(string1),                \
			GAL_TEMPLATE_STRING_TYPE(string2)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_3(string1, string2, string3) \
	::gal::gui::utility::template_multiple_string<                     \
			GAL_TEMPLATE_STRING_TYPE(string1),                         \
			GAL_TEMPLATE_STRING_TYPE(string2),                         \
			GAL_TEMPLATE_STRING_TYPE(string3)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_4(string1, string2, string3, string4) \
	::gal::gui::utility::template_multiple_string<                              \
			GAL_TEMPLATE_STRING_TYPE(string1),                                  \
			GAL_TEMPLATE_STRING_TYPE(string2),                                  \
			GAL_TEMPLATE_STRING_TYPE(string3),                                  \
			GAL_TEMPLATE_STRING_TYPE(string4)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_5(string1, string2, string3, string4, string5) \
	::gal::gui::utility::template_multiple_string<                                       \
			GAL_TEMPLATE_STRING_TYPE(string1),                                           \
			GAL_TEMPLATE_STRING_TYPE(string2),                                           \
			GAL_TEMPLATE_STRING_TYPE(string3),                                           \
			GAL_TEMPLATE_STRING_TYPE(string4),                                           \
			GAL_TEMPLATE_STRING_TYPE(string5)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_6(string1, string2, string3, string4, string5, string6) \
	::gal::gui::utility::template_multiple_string<                                                \
			GAL_TEMPLATE_STRING_TYPE(string1),                                                    \
			GAL_TEMPLATE_STRING_TYPE(string2),                                                    \
			GAL_TEMPLATE_STRING_TYPE(string3),                                                    \
			GAL_TEMPLATE_STRING_TYPE(string4),                                                    \
			GAL_TEMPLATE_STRING_TYPE(string5),                                                    \
			GAL_TEMPLATE_STRING_TYPE(string6)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_7(string1, string2, string3, string4, string5, string6, string7) \
	::gal::gui::utility::template_multiple_string<                                                         \
			GAL_TEMPLATE_STRING_TYPE(string1),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string2),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string3),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string4),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string5),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string6),                                                             \
			GAL_TEMPLATE_STRING_TYPE(string7)>
/**
 * @see gal.utility.constexpr_string.template_string
 */
#define GAL_TEMPLATE_MULTIPLE_STRING_TYPE_8(string1, string2, string3, string4, string5, string6, string7, string8) \
	::gal::gui::utility::template_multiple_string<                                                                  \
			GAL_TEMPLATE_STRING_TYPE(string1),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string2),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string3),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string4),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string5),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string6),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string7),                                                                      \
			GAL_TEMPLATE_STRING_TYPE(string8)>
