#include <stdbool.h>
#include "test-e2k.h"

static uint64_t basic_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0x0000000000012341, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000, 0x8000000000000000,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaa00012341,
    0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa80000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x123456789abcdef0,
};

static uint64_t basic_src2[] = {
    0x000000000812fada, 0x0000000000012341, 0xfffffffffffedcbf, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee0812fada, 0xeeeeeeee00012341, 0xeeeeeeeefffedcbf,
    0xeeeeeeee00000000, 0xeeeeeeeeffffffff, 0xeeeeeeee00000001, 0xeeeeeeee00000002,
    0xeeeeeeee00000000, 0xeeeeeeee00000001, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee00000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfedcba9876543210,
};

#define TEST2_DEFAULT_DATA(CHECK, EXEC, INSN) \
    TEST2_DATA(CHECK, EXEC, INSN, basic_src1, basic_src2, glue(INSN, _expect))

EXPECT_DATA(pandd, uint64_t) {
    0x0000000000105258, 0x0000000000012341, 0x0000000000000001, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0x7fffffffffffffff, 0x8000000000000000, 0x0000000000000000,
    0x8000000000000000, 0xaaaaaaaa00105258, 0xaaaaaaaa00012341, 0xaaaaaaaa00000001,
    0xaaaaaaaa00000000, 0xaaaaaaaaffffffff, 0xaaaaaaaa00000001, 0xaaaaaaaa00000002,
    0xaaaaaaaa00000000, 0xaaaaaaaa00000001, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa00000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x1214121812141210,
};
EXPECT_DATA(pandnd, uint64_t) {
    0x0000000012240420, 0x0000000000000000, 0x0000000000012340, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000,
    0x0000000000000000, 0x0000000012240420, 0x0000000000000000, 0x0000000000012340,
    0x00000000ffffffff, 0x0000000000000000, 0x00000000fffffffe, 0x00000000fffffffd,
    0x000000007fffffff, 0x000000007ffffffe, 0x0000000000000000, 0x0000000000000000,
    0x0000000080000000, 0x0000000000000000, 0x0000000000000000, 0x0020446088a8cce0,
};
EXPECT_DATA(pord, uint64_t) {
    0x000000001a36fefa, 0x0000000000012341, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x8000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee1a36fefa, 0xeeeeeeee00012341, 0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee7fffffff, 0xeeeeeeee7fffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee80000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfefcfef8fefcfef0,
};
EXPECT_DATA(pxord, uint64_t) {
    0x000000001a26aca2, 0x0000000000000000, 0xfffffffffffffffe, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x444444441a26aca2, 0x4444444400000000, 0x44444444fffffffe,
    0x44444444ffffffff, 0x4444444400000000, 0x44444444fffffffe, 0x44444444fffffffd,
    0x444444447fffffff, 0x444444447ffffffe, 0x4444444480000000, 0x444444447fffffff,
    0x4444444480000001, 0x444444447ffffffe, 0x0000000000000000, 0xece8ece0ece8ece0,
};
EXPECT_DATA(pminub, uint64_t) {
    0x0000000008125678, 0x0000000000012341, 0x0000000000012341, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0x7fffffffffffffff, 0x8000000000000000, 0x0000000000000000,
    0x8000000000000000, 0xaaaaaaaa08125678, 0xaaaaaaaa00012341, 0xaaaaaaaa00012341,
    0xaaaaaaaa00000000, 0xaaaaaaaaffffffff, 0xaaaaaaaa00000001, 0xaaaaaaaa00000002,
    0xaaaaaaaa00000000, 0xaaaaaaaa00000001, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa00000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x1234567876543210,
};
EXPECT_DATA(pminsh, uint64_t) {
    0x000000000812fada, 0x0000000000012341, 0xfffffffffffedcbf, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000ffffffffffff,
    0x0000ffffffffffff, 0xffffffffffffffff, 0x8000ffffffffffff, 0x8000000000000000,
    0x8000fffffffffffe, 0xaaaaaaaa0812fada, 0xaaaaaaaa00012341, 0xaaaaaaaafffedcbf,
    0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff,
    0xaaaaaaaa0000ffff, 0xaaaaaaaa0000ffff, 0xaaaaaaaaffffffff, 0xaaaaaaaa8000ffff,
    0xaaaaaaaa80000000, 0xaaaaaaaa8000fffe, 0x123456789abcdef0, 0xfedcba989abcdef0,
};
EXPECT_DATA(pmaxub, uint64_t) {
    0x000000001234fada, 0x0000000000012341, 0xfffffffffffedcbf, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x8000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee1234fada, 0xeeeeeeee00012341, 0xeeeeeeeefffedcbf,
    0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee7fffffff, 0xeeeeeeee7fffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee80000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfedcba989abcdef0,
};
EXPECT_DATA(pmaxsh, uint64_t) {
    0x0000000012345678, 0x0000000000012341, 0x0000000000012341, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x7fff000000000000,
    0x7fff000000000001, 0x7fffffffffffffff, 0xffff000000000000, 0x0000000000000001,
    0xffff000000000000, 0xeeeeeeee12345678, 0xeeeeeeee00012341, 0xeeeeeeee00012341,
    0xeeeeeeee00000000, 0xeeeeeeeeffffffff, 0xeeeeeeee00000001, 0xeeeeeeee00000002,
    0xeeeeeeee7fff0000, 0xeeeeeeee7fff0001, 0xeeeeeeee7fffffff, 0xeeeeeeeeffff0000,
    0xeeeeeeee00000001, 0xeeeeeeeeffff0000, 0x123456789abcdef0, 0x1234567876543210,
};
EXPECT_DATA(paddb, uint64_t) {
    0x000000001a465052, 0x0000000000024682, 0xffffffffffffff00, 0xffffffffffffffff,
    0xfefefefefefefefe, 0xffffffffffffff00, 0xffffffffffffff01, 0x7fffffffffffffff,
    0x7fffffffffffff00, 0x7efefefefefefefe, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x989898981a465052, 0x9898989800024682, 0x98989898ffffff00,
    0x98989898ffffffff, 0x98989898fefefefe, 0x98989898ffffff00, 0x98989898ffffff01,
    0x989898987fffffff, 0x989898987fffff00, 0x989898987efefefe, 0x989898987fffffff,
    0x9898989880000001, 0x989898987ffffffe, 0x2468acf03478bce0, 0x1010101010101000,
};
EXPECT_DATA(paddh, uint64_t) {
    0x000000001a465152, 0x0000000000024682, 0xffffffffffff0000, 0xffffffffffffffff,
    0xfffefffefffefffe, 0xffffffffffff0000, 0xffffffffffff0001, 0x7fffffffffffffff,
    0x7fffffffffff0000, 0x7ffefffefffefffe, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x999899981a465152, 0x9998999800024682, 0x99989998ffff0000,
    0x99989998ffffffff, 0x99989998fffefffe, 0x99989998ffff0000, 0x99989998ffff0001,
    0x999899987fffffff, 0x999899987fff0000, 0x999899987ffefffe, 0x999899987fffffff,
    0x9998999880000001, 0x999899987ffffffe, 0x2468acf03578bde0, 0x1110111011101100,
};
EXPECT_DATA(paddsb, uint64_t) {
    0x000000001a465052, 0x000000000002467f, 0xffffffffffffff00, 0xffffffffffffffff,
    0xfefefefefefefefe, 0xffffffffffffff00, 0xffffffffffffff01, 0x7fffffffffffffff,
    0x7fffffffffffff00, 0x7efefefefefefefe, 0x80ffffffffffffff, 0x8000000000000001,
    0x80fffffffffffffe, 0x989898981a465052, 0x989898980002467f, 0x98989898ffffff00,
    0x98989898ffffffff, 0x98989898fefefefe, 0x98989898ffffff00, 0x98989898ffffff01,
    0x989898987fffffff, 0x989898987fffff00, 0x989898987efefefe, 0x9898989880ffffff,
    0x9898989880000001, 0x9898989880fffffe, 0x24687f7f8080bce0, 0x1010101010101000,
};
EXPECT_DATA(paddsh, uint64_t) {
    0x000000001a465152, 0x0000000000024682, 0xffffffffffff0000, 0xffffffffffffffff,
    0xfffefffefffefffe, 0xffffffffffff0000, 0xffffffffffff0001, 0x7fffffffffffffff,
    0x7fffffffffff0000, 0x7ffefffefffefffe, 0x8000ffffffffffff, 0x8000000000000001,
    0x8000fffffffffffe, 0x999899981a465152, 0x9998999800024682, 0x99989998ffff0000,
    0x99989998ffffffff, 0x99989998fffefffe, 0x99989998ffff0000, 0x99989998ffff0001,
    0x999899987fffffff, 0x999899987fff0000, 0x999899987ffefffe, 0x999899988000ffff,
    0x9998999880000001, 0x999899988000fffe, 0x24687fff8000bde0, 0x1110111011101100,
};
EXPECT_DATA(paddusb, uint64_t) {
    0x000000001a46ffff, 0x0000000000024682, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x8000000000000001,
    0xfffffffffffffffe, 0xffffffff1a46ffff, 0xffffffff00024682, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffff7fffffff, 0xffffffff7fffffff, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffff80000001, 0xfffffffffffffffe, 0x2468acf0ffffffff, 0xffffffffffffffff,
};
EXPECT_DATA(paddush, uint64_t) {
    0x000000001a46ffff, 0x0000000000024682, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x8000000000000001,
    0xfffffffffffffffe, 0xffffffff1a46ffff, 0xffffffff00024682, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffff7fffffff, 0xffffffff7fffffff, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffff80000001, 0xfffffffffffffffe, 0x2468acf0ffffffff, 0xffffffffffffffff,
};
EXPECT_DATA(paddw, uint64_t) {
    0x000000001a475152, 0x0000000000024682, 0xffffffff00000000, 0xffffffffffffffff,
    0xfffffffefffffffe, 0xffffffff00000000, 0xffffffff00000001, 0x7fffffffffffffff,
    0x7fffffff00000000, 0x7ffffffefffffffe, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x999999981a475152, 0x9999999800024682, 0x9999999800000000,
    0x99999998ffffffff, 0x99999998fffffffe, 0x9999999800000000, 0x9999999800000001,
    0x999999987fffffff, 0x9999999880000000, 0x999999987ffffffe, 0x999999987fffffff,
    0x9999999880000001, 0x999999987ffffffe, 0x2468acf03579bde0, 0x1111111011111100,
};
EXPECT_DATA(paddd, uint64_t) {
    0x000000001a475152, 0x0000000000024682, 0x0000000000000000, 0xffffffffffffffff,
    0xfffffffffffffffe, 0x0000000000000000, 0x0000000000000001, 0x7fffffffffffffff,
    0x8000000000000000, 0x7ffffffffffffffe, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x999999981a475152, 0x9999999800024682, 0x9999999900000000,
    0x99999998ffffffff, 0x99999999fffffffe, 0x9999999900000000, 0x9999999900000001,
    0x999999987fffffff, 0x9999999880000000, 0x999999997ffffffe, 0x999999997fffffff,
    0x9999999880000001, 0x999999997ffffffe, 0x2468acf13579bde0, 0x1111111111111100,
};
EXPECT_DATA(psubb, uint64_t) {
    0x000000000a225c9e, 0x0000000000000000, 0x0101010101034782, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x8101010101010101, 0x80000000000000ff,
    0x8101010101010102, 0xbcbcbcbc0a225c9e, 0xbcbcbcbc00000000, 0xbcbcbcbc01034782,
    0xbcbcbcbcffffffff, 0xbcbcbcbc00000000, 0xbcbcbcbcfffffffe, 0xbcbcbcbcfffffffd,
    0xbcbcbcbc7fffffff, 0xbcbcbcbc7ffffffe, 0xbcbcbcbc80000000, 0xbcbcbcbc81010101,
    0xbcbcbcbc800000ff, 0xbcbcbcbc81010102, 0x0000000000000000, 0x14589ce02468ace0,
};
EXPECT_DATA(psubh, uint64_t) {
    0x000000000a225b9e, 0x0000000000000000, 0x0001000100034682, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x8001000100010001, 0x800000000000ffff,
    0x8001000100010002, 0xbbbcbbbc0a225b9e, 0xbbbcbbbc00000000, 0xbbbcbbbc00034682,
    0xbbbcbbbcffffffff, 0xbbbcbbbc00000000, 0xbbbcbbbcfffffffe, 0xbbbcbbbcfffffffd,
    0xbbbcbbbc7fffffff, 0xbbbcbbbc7ffffffe, 0xbbbcbbbc80000000, 0xbbbcbbbc80010001,
    0xbbbcbbbc8000ffff, 0xbbbcbbbc80010002, 0x0000000000000000, 0x13589be02468ace0,
};
EXPECT_DATA(psubsb, uint64_t) {
    0x000000000a225c7f, 0x0000000000000000, 0x010101010103477f, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x7f00000000000000, 0x8101010101010101, 0x80000000000000ff,
    0x8101010101010102, 0xbcbcbcbc0a225c7f, 0xbcbcbcbc00000000, 0xbcbcbcbc0103477f,
    0xbcbcbcbcffffffff, 0xbcbcbcbc00000000, 0xbcbcbcbcfffffffe, 0xbcbcbcbcfffffffd,
    0xbcbcbcbc7fffffff, 0xbcbcbcbc7ffffffe, 0xbcbcbcbc7f000000, 0xbcbcbcbc81010101,
    0xbcbcbcbc800000ff, 0xbcbcbcbc81010102, 0x0000000000000000, 0x14587f7f8080ace0,
};
EXPECT_DATA(psubsh, uint64_t) {
    0x000000000a225b9e, 0x0000000000000000, 0x0001000100034682, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x7fff000000000000, 0x8001000100010001, 0x800000000000ffff,
    0x8001000100010002, 0xbbbcbbbc0a225b9e, 0xbbbcbbbc00000000, 0xbbbcbbbc00034682,
    0xbbbcbbbcffffffff, 0xbbbcbbbc00000000, 0xbbbcbbbcfffffffe, 0xbbbcbbbcfffffffd,
    0xbbbcbbbc7fffffff, 0xbbbcbbbc7ffffffe, 0xbbbcbbbc7fff0000, 0xbbbcbbbc80010001,
    0xbbbcbbbc8000ffff, 0xbbbcbbbc80010002, 0x0000000000000000, 0x13587fff8000ace0,
};
EXPECT_DATA(psubusb, uint64_t) {
    0x000000000a220000, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000,
    0x0000000000000000, 0x000000000a220000, 0x0000000000000000, 0x0000000000000000,
    0x00000000ffffffff, 0x0000000000000000, 0x00000000fffffffe, 0x00000000fffffffd,
    0x000000007fffffff, 0x000000007ffffffe, 0x0000000000000000, 0x0000000000000000,
    0x0000000080000000, 0x0000000000000000, 0x0000000000000000, 0x000000002468ace0,
};
EXPECT_DATA(psubush, uint64_t) {
    0x000000000a220000, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000,
    0x0000000000000000, 0x000000000a220000, 0x0000000000000000, 0x0000000000000000,
    0x00000000ffffffff, 0x0000000000000000, 0x00000000fffffffe, 0x00000000fffffffd,
    0x000000007fffffff, 0x000000007ffffffe, 0x0000000000000000, 0x0000000000000000,
    0x0000000080000000, 0x0000000000000000, 0x0000000000000000, 0x000000002468ace0,
};
EXPECT_DATA(psubw, uint64_t) {
    0x000000000a215b9e, 0x0000000000000000, 0x0000000100024682, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x8000000100000001, 0x80000000ffffffff,
    0x8000000100000002, 0xbbbbbbbc0a215b9e, 0xbbbbbbbc00000000, 0xbbbbbbbc00024682,
    0xbbbbbbbcffffffff, 0xbbbbbbbc00000000, 0xbbbbbbbcfffffffe, 0xbbbbbbbcfffffffd,
    0xbbbbbbbc7fffffff, 0xbbbbbbbc7ffffffe, 0xbbbbbbbc80000000, 0xbbbbbbbc80000001,
    0xbbbbbbbc7fffffff, 0xbbbbbbbc80000002, 0x0000000000000000, 0x13579be02468ace0,
};
EXPECT_DATA(psubd, uint64_t) {
    0x000000000a215b9e, 0x0000000000000000, 0x0000000000024682, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x8000000000000001, 0x7fffffffffffffff,
    0x8000000000000002, 0xbbbbbbbc0a215b9e, 0xbbbbbbbc00000000, 0xbbbbbbbb00024682,
    0xbbbbbbbcffffffff, 0xbbbbbbbc00000000, 0xbbbbbbbcfffffffe, 0xbbbbbbbcfffffffd,
    0xbbbbbbbc7fffffff, 0xbbbbbbbc7ffffffe, 0xbbbbbbbb80000000, 0xbbbbbbbb80000001,
    0xbbbbbbbc7fffffff, 0xbbbbbbbb80000002, 0x0000000000000000, 0x13579be02468ace0,
};
EXPECT_DATA(pcmpeqb, uint64_t) {
    0xffffffff00000000, 0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x00ffffffffffffff, 0x0000000000000000, 0x00ffffffffffff00,
    0x0000000000000000, 0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000,
    0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000ffffff, 0x0000000000000000,
    0x0000000000ffff00, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
};
EXPECT_DATA(pcmpeqh, uint64_t) {
    0xffffffff00000000, 0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000ffffffffffff, 0x0000000000000000, 0x0000ffffffff0000,
    0x0000000000000000, 0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000,
    0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x000000000000ffff, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
};
EXPECT_DATA(pcmpeqw, uint64_t) {
    0xffffffff00000000, 0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000,
    0x0000000000000000, 0x00000000ffffffff, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
};
EXPECT_DATA(pcmpgtb, uint64_t) {
    0x00000000ffffffff, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xff00000000000000,
    0xff00000000000000, 0xff00000000000000, 0x00ffffffffffffff, 0x0000000000000000,
    0x00ffffffffffffff, 0x00000000ffffffff, 0x0000000000000000, 0x00000000ffffffff,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000, 0x0000000000ffffff,
    0x0000000000000000, 0x0000000000ffffff, 0x0000000000000000, 0xffffffff00000000,
};
EXPECT_DATA(pcmpgth, uint64_t) {
    0x00000000ffffffff, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xffff000000000000,
    0xffff000000000000, 0xffff000000000000, 0x0000ffffffffffff, 0x0000000000000000,
    0x0000ffffffffffff, 0x00000000ffffffff, 0x0000000000000000, 0x00000000ffffffff,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x00000000ffff0000, 0x00000000ffff0000, 0x00000000ffff0000, 0x000000000000ffff,
    0x0000000000000000, 0x000000000000ffff, 0x0000000000000000, 0xffffffff00000000,
};
EXPECT_DATA(pcmpgtw, uint64_t) {
    0x00000000ffffffff, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xffffffff00000000,
    0xffffffff00000000, 0xffffffff00000000, 0x00000000ffffffff, 0x0000000000000000,
    0x00000000ffffffff, 0x00000000ffffffff, 0x0000000000000000, 0x00000000ffffffff,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x00000000ffffffff, 0x00000000ffffffff, 0x00000000ffffffff, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xffffffff00000000,
};
EXPECT_DATA(pavgusb, uint64_t) {
    0x000000000d23a8a9, 0x0000000000012341, 0x8080808080808080, 0x8080808080808080,
    0xffffffffffffffff, 0x8080808080808080, 0x8080808080808081, 0x4080808080808080,
    0x4080808080808080, 0xbfffffffffffffff, 0xc080808080808080, 0x4000000000000001,
    0xc08080808080807f, 0xcccccccc0d23a8a9, 0xcccccccc00012341, 0xcccccccc80808080,
    0xcccccccc80808080, 0xccccccccffffffff, 0xcccccccc80808080, 0xcccccccc80808081,
    0xcccccccc40808080, 0xcccccccc40808080, 0xccccccccbfffffff, 0xccccccccc0808080,
    0xcccccccc40000001, 0xccccccccc080807f, 0x123456789abcdef0, 0x8888888888888880,
};
EXPECT_DATA(pavgush, uint64_t) {
    0x000000000d23a8a9, 0x0000000000012341, 0x8000800080008000, 0x8000800080008000,
    0xffffffffffffffff, 0x8000800080008000, 0x8000800080008001, 0x4000800080008000,
    0x4000800080008000, 0xbfffffffffffffff, 0xc000800080008000, 0x4000000000000001,
    0xc000800080007fff, 0xcccccccc0d23a8a9, 0xcccccccc00012341, 0xcccccccc80008000,
    0xcccccccc80008000, 0xccccccccffffffff, 0xcccccccc80008000, 0xcccccccc80008001,
    0xcccccccc40008000, 0xcccccccc40008000, 0xccccccccbfffffff, 0xccccccccc0008000,
    0xcccccccc40000001, 0xccccccccc0007fff, 0x123456789abcdef0, 0x8888888888888880,
};
EXPECT_DATA(punpckhbh, uint64_t) {
    0x0000000000000000, 0x0000000000000000, 0x00ff00ff00ff00ff, 0xff00ff00ff00ff00,
    0xffffffffffffffff, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, 0x7f00ff00ff00ff00,
    0x7f00ff00ff00ff00, 0x7fffffffffffffff, 0x80ff00ff00ff00ff, 0x8000000000000000,
    0x80ff00ff00ff00ff, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee,
    0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee,
    0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee,
    0xaaeeaaeeaaeeaaee, 0xaaeeaaeeaaeeaaee, 0x1212343456567878, 0x12fe34dc56ba7898,
};
EXPECT_DATA(punpcklbh, uint64_t) {
    0x1208341256fa78da, 0x0000010123234141, 0x00ff01fe23dc41bf, 0xff00ff00ff00ff00,
    0xffffffffffffffff, 0xff00ff00ff00ff01, 0xff00ff00ff00ff02, 0xff00ff00ff00ff00,
    0xff00ff00ff00ff01, 0xffffffffffffffff, 0x00ff00ff00ff00ff, 0x0000000000000001,
    0x00ff00ff00ff00fe, 0x1208341256fa78da, 0x0000010123234141, 0x00ff01fe23dc41bf,
    0xff00ff00ff00ff00, 0xffffffffffffffff, 0xff00ff00ff00ff01, 0xff00ff00ff00ff02,
    0x7f00ff00ff00ff00, 0x7f00ff00ff00ff01, 0x7fffffffffffffff, 0x80ff00ff00ff00ff,
    0x8000000000000001, 0x80ff00ff00ff00fe, 0x9a9abcbcdedef0f0, 0x9a76bc54de32f010,
};
EXPECT_DATA(punpckhhw, uint64_t) {
    0x0000000000000000, 0x0000000000000000, 0x0000ffff0000ffff, 0xffff0000ffff0000,
    0xffffffffffffffff, 0xffff0000ffff0000, 0xffff0000ffff0000, 0x7fff0000ffff0000,
    0x7fff0000ffff0000, 0x7fffffffffffffff, 0x8000ffff0000ffff, 0x8000000000000000,
    0x8000ffff0000ffff, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee,
    0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee,
    0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee,
    0xaaaaeeeeaaaaeeee, 0xaaaaeeeeaaaaeeee, 0x1234123456785678, 0x1234fedc5678ba98,
};
EXPECT_DATA(punpcklhw, uint64_t) {
    0x123408125678fada, 0x0001000123412341, 0x0001fffe2341dcbf, 0xffff0000ffff0000,
    0xffffffffffffffff, 0xffff0000ffff0001, 0xffff0000ffff0002, 0xffff0000ffff0000,
    0xffff0000ffff0001, 0xffffffffffffffff, 0x0000ffff0000ffff, 0x0000000000000001,
    0x0000ffff0000fffe, 0x123408125678fada, 0x0001000123412341, 0x0001fffe2341dcbf,
    0xffff0000ffff0000, 0xffffffffffffffff, 0xffff0000ffff0001, 0xffff0000ffff0002,
    0x7fff0000ffff0000, 0x7fff0000ffff0001, 0x7fffffffffffffff, 0x8000ffff0000ffff,
    0x8000000000000001, 0x8000ffff0000fffe, 0x9abc9abcdef0def0, 0x9abc7654def03210,
};
EXPECT_DATA(punpckhwd, uint64_t) {
    0x0000000000000000, 0x0000000000000000, 0x00000000ffffffff, 0xffffffff00000000,
    0xffffffffffffffff, 0xffffffff00000000, 0xffffffff00000000, 0x7fffffff00000000,
    0x7fffffff00000000, 0x7fffffffffffffff, 0x80000000ffffffff, 0x8000000000000000,
    0x80000000ffffffff, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee,
    0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee,
    0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee,
    0xaaaaaaaaeeeeeeee, 0xaaaaaaaaeeeeeeee, 0x1234567812345678, 0x12345678fedcba98,
};
EXPECT_DATA(punpcklwd, uint64_t) {
    0x123456780812fada, 0x0001234100012341, 0x00012341fffedcbf, 0xffffffff00000000,
    0xffffffffffffffff, 0xffffffff00000001, 0xffffffff00000002, 0xffffffff00000000,
    0xffffffff00000001, 0xffffffffffffffff, 0x00000000ffffffff, 0x0000000000000001,
    0x00000000fffffffe, 0x123456780812fada, 0x0001234100012341, 0x00012341fffedcbf,
    0xffffffff00000000, 0xffffffffffffffff, 0xffffffff00000001, 0xffffffff00000002,
    0x7fffffff00000000, 0x7fffffff00000001, 0x7fffffffffffffff, 0x80000000ffffffff,
    0x8000000000000001, 0x80000000fffffffe, 0x9abcdef09abcdef0, 0x9abcdef076543210,
};
EXPECT_DATA(pmovmskps, uint64_t) {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000003, 0x000000000000000c,
    0x000000000000000f, 0x000000000000000c, 0x000000000000000c, 0x0000000000000004,
    0x0000000000000004, 0x0000000000000007, 0x000000000000000b, 0x0000000000000008,
    0x000000000000000b, 0x000000000000000a, 0x000000000000000a, 0x000000000000000b,
    0x000000000000000e, 0x000000000000000f, 0x000000000000000e, 0x000000000000000e,
    0x000000000000000a, 0x000000000000000a, 0x000000000000000b, 0x000000000000000f,
    0x000000000000000e, 0x000000000000000f, 0x0000000000000005, 0x0000000000000006,
};
EXPECT_DATA(pmovmskpd, uint64_t) {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000001, 0x0000000000000002,
    0x0000000000000003, 0x0000000000000002, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000001, 0x0000000000000003, 0x0000000000000002,
    0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
    0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
    0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
    0x0000000000000003, 0x0000000000000003, 0x0000000000000000, 0x0000000000000001,
};
EXPECT_DATA(packsshb, uint64_t) {
    0x00007f7f00007f80, 0x0000017f0000017f, 0x0000017ffffffe80, 0xffffffff00000000,
    0xffffffffffffffff, 0xffffffff00000001, 0xffffffff00000002, 0x7fffffff00000000,
    0x7fffffff00000001, 0x7fffffffffffffff, 0x80000000ffffffff, 0x8000000000000001,
    0x80000000fffffffe, 0x80807f7f80807f80, 0x8080017f8080017f, 0x8080017f8080fe80,
    0x8080ffff80800000, 0x8080ffff8080ffff, 0x8080ffff80800001, 0x8080ffff80800002,
    0x80807fff80800000, 0x80807fff80800001, 0x80807fff8080ffff, 0x808080008080ffff,
    0x8080800080800001, 0x808080008080fffe, 0x7f7f80807f7f8080, 0x7f7f808080807f7f,
};
EXPECT_DATA(packushb, uint64_t) {
    0x0000ffff0000ff00, 0x000001ff000001ff, 0x000001ff00000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0xff00000000000000,
    0xff00000000000001, 0xff00000000000000, 0x0000000000000000, 0x0000000000000001,
    0x0000000000000000, 0x0000ffff0000ff00, 0x000001ff000001ff, 0x000001ff00000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000001, 0x0000000000000002,
    0x0000ff0000000000, 0x0000ff0000000001, 0x0000ff0000000000, 0x0000000000000000,
    0x0000000000000001, 0x0000000000000000, 0xffff0000ffff0000, 0xffff00000000ffff,
};
EXPECT_DATA(packsswh, uint64_t) {
    0x00007fff00007fff, 0x00007fff00007fff, 0x00007fffffff8000, 0xffffffff00000000,
    0xffffffffffffffff, 0xffffffff00000001, 0xffffffff00000002, 0x7fffffff00000000,
    0x7fffffff00000001, 0x7fffffffffffffff, 0x80000000ffffffff, 0x8000000000000001,
    0x80000000fffffffe, 0x80007fff80007fff, 0x80007fff80007fff, 0x80007fff80008000,
    0x8000ffff80000000, 0x8000ffff8000ffff, 0x8000ffff80000001, 0x8000ffff80000002,
    0x80007fff80000000, 0x80007fff80000001, 0x80007fff8000ffff, 0x800080008000ffff,
    0x8000800080000001, 0x800080008000fffe, 0x7fff80007fff8000, 0x7fff800080007fff,
};
EXPECT_DATA(pmovmskb, uint64_t) {
    0x0000000000000003, 0x0000000000000000, 0x00000000000000ff, 0x000000000000ff00,
    0x000000000000ffff, 0x000000000000ff00, 0x000000000000ff00, 0x0000000000007f00,
    0x0000000000007f00, 0x0000000000007fff, 0x00000000000080ff, 0x0000000000008000,
    0x00000000000080ff, 0x000000000000f0f3, 0x000000000000f0f0, 0x000000000000f0ff,
    0x000000000000fff0, 0x000000000000ffff, 0x000000000000fff0, 0x000000000000fff0,
    0x000000000000f7f0, 0x000000000000f7f0, 0x000000000000f7ff, 0x000000000000f8ff,
    0x000000000000f8f0, 0x000000000000f8ff, 0x0000000000000f0f, 0x0000000000000ff0,
};
EXPECT_DATA(pmulhh, uint64_t) {
    0x000000000092fe42, 0x00000000000004da, 0x00000000fffffb25, 0x0000000000000000,
    0x0000000000000000, 0x000000000000ffff, 0x000000000000ffff, 0x0000000000000000,
    0x000000000000ffff, 0xffff000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x05b005b00092fe42, 0x05b005b0000004da, 0x05b005b0fffffb25,
    0x05b005b000000000, 0x05b005b000000000, 0x05b005b00000ffff, 0x05b005b00000ffff,
    0x05b005b000000000, 0x05b005b00000ffff, 0x05b005b0ffff0000, 0x05b005b000000000,
    0x05b005b000000000, 0x05b005b000000000, 0x014b1d34280e0445, 0xffebe88ed131f988,
};
EXPECT_DATA(pmullh, uint64_t) {
    0x00000000e7a8d230, 0x000000000001d681, 0x00000000fffe297f, 0x0000000000000000,
    0x0001000100010001, 0x000000000000ffff, 0x000000000000fffe, 0x0000000000000000,
    0x000000000000ffff, 0x8001000100010001, 0x8000000000000000, 0x0000000000000000,
    0x8000000000000000, 0xb60cb60ce7a8d230, 0xb60cb60c0001d681, 0xb60cb60cfffe297f,
    0xb60cb60c00000000, 0xb60cb60c00010001, 0xb60cb60c0000ffff, 0xb60cb60c0000fffe,
    0xb60cb60c00000000, 0xb60cb60c0000ffff, 0xb60cb60c80010001, 0xb60cb60c80000000,
    0xb60cb60c00000000, 0xb60cb60c80000000, 0x5a90d840ba102100, 0x3cb087406db0cf00,
};
EXPECT_DATA(pmaddh, uint64_t) {
    0x00000000fed5b9d8, 0x0000000004dad682, 0x00000000fb25297d, 0x0000000000000000,
    0x0000000200000002, 0x00000000ffffffff, 0x00000000fffffffe, 0x0000000000000000,
    0x00000000ffffffff, 0xffff800200000002, 0x0000800000000000, 0x0000000000000000,
    0x0000800000000000, 0x0b616c18fed5b9d8, 0x0b616c1804dad682, 0x0b616c18fb25297d,
    0x0b616c1800000000, 0x0b616c1800000002, 0x0b616c18ffffffff, 0x0b616c18fffffffe,
    0x0b616c1800000000, 0x0b616c18ffffffff, 0x0b616c18ffff8002, 0x0b616c1800008000,
    0x0b616c1800000000, 0x0b616c1800008000, 0x1e8032d02c53db10, 0xe879c3f0caba3cb0,
};
EXPECT_DATA(pmulhuh, uint64_t) {
    0x00000000009254ba, 0x00000000000004da, 0x0000000000001e66, 0x0000000000000000,
    0xfffefffefffefffe, 0x0000000000000000, 0x0000000000000001, 0x0000000000000000,
    0x0000000000000000, 0x7ffefffefffefffe, 0x7fff000000000000, 0x0000000000000000,
    0x7fff000000000000, 0x9f489f48009254ba, 0x9f489f48000004da, 0x9f489f4800001e66,
    0x9f489f4800000000, 0x9f489f48fffefffe, 0x9f489f4800000000, 0x9f489f4800000001,
    0x9f489f4800000000, 0x9f489f4800000000, 0x9f489f487ffefffe, 0x9f489f487fff0000,
    0x9f489f4800000000, 0x9f489f487fff0000, 0x014b1d345d86c225, 0x121f3f0647852b98,
};
EXPECT_DATA(psadbw, uint64_t) {
    0x0000000000000132, 0x0000000000000000, 0x000000000000072f, 0x00000000000007f8,
    0x0000000000000000, 0x00000000000007f7, 0x00000000000007f6, 0x0000000000000778,
    0x0000000000000777, 0x0000000000000080, 0x0000000000000778, 0x0000000000000081,
    0x0000000000000777, 0x0000000000000242, 0x0000000000000110, 0x0000000000000443,
    0x000000000000050c, 0x0000000000000110, 0x000000000000050b, 0x000000000000050a,
    0x000000000000048c, 0x000000000000048b, 0x0000000000000190, 0x000000000000048c,
    0x0000000000000191, 0x000000000000048b, 0x0000000000000000, 0x0000000000000430,
};

