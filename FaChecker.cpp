/**
 * @file FaChecker.cpp
 * @author Hikmat Farhat (hfarhat@ndu.edu.lb)
 * @brief Checks the correctness of a Finite State Machine
 * in json format against a set of tests
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "json.hpp"
#include "FA.h"
#include "utils.h"
//#include "unistd.h"
using nlohmann::json;
bool debug=false;
/**
 * @brief Return the final json object
 * as string 
 * @param tree 
 * @return std::string 
 */
std::string report(const json& tree) {
	std::ostringstream STREAM;
	json problems = tree["problems"];
	int total = 0, max = 0;
	STREAM << "# TOTAL " << tree["total"] << "/" << tree["max_points"];
	STREAM << "\n---------------------------------\n";
	STREAM << "DETAILS\n\n";

	for (auto& p : problems) {
		json& tests = p["tests"];
		STREAM << "### " << p["name"] << "\n";
		STREAM << "---------------\n";
		for (auto& t : tests) {
			STREAM <<t["name"];
			if (t["points"] == 0) {
				STREAM << " __FAILED__..........." << "\t__Points__=0\n";
				STREAM << "\tfor input = " << t["input"];
				STREAM <<"\texpected:" << t["expected-output"];
				STREAM << "\tresult:" << t["output"] << "\n\n";
			}
			else {
				total +=static_cast<int>( t["points"]);
				STREAM << " __PASSED__..........." << "\t__Points__=" << t["points"] << "\n\n";
			}
		}
		STREAM << "__SUBTOTAL__=   " << p["total"] << "/" << p["max_points"] << "\n\n";

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
		std::cerr << message("SUBTOTAL=   ")<<p["total"]<<"/"<<p["max_points"]<<"\n\n";

	}
	return total;
}
void runTests(json& p,std::string parent_path) {
	if(debug)std::cerr<<"Processing problem "<<p["name"]<<"\n";
	auto filename = parent_path + "/" + static_cast<std::string>(p["NFA-specs"]);
	FA nfa;
	try {
		nfa = parse_fa(filename);
	}
	catch (...) {
		std::cerr << "Could not read " << filename << "\n";
		for (auto& t : p["tests"])
			t["points"] = 0;
		return;
	}
	json& tests = p["tests"];
	bool flip = false;
	int total = 0;
	//int possible = 0;
	
	for (auto& t : tests) {
	//	possible += static_cast<int> (t["points"]);
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
	if(debug){
		std::cerr<<"subtotal="<<p["total"]<<"/"<<p["max_points"]<<"\n";
	}
	

}

/**
 * @brief Run all the problems given
 * in the json file 
 * @param filename 
 * @return json 
 */
json  runProblems(std::string filename) {
	auto dir = [=]() {
		auto pos=filename.find_last_of("\\/");
		return (std::string::npos == pos) ? "" : filename.substr(0, pos);
	}();
	if (debug) {
		std::cerr << "parent path= " << dir << "\n";
	}
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
	if(debug)std::cerr<<"Processing problems\n";
	json tree = json::parse(input);
	/* retrieve all problems */
	json& problems = tree["problems"];
	int max_possible=0;
	int total = 0;

	for (auto& p : problems) {
		/* make sure the entry p["total"]
		* exists in case test fails
		*/
		p["total"] = 0;
		runTests(p,dir);
		total += static_cast<int>(p["total"]);
	}
	tree["total"] = total;
	//tree["max_points"]=max_possible;
	return tree;
	
}
int main(int argc,char **argv) {
/*	char *buf=(char *)malloc(1024);
	std::cout<<"working dir="<<getwd(buf)<<std::endl;*/
	if (argc < 3) {
		std::cerr << "usage: FaChecker filename.json report.md\n";
		exit(1);
	}
	auto config_filename = argv[1];
	auto report_filename=argv[2];
	std::cerr<<"START \n";
	try {
		json tree = runProblems(config_filename);
		dump(tree);
		std::cout << tree["total"];
		if(tree["total"]!=tree["max_points"]){
			std::cerr<<"total="<<tree["total"]<<"   max="<<tree["max_points"]<<"\n";
		    std::cerr<<"some tests failed\n";
		//    exit(1);
		}
		std::ofstream report_file;
		report_file.open(report_filename);
		std::string rpt = report(tree);
		report_file << rpt;
		report_file.close();
	/* TODO: figure out a way instead of processing
	*  the tree twice
	*/
		//std::cout<<report(tree);
	}
	catch (...) {
		std::cerr << fail("ERROR\n");
		std::cerr << fail("File(s) not found or corrupted\n");
		exit(1);
		//std::cerr << e.what() << "\n";
	}
	return 0;
}
