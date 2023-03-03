project(
		G-unit-test
		LANGUAGES CXX
)

add_executable(
		${PROJECT_NAME}

		# =================================
		# IMAGE
		# =================================
		${PROJECT_SOURCE_DIR}/src/image/test_pixmap.cpp
		${PROJECT_SOURCE_DIR}/src/image/test_pixmap_view.cpp

		${PROJECT_SOURCE_DIR}/src/main.cpp
)

#include(${${PROJECT_NAME_PREFIX}3RD_PARTY_PATH}/ut/ut.cmake)
#
#target_compile_definitions(
#		${PROJECT_NAME}
#		PUBLIC
#		BOOST_UT_PATH=${BOOST_UT_PATH}
#)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		gal::G
)

add_test(
		NAME ${PROJECT_NAME}
		COMMAND ${PROJECT_NAME}
)
