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

	GAL_NO_DESTROY suite test_image_pixmap_view = []
	{
		"default_constructor"_test = []
		{
			const pixmap_view_type pixmap{};

			expect((pixmap.empty() == "empty"_b) >> fatal);
			expect((pixmap.width() == 0_ull) >> fatal);
			expect((pixmap.height() == 0_ull) >> fatal);
			expect((pixmap.stride() == 0_ull) >> fatal);
		};

		"convert_from_pixmap"_test = []
		{
			auto pixmap		 = make_pixmap();
			auto pixmap_view = pixmap_view_type{pixmap};

			expect((not pixmap_view.empty() == "not empty"_b) >> fatal);
			expect((pixmap_view.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap_view.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap_view.stride() == _ull{pixmap_default_width}) >> fatal);

			value_type value{0};
			for (size_type y = 0; y < pixmap_default_height; ++y)
			{
				for (size_type x = 0; x < pixmap_default_width; ++x)
				{
					expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(x, y) == _ull{value}) >> fatal);
					++value;
				}
			}
		};

		"convert_to_pixmap"_test = []
		{
			auto origin_pixmap		= make_pixmap();
			auto origin_pixmap_view = pixmap_view_type{origin_pixmap};

			// 1 2
			// 5 6
			auto pixmap_view		= origin_pixmap_view.sub_view(1, 0, 2, 2);

			expect((not pixmap_view.empty() == "no empty"_b) >> fatal);
			expect((pixmap_view.width() == 2_ull) >> fatal);
			expect((pixmap_view.height() == 2_ull) >> fatal);
			expect((pixmap_view.stride() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 0) == 1_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 0) == 2_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 1) == 5_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 1) == 6_ull) >> fatal);

			auto pixmap = image::Pixmap{pixmap_view};

			expect((not pixmap.empty() == "no empty"_b) >> fatal);
			expect((pixmap.width() == 2_ull) >> fatal);
			expect((pixmap.height() == 2_ull) >> fatal);
			expect((pixmap.capacity() == _ull{2 * 2}) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 0) == 1_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 0) == 2_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 1) == 5_ull) >> fatal);
			expect((pixmap.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 1) == 6_ull) >> fatal);
		};

		"construct_from_data"_test = []
		{
			auto pixmap		 = make_pixmap();

			// 0    1   2
			// 4    5   6
			// 8    9   10
			auto pixmap_view = image::PixmapView{pixmap.data(), 3, 3, 4};

			expect((not pixmap_view.empty() == "not empty"_b) >> fatal);
			expect((pixmap_view.width() == 3_ull) >> fatal);
			expect((pixmap_view.height() == 3_ull) >> fatal);
			expect((pixmap_view.stride() == 4_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 0) == 0_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 0) == 1_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 0) == 2_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 1) == 4_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 1) == 5_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 1) == 6_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(0, 2) == 8_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(1, 2) == 9_ull) >> fatal);
			expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(2, 2) == 10_ull) >> fatal);
		};

		"copy_assign"_test = []
		{
			auto pixmap		 = make_pixmap();
			auto pixmap_view = pixmap_view_type{};

			expect((pixmap_view.empty() == "empty"_b) >> fatal);
			expect((pixmap_view.width() == 0_ull) >> fatal);
			expect((pixmap_view.height() == 0_ull) >> fatal);
			expect((pixmap_view.stride() == 0_ull) >> fatal);
			expect((not pixmap_view.data() == "nullptr"_b) >> fatal);

			pixmap_view = pixmap;

			expect((not pixmap_view.empty() == "not empty"_b) >> fatal);
			expect((pixmap_view.width() == _ull{pixmap_default_width}) >> fatal);
			expect((pixmap_view.height() == _ull{pixmap_default_height}) >> fatal);
			expect((pixmap_view.stride() == _ull{pixmap_default_width}) >> fatal);

			value_type value{0};
			for (size_type y = 0; y < pixmap_default_height; ++y)
			{
				for (size_type x = 0; x < pixmap_default_width; ++x)
				{
					expect((pixmap_view.GAL_ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(x, y) == _ull{value}) >> fatal);
					++value;
				}
			}
		};
	};
}// namespace
