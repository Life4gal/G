import std;
import gal.utility;
import gal.image;
import gal.test;

#include <macro.hpp>

int main()
{
	/**
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(278): error C2752: "std::pointer_traits<char *>": Multiple partial specializations match the template argument list
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\xutility(306): note: maybe "std::pointer_traits<_Ty*>"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\xutility(306): note: or "std::pointer_traits<_Ty*>"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\xutility(306): note: or "std::pointer_traits<_Ty*>"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(229): note: when compile class template member function "int std::basic_stringbuf<_Elem,_Traits,_Alloc>::overflow(int)"
	 *         with
	 *         [
	 *             _Elem=char,
	 *             _Traits=std::char_traits<char>,
	 *             _Alloc=std::allocator<char>
	 *         ]
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(890): note: see the reference of the compiling class template specialization "std::basic_stringbuf<_Elem,_Traits,_Alloc>"
	 *         with
	 *         [
	 *             _Elem=char,
	 *             _Traits=std::char_traits<char>,
	 *             _Alloc=std::allocator<char>
	 *         ]
	 * C:\workspace\life4gal\G\src\test\test.ixx(1353): note: see the reference of the compiling class template specialization "std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>"
	 * C:\workspace\life4gal\G\src\test\test.ixx(1581): note: see the reference of the compiling class template specialization "gal::gui::test::reporter<gal::gui::test::printer>"
	 * C:\workspace\life4gal\G\src\test\test.ixx(1599): note: see the reference of the compiling class template specialization "gal::gui::test::runner<gal::gui::test::reporter<gal::gui::test::printer>,16>"
	 * C:\workspace\life4gal\G\src\test\test.ixx(1612): note: see the reference of the compiling variable template "gal::gui::test::runner<gal::gui::test::reporter<gal::gui::test::printer>,16> cfg<gal::gui::test::override>"
	 * C:\workspace\life4gal\G\src\test\test.ixx(1641): note: see the reference of the compiling function template specialization "void gal::gui::test::detail::on<,gal::gui::test::events::test<void (__cdecl *)(void),gal::gui::test::none>>(TEvent &&)"
	 *        with
	 *        [
	 *            TEvent=gal::gui::test::events::test<void (__cdecl *)(void),gal::gui::test::none>
	 *        ]
	 * C:\workspace\life4gal\G\unit_test\src\xxx.cpp(`The first place where ""_test is used.`): note: see the reference of the compiling function template specialization "void (__cdecl *gal::gui::test::detail::test::operator =<>(gal::gui::test::detail::test_location<void (__cdecl *)(void)>))(void)"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(278): error C2039: "pointer_to": it not the member of "std::pointer_traits<char *>"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(496): note: see the declaration of "std::pointer_traits<char *>"
	 * C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Tools\MSVC\14.36.32323\include\sstream(278): error C3861: “pointer_to”: Cannot find identifier
	 */

	using dummy = GAL_TEMPLATE_STRING_TYPE("I don't know why this declaration is required, but without it the compiler will report the above. (Translated from other languages into English, which may not be entirely accurate.)");
}
