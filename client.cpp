#include "client.hpp"
#include <wininet.h>

namespace githubactivity {

	connection_error::connection_error(const std::string& c_error) : error(c_error) {}
	const char* connection_error::what() const noexcept { return this->error.c_str(); }


	HINTERNET open() {
		if (DWORD res = InternetAttemptConnect(0); res != ERROR_SUCCESS) {
			throw connection_error("InternetAttemptConnect() failure, error code " + std::to_string(res));
		}
		HINTERNET internet = InternetOpen(USER_AGENT.data(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if (internet == NULL) {
			throw connection_error("InternetOpen() failure, error code " + std::to_string(GetLastError()));
		}
		return internet;
	}

	HINTERNET get(HINTERNET internet, const std::string& url) {
		HINTERNET connection = InternetOpenUrl(internet, url.c_str(), NULL, 0, 0, 0);

		if (connection == NULL) {
			throw connection_error("InternetOpenUrl() failure, error code " + std::to_string(GetLastError()));
		}

		return connection;

		// char buf[1024];
		// DWORD read;
		// while (InternetReadFile(connection, buf, std::size(buf), &read) && read > 0) {
		// 	result.append(buf, read);
		// }

		// DWORD statusCode = 0;
		// DWORD size = sizeof(statusCode);
		// if (bool res = HttpQueryInfo(connection, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &size, NULL); !res) {
		// 	DWORD queryError = GetLastError();
		// 	if (bool res = InternetCloseHandle(connection); !res) {
		// 		throw connection_error("InternetCloseHandle() failure, error code " + std::to_string(GetLastError()));
		// 	}
		// 	throw connection_error("HttpQueryInfo() failure, error code " + std::to_string(queryError));
		// }

		// if (bool res = InternetCloseHandle(connection); !res) {
		// 	throw connection_error("InternetCloseHandle() failure, error code " + std::to_string(GetLastError()));
		// }

		// if (200 <= statusCode && statusCode <= 299) {
		// 	return;
		// }
		// throw connection_error("Bad status code received: " + std::to_string(statusCode));
	}

	DWORD get_status_code(HINTERNET connection) {
		DWORD statusCode = 0;
		DWORD size = sizeof(statusCode);
		if (bool res = HttpQueryInfo(connection, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &size, NULL); !res) {
			DWORD queryError = GetLastError();
			throw connection_error("HttpQueryInfo() failure, error code " + std::to_string(queryError));
		}
		return statusCode;
	}

	void close(HINTERNET internet) {
		if (bool res = InternetCloseHandle(internet); !res) {
			throw connection_error("InternetCloseHandle() failure, error code " + std::to_string(GetLastError()));
		}
	}


	HINTERNET get_user(HINTERNET internet, const std::string& user) {
		return get(internet, "https://api.github.com/users/" + user + "/events?per_page=100");
	}
	HINTERNET get_repo(HINTERNET internet, const std::string& repo) {
		return get(internet, "https://api.github.com/repos/" + repo + "/events?per_page=100");
	}
	HINTERNET get_network(HINTERNET internet, const std::string& repo) {
		return get(internet, "https://api.github.com/networks/" + repo + "/events?per_page=100");
	}
	HINTERNET get_org(HINTERNET internet, const std::string& org) {
		return get(internet, "https://api.github.com/orgs/" + org + "/events?per_page=100");
	}
	HINTERNET get_public(HINTERNET internet) {
		return get(internet, "https://api.github.com/events?per_page=100");
	}
}