int main(int argc, char *argv[]) {
    TEST2_DEFAULT_DATA(CHECK2_0134, EXEC_RR, pandd);
    TEST2_DEFAULT_DATA(CHECK2_0134, EXEC_RR, pandnd);
    TEST2_DEFAULT_DATA(CHECK2_0134, EXEC_RR, pord);
    TEST2_DEFAULT_DATA(CHECK2_0134, EXEC_RR, pxord);

    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pminub);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pminsh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pmaxub);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pmaxsh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddsb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddsh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddusb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddush);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddw);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, paddd);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubsb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubsh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubusb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubush);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubw);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, psubd);

    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpeqb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpeqh);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpeqw);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpgtb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpgth);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pcmpgtw);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pavgusb);
    TEST2_DEFAULT_DATA(CHECK2_03, EXEC_RR, pavgush);

    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpckhbh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpcklbh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpckhhw);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpcklhw);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpckhwd);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, punpcklwd);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmovmskps);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmovmskpd);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, packsshb);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, packushb);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, packsswh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmovmskb);

    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmulhh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmullh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmaddh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, pmulhuh);
    TEST2_DEFAULT_DATA(CHECK2_14, EXEC_RR, psadbw);

    // TODO: psrlqh
    // TODO: psrlql
    // TODO: psllqh
    // TODO: psllql
    // TODO: psrlw
    // TODO: psrlh
    // TODO: psraw
    // TODO: psrah
    // TODO: psllw
    // TODO: psllh
    // TODO: pshufw
    // TODO: pshufh

    // TODO: pextrh
    // TODO: pinsh
    // TODO: psrld
    // TODO: pslld

    return is_failed();
}
