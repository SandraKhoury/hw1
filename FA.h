#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <initializer_list>
#include <boost/container_hash/hash.hpp>
constexpr auto EPSILON = 'e';
using STATE = int;
using set= std::unordered_set<STATE>;
class FA {

	// We have to supply a hash function
	// for std::pair. Instead we are using
	// Boost to do that
	STATE _starting;
	set _accepting;
	//_transitions of the form state X char -> set of states
	std::unordered_map < std::pair<STATE, char>,
		set,
		boost::hash<std::pair<STATE, char>>
		> _transitions;

	// returns the result of a single step 
	set step(set start,char a){
		set result{};
		for (const STATE& p : start) {
			auto itr
				= _transitions.find(std::make_pair(p, a));
			if (itr != _transitions.end()) {
				result.insert(itr->second.begin(), itr->second.end());
				result = closure(result);
			}

		}
		return result;
	}
public:
	FA() {}
	FA(STATE i, set a)
		:_starting(i), _accepting(a) {}
	FA(STATE i,std::initializer_list<STATE> a)
		:_starting(i),_accepting(a) {}
	
	void addTransition(STATE p, char a, set s) {
		//check if the transition exists
		auto itr = _transitions.find(std::make_pair(p, a));
		if (itr != _transitions.end()) 
			itr->second.insert(s.begin(), s.end());
		else
			_transitions.insert(
				std::make_pair(std::make_pair(p,a) ,s)
		);
	}
	
	
	set run(std::string input) {
		set result = closure({ _starting });
		for (auto& a : input) 
			result = step(result, a);
		return result;
	}
	bool accept(std::string input) {
		auto res = run(input);
		for (auto& p : res) {
			if (_accepting.count(p) > 0)
				return true;
		}
		return false;

	}
	STATE& starting() {
		return _starting;
	}
	set & accepting() {
		return _accepting;
	}

	set closure(set s) {
		set result;
		// define "single step " closure function
		auto cl = [this](auto q) {
			set result{ q };
			auto itr = _transitions.find(std::make_pair(q, EPSILON));
			if (itr != _transitions.end())
				result.insert(itr->second.begin(), itr->second.end());

			return result;
		};

		for (auto p : s) {
			auto q = cl(p);
			result.insert(q.begin(), q.end());
		}
		if (s == result)return result;
		else return closure(result);
	}
	std::unordered_set<STATE>
		closure(std::initializer_list<STATE> list) {
		return closure(std::unordered_set<STATE>(list));
	}

	std::unordered_map < std::pair<STATE, char>,
		set,boost::hash<std::pair<STATE, char>>>& 
		transitions() {
		return _transitions;
	}
	void debug() {
		std::cerr << "starting state: " << _starting << std::endl;
		std::cerr << "accepting state(s): ";
		for (auto& x : _accepting)
			std::cerr << x << ",";
		std::cerr << std::endl;
		std::cerr << "transitions\n";
		for (auto& [key, val] : _transitions) {
			std::cerr << key.first << "," << key.second << "->";
			for (auto& s : val)
				std::cerr << s << ",";
			std::cerr << std::endl;

		}

	}
};