e2k_ss = ss.source_set()
e2k_ss.add(files(
  'cpu.c',
  'helper.c',
  'helper_int.c',
  'translate.c',
  'translate/state.c',
  'translate/control.c',
  'translate/alc.c',
))

# no softmmu support yet

target_arch += {'e2k': e2k_ss}
