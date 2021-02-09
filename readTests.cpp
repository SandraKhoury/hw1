#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <exception>
#include <vector>
#include <tuple>
#include "FA.h"
#include "json.hpp"
#include "utils.h"

std::vector<std::tuple<std::string, bool, float>>
parse_tests(std::string filename) {
	std::vector<std::tuple<std::string, bool, float>>
		results;
	using nlohmann::json;
	std::ifstream file;
	file.open(filename);
	std::string input(std::istreambuf_iterator<char>{file}, {});
	file.close();
	auto p = json::parse(input);

	auto tests = p["tests"];
	for (auto& t : tests) {
		results.push_back(
			std::make_tuple(t["input"], t["accept"], t["points"])
		);

	}
	
	return results;
}