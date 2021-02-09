#pragma once
#include <string>
#include "FA.h"


std::string fail(std::string s);
std::string success(std::string s);
std::string message(std::string);
std::string error(std::string);

FA parse_fa(std::string);


class invalid_spec :std::exception {
	std::string _message;
public:
	invalid_spec(std::string message) :_message(message) {}
	const char* what() const noexcept override {
		return _message.c_str();
	}
};