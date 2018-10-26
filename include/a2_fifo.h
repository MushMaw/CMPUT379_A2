#if !defined(A2_FIFO_H)
#define A2_FIFO_H 1


std::string get_fifo_name(int reader, int writer);
int create_fifo(int reader, int writer);
void delete_fifo(int reader, int writer);
#endif
