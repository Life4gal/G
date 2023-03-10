module;

export module gal.utility:constexpr_string.base;

import std;

export namespace gal::gui::utility
{
	template<typename Derived, typename ValueType, typename SizeType = std::size_t>
	struct constexpr_string_base
	{
		using derived_type = std::remove_cvref_t<Derived>;

		using value_type   = ValueType;
		using size_type	   = SizeType;

		[[nodiscard]] constexpr operator std::basic_string_view<value_type>() const noexcept// NOLINT
		{
			return std::basic_string_view<value_type>{derived_type::value, derived_type::size_no_0};
		}

		[[nodiscard]] constexpr static auto as_string_view() noexcept -> std::basic_string_view<value_type>
		{
			return std::basic_string_view<value_type>{derived_type::value, derived_type::size_no_0};
		}

		[[nodiscard]] constexpr operator std::basic_string<value_type>() const noexcept// NOLINT
		{
			return std::basic_string<value_type>{derived_type::value, derived_type::size_no_0};
		}

		[[nodiscard]] constexpr static auto as_string() noexcept -> std::basic_string<value_type>
		{
			return std::basic_string<value_type>{derived_type::value, derived_type::size_no_0};
		}

		template<std::size_t Index>
			requires(Index < derived_type::size_no_0) and
					requires {
						std::get<Index>(derived_type::value);
					}
		[[nodiscard]] constexpr static auto get(constexpr_string_base& string) noexcept -> value_type&
		{
			return std::get<Index>(derived_type::value);
		}

		template<std::size_t Index>
			requires(Index < derived_type::size_no_0) and
					requires {
						std::get<Index>(derived_type::value);
					}
		[[nodiscard]] constexpr static auto get(const constexpr_string_base& string) noexcept -> value_type
		{
			return std::get<Index>(derived_type::value);
		}

		[[nodiscard]] constexpr static bool match(const value_type* string) noexcept
			requires requires {
						 derived_type::size_no_0;
					 }
		{
			return std::char_traits<value_type>::length(string) == derived_type::size_no_0 and
				   std::char_traits<value_type>::compare(derived_type::value, string, derived_type::size_no_0) == 0;
		}

