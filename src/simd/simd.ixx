module;

#include <eve/wide.hpp>

export module gal.simd;

namespace gal::gui::simd
{
	template<typename T, std::size_t N>
	using simd = eve::wide<T, eve::fixed<N>>;
}// namespace gal::gui::simd
