#include "client.hpp"
#include "event.hpp"

#include "json/input.hpp"
#include "json/json_array.hpp"
#include "json/json_object.hpp"
#include "json/parse.hpp"
#include "json/json_type.hpp"

#include <assert.h>
#include <iostream>
#include <utility>


constexpr char HELP[] =
R"(usage: github-activity user <username>
usage: github-activity repo <repo>
usage: github-activity network <network>
usage: github-activity org <organization>
usage: github-activity public)";


int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << HELP;
		return 10;
	}
	std::string targetType = argv[1];

	if (argc < 3 && targetType != "public") {
		std::cout << HELP;
		return 10;
	}

	std::vector<githubactivity::Event> events;

	try {
		std::cout << "[1] Downloading data\n";
		HINTERNET internet = githubactivity::open();
		HINTERNET connection;
		if (targetType == "public") {
			connection = githubactivity::get_public(internet);
		} else if (targetType == "user") {
			connection = githubactivity::get_user(internet, argv[2]);
		} else if (targetType == "repo") {
			connection = githubactivity::get_repo(internet, argv[2]);
		} else if (targetType == "org") {
			connection = githubactivity::get_org(internet, argv[2]);
		} else if (targetType == "network") {
			connection = githubactivity::get_network(internet, argv[2]);
		} else {
			std::cerr << "error: unknown type at arg 1\n";
			githubactivity::close(internet);
			return 10;
		}
		size_t status = githubactivity::get_status_code(connection);
		if (!(200 <= status && status <= 299)) {
			std::cerr << "error: bad status code " << status << '\n';
			try { githubactivity::close(connection); } catch (const githubactivity::connection_error& ignored) {}
			try { githubactivity::close(internet); } catch (const githubactivity::connection_error& ignored) {}
			return 1;
		}

		std::cout << "[2] Parsing JSON data\n";

		json::JsonInput in(connection);
		auto json_element = json::parse(in);
		assert(json_element->type() == json::JsonType::Array);

		for (auto& eventJsonElement : static_cast<json::JsonArray*>(json_element.get())->elements) {
			assert(eventJsonElement->type() == json::JsonType::Object);
			std::optional<githubactivity::Event> parsed = githubactivity::Event::parse(static_cast<json::JsonObject*>(eventJsonElement.get()));
			if (parsed.has_value()) {
				events.push_back(std::move(parsed.value()));
			} else {
				std::cerr << "Error while parsing json event: ";
				eventJsonElement->write(std::cerr);
				std::cerr << "\n\n";
			}
		}

		std::cout << "[3] Closing connections\n";

		try { githubactivity::close(connection); } catch (const githubactivity::connection_error& ignored) {}
		try { githubactivity::close(internet); } catch (const githubactivity::connection_error& ignored) {}

		std::cout << "[4] Results\n";
		for (auto& event : events) {
			std::cout << "[" << event.actor << "] ";
			switch (event.type) {
			case githubactivity::EventType::CommitComment:
				std::cout << "Created a comment on a commit in " << event.repo_name;
				break;
			case githubactivity::EventType::Create:
				std::cout << "Created a " << githubactivity::to_string(event.ref_type.value()) << " ";
				if (event.ref_type != githubactivity::RefType::Repository) {
					std::cout << "on ";
				}
				std::cout << event.repo_name;
				break;
			case githubactivity::EventType::Delete:
				std::cout << "Deleted a " << githubactivity::to_string(event.ref_type.value()) << " on " << event.repo_name;
				break;
			case githubactivity::EventType::Fork:
				std::cout << "Forked repository " << event.forkee_full_name.value() << " to " << event.repo_name;
				break;
			case githubactivity::EventType::Gollum:
				std::cout << "Updated " << event.updated_pages.value() << " pages of the wiki";
				break;
			case githubactivity::EventType::IssueComment:
				std::cout << githubactivity::to_string(event.action.value()) << " a comment on an issue/pr in " << event.repo_name;
				break;
			case githubactivity::EventType::Issues:
				switch (event.action.value()) {
				case githubactivity::Action::Assigned:
					std::cout << "Assigned " << event.assignee.value() << " to an issue in " << event.repo_name;
					break;
				case githubactivity::Action::Unassigned:
					std::cout << "Unassigned " << event.assignee.value() << " to an issue in " << event.repo_name;
					break;
				default:
					std::cout << githubactivity::to_string(event.action.value()) << " an issue in " << event.repo_name;
					break;
				}
				break;
			case githubactivity::EventType::Member:
				std::cout << githubactivity::to_string(event.action.value()) << " a member relating to collabotors in " << event.repo_name;
				break;
			case githubactivity::EventType::Public:
				std::cout << "Opened " << event.repo_name << " to the public";
				break;
			case githubactivity::EventType::PullRequest:
				std::cout << githubactivity::to_string(event.action.value()) << " a pull request in " << event.repo_name;
				break;
			case githubactivity::EventType::PullRequestReview:
				std::cout << githubactivity::to_string(event.action.value()) << " a pull request review in " << event.repo_name;
				break;
			case githubactivity::EventType::PullRequestReviewComment:
				std::cout << githubactivity::to_string(event.action.value()) << " a pull request review comment in " << event.repo_name;
				break;
			case githubactivity::EventType::PullRequestReviewThread:
				std::cout << githubactivity::to_string(event.action.value()) << " a pull request review thread in " << event.repo_name;
				break;
			case githubactivity::EventType::Push:
				std::cout << "Pushed " << event.push.value() << " commits to " << event.repo_name;
				break;
			case githubactivity::EventType::Release:
				std::cout << githubactivity::to_string(event.action.value()) << " a release in " << event.repo_name;
				break;
			case githubactivity::EventType::Sponsorship:
				std::cout << githubactivity::to_string(event.action.value()) << " a sponsorship listing on " << event.repo_name;
				break;
			case githubactivity::EventType::Watch:
				std::cout << "Starred " << event.repo_name;
				break;
			default:
				std::unreachable();
			}

			std::cout << '\n';
		}

	} catch (const githubactivity::connection_error& err) {
		std::cerr << "error: " << err.what() << '\n';
		return 1;
	}

	return 0;
}