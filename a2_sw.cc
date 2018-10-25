#include "a2_sw.h"

int run_switch(int argc, char *argv[]) {
	Switch *swi = NULL;
	try {
		swi = new Switch(argc, argv);
	} catch (Sw_Exception& e) {
		std::cout << e.what();
	}
	delete swi;
	return 0;
}
