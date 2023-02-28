module;

export module gal.utility:breakpoint;

import std;

export namespace gal::gui::utility
{
	/**
	 * @brief Prepare for breaking in the debugger.
	 *
	 * @note
	 * This will check if a debugger exists and potentially launch the Just-In-Time debugger if one is configured.
	 * This function will terminate the application if no debugger is found.
	 */
	[[noreturn]] auto the_world() noexcept -> void
	{
#if defined(G_COMPILER_MSVC)
		__debugbreak();
#else
	#warning "FIXME"
		std::unreachable();
#endif
	}
}// namespace gal::gui::utility
