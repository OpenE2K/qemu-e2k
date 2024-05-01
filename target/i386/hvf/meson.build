i386_system_ss.add(when: [hvf, 'CONFIG_HVF'], if_true: files(
  'hvf.c',
  'x86.c',
  'x86_cpuid.c',
  'x86_decode.c',
  'x86_descr.c',
  'x86_emu.c',
  'x86_flags.c',
  'x86_mmu.c',
  'x86_task.c',
  'x86hvf.c',
  'hvf-cpu.c',
))

if mcst_lcc_sucks
  i386_ss.add(when: [hvf, 'CONFIG_HVF'], if_false: files('hvf-stub.c'))
endif
