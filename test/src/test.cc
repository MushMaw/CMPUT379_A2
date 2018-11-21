#include "test.h"

int main() {
	try {
		Header header(1, 100, 200);
		std::string ser_he(""), test_ser("sw4 400 600");

		header.serialize(ser_he);
		std::cout << "ser header: " << ser_he << "\n";
		header.deserialize(test_ser);
		std::cout << "Printing..." << test_ser << "\n";
		header.print();
		std::cout << "swi id is " << header.swi << "\n";
	} catch (Header_Exception& e) { e.print_traceback(); }
}
