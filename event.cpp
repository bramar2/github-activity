#include "event.hpp"
#include "json/json_array.hpp"
#include "json/json_number.hpp"
#include "json/json_string.hpp"
#include "json/path.hpp"

#include "assert.h"
#include <unordered_map>
#include <utility>
#include <iostream>

namespace githubactivity {
	std::optional<EventType> get_event_type(const std::string& str) {
		static const std::unordered_map<std::string, EventType> mapping = {
	        {"CommitCommentEvent", EventType::CommitComment},
	        {"CreateEvent", EventType::Create},
	        {"DeleteEvent", EventType::Delete},
	        {"ForkEvent", EventType::Fork},
	        {"GollumEvent", EventType::Gollum},
	        {"IssueCommentEvent", EventType::IssueComment},
	        {"IssuesEvent", EventType::Issues},
	        {"MemberEvent", EventType::Member},
	        {"PublicEvent", EventType::Public},
	        {"PullRequestEvent", EventType::PullRequest},
	        {"PullRequestReviewEvent", EventType::PullRequestReview},
	        {"PullRequestReviewCommentEvent", EventType::PullRequestReviewComment},
	        {"PullRequestReviewThreadEvent", EventType::PullRequestReviewThread},
	        {"PushEvent", EventType::Push},
	        {"ReleaseEvent", EventType::Release},
	        {"SponsorshipEvent", EventType::Sponsorship},
	        {"WatchEvent", EventType::Watch}
	    };
	    if (mapping.count(str)) return mapping.at(str);
	    return std::nullopt;
	}

	std::optional<RefType> get_ref_type(const std::string& str) {
		static const std::unordered_map<std::string, RefType> mapping = {
	        {"branch", RefType::Branch},
	        {"tag", RefType::Tag},
	        {"repository", RefType::Repository}
	    };
	    if (mapping.count(str)) return mapping.at(str);
	    return std::nullopt;
	}
	std::optional<Action> get_action(const std::string& str) {
		static const std::unordered_map<std::string, Action> mapping = {
	        {"opened", Action::Opened},
			{"edited", Action::Edited},
			{"closed", Action::Closed},
			{"reopened", Action::Reopened},
			{"assigned", Action::Assigned},
			{"unassigned", Action::Unassigned},
			{"labeled", Action::Labeled},
			{"unlabeled", Action::Unlabeled},
			{"created", Action::Created},
			{"deleted", Action::Deleted},
			{"added", Action::Added},
			{"review_requested", Action::ReviewRequested},
			{"review_request_removed", Action::ReviewRequestRemoved},
			{"synchronize", Action::Synchronize},
			{"resolved", Action::Resolved},
			{"unresolved", Action::Unresolved},
			{"published", Action::Published},
			{"starred", Action::Starred},
			{"started", Action::Started}
	    };
	    if (mapping.count(str)) return mapping.at(str);
	    return std::nullopt;
	}

	std::string to_string(EventType type) {
		switch (type) {
		case EventType::CommitComment: return "CommitCommentEvent";
		case EventType::Create: return "CreateEvent";
		case EventType::Delete: return "DeleteEvent";
		case EventType::Fork: return "ForkEvent";
		case EventType::Gollum: return "GollumEvent";
		case EventType::IssueComment: return "IssueCommentEvent";
		case EventType::Issues: return "IssuesEvent";
		case EventType::Member: return "MemberEvent";
		case EventType::Public: return "PublicEvent";
		case EventType::PullRequest: return "PullRequestEvent";
		case EventType::PullRequestReview: return "PullRequestReviewEvent";
		case EventType::PullRequestReviewComment: return "PullRequestReviewCommentEvent";
		case EventType::PullRequestReviewThread: return "PullRequestReviewThreadEvent";
		case EventType::Push: return "PushEvent";
		case EventType::Release: return "ReleaseEvent";
		case EventType::Sponsorship: return "SponsorshipEvent";
		case EventType::Watch: return "WatchEvent";
		}
		std::unreachable();
	}

	std::string to_string(RefType type) {
		switch (type) {
		case RefType::Branch: return "branch";
		case RefType::Tag: return "tag";
		case RefType::Repository: return "repository";
		}
		std::unreachable();
	}

