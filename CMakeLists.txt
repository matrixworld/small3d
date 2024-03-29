include(${CMAKE_HOME_DIRECTORY}/biicode.cmake)

IF(APPLE)
  SET(BII_IMPLICIT_RULES_ENABLED FALSE)
ENDIF()

# Actually create targets: EXEcutables and libraries.
ADD_BII_TARGETS()

# Link OpenGL things and set to use C++11
if(NOT WIN32 OR MINGW)
  IF(APPLE)
    FIND_PACKAGE(OpenGL REQUIRED)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    TARGET_LINK_LIBRARIES(${BII_LIB_TARGET} PUBLIC ${OPENGL_LIBRARIES})
  ELSE()
    target_compile_options(${BII_LIB_TARGET} PUBLIC "-std=c++11" "-Wl,--no-as-needed") # Ubuntu if gcc problems
  ENDIF()
endif()

IF(WIN32 AND NOT MINGW)
  IF (NOT ${BII_samplegame_main_TARGET} STREQUAL "")
    SET_TARGET_PROPERTIES(${BII_samplegame_main_TARGET} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:\"LIBCMTD\" /NODEFAULTLIB:\"LIBCMT\"")
  ENDIF()
  IF (NOT ${BII_main_TARGET} STREQUAL "")
    SET_TARGET_PROPERTIES(${BII_main_TARGET} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:\"LIBCMTD\" /NODEFAULTLIB:\"LIBCMT\"")
  ENDIF()
ENDIF()
