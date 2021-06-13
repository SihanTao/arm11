#include "../../global_utils/types_and_macros.h"

#include "../decode/decode.h"

#include "../../global_utils/tools.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("Test decode:");
  {
    /* The example from P5 in the spec:
     * mov r1, #1       data processing
     * b foo            branch
     * mov r2, #2       data processing
     * foo: mov r3, #3  data processing
     */

    bitfield fetched[] = { 0xe3a01001, 0xea000000, 0xe3a02002, 0xe3a03003 };

    instruction_t instruction1 = decode(fetched[0]);
    test_int_v(instruction1.tag, PROC,
               "mov r1, #1 == data processing");
    test_int_v(instruction1.cond, AL, "mov r1, #1 cond == always");
    test_int_v(instruction1.word.proc.operand2.rot_imm.imm, 1,
               "mov r1, #1 oprand2.imm = 1");
    test_int_v(instruction1.word.proc.operand2.rot_imm.amount, 0,
               "rotate amount = 0");
    test_int_v(instruction1.word.proc.Rd, 1, "mov r1, #1, Rn == 1");

    instruction1 = decode(fetched[1]);
    test_int_v(instruction1.tag, BRAN, "b foo == branch");
    test_int_v(instruction1.cond, AL, "b foo cond == always");
    test_int_v(instruction1.word.branch.offset, 0, "b foo cond == always");

    instruction1 = decode(fetched[2]);
    test_int_v(instruction1.tag, PROC,
               "mov r2, #2 == data processing");
    test_int_v(instruction1.cond, AL, "mov r2, #2 cond == always");
    test_int_v(instruction1.word.proc.operand2.rot_imm.imm, 2,
               "mov r2, #2 oprand2.imm = 2");
    test_int_v(instruction1.word.proc.operand2.rot_imm.amount, 0,
               "rotate amount = 0");
    test_int_v(instruction1.word.proc.Rd, 2, "mov r2, #2, Rn == 2");

    instruction1 = decode(fetched[3]);
    test_int_v(instruction1.tag, PROC,
               "foo: mov r3, #3 == data processing");
    test_int_v(instruction1.cond, AL, "mov r3, #3 cond == always");
    test_int_v(instruction1.word.proc.operand2.rot_imm.imm, 3,
               "mov r3, #3 oprand2.imm = 3");
    test_int_v(instruction1.word.proc.operand2.rot_imm.amount, 0,
               "rotate amount = 0");
    test_int_v(instruction1.word.proc.Rd, 3, "mov r3, #3, Rn == 3");

    bitfield mul = { 0xe0020091 }; // 9100 02e0 mul r2,r1,r0

    instruction_t instruction2 = decode(mul);
    test_int_v(instruction2.tag, MUL, "mul r2,r1,r0 == multiply");
    test_int_v(instruction2.cond, AL, "mul r2,r1,r0 cond == always");
    test_int_v(instruction2.word.mul.acc, 0, "mul r2,r1,r0 acc == 0");
    test_int_v(instruction2.word.mul.Rd, 2, "mul r2,r1,r0 Rd == 2");
    test_int_v(instruction2.word.mul.Rn, 0, "mul r2,r1,r0 Rn == 0");
    test_int_v(instruction2.word.mul.Rs, 0, "mul r2,r1,r0 Rs == 0");
    test_int_v(instruction2.word.mul.Rm, 1, "mul r2,r1,r0 Rm == 1");

    /*
     * ldr r0,=0x02 0200 a0e3
     * ldr r2,[r0]  0020 90e5
     * cmp r2,r0    0000 52e1
     */
    bitfield ldr01 = { 0xe5902000 };

    /* Note here the instruction ldr r0,=0x02 works as mov r0, =0x42
     * I realise it after reading the example on page 15.
     * This confused me a lot when testing
     */
    instruction_t instruction = decode(ldr01);
    test_int_v(instruction.tag, TRANS, "ldr r2,[r0] == TRANS");
    test_int_v(instruction.cond, AL, "ldr r2,[r0] cond == AL");
    test_int_v(instruction.word.trans.iFlag, 0, "ldr r2,[r0] I == 0");
    test_int_v(instruction.word.trans.is_pre, 1, "ldr r2,[r0] P == 1");
    test_int_v(instruction.word.trans.is_up, 1, "ldr r2,[r0] U == 1");
    test_int_v(instruction.word.trans.is_load, 1, "ldr r2,[r0] L == 1");
    test_int_v(instruction.word.trans.Rn, 0, "ldr r2,[r0] Rn == 0");
    test_int_v(instruction.word.trans.Rd, 2, "ldr r2,[r0] Rd == 2");
    test_int_v(instruction.word.trans.offset.rot_imm.imm, 0,
               "ldr r2,[r0] imm == 0");
    test_int_v(instruction.word.trans.offset.rot_imm.amount, 0,
               "ldr r2,[r0] amount == 0");
  }

  end_all_tests();

  return 0;
}
