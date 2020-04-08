# CMakeList.txt : CMake project for HelloSqlite3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("HelloSqlite3")


# --------------------------------------------------------------------------
# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")



IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")  # deal with MSVC Warning C4530
ENDIF(MSVC)

#-------------------------------------
#  sqlite3 lib.
#-------------------------------------
set(SQLITE3_LIBRARY "${PROJECT_SOURCE_DIR}/lib/sqlite3.lib")



add_executable ( ${PROJECT_NAME}
	"src/main.cpp"
 "src/createTable.cpp" "src/deleteTable.cpp" "src/execExample.cpp" "src/stmtExample.cpp")


target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${SQLITE3_LIBRARY}
)

target_include_directories( ${PROJECT_NAME}
	PRIVATE
		"${PROJECT_SOURCE_DIR}/include"
)




if(WIN32)
    # Copy DLL files to output directory
	message("COPY ZLIB DLL")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}/lib/sqlite3.dll"              
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endif()