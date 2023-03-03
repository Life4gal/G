#include <macro.hpp>

import std;
import gal.utility;
import gal.image;
import gal.test;

namespace
{
	/**
	 * @see main.cpp :)
	 */
	using dummy = GAL_TEMPLATE_STRING_TYPE("I don't know why this declaration is required, but without it the compiler will report the above. (Translated from other languages into English, which may not be entirely accurate.)");

	using namespace gal::gui;
	using namespace gal::gui::test;

	using pixmap_type	   = image::Pixmap<std::uint8_t>;
	using pixmap_view_type = image::PixmapView<std::uint8_t>;
	using value_type	   = pixmap_type::value_type;
	using size_type		   = pixmap_type::size_type;

	constexpr size_type			 pixmap_default_width{4};
	constexpr size_type			 pixmap_default_height{3};

	// 0    1   2   3
	// 4    5   6   7
	// 8    9   10  11
	[[nodiscard]] constexpr auto make_pixmap() -> pixmap_type
	{
		pixmap_type result{pixmap_default_width, pixmap_default_height};

		value_type	value{0};
		for (auto& pixel: result)
		{
			pixel = value++;
		}
		return result;
	}

	GAL_NO_DESTROY suite test_image_pixmap = []
	{
		"default_constructor"_test = []
		{
			const pixmap_type pixmap{};

			expect((pixmap.empty() == "empty"_b) >> fatal);
			expect((pixmap.width() == 0_ull) >> fatal);
			expect((pixmap.height() == 0_ull) >> fatal);
			expect((pixmap.size() == 0_ull) >> fatal);
			expect((pixmap.capacity() == 0_ull) >> fatal);
		};

		"construct_with_zero_fill"_test = []
		{
			pixmap_type pixmap{pixmap_default_width, pixmap_default_height};

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			for (auto& pixel: pixmap)
			{
				expect((pixel == 0_ull) >> fatal);
			}
		};

		"copy_construct"_test = []
		{
			auto origin_pixmap = make_pixmap();
			auto pixmap		   = origin_pixmap;

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			value_type value{0};
			for (auto& pixel: pixmap)
			{
				expect((pixel == _ull{value}) >> fatal);
				++value;
			}
		};

		"move_construct"_test = []
		{
			auto origin_pixmap = make_pixmap();
			auto pixmap		   = std::move(origin_pixmap);

			expect((origin_pixmap.empty() == "empty"_b) >> fatal);
			expect((origin_pixmap.width() == 0_ull) >> fatal);
			expect((origin_pixmap.height() == 0_ull) >> fatal);
			expect((origin_pixmap.size() == 0_ull) >> fatal);
			expect((origin_pixmap.capacity() == 0_ull) >> fatal);

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			value_type value{0};
			for (auto& pixel: pixmap)
			{
				expect((pixel == _ull{value}) >> fatal);
				++value;
			}
		};

		"construct_from_data"_test = []
		{
			auto origin_pixmap = make_pixmap();

			// 0    1   2
			// 4    5   6
			// 8    9   10
			auto pixmap		   = image::Pixmap{origin_pixmap.data(), 3, 3, 4};

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == 3_ull) >> fatal);
			expect((pixmap.height() == 3_ull) >> fatal);
			expect((pixmap.size() == _ull{3 * 3}) >> fatal);
			expect((pixmap.capacity() == _ull{3 * 3}) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 0) == 0_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 0) == 1_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 0) == 2_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 1) == 4_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 1) == 5_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 1) == 6_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 2) == 8_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 2) == 9_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 2) == 10_ull) >> fatal);
		};

		"copy_assign"_test = []
		{
			auto origin_pixmap = make_pixmap();
			auto pixmap		   = pixmap_type{10, 10};

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == 10_ull) >> fatal);
			expect((pixmap.height() == 10_ull) >> fatal);
			expect((pixmap.size() == _ull{10 * 10}) >> fatal);
			expect((pixmap.capacity() == _ull{10 * 10}) >> fatal);

			pixmap = origin_pixmap;

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{10 * 10}) >> fatal);

			value_type value{0};
			for (auto& pixel: pixmap)
			{
				expect((pixel == _ull{value}) >> fatal);
				++value;
			}
		};

		"move_assign"_test = []
		{
			auto origin_pixmap = make_pixmap();
			auto pixmap		   = pixmap_type{10, 10};

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == 10_ull) >> fatal);
			expect((pixmap.height() == 10_ull) >> fatal);
			expect((pixmap.size() == _ull{10 * 10}) >> fatal);
			expect((pixmap.capacity() == _ull{10 * 10}) >> fatal);

			pixmap = std::move(origin_pixmap);

			expect((origin_pixmap.empty() == "empty"_b) >> fatal);
			expect((origin_pixmap.width() == 0_ull) >> fatal);
			expect((origin_pixmap.height() == 0_ull) >> fatal);
			expect((origin_pixmap.size() == 0_ull) >> fatal);
			expect((origin_pixmap.capacity() == 0_ull) >> fatal);

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			value_type value{0};
			for (auto& pixel: pixmap)
			{
				expect((pixel == _ull{value}) >> fatal);
				++value;
			}
		};

		"shrink_to_fit"_test = []
		{
			auto origin_pixmap = make_pixmap();
			auto pixmap		   = pixmap_type{10, 10};

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == 10_ull) >> fatal);
			expect((pixmap.height() == 10_ull) >> fatal);
			expect((pixmap.size() == _ull{10 * 10}) >> fatal);
			expect((pixmap.capacity() == _ull{10 * 10}) >> fatal);

			pixmap = origin_pixmap;

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{10 * 10}) >> fatal);

			pixmap.shrink_to_fit();

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
		};

		"clear"_test = []
		{
			auto pixmap = make_pixmap();

			expect((not pixmap.empty() == "not empty"_b) >> fatal);
			expect((pixmap.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap.size() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			pixmap.clear();

			expect((pixmap.empty() == "empty"_b) >> fatal);
			expect((pixmap.width() == 0_ull) >> fatal);
			expect((pixmap.height() == 0_ull) >> fatal);
			expect((pixmap.size() == 0_ull) >> fatal);
			expect((pixmap.capacity() == _ull{pixmap_default_width * pixmap_default_height}) >> fatal);

			pixmap.shrink_to_fit();

			expect((pixmap.empty() == "empty"_b) >> fatal);
			expect((pixmap.width() == 0_ull) >> fatal);
			expect((pixmap.height() == 0_ull) >> fatal);
			expect((pixmap.size() == 0_ull) >> fatal);
			expect((pixmap.capacity() == 0_ull) >> fatal);
		};

		"sub_pixmap"_test = []
		{
			auto origin_pixmap = make_pixmap();

			// 1 2
			// 5 6
			auto pixmap		   = origin_pixmap.sub_pixmap(1, 0, 2, 2);

			expect((not pixmap.empty() == "no empty"_b) >> fatal);
			expect((pixmap.width() == 2_ull) >> fatal);
			expect((pixmap.height() == 2_ull) >> fatal);
			expect((pixmap.size() == _ull{2 * 2}) >> fatal);
			expect((pixmap.capacity() == _ull{2 * 2}) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 0) == 1_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 0) == 2_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 1) == 5_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 1) == 6_ull) >> fatal);
		};
	};
}// namespace
