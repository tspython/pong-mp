#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <fstream>
#include <sstream>

namespace Util {
	std::string readFile(std::string path) {
		std::ifstream file(path);
		std::string str;
		std::string line;

		while(std::getline(file, line)) str += line + "\n";
		return str;
	}
}
#endif // UTIL_H