	std::string to_string(Action action) {
		switch (action) {
		case Action::Opened: return "Opened";
		case Action::Edited: return "Edited";
		case Action::Closed: return "Closed";
		case Action::Reopened: return "Reopened";
		case Action::Assigned: return "Assigned";
		case Action::Unassigned: return "Unassigned";
		case Action::Labeled: return "Labeled";
		case Action::Unlabeled: return "Unlabeled";
		case Action::Created: return "Created";
		case Action::Deleted: return "Deleted";
		case Action::Added: return "Added";
		case Action::ReviewRequested: return "Requested review of";
		case Action::ReviewRequestRemoved: return "Removed review request of";
		case Action::Synchronize: return "Synchronize";
		case Action::Resolved: return "Resolved";
		case Action::Unresolved: return "Unresolved";
		case Action::Published: return "Published";
		case Action::Starred: return "Starred";
		case Action::Started: return "Started";
		}
		std::unreachable();
	}

	std::optional<Event> Event::parse(json::JsonObject* json) {
		static const json::JsonPath J_TYPE = json::JsonPath::create("$.type").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_TYPE"); }).value();
		static const json::JsonPath J_ACTOR = json::JsonPath::create("$.actor.login").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_ACTOR"); }).value();
		static const json::JsonPath J_REPO_NAME = json::JsonPath::create("$.repo.name").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_REPO_NAME"); }).value();
		static const json::JsonPath J_TIME = json::JsonPath::create("$.created_at").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_TIME"); }).value();
		static const json::JsonPath J_REF_TYPE = json::JsonPath::create("$.payload.ref_type").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_REF_TYPE"); }).value();
		static const json::JsonPath J_FORKEE_FULL_NAME = json::JsonPath::create("$.payload.forkee.full_name").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_FORKEE_FULL_NAME"); }).value();
		static const json::JsonPath J_UPDATED_PAGES = json::JsonPath::create("$.payload.pages").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_UPDATED_PAGES"); }).value();
		static const json::JsonPath J_ACTION = json::JsonPath::create("$.payload.action").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_ACTION"); }).value();
		static const json::JsonPath J_ASSIGNEE = json::JsonPath::create("$.assignee.login").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_ASSIGNEE"); }).value();
		static const json::JsonPath J_PUSH = json::JsonPath::create("$.payload.size").or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in J_PUSH"); }).value();

		json::JsonElement* type = J_TYPE.query(json);
		json::JsonElement* actor = J_ACTOR.query(json);
		json::JsonElement* repoName = J_REPO_NAME.query(json);
		json::JsonElement* time = J_TIME.query(json);

		if (type == nullptr || actor == nullptr || repoName == nullptr || time == nullptr ||
			type->type() != json::JsonType::String || actor->type() != json::JsonType::String || 
			repoName->type() != json::JsonType::String || time->type() != json::JsonType::String) {
			return std::nullopt;
		}

		std::optional<EventType> eventType = get_event_type(static_cast<json::JsonString*>(type)->str());
		if (!eventType.has_value()) {
			return std::nullopt;
		}

		Event event;
		event.type = eventType.value();
		event.actor = static_cast<json::JsonString*>(actor)->str();
		event.repo_name = static_cast<json::JsonString*>(repoName)->str();
		event.time = static_cast<json::JsonString*>(time)->str();


		if (json::JsonElement* obj = J_REF_TYPE.query(json); obj != nullptr && obj->type() == json::JsonType::String) {
			std::optional<RefType> refType = get_ref_type(static_cast<json::JsonString*>(obj)->str());
			if (!refType.has_value()) {
				return std::nullopt;
			}
			event.ref_type = refType;
		}

		if (json::JsonElement* obj = J_ASSIGNEE.query(json); obj != nullptr && obj->type() == json::JsonType::String) {
			event.assignee = static_cast<json::JsonString*>(obj)->str();
		}

		if (json::JsonElement* obj = J_UPDATED_PAGES.query(json); obj != nullptr && obj->type() == json::JsonType::Array) {
			event.updated_pages = static_cast<json::JsonArray*>(obj)->elements.size();
		}

		if (json::JsonElement* obj = J_ACTION.query(json); obj != nullptr && obj->type() == json::JsonType::String) {
			std::optional<Action> action = get_action(static_cast<json::JsonString*>(obj)->str());
			if (!action.has_value()) {
				return std::nullopt;
			}
			event.action = action;
		}

		if (json::JsonElement* obj = J_FORKEE_FULL_NAME.query(json); obj != nullptr && obj->type() == json::JsonType::String) {
			event.forkee_full_name = static_cast<json::JsonString*>(obj)->str();
		}

		if (json::JsonElement* obj = J_PUSH.query(json); obj != nullptr && obj->type() == json::JsonType::Number) {
			event.push = static_cast<size_t>(*static_cast<json::JsonNumber*>(obj));
		}

		return event;
	}
}