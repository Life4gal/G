module;

export module gal.utility:exception;

import std;

export namespace gal::gui::utility
{
	inline std::atomic<const char*> terminate_reason{nullptr};
}// namespace gal::gui::utility
