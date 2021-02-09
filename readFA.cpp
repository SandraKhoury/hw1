#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <exception>
#include "FA.h"
#include "json.hpp"
#include "utils.h"

#include <vector>
#include <sstream>
#include <string>
std::vector<char> split(std::string& s, char delimiter)
{
	std::vector<char> tokens;
	std::string token;
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token[0]);
	}
	return tokens;
}

FA parse_fa (std::string filename) {
	bool has_starting = false;
	bool has_accepting = false;
	bool has_missing_label = false;
	FA nfa{};
	std::unordered_set<STATE> accepting;
	using nlohmann::json;
	std::ifstream file;
	file.open(filename);
	std::string input;
	if(file.is_open()){
		input=std::string(std::istreambuf_iterator<char>{file}, {});
	file.close();
	}
	else{
		std::cerr<<"cannot open file "<<filename <<"\n";
		exit(1);
	}
	auto fsa = json::parse(input);
	auto nodes = fsa["nodes"];
	auto links = fsa["links"];
	int i = 0;
	for (auto itr = nodes.begin(); itr != nodes.end(); ++itr,++i) {
		if ((*itr)["isAcceptState"] == true) {
			has_accepting = true;
			accepting.insert(i);
		}
	}
	nfa.accepting() = accepting;
	std::string s;
	char c;
	int idx1, idx2;
	for (auto itr = links.begin(); itr != links.end(); ++itr) {
		s = (*itr)["text"];
		if (s.size() == 0 && (*itr)["type"]!="StartLink") {
			has_missing_label = true;
			break;
		}
		c = s[0];
		/* not very efficient but clearer
		* and more conveniant */
		
		
		if ((*itr)["type"] == "Link") {
			idx1 = (*itr)["nodeA"];
			idx2 = (*itr)["nodeB"];
			
		}
		if ((*itr)["type"] == "SelfLink") {
			idx1 = (*itr)["node"];
			idx2 = idx1;
		}
		if ((*itr)["type"] == "StartLink") {
			nfa.starting() = (*itr)["node"];
			has_starting = true;
		}
		else {
			auto labels = split(s, ',');
			for(auto c:labels)
				nfa.addTransition(idx1, c, set{ idx2 });

		}
	}
	if (has_missing_label == true)
		throw invalid_spec("missing label on transition");
	if (has_starting == false)
		throw invalid_spec("no starting state");
	if (has_accepting == false)
		throw invalid_spec("no accepting state");
	
	return nfa;
}
