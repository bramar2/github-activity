cmake_minimum_required(VERSION 3.27)
project(github-activity)

set(CMAKE_CXX_STANDARD 23)
add_executable(github-activity
	program.cpp client.cpp client.hpp event.cpp event.hpp

	json/json.hpp json/input.cpp json/input.hpp
	json/json_array.cpp json/json_array.hpp
	json/json_bool.cpp json/json_bool.hpp
	json/json_element.hpp
	json/json_null.cpp json/json_null.hpp
	json/json_number.cpp json/json_number.hpp
	json/json_object.cpp json/json_object.hpp
	json/json_string.cpp json/json_string.hpp
	json/json_type.hpp
	json/parse.cpp json/parse.hpp
	json/path.cpp json/path.hpp)

target_link_libraries(github-activity wininet)