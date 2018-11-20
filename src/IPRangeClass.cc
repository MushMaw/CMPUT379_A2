#include "IPRangeClass.h"


IP_Range::IP_Range() {
	this->low = -1;
	this->high = -1;
}

IP_Range::IP_Range(int low, int high) {
	this.low = low;
	this.high = high;
}

bool IP_Range::is_in_range(int ip_val) {
	if (this->low <= ip_val && ip_val <= this->high) {
		return true;
	} else { return false; }
}
