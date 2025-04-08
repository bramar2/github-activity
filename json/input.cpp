#include "input.hpp"

#include <iterator>


namespace json {
	JsonInput::JsonInput(HINTERNET c_file) : file(c_file) {}

	bool JsonInput::refill() {
		if (ptr != available) return true;
		DWORD read;
		if (!InternetReadFile(file, buffer, std::size(buffer), &read)) {
			read = 0;
		}
		ptr = 0;
		available = read;
		return available != 0;
	}

	char JsonInput::next() {
		if (!refill()) return '\0';
		return buffer[ptr++];
	}

	char JsonInput::peek() {
		if (!refill()) return '\0';
		return buffer[ptr];
	}
	
	char JsonInput::next_iw() {
		char res;
		while (std::isspace( res = next() ));
		return res;
	}
}