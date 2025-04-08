#pragma once
#include "json/json_object.hpp"

#include <string>
#include <optional>

namespace githubactivity {

	enum class EventType {
		CommitComment,
		Create,
		Delete,
		Fork,
		Gollum,
		IssueComment,
		Issues,
		Member,
		Public,
		PullRequest,
		PullRequestReview,
		PullRequestReviewComment,
		PullRequestReviewThread,
		Push,
		Release,
		Sponsorship,
		Watch
	};
	enum class RefType {
		Branch,
		Tag,
		Repository
	};
	enum class Action {
		Opened,
		Edited,
		Closed,
		Reopened,
		Assigned,
		Unassigned,
		Labeled,
		Unlabeled,

		Created,
		Deleted,

		Added,

		ReviewRequested, // review_requested
		ReviewRequestRemoved,
		Synchronize,

		Resolved,
		Unresolved,

		Published,


		Starred,
		Started
	};

	std::optional<EventType> get_event_type(const std::string& str);
	std::optional<RefType> get_ref_type(const std::string& str);
	std::optional<Action> get_action(const std::string& str);

	std::string to_string(EventType type);
	std::string to_string(RefType type);
	std::string to_string(Action action);

	class Event {
	public:
		EventType type;
		std::string_view actor; // actor.login
		std::string_view repo_name;
		std::string time;

		// payload-specific : payload.[key...]
		std::optional<RefType> ref_type; // ref_type
		std::optional<std::string> forkee_full_name; // forkee.full_name
		std::optional<size_t> updated_pages; // len(pages)
		std::optional<Action> action; // action
		std::optional<std::string> assignee; // assignee.login
		std::optional<size_t> push; // size

		static std::optional<Event> parse(json::JsonObject* json);
	};
}