		[[nodiscard]] constexpr bool match(const value_type* string) const noexcept
			requires std::is_member_function_pointer_v<decltype(&derived_type::size_no_0)>
		{
			return std::char_traits<value_type>::length(string) == static_cast<const derived_type&>(*this).size_no_0() and
				   std::char_traits<value_type>::compare(static_cast<const derived_type&>(*this).value, string, static_cast<const derived_type&>(*this).size_no_0()) == 0;
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match(
				const Container<value_type, AnyOther...>& container,
				CharGetter								  getter,
				CharComparator							  comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
																			   std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 derived_type::size_no_0;
						 derived_type::value;
					 }
		{
			return container.size() == derived_type::size_no_0 and [&]<std::size_t... Index>(std::index_sequence<Index...>)
			{ return ((comparator(getter(container, Index), derived_type::value[Index])) and ...); }(std::make_index_sequence<derived_type::size_no_0>{});
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match(
				const Container<value_type, AnyOther...>& container,
				CharGetter								  getter,
				CharComparator							  comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&derived_type::size_no_0)>
		{
			if (container.size() != static_cast<const derived_type&>(*this).size_no_0()) { return false; }

			for (decltype(container.size()) i = 0; i < container.size(); ++i)
			{
				if (not comparator(getter(container, i), static_cast<const derived_type&>(*this).value[i])) { return false; }
			}

			return true;
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match(
				const Container& container,
				CharGetter		 getter,
				CharComparator	 comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
																			   std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 derived_type::size_no_0;
						 derived_type::value;
					 }
		{
			return container.size() == derived_type::size_no_0 and [&]<std::size_t... Index>(std::index_sequence<Index...>)
			{ return ((comparator(getter(container, Index), derived_type::value[Index])) and ...); }(std::make_index_sequence<derived_type::size_no_0>{});
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match(
				const Container& container,
				CharGetter		 getter,
				CharComparator	 comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&derived_type::size_no_0)>
		{
			if (container.size() != static_cast<const derived_type&>(*this).size_no_0()) { return false; }

			for (decltype(container.size()) i = 0; i < container.size(); ++i)
			{
				if (not comparator(getter(container, i), static_cast<const derived_type&>(*this).value[i])) { return false; }
			}

			return true;
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(constexpr_string_base::match(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and
					 requires {
						 derived_type::size_no_0;
						 derived_type::value;
					 }
		{
			return constexpr_string_base::match(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const constexpr_string_base&>.match(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and std::is_member_function_pointer_v<decltype(&derived_type::size_no_0)>
		{
			return this->match(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match(
				const Container& container,
				CharComparator	 comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(constexpr_string_base::match(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and
					 requires {
						 derived_type::size_no_0;
						 derived_type::value;
					 }
		{
			return constexpr_string_base::match(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match(
				const Container& container,
				CharComparator	 comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const constexpr_string_base&>.match(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and std::is_member_function_pointer_v<decltype(&derived_type::size_no_0)>
		{
			return this->match(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}
	};

	template<typename Derived, typename LeftType, typename RightType, typename ValueType, typename SizeType = std::size_t>
		requires std::is_base_of_v<constexpr_string_base<LeftType, ValueType, SizeType>, LeftType> and std::is_base_of_v<constexpr_string_base<RightType, ValueType, SizeType>, RightType>
	struct bilateral_constexpr_string_base
	{
		using derived_type = Derived;

		using left_type	   = LeftType;
		using right_type   = RightType;

		static_assert(std::is_same_v<ValueType, typename left_type::value_type>);
		static_assert(std::is_same_v<ValueType, typename right_type::value_type>);
		static_assert(std::is_same_v<SizeType, typename left_type::size_type>);
		static_assert(std::is_same_v<SizeType, typename right_type::size_type>);

		using value_type = ValueType;
		using size_type	 = SizeType;

		[[nodiscard]] constexpr static bool match_left(const value_type* string) noexcept
			requires requires {
						 left_type::size_no_0;
					 }
		{
			return left_type::match(string);
		}

		[[nodiscard]] constexpr bool match_left(const value_type* string) const noexcept
			requires std::is_member_function_pointer_v<decltype(&left_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).left_value.match(string);
		}

		[[nodiscard]] constexpr static bool match_right(const value_type* string) noexcept
			requires requires {
						 right_type::size_no_0;
					 }
		{
			return right_type::match(string);
		}

		[[nodiscard]] constexpr bool match_right(const value_type* string) const noexcept
			requires std::is_member_function_pointer_v<decltype(&right_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).right_value.match(string);
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match_left(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 left_type::size_no_0;
						 left_type::value;
					 }
		{
			return left_type::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match_left(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&left_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).left_value.match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match_right(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 right_type::size_no_0;
						 right_type::value;
					 }
		{
			return right_type::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match_right(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&right_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).right_value.match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match_left(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 left_type::size_no_0;
						 left_type::value;
					 }
		{
			return left_type::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_left(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&left_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).left_value.match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match_right(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 right_type::size_no_0;
						 right_type::value;
					 }
		{
			return right_type::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																							 { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_right(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&right_type::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).right_value.match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match_left(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(bilateral_constexpr_string_base::match_left(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and
					 requires {
						 left_type::size_no_0;
						 left_type::value;
					 }
		{
			return bilateral_constexpr_string_base::match_left(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match_left(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const bilateral_constexpr_string_base&>.match_left(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and std::is_member_function_pointer_v<decltype(&left_type::size_no_0)>
		{
			return this->match_left(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match_right(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(bilateral_constexpr_string_base::match_right(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and
					 requires {
						 right_type::size_no_0;
						 right_type::value;
					 }
		{
			return bilateral_constexpr_string_base::match_right(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match_right(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const bilateral_constexpr_string_base&>.match_right(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and std::is_member_function_pointer_v<decltype(&right_type::size_no_0)>
		{
			return this->match_right(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match_left(
				const Container& container,
				CharComparator	 comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(bilateral_constexpr_string_base::match_left(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and
					 requires {
						 left_type::size_no_0;
						 left_type::value;
					 }
		{
			return bilateral_constexpr_string_base::match_left(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_left(
				const Container& container,
				CharComparator	 comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const bilateral_constexpr_string_base&>.match_left(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and std::is_member_function_pointer_v<decltype(&left_type::size_no_0)>
		{
			return this->match_left(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match_right(
				const Container& container,
				CharComparator	 comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(bilateral_constexpr_string_base::match_right(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and
					 requires {
						 right_type::size_no_0;
						 right_type::value;
					 }
		{
			return bilateral_constexpr_string_base::match_right(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																		{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_right(
				const Container& container,
				CharComparator	 comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const bilateral_constexpr_string_base&>.match_right(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and std::is_member_function_pointer_v<decltype(&right_type::size_no_0)>
		{
			return this->match_right(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}
	};

	template<typename Derived, typename ValueType, typename SizeType, typename... ConstexprString>
		requires(sizeof...(ConstexprString) >= 1) and (std::is_base_of_v<constexpr_string_base<ConstexprString, ValueType, SizeType>, ConstexprString> and ...)
	struct multiple_constexpr_string_base
	{
		using derived_type						   = Derived;

		using multiple_type						   = std::tuple<ConstexprString...>;
		using multiple_size_type				   = std::tuple_size<multiple_type>;
		constexpr static std::size_t multiple_size = multiple_size_type::value;

		template<std::size_t Index>
			requires(Index < multiple_size)
		using subtype	 = std::tuple_element_t<Index, multiple_type>;

		using value_type = ValueType;// typename subtype<0>::value_type;
		using size_type	 = SizeType; //typename subtype<0>::size_type;

		template<std::size_t Index>
		[[nodiscard]] constexpr static bool match(const value_type* string) noexcept
			requires requires {
						 subtype<Index>::size_no_0;
					 }
		{
			return subtype<Index>::match(string);
		}

		template<std::size_t Index>
		[[nodiscard]] constexpr bool match(const value_type* string) const noexcept
			requires std::is_member_function_pointer_v<decltype(&subtype<Index>::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).template value<Index>().match(string);
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<std::size_t Index, template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 subtype<Index>::size_no_0;
						 subtype<Index>::value;
					 }
		{
			return subtype<Index>::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<std::size_t Index, template<typename...> typename Container, typename CharGetter, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match(
				const Container<value_type, AnyOther...>& container,
				CharGetter&&							  getter,
				CharComparator&&						  comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container<value_type, AnyOther...>&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&subtype<Index>::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).template value<Index>().match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
	 * @note CharGetter' s parameters are the indices of value
	 * @note CharComparator' s first parameter is container's element
	 */
		template<std::size_t Index, typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																												{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match_left(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
																				 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and
					 requires {
						 subtype<Index>::size_no_0;
						 subtype<Index>::value;
					 }
		{
			return subtype<Index>::match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		/**
		 * @note CharGetter' s parameters are the indices of value
		 * @note CharComparator' s first parameter is container's element
		 */
		template<std::size_t Index, typename Container, typename CharGetter, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																												{ return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_left(
				const Container& container,
				CharGetter&&	 getter,
				CharComparator&& comparator = CharComparator{}) const
				noexcept(std::is_nothrow_invocable_v<CharGetter, const Container&, size_type>and
								 std::is_nothrow_invocable_v<CharComparator, decltype(getter(container, std::declval<size_type>())), value_type>)
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 comparator(getter(container, std::declval<size_type>()), std::declval<value_type>())
						 } -> std::convertible_to<bool>;
					 } and std::is_member_function_pointer_v<decltype(&subtype<Index>::size_no_0)>
		{
			return static_cast<const derived_type&>(*this).template value<Index>().match(container, std::forward<CharGetter>(getter), std::forward<CharComparator>(comparator));
		}

		template<std::size_t Index, template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr static bool match(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(multiple_constexpr_string_base::match<Index>(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and
					 requires {
						 subtype<Index>::size_no_0;
						 subtype<Index>::value;
					 }
		{
			return multiple_constexpr_string_base::match<Index>(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<std::size_t Index, template<typename...> typename Container, typename CharComparator = std::equal_to<>, typename... AnyOther>
		[[nodiscard]] constexpr bool match_left(
				const Container<value_type, AnyOther...>& container,
				CharComparator							  comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const multiple_constexpr_string_base&>.template match<Index>(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
						 {
							 container[std::declval<size_type>()]
						 } -> std::same_as<value_type>;
					 } and std::is_member_function_pointer_v<decltype(&subtype<Index>::size_no_0)>
		{
			return this->template match<Index>(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<std::size_t Index, typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																						   { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr static bool match(
				const Container& container,
				CharComparator	 comparator = {}) noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(multiple_constexpr_string_base::match<Index>(
				container,
				[](const auto& c, size_type index)
				{ return c[index]; },
				comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and
					 requires {
						 subtype<Index>::size_no_0;
						 subtype<Index>::value;
					 }
		{
			return multiple_constexpr_string_base::match<Index>(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}

		template<std::size_t Index, typename Container, typename CharComparator = decltype([](const std::decay_t<decltype(std::declval<Container>()[0])> lhs, const value_type rhs)
																						   { return static_cast<value_type>(lhs) == rhs; })>
		[[nodiscard]] constexpr bool match_left(
				const Container& container,
				CharComparator	 comparator = {}) const
				noexcept(noexcept(container[std::declval<size_type>()]) and noexcept(std::declval<const multiple_constexpr_string_base&>.template match<Index>(
						container,
						[](const auto& c, size_type index)
						{ return c[index]; },
						comparator)))
			requires requires {
						 {
							 container.size()
						 } -> std::convertible_to<size_type>;
					 } and std::is_member_function_pointer_v<decltype(&subtype<Index>::size_no_0)>
		{
			return this->template match<Index>(
					container,
					[](const auto& c, size_type index)
					{ return c[index]; },
					comparator);
		}
	};
}// namespace gal::gui::utility
