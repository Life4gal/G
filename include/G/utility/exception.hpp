#pragma once

#include <G/utility/macro.hpp>
#include <atomic>
#include <exception>

namespace gal::gui::utility
{
	inline std::atomic<const char*> terminate_reason{nullptr};

	/**
	 * @brief Set the message to display when the application terminates.
	 */
#define GAL_GUI_SET_TERMINATE_REASON(...) \
	terminate_reason.store("[" __FILE__ ":" GAL_GUI_TO_STRING(__LINE__) "] -> " __VA_ARGS__, std::memory_order::relaxed)
}// namespace gal::gui::utility
