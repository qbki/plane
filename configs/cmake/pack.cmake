set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP")
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_SOURCE_DIR}/build/pack)
include(CPack)