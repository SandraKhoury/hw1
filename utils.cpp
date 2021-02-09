#include <string>
std::string fail(std::string s) {
	return    "\x1B[31m" + s + "\033[0m";
}
std::string success(std::string s) {
	return    "\x1B[32m" + s + "\033[0m";
}
std::string error(std::string s) {
	return    "\x1B[33m" + s + "\033[0m";
}
std::string message(std::string s) {
	return    "\x1B[36m" + s + "\033[0m";
}