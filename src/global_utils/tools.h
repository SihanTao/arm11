#ifndef GLOBAL_TOOLS
#define GLOBAL_TOOLS


// target = 0b011101001 start = 0 end = 3
// returns : 0b1001
int get_bit_range(int target,int start,int end);

// dest = 101001000101 src = 10001 start = 0 end = 5
// dest = 101001010001
void set_bit_range(int* dest, int src, int start, int end);

void set_bit(int* dest, bool value, int position);


#endif //GLOBAL_TOOLS
