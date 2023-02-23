#pragma once

#include <G/utility/breakpoint.hpp>

#define GAL_GUI_ASSERT(expression, ...)                                                                        \
	do {                                                                                                       \
		if (not static_cast<bool>(expression))                                                                 \
		{                                                                                                      \
			GAL_GUI_THE_WORLD("[ASSERT FAILED]: \"" __VA_ARGS__ "\" --> {" GAL_GUI_TO_STRING(expression) "}"); \
		}                                                                                                      \
	} while (false)

#define GAL_GUI_ASSUME(expression, ...) GAL_GUI_ASSERT(expression __VA_OPT__(, ) __VA_ARGS__)

#define GAL_GUI_NOT_NULL(pointer, ...)                                                                        \
	do {                                                                                                      \
		if (pointer == nullptr)                                                                               \
		{                                                                                                     \
			GAL_GUI_THE_WORLD("[NOT-NULL FAILED]: \"" __VA_ARGS__ "\" --> {" GAL_GUI_TO_STRING(pointer) "}"); \
		}                                                                                                     \
	} while (false)
