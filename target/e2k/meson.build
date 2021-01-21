e2k_ss = ss.source_set()
e2k_ss.add(files(
  'cpu.c',
  'gdbstub.c',
  'helper.c',
  'helper_aau.c',
  'helper_fpu.c',
  'helper_int.c',
  'helper_sm.c',
  'helper_vec.c',
  'translate.c',
  'translate/alc.c',
  'translate/aau.c',
  'translate/plu.c',
  'translate/state.c',
))

# no softmmu support yet

target_arch += {'e2k': e2k_ss}
