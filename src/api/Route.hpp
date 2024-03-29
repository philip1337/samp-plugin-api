#ifndef SRC_API_ROUTE_HPP
#define SRC_API_ROUTE_HPP

#pragma once

#include <Config.hpp>

#include <regex>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>

SAMP_API_BEGIN_NS

using namespace std;

class Match;
class Route;

struct Path {
	regex re;
	vector<string> keys;
};

class Route {

	friend class Match;

private:
	string url;
	regex path_to_regex;

	//
	// TODO
	// Support a more sophisticated matching pattern with something like
	// https://github.com/pillarjs/path-to-regexp/blob/master/index.js#L11
	//

	//
	// a pattern used to replace the url template's "captures".
	// 
	const string capture_pattern = "(?:([^\\/]+?))";

	//
	// a pattern for finding "captures" in a url template.
	//
	const string path_pattern = ":([^\\/]+)?";

	//
	// cache each regex and its keys as it is created by the 
	// test method.
	// 

	map<string, Path> cache;

public:

	//
	// set the url for this instance, create a regex
	// from the url and return a match instance.
	//
	Match set(const string& u);

	Route(/* options */) {
		//
		// TODO
		// Possibly support strict mode, wild cards, etc.
		//
	}
	~Route() {
	}
};

class Match {

private:
	Route * route;
	map<string, string> pairs;

public:
	int keys;
	string get(const string& key);
	bool test(const string& tmpl);

	Match(Route &r) : route(&r) {}
	~Match() {}
};

SAMP_API_END_NS

#endif
