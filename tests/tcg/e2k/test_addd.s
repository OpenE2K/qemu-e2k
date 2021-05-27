.global _start
.include "macros.inc"

_start:
{
    setwd wsz=0x8, nfx=1
    setbn rbs=0x4, rsz=0x3, rcur=0
}
{
    addd 0x0, 0x0, %r0 ! counter
    addd 0x2, 0xFFFFFFFFFFFFFFFF, %r1
}
{
    addd 0x2, 0x2, %r2
    adds 0x2, 0x2, %r3
    adds 0x2, 0xFFFFFFFF, %r4
}
    assert cmpedb, %r1, 0x1, %r0
    assert cmpedb, %r2, 0x4, %r0
    assert cmpesb, %r3, 0x4, %r0
    assert cmpesb, %r4, 0x1, %r0

.include "macros_end.inc"
