#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "json.hpp"
#include "FA.h"
#include "utils.h"
using nlohmann::json;

std::string report(const json& tree) {
	std::ostringstream STREAM;
	json problems = tree["problems"];
	int total = 0, max = 0;
	for (auto& p : problems) {
		json& tests = p["tests"];
		STREAM << "Problem " << p["name"] << "\n";
		STREAM << "---------------\n";
		for (auto& t : tests) {
			STREAM << t["name"];
			if (t["points"] == 0) {
				STREAM << "FAILED" << "\tPoints=0\n";
				STREAM << "\tFOR INPUT = " << t["input"];
				STREAM <<"\tEXPECTED " << t["expected-output"];
				STREAM << "\tRESULT " << t["output"] << "\n";
			}
			else {
				total +=static_cast<int>( t["points"]);
				STREAM << " PASSED" << "\tPoints=" << t["points"] << "\n";
			}
		}
		STREAM << "SUBTOTAL=   " << p["total"] << "/" << p["max points"] << "\n\n";

	}
	return STREAM.str();
}

int dump(const json& tree) {
	json problems = tree["problems"];
	int total = 0, max = 0;
	for (auto& p : problems) {
		json& tests = p["tests"];
		std::cerr << "Problem " << p["name"] << "\n";
		std::cerr << "---------------\n";
		for (auto& t : tests) {
			std::cerr << t["name"];
			if (t["points"] == 0) {
				std::cerr << fail(" FAILED") << "\tPoints=0\n";
				std::cerr <<error( "\tFOR INPUT = ") << t["input"];
				std::cerr << error("\tEXPECTED ") << t["expected-output"];
				std::cerr << error("\tRESULT " )<< t["output"] << "\n";
				}
			else {
				total += static_cast<int>(t["points"]);
				std::cerr << success(" PASSED") << "\tPoints=" << t["points"] << "\n";
			}
		}
		std::cerr << message("SUBTOTAL=   ")<<p["total"]<<"/"<<p["max points"]<<"\n\n";

	}
	return total;
}
void runTests(json& p) {
	
	FA nfa=parse_fa(p["NFA-specs"]);

	json& tests = p["tests"];
	bool flip = false;
	int total = 0;
	int possible = 0;
	for (auto& t : tests) {
		possible += static_cast<int> (t["points"]);
		bool r = nfa.accept(t["input"]);
		bool er = t["expected-output"];
		if (r != er) {
			t["output"] = r;
			t["points"] = 0;
		}
		else {
			total += static_cast<int>(t["points"]);
		}
	
	}
	p["total"] = total;
	p["max points"] = possible;

}


json  runProblems(std::string filename) {
	int total = 0;
	std::ifstream file;
	file.open(filename);
	std::string input;
	if(file.is_open()){
	  input=std::string(std::istreambuf_iterator<char>{file}, {});
	  file.close();
	}
	else {
		std::cerr<<"cannot open file "<<filename<<"\n";
		exit(1);
	}
	json tree = json::parse(input);
	json& problems = tree["problems"];

	for (auto& p : problems) {
		runTests(p);
		total += static_cast<int>(p["total"])	;
	}
	tree["total"] = total;
	return tree;
	
}
int main(int argc,char **argv) {
	if (argc < 2) {
		std::cerr << "usage: FaChecker filename.json\n";
		exit(1);
	}
	try {
		json tree = runProblems(argv[1]);
		dump(tree);
		std::cout << tree["total"];
		if(tree["total"]!=tree["max_points"]){
			std::cerr<<"total="<<tree["total"]<<"   max="<<tree["max_points"]<<"\n";
		    std::cerr<<"some tests failed\n";
		    exit(1);
		}
	/* TODO: figure out a way instead of processing
	*  the tree twice
	*/
		//std::cout<<report(tree);
	}
	catch (std::exception& e) {
		std::cerr << "ERROR\n";
		std::cerr << e.what() << "\n";
	}
	return 0;
}
