e2k_ss = ss.source_set()
e2k_ss.add(files(
  'cpu.c',
  'cpu-dump.c',
  'gdbstub.c',
  'helper.c',
  'helper_aau.c',
  'helper_fpu.c',
  'helper_int.c',
  'helper_sm.c',
  'helper_vec.c',
  'translate.c',
))

e2k_softmmu_ss = ss.source_set()

target_arch += {'e2k': e2k_ss}
target_softmmu_arch += {'e2k': e2k_softmmu_ss}
