#pragma once

#include <windows.h>
#include <wininet.h>

namespace json {
	class JsonInput {
	public:
		static constexpr size_t BUFFER_SIZE = 4096;
		HINTERNET file;
		char buffer[BUFFER_SIZE];
		size_t ptr = 0, available = 0;
	private:
		bool refill();
	public:
		JsonInput(HINTERNET c_file);
		char next();
		char peek();
		char next_iw(); // next ignore whitespace
	};
}