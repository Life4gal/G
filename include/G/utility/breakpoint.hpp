#pragma once

#include <G/utility/exception.hpp>

namespace gal::gui::utility
{
	/**
	 * @brief Prepare for breaking in the debugger.
	 *
	 * @note
	 * This will check if a debugger exists and potentially launch the Just-In-Time debugger if one is configured.
	 * This function will terminate the application if no debugger is found.
	 */
	[[noreturn]] auto the_world() noexcept -> void;

#define GAL_GUI_THE_WORLD(...)                 \
	GAL_GUI_SET_TERMINATE_REASON(__VA_ARGS__); \
	the_world()
}// namespace gal::gui::utility
