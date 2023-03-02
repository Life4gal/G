# https://github.com/ivmai/bdwgc/blob/6c47e6d4c929d97aa59efc970c0b3a07d6ee7ad6/CMakeLists.txt#L201
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(GC_ENABLE_THREADS ON)
else ()
	set(GC_ENABLE_THREADS OFF)
endif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

CPMAddPackage(
		NAME gc
		GIT_TAG v8.2.2
		GITHUB_REPOSITORY "ivmai/bdwgc"
		OPTIONS "enable_cplusplus ON" "enable_threads ${GC_ENABLE_THREADS}"
)

cpm_install(${PROJECT_NAME} gc PRIVATE)
list(APPEND ${PROJECT_NAME_PREFIX}3RD_PARTY_DEPENDENCIES "bdwgc 8.2.2")
