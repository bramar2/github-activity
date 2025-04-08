#pragma once
#include <string>
#include <windows.h>
#include <wininet.h>

namespace githubactivity {

	inline constexpr std::string_view USER_AGENT = "github-activity github.com/bramar2/github-activity";
	inline constexpr char HEADERS[] =
R"(Accept: */*
Accept-Language: en-US,en;q=0.9,id;q=0.8)";

	class connection_error : std::exception {
	public:
		std::string error;
		connection_error(const std::string& c_error);
		const char* what() const noexcept override;
	};

	HINTERNET open();
	HINTERNET get(HINTERNET internet, const std::string& username);
	DWORD get_status_code(HINTERNET connection);
	void close(HINTERNET internet);

	HINTERNET get_user(HINTERNET internet, const std::string& user);
	HINTERNET get_repo(HINTERNET internet, const std::string& repo);
	HINTERNET get_network(HINTERNET internet, const std::string& repo);
	HINTERNET get_org(HINTERNET internet, const std::string& org);
	HINTERNET get_public(HINTERNET internet);
}
