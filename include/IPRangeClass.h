#if !defined(IPRANGE_CLASS_H)
#define IPRANGE_CLASS_H 1

class IP_Range {
	public:
		int low, high;

		IP_Range();
		IP_Range(int low, int high);
		bool is_in_range(int ip_val);
}

#endif
