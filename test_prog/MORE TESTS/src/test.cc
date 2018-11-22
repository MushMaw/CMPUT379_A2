#include "test.h"

int main(int argc, char *argv[]) {
	std::ifstream tfile;
	char line[100];
	std::string line_str("");

	tfile.open(argv[1]);
	tfile.getline(line, 100);
	std::cout << "Next line is: " << line << "\n";
	line_str = line;
	std::cout << "Length: " << line_str.length() << "\n";
}
