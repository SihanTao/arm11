#include "tools.h"
#include "unit_test.h"

int main(void) 
{
    add_test("Test bitfield_to_uint32_t:");
    {
        bitfield b = {.byte1 = 0x12, .byte2 = 0x34, .byte3 = 0x56, .byte4 = 0x78};
        test_int_v(bitfield_to_uint32(b), 0x12345678, "bitfield == 0x12345678");
    }
    end_all_tests();
}