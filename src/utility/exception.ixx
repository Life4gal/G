module;

#include <macro.hpp>

export module gal.utility:exception;

import std;
import :constexpr_string.template_string;

export namespace gal::gui::utility
{
	inline std::atomic<const char*> terminate_reason{nullptr};

	class Exception : public std::runtime_error
	{
	public:
	private:
		template<typename TemplateString, typename... Args>
		[[nodiscard]] constexpr static auto make_what(const std::source_location& location, const Args&... args) noexcept(false) -> std::string
		{
			return std::format("{}:{:d}: {}", location.file_name(), location.line(), std::format(TemplateString::value, args...));
		}

		using runtime_error::runtime_error;

	public:
		template<typename E = Exception>
		friend auto make_exception(const std::string_view message, const std::source_location& location = std::source_location::current()) -> E
		{
			return E{make_what<GAL_GUI_TEMPLATE_STRING_TYPE("{}")>(location, message)};
		}

		template<typename TemplateString, typename E = Exception, typename... Args>
		friend auto make_exception(const std::source_location& location, const Args&... args) -> E
		{
			return E{make_what<TemplateString>(location, args...)};
		}
	};
}// namespace gal::gui::utility
