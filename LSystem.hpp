#pragma once
#include <string>
#include "debug_utils.h"
using namespace std;

class LSystem {
public:
	string system_string;
	LSystem() : system_string{} {

	}
	virtual string encode_string(const string& s) {
		string tmp{};
		for (const auto c : s) {
			tmp += translate(c);
		}

		return tmp;
	}

	virtual string translate(const char c) = 0;

	virtual void perform(const char c) = 0;

	virtual void draw() {
		for (auto& c : system_string) {
			perform(c);
		}
	}
};