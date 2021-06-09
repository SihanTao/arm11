#include "../utils/types_and_macros.h"

#include "../utils/tools.h"
#include "../utils/unit_test.h"

int main (void)
{
  add_test ("Test bitfield_to_uint32_t:");
  {
    bitfield b
        = { .byte1 = 0x12, .byte2 = 0x34, .byte3 = 0x56, .byte4 = 0x78 };
    test_int_v (to_int (b), 0x12345678, "bitfield == 0x12345678");
  }

  add_test ("Test uint32_t_to_bitfield:");
  {
    uint32_t i = 0x12345678;
    bitfield bf = to_bf (i);
    test_int_v (bf.byte1, 0x12, "bf.byte1 == 0x12");
    test_int_v (bf.byte2, 0x34, "bf.byte2 == 0x34");
    test_int_v (bf.byte3, 0x56, "bf.byte3 == 0x56");
    test_int_v (bf.byte4, 0x78, "bf.byte4 == 0x78");
  }
  end_all_tests ();
}
