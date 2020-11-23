e2k_ss = ss.source_set()
e2k_ss.add(files(
  'cpu.c',
  'gdbstub.c',
  'helper.c',
  'helper_int.c',
  'translate.c',
  'translate/state.c',
  'translate/control.c',
  'translate/alc.c',
  'translate/plu.c',
))

# no softmmu support yet

target_arch += {'e2k': e2k_ss}
