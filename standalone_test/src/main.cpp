import std;
import gal.utility;

#include <macro.hpp>

auto main() -> int
{
	std::cout << "Hello CMakeTemplateProject!"
			  << "\nCompiler Name: " << G_COMPILER_ID
			  << "\nCompiler Version: " << G_COMPILER_VERSION
			  << "\nCTP Version: " << G_VERSION
			  << "\n";

	{
		using format_type_1 = GAL_GUI_TEMPLATE_STRING_TYPE("{} is not {}");
		GAL_GUI_ASSUME(std::format(format_type_1::value, "hello", "world") == "hello is not world");

		using format_type_2 = GAL_GUI_TEMPLATE_STRING_TYPE("{:<10} == {:>10}");
		GAL_GUI_ASSUME(std::format(format_type_2::value, "hello", "world") == "hello      ==      world");
	}
}
