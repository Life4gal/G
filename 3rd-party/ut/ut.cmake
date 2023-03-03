#CPMAddPackage(
#		NAME ut
#		GIT_TAG v1.1.9
#		GITHUB_REPOSITORY "boost-ext/ut"
#		OPTIONS "BOOST_UT_USE_WARNINGS_AS_ERORS ON" "BOOST_UT_DISABLE_MODULE OFF"
#)
#
#cpm_install(${PROJECT_NAME} ut PRIVATE)

function(check_and_download_boost_ut)
	set(download_file "ut.hpp")
	set(download_version "1.1.9")

	set(download_url "https://raw.githubusercontent.com/boost-ext/ut/v${download_version}/include/boost/${download_file}")

	set(dest_path ${${PROJECT_NAME_PREFIX}TEMP_HEADER_PATH}/boost)
	set(dest_file ${dest_path}/${download_file})

	if (NOT EXISTS ${dest_file})
		message(STATUS "The file ${download_file} is not exists, downloading...(dest: ${dest_path})")

		file(
				DOWNLOAD
				${download_url}
				${dest_file}
				SHOW_PROGRESS
				STATUS download_result
		)

		list(GET download_result 0 error_code)
		list(GET download_result 1 error_string)
		if (NOT error_code EQUAL 0)
			message(FATAL_ERROR "Cannot download ${download_file} ! --> ${error_string}")
		endif (NOT error_code EQUAL 0)

		# todo: :(
		file(COPY_FILE ${dest_file} ${dest_file}.bak)

		# generate temp file for write
		set(temp_file_path "${dest_file}.generated")
		file(WRITE ${temp_file_path})

		function(do_append line)
			file(APPEND ${temp_file_path} "// Modified by '${PROJECT_NAME}', !!! DO NOT EDIT !!!\n")
			file(APPEND ${temp_file_path} "${line}\n")
		endfunction(do_append line)

		# read content
		file(STRINGS ${dest_file} file_content)

		# iteration
		foreach (line IN LISTS file_content)
			# #include <xxx> --> // #include <xxx>
			string(REGEX MATCH "^#include[ ]*(.*)$" this_line_include_matched "${line}")
			if (this_line_include_matched)
				# replace
				do_append("// ${line}")

				continue()
			endif (this_line_include_matched)

			# note: Multi-line macro definitions will be parsed incorrectly.
			# such as:
			# #define macro_name \
			#   macro_definition_line_1 \
			#   macro_definition_line_2 \
			#   macro_definition_line_3 \
			#   ...
			# parsed as: #define macro_name ; macro_definition_line_1 ; macro_definition_line_2 ; macro_definition_line_3 ; ...
			string(REGEX MATCH "^#define .*$" this_line_macro_definition_matched "${line}")
			if (NOT this_line_macro_definition_matched)
				string(REGEX MATCH "^#if .*$" this_line_macro_definition_matched "${line}")
			endif (NOT this_line_macro_definition_matched)

			string(FIND "${line}" ";" this_line_macro_definition_multi_line_index)
			if (this_line_macro_definition_matched AND NOT ${this_line_macro_definition_multi_line_index} EQUAL -1)
				# note: NOT A LIST
				#foreach (macro_line ${line})
				#	message("macro_line --> ${macro_line}")
				#endforeach (macro_line ${line})

				# ';' --> '\' + '\n'
				string(REPLACE ";" "\\\n" real_macro "${line}")

				# append
				file(APPEND ${temp_file_path} "${real_macro}\n")
				continue()
			endif (this_line_macro_definition_matched AND NOT ${this_line_macro_definition_multi_line_index} EQUAL -1)

			# just append
			file(APPEND ${temp_file_path} "${line}\n")
		endforeach (line IN LISTS file_content)

		# replace file
		file(RENAME ${temp_file_path} ${dest_file})
	else ()
		message(STATUS "The file ${download_file} is exists(${dest_path}), no need to download...")
	endif (NOT EXISTS ${dest_file})

	set(BOOST_UT_PATH ${dest_file} PARENT_SCOPE)
endfunction(check_and_download_boost_ut)

check_and_download_boost_ut()
