 
{
	static e2k_alf1_opcode_templ dummy =
		{"adds", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x10, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"addd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x11, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"subs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x12, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"subd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x13, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"udivx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x44, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"umodx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x45, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sdivx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x46, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"smodx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x47, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"udivs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x40, {0, 0, 0, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"udivd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x41, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sdivs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x42, {0, 0, 0, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sdivd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x43, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ands", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x0, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"andd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"andns", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x2, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"andnd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x3, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ors", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ord", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"orns", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x6, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ornd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x7, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"xors", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x8, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"xord", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x9, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"xorns", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0xa, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"xornd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0xb, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"shls", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x18, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"shld", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x19, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"shrs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1a, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"shrd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1b, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"scls", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x14, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"scld", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x15, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"scrs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x16, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"scrd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x17, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sars", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1c, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sard", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1d, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"getfs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1e, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"getfd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x1f, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"sxt", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0xc, {1, 1, 1, 1, 1, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"merges", parse_alf_args, merge_alopf_simple, MERGE, NO_MAS, 0xe, {1, 1, 1, 1, 1, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"merged", parse_alf_args, merge_alopf_simple, MERGE, NO_MAS, 0xf, {1, 1, 1, 1, 1, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fadds", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"faddd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fsubs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fsubd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmins", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmind", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmaxs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmaxd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmuls", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fmuld", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxaddss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxadddd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxaddsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxadddx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxaddxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxaddxd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxaddxs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubdd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubdx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubxd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsubxs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxrsubss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxrsubdd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxrsubsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxrsubdx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmulss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmuldd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmulsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmuldx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmulxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmulxd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxmulxs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x48, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivdd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x49, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4a, {0, 0, 0, 0, 0, 1}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivdx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4b, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4f, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivxd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4d, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivxs", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x4c, {0, 0, 0, 0, 0, 1}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivtss", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x60, {0, 0, 0, 0, 0, 1}, ARGS_DSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivtdd", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x61, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivtsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x62, {0, 0, 0, 0, 0, 1}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxdivtdx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x63, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrtusx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5a, {0, 0, 0, 0, 0, 1}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrtudx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5b, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrtuxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x59, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrttsx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5e, {0, 0, 0, 0, 0, 1}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrttdx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5f, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"fxsqrttxx", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5d, {0, 0, 0, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"movif", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"vfsi", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_SDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldcsb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x68, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"lddsb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6c, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldesb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x70, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldfsb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x74, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldgsb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x78, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldssb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7c, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldcsh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x69, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"lddsh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6d, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldesh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x71, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldfsh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x75, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldgsh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x79, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldssh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7d, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldcsw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6a, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"lddsw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6e, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldesw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x72, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldfsw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x76, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldgsw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7a, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldssw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7e, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldcsd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6b, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"lddsd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x6f, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldesd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x73, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldfsd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x77, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldgsd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7b, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldssd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x7f, {1, 0, 1, 1, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldb", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x64, {1, 0, 1, 1, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldh", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x65, {1, 0, 1, 1, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldw", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x66, {1, 0, 1, 1, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf1_opcode_templ dummy =
		{"ldd", parse_alf_args, merge_alopf_simple, ALOPF1, MAS, 0x67, {1, 0, 1, 1, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxsqrtisx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x52, {0, 0, 0, 0, 0, 1}, ARGS_SD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxsqrtidx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x53, {0, 0, 0, 0, 0, 1}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxsqrtixx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x57, {0, 0, 0, 0, 0, 1}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movfi", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_DS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movts", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtcs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrcs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtcd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrcd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fstois", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fstoid", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtois", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtoid", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtois", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtoid", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fstoistr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtoistr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"istofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"istofd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"idtofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"idtofd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"istofx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc5};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"idtofx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc5};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fstofd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc6};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc6};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtofd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc6};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fstofx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc7};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc7};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtofx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc7};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfdtois", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc8};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfstois", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc8};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfdtoistr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xca};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfstoistr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xca};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pistofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xcc};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfstofd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xce};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"pfdtofs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xce};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"getpl", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x63, {1, 0, 0, 1, 0, 0}, ARGS_SD, 0xf0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"getsap", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x62, {1, 1, 0, 0, 0, 0}, ARGS_SQ, 0xec};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"cudtoap", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_SQ, 0xf0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"gdtoap", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_SQ, 0xf2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stcsb", parse_alf_args, NULL, ALOPF3, MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stdsb", parse_alf_args, NULL, ALOPF3, MAS, 0x2c, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stesb", parse_alf_args, NULL, ALOPF3, MAS, 0x30, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stfsb", parse_alf_args, NULL, ALOPF3, MAS, 0x34, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stgsb", parse_alf_args, NULL, ALOPF3, MAS, 0x38, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stssb", parse_alf_args, NULL, ALOPF3, MAS, 0x3c, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stcsh", parse_alf_args, NULL, ALOPF3, MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stdsh", parse_alf_args, NULL, ALOPF3, MAS, 0x2d, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stesh", parse_alf_args, NULL, ALOPF3, MAS, 0x31, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stfsh", parse_alf_args, NULL, ALOPF3, MAS, 0x35, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stgsh", parse_alf_args, NULL, ALOPF3, MAS, 0x39, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stssh", parse_alf_args, NULL, ALOPF3, MAS, 0x3d, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stcsw", parse_alf_args, NULL, ALOPF3, MAS, 0x2a, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stdsw", parse_alf_args, NULL, ALOPF3, MAS, 0x2e, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stesw", parse_alf_args, NULL, ALOPF3, MAS, 0x32, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stfsw", parse_alf_args, NULL, ALOPF3, MAS, 0x36, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stgsw", parse_alf_args, NULL, ALOPF3, MAS, 0x3a, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stssw", parse_alf_args, NULL, ALOPF3, MAS, 0x3e, {0, 0, 1, 0, 0, 1}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stcsd", parse_alf_args, NULL, ALOPF3, MAS, 0x2b, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stdsd", parse_alf_args, NULL, ALOPF3, MAS, 0x2f, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stesd", parse_alf_args, NULL, ALOPF3, MAS, 0x33, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stfsd", parse_alf_args, NULL, ALOPF3, MAS, 0x37, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stgsd", parse_alf_args, NULL, ALOPF3, MAS, 0x3b, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stssd", parse_alf_args, NULL, ALOPF3, MAS, 0x3f, {0, 0, 1, 0, 0, 1}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stb", parse_alf_args, NULL, ALOPF3, MAS, 0x24, {0, 0, 1, 0, 0, 1}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"sth", parse_alf_args, NULL, ALOPF3, MAS, 0x25, {0, 0, 1, 0, 0, 1}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"stw", parse_alf_args, NULL, ALOPF3, MAS, 0x26, {0, 0, 1, 0, 0, 1}, ARGS_DDS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf3_opcode_templ dummy =
		{"std", parse_alf_args, NULL, ALOPF3, MAS, 0x27, {0, 0, 1, 0, 0, 1}, ARGS_DDD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmposb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x0, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpbsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x1, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpbesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x3, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpssb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmppsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmplsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x6, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmplesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpodb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x0, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpbdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x1, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpedb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpbedb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x3, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpsdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmppdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpldb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x6, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandssb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandpsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandlesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandedb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandsdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandpdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"cmpandledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpeqsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x0, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpltsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x1, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmplesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x2, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpuodsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x3, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpneqsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x4, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpnltsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x5, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpnlesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x6, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpodsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x7, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpeqdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x0, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpltdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x1, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x2, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpuoddb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x3, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpneqdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x4, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpnltdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x5, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpnledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x6, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fcmpoddb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x7, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpeqsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x0, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpltsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x1, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmplesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x2, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpuodsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x3, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpneqsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x4, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnltsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x5, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnlesb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x6, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpodsb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0x7, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpeqdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x0, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpltdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x1, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x2, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpuoddb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x3, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpneqdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x4, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnltdb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x5, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnledb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x6, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpoddb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x7, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpeqxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x0, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpltxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x1, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmplexb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x2, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpuodxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x3, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpneqxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x4, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnltxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x5, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpnlexb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x6, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf7_opcode_templ dummy =
		{"fxcmpodxb", parse_alf_args, NULL, ALOPF7, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0x7, 4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctopo", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctopb", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctope", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctopbe", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctops", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x4};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctopp", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x5};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctopl", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x6};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf8_opcode_templ dummy =
		{"cctople", parse_alf_args, NULL, ALOPF8, NO_MAS, 0x24, {1, 0, 0, 1, 0, 0}, ARGS_S, 0x7};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"staab", parse_alf_args, NULL, ALOPF10, MAS, 0x1c, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"staah", parse_alf_args, NULL, ALOPF10, MAS, 0x1d, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"staaw", parse_alf_args, NULL, ALOPF10, MAS, 0x1e, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"staad", parse_alf_args, NULL, ALOPF10, MAS, 0x1f, {0, 0, 1, 0, 0, 1}, ARGS_D};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"staaq", parse_alf_args, NULL, ALOPF10, MAS, 0x3f, {0, 0, 1, 0, 0, 1}, ARGS_Q};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"aaurw", parse_alf_args, NULL, AAURW, NO_MAS, 0x1e, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"aaurws", parse_alf_args, NULL, AAURW, NO_MAS, 0x1e, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"aaurwd", parse_alf_args, NULL, AAURW, NO_MAS, 0x1f, {0, 0, 1, 0, 0, 1}, ARGS_D};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf10_opcode_templ dummy =
		{"aaurwq", parse_alf_args, NULL, AAURW, NO_MAS, 0x3f, {0, 0, 1, 0, 0, 1}, ARGS_Q};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf9_opcode_templ dummy =
		{"aaurr", parse_alf_args, NULL, AAURR, NO_MAS, 0x5e, {0, 0, 1, 0, 0, 1}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf9_opcode_templ dummy =
		{"aaurrd", parse_alf_args, NULL, AAURR, NO_MAS, 0x5f, {0, 0, 1, 0, 0, 1}, ARGS_D};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf9_opcode_templ dummy =
		{"aaurrq", parse_alf_args, NULL, AAURR, NO_MAS, 0x7f, {0, 0, 1, 0, 0, 1}, ARGS_Q};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"muls", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SSS, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"muld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"umulx", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SSD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"smulx", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_SSD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fdivs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x48, {0, 0, 0, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fdivd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x49, {0, 0, 0, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fsqrttd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x51, {0, 0, 0, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmuls", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmuld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x9, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xe, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xf, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xb, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xc, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"paddush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xd, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x11, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x16, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x17, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x13, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x14, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psubush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x15, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxub", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminub", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psadbw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1c, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmulhuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1b, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmulhh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x18, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmullh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x19, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1a, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pslld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4e, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psllw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x14, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psllh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x15, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4c, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrlw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrlh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x11, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psraw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrah", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x13, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_SSS, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfaddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_SSS, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"aptoap", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_QSQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"aptoapb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_QSQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"getva", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x52, {1, 0, 0, 1, 0, 0}, ARGS_QSD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pandd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pandnd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pord", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pxord", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldrd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5b, {1, 0, 1, 1, 0, 1}, ARGS_DDD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"puttc", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x25, {1, 0, 0, 0, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, 0, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pavgusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1e, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pavgush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1f, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfdivs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4a, {0, 0, 0, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfdivd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4b, {0, 0, 0, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmins", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmind", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmaxs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmaxd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfsqrttd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x53, {0, 0, 0, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, 0, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"pextrh", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0x1e, {0, 1, 0, 0, 1, 0}, ARGS_DDS, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 7, "use of 'pextrh' with count > 7 leads to an undefined result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"pinsh", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0x1f, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 4, "use of 'pinsh' with count > 4 leads to an undefined result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"psllqh", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0xe, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 15, "use of 'psllqh' with count > 15 leads to a zero result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"psllql", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0xf, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 7, "use of 'psllql' with count > 7 leads to a zero result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"psrlqh", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0xc, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 7, "use of 'psrlqh' with count > 7 leads to a zero result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"psrlql", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0xd, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 15, "use of 'psrlql' with count > 15 leads to a zero result"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"cast", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_QQQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"tdtomp", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"odtoap", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_QDQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"puttags", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0xa, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"puttagd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0xb, {0, 0, 1, 0, 0, 1}, ARGS_DSD, NONE, {0, 0, EXT, 0, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpeqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc0, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmplts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc1, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmples", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc2, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpuods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpneqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc4, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpnlts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc5, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpnles", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc6, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpeqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc0, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc1, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc2, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpuodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpneqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc4, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpnltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc5, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpnled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc6, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpeqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc0, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmplts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc1, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmples", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc2, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpuods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpneqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc4, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpnlts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc5, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpnles", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc6, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpeqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc0, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc1, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc2, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpuodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpneqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc4, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpnltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc5, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpnled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc6, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfcmpodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDD, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpodsf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpudsf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_SSS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpoddf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fcmpuddf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpodsf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DSS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpudsf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DSS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpoddf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpuddf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpodxf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc7, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxcmpudxf", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDS, 0xc3, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpeqb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x18, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpeqh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x19, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpeqw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1a, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpgtb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1b, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpgth", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1c, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpgtw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1d, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmovmskb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xb, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmovmskps", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmovmskpd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"packsshb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"packushb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x9, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_lit8_opcode_templ dummy =
		{"pshufw", parse_alf_args, merge_alopf11, ALOPF11_LIT8, NO_MAS, 0x16, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0, 15, "'pshufw' uses only four low-order bits of literal8"};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"packsswh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpckhbh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpckhhw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpckhwd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpcklbh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpcklhw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"punpcklwd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x64, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x65, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x66, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x67, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x79, {1, 0, 1, 1, 0, 1}, ARGS_SSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x60, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x61, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x62, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x63, {1, 0, 1, 1, 0, 1}, ARGS_SSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x78, {1, 0, 1, 1, 0, 1}, ARGS_SSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldapb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x68, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldaph", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x69, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldapw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6a, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldapd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6b, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldapq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7a, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x70, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x73, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x71, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7c, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodww", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x72, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x74, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x77, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodph", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x75, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7d, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x76, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6c, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6f, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6d, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7b, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6e, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcsq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x42, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"lddsq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x43, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldesq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x44, {1, 0, 1, 1, 0, 1}, ARGS_SSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldfsq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x45, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgsq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x46, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldssq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x47, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"adds_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"adds_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"adds_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"subs_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"subs_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"subs_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"umulx_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x22, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"umulx_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x22, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"umulx_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x22, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"smulx_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x23, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"smulx_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x23, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"smulx_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x23, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ands_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ands_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ands_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"andns_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"andns_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"andns_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ors_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ors_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"ors_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"orns_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"orns_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"orns_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xors_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xors_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xors_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xorns_fb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLB, 0, 0, FLB, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xorns_fh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLH, 0, 0, FLH, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf11_opcode_templ dummy =
		{"xorns_fw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {FLW, 0, 0, FLW, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"frcps", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x50, {0, 0, 0, 0, 0, 1}, ARGS_SS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"fsqrts", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x4c, {0, 0, 0, 0, 0, 1}, ARGS_SS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"fsqrtid", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x4d, {0, 0, 0, 0, 0, 1}, ARGS_DD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"frsqrts", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x54, {0, 0, 0, 0, 0, 1}, ARGS_SS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"pfsqrts", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x4e, {0, 0, 0, 0, 0, 1}, ARGS_SS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf12_opcode_templ dummy =
		{"gettd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x56, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"gettags", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x8, {0, 0, 1, 0, 0, 1}, ARGS_SS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"gettagd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x9, {0, 0, 1, 0, 0, 1}, ARGS_DS, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"gettc", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x24, {1, 0, 0, 0, 0, 0}, ARGS_SD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"invtc", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x26, {1, 0, 0, 0, 0, 0}, ARGS_SD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf12_opcode_templ dummy =
		{"getsod", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xec, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"getsp", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x58, {1, 0, 0, 1, 0, 0}, ARGS_SD, 0xec, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf12_opcode_templ dummy =
		{"pshufh", parse_alf_args, NULL, ALOPF12_PSHUFH, NO_MAS, 0x17, {0, 1, 0, 0, 1, 0}, ARGS_DD, 0xc0, 0x0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stcsq", parse_alf_args, NULL, ALOPF13, MAS, 0x2, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stdsq", parse_alf_args, NULL, ALOPF13, MAS, 0x3, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stesq", parse_alf_args, NULL, ALOPF13, MAS, 0x4, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stfsq", parse_alf_args, NULL, ALOPF13, MAS, 0x5, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgsq", parse_alf_args, NULL, ALOPF13, MAS, 0x6, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stssq", parse_alf_args, NULL, ALOPF13, MAS, 0x7, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"strd", parse_alf_args, NULL, ALOPF13, MAS, 0x1b, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdb", parse_alf_args, NULL, ALOPF13, MAS, 0x24, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdh", parse_alf_args, NULL, ALOPF13, MAS, 0x25, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdw", parse_alf_args, NULL, ALOPF13, MAS, 0x26, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdd", parse_alf_args, NULL, ALOPF13, MAS, 0x27, {0, 0, 1, 0, 0, 1}, ARGS_SSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdq", parse_alf_args, NULL, ALOPF13, MAS, 0x39, {0, 0, 1, 0, 0, 1}, ARGS_SSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapb", parse_alf_args, NULL, ALOPF13, MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"staph", parse_alf_args, NULL, ALOPF13, MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapw", parse_alf_args, NULL, ALOPF13, MAS, 0x2a, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapd", parse_alf_args, NULL, ALOPF13, MAS, 0x2b, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapq", parse_alf_args, NULL, ALOPF13, MAS, 0x3a, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpb", parse_alf_args, NULL, ALOPF13, MAS, 0x34, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpd", parse_alf_args, NULL, ALOPF13, MAS, 0x37, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodph", parse_alf_args, NULL, ALOPF13, MAS, 0x35, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpq", parse_alf_args, NULL, ALOPF13, MAS, 0x3d, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpw", parse_alf_args, NULL, ALOPF13, MAS, 0x36, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrb", parse_alf_args, NULL, ALOPF13, MAS, 0x2c, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrd", parse_alf_args, NULL, ALOPF13, MAS, 0x2f, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrh", parse_alf_args, NULL, ALOPF13, MAS, 0x2d, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrq", parse_alf_args, NULL, ALOPF13, MAS, 0x3b, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrw", parse_alf_args, NULL, ALOPF13, MAS, 0x2e, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwb", parse_alf_args, NULL, ALOPF13, MAS, 0x30, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwd", parse_alf_args, NULL, ALOPF13, MAS, 0x33, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwh", parse_alf_args, NULL, ALOPF13, MAS, 0x31, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwq", parse_alf_args, NULL, ALOPF13, MAS, 0x3c, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodww", parse_alf_args, NULL, ALOPF13, MAS, 0x32, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf15_opcode_templ dummy =
		{"rws", parse_alf_args, NULL, ALOPF15, NO_MAS, 0x3c, {1, 0, 0, 0, 0, 0}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf15_opcode_templ dummy =
		{"rwd", parse_alf_args, NULL, ALOPF15, NO_MAS, 0x3d, {1, 0, 0, 0, 0, 0}, ARGS_D};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf16_opcode_templ dummy =
		{"rrs", parse_alf_args, NULL, ALOPF16, NO_MAS, 0x3e, {1, 0, 0, 0, 0, 0}, ARGS_S};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf16_opcode_templ dummy =
		{"rrd", parse_alf_args, NULL, ALOPF16, NO_MAS, 0x3f, {1, 0, 0, 0, 0, 0}, ARGS_D};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"incs_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x30, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"incs_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x30, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"incs_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x30, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"decs_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x32, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"decs_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x32, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"decs_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x32, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shls_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x18, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shls_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x18, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shls_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x18, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shrs_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1a, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shrs_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1a, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shrs_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1a, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scls_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x14, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scls_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x14, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scls_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x14, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scrs_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x16, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scrs_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x16, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scrs_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x16, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sars_fb", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1c, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLB};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sars_fh", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1c, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLH};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sars_fw", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1c, {1, 0, 0, 1, 0, 0}, ARGS_SSS, FLW};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtq", parse_alf_args, NULL, ALOPF22, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtcq", parse_alf_args, NULL, ALOPF22, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrq", parse_alf_args, NULL, ALOPF22, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alf2_opcode_templ dummy =
		{"movtrcq", parse_alf_args, NULL, ALOPF22, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xc3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf1_opcode_templ dummy =
		{"movif", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x5e, {1, 0, 0, 1, 0, 0}, ARGS_DSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"movfi", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x5c, {1, 0, 0, 1, 0, 0}, ARGS_DS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"bitrevs", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"bitrevd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"lzcnts", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x64, {0, 1, 0, 0, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"lzcntd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x65, {0, 1, 0, 0, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"popcnts", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x66, {0, 1, 0, 0, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"popcntd", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x67, {0, 1, 0, 0, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"fstoidtr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_SD, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"fdtoidtr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtoistr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DS, 0xc3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"fxtoidtr", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"movx", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"movxa", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alf2_opcode_templ dummy =
		{"movxc", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DD, 0xc2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmulubhh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1d, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"cast", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_QQQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"tdtomp", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"odtoap", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_QDQ, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x70, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x73, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x71, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodwq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7c, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodww", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x72, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x74, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x77, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodph", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x75, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7d, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodpw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x76, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrb", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6c, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrd", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6f, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrh", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6d, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x7b, {1, 0, 1, 1, 0, 1}, ARGS_QSQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldodrw", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6e, {1, 0, 1, 1, 0, 1}, ARGS_QSD, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf12_opcode_templ dummy =
		{"gettd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x56, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf12_opcode_templ dummy =
		{"getsod", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_QQ, 0xec, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpb", parse_alf_args, NULL, ALOPF13, MAS, 0x34, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpd", parse_alf_args, NULL, ALOPF13, MAS, 0x37, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodph", parse_alf_args, NULL, ALOPF13, MAS, 0x35, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpq", parse_alf_args, NULL, ALOPF13, MAS, 0x3d, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodpw", parse_alf_args, NULL, ALOPF13, MAS, 0x36, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrb", parse_alf_args, NULL, ALOPF13, MAS, 0x2c, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrd", parse_alf_args, NULL, ALOPF13, MAS, 0x2f, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrh", parse_alf_args, NULL, ALOPF13, MAS, 0x2d, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrq", parse_alf_args, NULL, ALOPF13, MAS, 0x3b, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodrw", parse_alf_args, NULL, ALOPF13, MAS, 0x2e, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwb", parse_alf_args, NULL, ALOPF13, MAS, 0x30, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwd", parse_alf_args, NULL, ALOPF13, MAS, 0x33, {0, 0, 1, 0, 0, 1}, ARGS_QSD, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwh", parse_alf_args, NULL, ALOPF13, MAS, 0x31, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodwq", parse_alf_args, NULL, ALOPF13, MAS, 0x3c, {0, 0, 1, 0, 0, 1}, ARGS_QSQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 2)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stodww", parse_alf_args, NULL, ALOPF13, MAS, 0x32, {0, 0, 1, 0, 0, 1}, ARGS_QSS, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psllw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x14, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psllh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x15, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrlw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x10, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrlh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x11, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psraw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x12, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrah", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x13, {1, 0, 0, 1, 0, 0}, ARGS_SSS, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fstoifs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_SSS, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fdtoifd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"umulhd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"smulhd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminuw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x20, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminsw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x21, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxuw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x22, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxsw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x23, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"mpsadbh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"packuswh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpeqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4d, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pcmpgtd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4f, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfhadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfhsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfaddsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pminuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaxuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT, 0, 0, EXT, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfstoifs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfdtoifd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phaddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x29, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phaddw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2a, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phaddsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2b, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phsubh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2d, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phsubw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2e, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phsubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2f, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psignb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x30, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psignh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x31, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psignw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x32, {1, 0, 0, 1, 0, 0}, ARGS_DDD, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmaddubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmulhrsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"phminposuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"addd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x11, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"subd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x13, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"andd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"andnd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ord_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ornd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"xord_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x9, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf11_opcode_templ dummy =
		{"xornd_fd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xb, {1, 0, 0, 1, 0, 0}, ARGS_DDS, NONE, {FLD, 0, 0, FLD, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf12_opcode_templ dummy =
		{"puttst", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x25, {0, 0, 0, 1, 0, 0}, ARGS_DD, 0xc0, 0xc0, 0x1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"incd_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x31, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"decd_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x33, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shld_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x19, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shrd_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1b, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scld_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x15, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scrd_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x17, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sard_fd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1d, {1, 0, 0, 1, 0, 0}, ARGS_DDS, FLD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfmuls", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x38, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x30, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfaddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x31, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x32, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x33, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fscales", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x24, {0, 1, 0, 0, 1, 0}, ARGS_SSS, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fscaled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x25, {0, 1, 0, 0, 1, 0}, ARGS_DSD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fxscalesx", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x27, {0, 1, 0, 0, 1, 0}, ARGS_DSD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfhadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x72, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfhsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x73, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pfaddsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x77, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT, 0, 0, EXT}, 1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x30, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"faddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x31, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x32, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x33, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fmuls", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x38, {0, 0, 1, 0, 0, 1}, ARGS_SSS, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf11_opcode_templ dummy =
		{"fmuld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x39, {0, 0, 1, 0, 0, 1}, ARGS_DDD, NONE, {0, 0, EXT1, 0, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alf10_opcode_templ dummy =
		{"staaqp", parse_alf_args, NULL, ALOPF10, MAS, 0x20, {0, 0, 1, 0, 0, 1}, ARGS_P};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpand", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpandn", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpor", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpxor", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x48, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x49, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4a, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4b, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4c, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4d, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4e, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpaddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4f, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x50, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x51, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x52, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x53, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x54, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x55, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x56, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x57, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x70, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfaddd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x71, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfhadds", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7c, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfhsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7d, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfaddsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7e, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfaddsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7f, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfstoifs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfdtoifd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmins", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmind", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmaxs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmaxd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmuls", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x78, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfmuld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x79, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfsubs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x72, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfsubd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x73, {1, 1, 1, 1, 1, 1}, ARGS_PPP, NONE, {EXT1, EXT1, EXT1, EXT1, EXT1, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmsk2sgnb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_PSP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qppackdl", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsllh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsllw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpslld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrlh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrlw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrld", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrah", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsraw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpacksshb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x48, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpacksswh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4a, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpackushb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x49, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpackuswh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x4b, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpavgusb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5e, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpavgush", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5f, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpeqb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x58, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpeqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x18, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpeqh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x59, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpeqw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5a, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpgtb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5b, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpgtd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1c, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpgth", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5c, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpcmpgtw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5d, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphaddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x19, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphaddsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1b, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphaddw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1a, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphsubh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1d, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphsubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1f, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphsubw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x1e, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x46, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x43, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxsw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xf, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxub", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x42, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x47, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaxuw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xe, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminsb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x44, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x41, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminsw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xd, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminub", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x40, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x45, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpminuw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0xc, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmulhh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmulhrsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmulhuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5b, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmullh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x59, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmulubhh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5d, {0, 1, 0, 0, 1, 0}, ARGS_DPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsignb", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x38, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsignh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x39, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsignw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x3a, {1, 0, 0, 1, 0, 0}, ARGS_PPP, NONE, {EXT1, 0, 0, EXT1, 0, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qphminposuh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_PPD, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaddh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5a, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmaddubsh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmpsadbh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_PSP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsadbw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrcd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpsrcw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_PDP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrcd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"psrcw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"getfzs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x74, {1, 1, 1, 1, 1, 1}, ARGS_SSS, NONE, {EXT, EXT, EXT, EXT, EXT, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"getfzd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x75, {1, 1, 1, 1, 1, 1}, ARGS_DDD, NONE, {EXT, EXT, EXT, EXT, EXT, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"puttagqp", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_PSP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmullw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x26, {0, 1, 0, 0, 1, 0}, ARGS_DDD, NONE, {0, EXT, 0, 0, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmullw", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_PPP, NONE, {0, EXT1, 0, 0, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpeqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc0, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmplts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc1, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmples", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc2, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpuods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc3, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpneqs", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc4, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpnlts", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc5, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpnles", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc6, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpods", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc7, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpeqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc0, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc1, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc2, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpuodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc3, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpneqd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc4, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpnltd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc5, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpnled", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc6, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpfcmpodd", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_PPP, 0xc7, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldq", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x41, {1, 0, 1, 1, 0, 1}, ARGS_DDQ, NONE, {EXT, 0, EXT, EXT, 0, EXT}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x61, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgdqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x69, {1, 0, 1, 1, 0, 1}, ARGS_SSP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcudqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x68, {1, 0, 1, 1, 0, 1}, ARGS_SSP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldcsqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x62, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"lddsqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x63, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldesqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x64, {1, 0, 1, 1, 0, 1}, ARGS_SSP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldfsqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x65, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldgsqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x66, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldssqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x67, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldapqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6a, {1, 0, 1, 1, 0, 1}, ARGS_QSP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf11_opcode_templ dummy =
		{"ldrqp", parse_alf_args, merge_alopf11, ALOPF11, MAS, 0x6b, {1, 0, 1, 1, 0, 1}, ARGS_DDP, NONE, {EXT1, 0, EXT1, EXT1, 0, EXT1}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpsgn2mskb", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_PS, NONE, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpswitchw", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_PP, NONE, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpswitchd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_PP, NONE, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfstois", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xc8, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfstoistr", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xca, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpistofs", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xcc, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfstoid", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DP, 0xd8, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfstoidtr", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DP, 0xda, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpistofd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DP, 0xdc, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfstofd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DP, 0xde, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfdtois", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PD, 0xe8, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfdtoistr", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PD, 0xea, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpidtofs", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PD, 0xec, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfdtofs", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PD, 0xee, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfdtoid", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xf8, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpfdtoidtr", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xfa, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpidtofd", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PP, 0xfc, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stq", parse_alf_args, NULL, ALOPF13, MAS, 0x1, {0, 0, 1, 0, 0, 1}, ARGS_DDQ, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x19, {0, 0, 1, 0, 0, 1}, ARGS_SSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgdqp", parse_alf_args, NULL, ALOPF13, MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_SSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapqp", parse_alf_args, NULL, ALOPF13, MAS, 0x2a, {0, 0, 1, 0, 0, 1}, ARGS_QSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stapmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x1a, {0, 0, 1, 0, 0, 1}, ARGS_QSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x11, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stqp", parse_alf_args, NULL, ALOPF13, MAS, 0x21, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stcsmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x12, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stcsqp", parse_alf_args, NULL, ALOPF13, MAS, 0x22, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stdsmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x13, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stdsqp", parse_alf_args, NULL, ALOPF13, MAS, 0x23, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stesmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x14, {0, 0, 1, 0, 0, 1}, ARGS_SSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stesqp", parse_alf_args, NULL, ALOPF13, MAS, 0x24, {0, 0, 1, 0, 0, 1}, ARGS_SSP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stfsmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x15, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stfsqp", parse_alf_args, NULL, ALOPF13, MAS, 0x25, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgsmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x16, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stgsqp", parse_alf_args, NULL, ALOPF13, MAS, 0x26, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stssmqp", parse_alf_args, NULL, ALOPF13, MAS, 0x17, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"stssqp", parse_alf_args, NULL, ALOPF13, MAS, 0x27, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf13_opcode_templ dummy =
		{"strqp", parse_alf_args, NULL, ALOPF13, MAS, 0x2b, {0, 0, 1, 0, 0, 1}, ARGS_DDP, NONE, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"addcd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x60, {0, 1, 0, 0, 1, 0}, ARGS_DDSD, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"addcd_c", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x61, {0, 1, 0, 0, 1, 0}, ARGS_DDSD, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"subcd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x62, {0, 1, 0, 0, 1, 0}, ARGS_DDSD, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"subcd_c", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_DDSD, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf1_opcode_templ dummy =
		{"vfbgv", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_SSS};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf1_opcode_templ dummy =
		{"mkfsw", parse_alf_args, merge_alopf_simple, ALOPF1, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_SSD};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf2_opcode_templ dummy =
		{"modbgv", parse_alf_args, merge_alopf_simple, ALOPF2, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_SS, 0xc0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqbop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc0, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqhop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc1, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqwop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqdop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc3, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtbop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgthop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtwop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc6, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtdop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqbap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc8, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqhap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xc9, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqwap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xca, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpeqdap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xcb, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtbap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xcc, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgthap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xcd, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtwap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xce, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"pcmpgtdap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x28, {1, 0, 0, 1, 0, 0}, ARGS_DD, 0xcf, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqbop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc0, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqhop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc1, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqwop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc2, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqdop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc3, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtbop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc4, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgthop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc5, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtwop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc6, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtdop", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc7, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqbap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc8, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqhap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xc9, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqwap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xca, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpeqdap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xcb, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtbap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xcc, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgthap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xcd, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtwap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xce, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alf7_opcode_templ dummy =
		{"qpcmpgtdap", parse_alf_args, NULL, ALOPF17, NO_MAS, 0x2c, {1, 0, 0, 1, 0, 0}, ARGS_PP, 0xcf, 2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf11_opcode_templ dummy =
		{"pmrgp", parse_alf_args, merge_alopf11, ALOPF11_MERGE, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT, EXT, 0, EXT, EXT, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf11_opcode_templ dummy =
		{"qpmrgp", parse_alf_args, merge_alopf11, ALOPF11_MERGE, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_PPP, NONE, {EXT1, EXT1, 0, EXT1, EXT1, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf11_opcode_templ dummy =
		{"clmulh", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT2, EXT2, 0, EXT2, EXT2, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf11_opcode_templ dummy =
		{"clmull", parse_alf_args, merge_alopf11, ALOPF11, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDD, NONE, {EXT2, EXT2, 0, EXT2, EXT2, 0}, 0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"ibranchd", parse_alf_args, NULL, ALOPF12_IBRANCHD, NO_MAS, 0x53, {1, 0, 0, 0, 0, 0}, ARGS_DD, NONE, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"icalld", parse_alf_args, NULL, ALOPF12_ICALLD, NO_MAS, 0x54, {1, 0, 0, 0, 0, 0}, ARGS_DD, NONE, NONE, EXT};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpcext_0x00", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DP, NONE, 0xc0, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpcext_0x7f", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DP, NONE, 0xc2, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpcext_0x80", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DP, NONE, 0xc4, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf12_opcode_templ dummy =
		{"qpcext_0xff", parse_alf_args, NULL, ALOPF12, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DP, NONE, 0xc6, EXT1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmas", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x0, {1, 1, 1, 1, 1, 1}, ARGS_SSSS, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmss", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x2, {1, 1, 1, 1, 1, 1}, ARGS_SSSS, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fnmas", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x4, {1, 1, 1, 1, 1, 1}, ARGS_SSSS, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fnmss", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x6, {1, 1, 1, 1, 1, 1}, ARGS_SSSS, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmad", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x1, {1, 1, 1, 1, 1, 1}, ARGS_DDDD, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmsd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x3, {1, 1, 1, 1, 1, 1}, ARGS_DDDD, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fnmad", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x5, {1, 1, 1, 1, 1, 1}, ARGS_DDDD, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fnmsd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x7, {1, 1, 1, 1, 1, 1}, ARGS_DDDD, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmas", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x8, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmss", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xa, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfnmas", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xc, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfnmss", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xe, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmad", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x9, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmsd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xb, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfnmad", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xd, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfnmsd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0xf, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmass", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x10, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmsas", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x12, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmasd", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x11, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 6)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmsad", parse_alf_args, NULL, ALOPF21, NO_MAS, 0x13, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, EXT2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_ands", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xe, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_andd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xf, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_ands", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_andd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_andns", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_andnd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_andns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_andnd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_ors", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_ord", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_ors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_ord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_orns", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_ornd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_orns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_ornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x60, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x61, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x62, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x64, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x65, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x66, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x67, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x70, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x71, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x72, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x73, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x74, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x75, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x76, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x77, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x78, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x79, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_merges", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_merged", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x7f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_xors", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xe, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_xord", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xf, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_xors", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_xord", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_xorns", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_xornd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_xorns", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_xornd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_scls", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_scld", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_scls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_scld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_scrs", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_scrd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_scrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_scrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_adds", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xe, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_addd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xf, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_subs", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_subd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB2};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_shls", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xe, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_shld", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0xf, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_shls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_shld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_shrs", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_shrd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x2f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_shrs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_shrd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_sars", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_sard", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x4f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_sars", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_sard", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"and_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"andn_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"or_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"orn_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xor_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"xorn_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_getfs", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"merge_getfd", parse_alf_args, merge_alopf_simple, ALOPF21_MERGE, NO_MAS, 0x6f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"add_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sub_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scl_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"scr_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shl_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"shr_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"sar_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_getfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {0, 1, 0, 0, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"getf_getfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {0, 1, 0, 0, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"insfs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"insfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, ICMB3};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_muls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_muld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_muls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_muld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fadd_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_SSSS, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"fmul_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, FCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_muls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_muld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_muls", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu == 1)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_muld", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pshufb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 2)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pmerge", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 3)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfhsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfmul_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 4)
{
	static e2k_alopf21_opcode_templ dummy =
		{"pfaddsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {0, 0, 1, 0, 0, 1}, ARGS_DDDD, PFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_adds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_addd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_subs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_subd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_hadds", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_hsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qppermb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_rsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_rsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpshufb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfadd_addsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfsub_addsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhadd_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfhsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfmul_addsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpmerge", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_addsubs", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qpfaddsub_addsubd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 1, 1, 1, 1}, ARGS_PPPP, QPFCMB1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_10", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_11", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_12", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_13", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_14", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_15", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_16", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_17", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_18", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_19", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_20", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_21", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_22", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_23", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_24", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_25", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_26", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_27", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_28", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_29", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_30", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_31", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_32", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_33", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_34", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_35", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_36", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_37", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_38", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_39", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_40", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_41", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_42", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_43", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_44", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_45", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_46", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_47", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_48", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_49", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_50", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_51", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_52", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_53", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_54", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_55", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_56", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_57", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_58", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_59", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_60", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_61", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_62", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_63", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_64", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_65", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_66", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_67", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_68", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_69", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_70", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_71", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_72", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_73", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_74", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_75", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_76", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_77", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_78", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_79", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_80", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_81", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_82", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_83", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_84", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_85", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_86", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_87", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_88", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_89", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_90", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_91", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_92", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_93", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_94", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_95", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_96", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_97", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_98", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_99", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_100", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_101", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_102", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_103", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_104", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_105", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_106", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_107", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_108", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_109", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_110", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_111", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_112", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_113", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_114", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_115", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_116", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_117", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_118", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_119", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_120", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_121", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_122", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_123", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_124", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_125", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_126", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_127", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x00", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x01", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x02", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x03", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x04", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x05", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x06", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x07", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x08", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x09", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x0f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x10", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x11", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x12", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x13", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x14", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x15", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x16", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x17", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x18", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x19", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x1f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x20", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x21", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x22", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x23", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x24", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x25", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x26", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x27", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x28", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x29", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x2f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x30", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x31", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x32", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x33", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x34", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x35", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x36", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x37", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x38", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x39", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x3f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x40", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x41", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x42", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x43", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x44", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x45", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x46", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x47", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x48", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x49", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x4f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x50", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x51", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x52", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x53", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x54", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x55", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x56", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x57", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x58", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x59", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x5f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x60", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x61", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x62", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x63", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x64", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x65", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x66", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x67", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x68", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x69", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x6f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x70", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x71", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x72", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x73", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x74", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x75", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x76", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x77", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x78", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x79", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x7f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_128", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_129", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_130", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_131", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_132", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_133", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_134", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_135", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_136", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_137", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_138", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_139", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_140", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_141", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_142", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_143", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_144", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_145", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_146", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_147", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_148", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_149", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_150", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_151", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_152", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_153", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_154", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_155", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_156", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_157", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_158", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_159", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_160", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_161", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_162", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_163", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_164", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_165", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_166", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_167", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_168", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_169", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_170", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_171", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_172", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_173", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_174", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_175", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_176", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_177", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_178", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_179", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_180", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_181", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_182", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_183", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_184", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_185", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_186", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_187", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_188", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_189", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_190", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_191", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_192", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_193", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_194", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_195", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_196", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_197", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_198", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_199", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_200", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_201", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_202", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_203", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_204", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_205", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_206", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_207", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_208", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_209", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_210", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_211", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_212", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_213", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_214", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_215", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_216", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_217", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_218", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_219", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_220", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_221", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_222", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_223", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_224", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_225", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_226", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_227", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_228", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_229", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_230", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_231", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_232", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_233", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_234", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_235", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_236", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_237", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_238", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_239", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_240", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_241", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_242", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_243", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_244", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_245", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_246", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_247", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_248", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_249", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_250", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_251", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_252", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_253", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_254", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_255", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x80", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x81", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x82", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x83", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x84", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x85", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x86", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x87", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x88", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x89", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x8f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x90", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x91", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x92", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x93", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x94", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x95", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x96", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x97", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x98", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x99", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0x9f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xa9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xaa", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xab", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xac", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xad", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xae", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xaf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xb9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xba", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xbb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xbc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xbd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xbe", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xbf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xc9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xca", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xcb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xcc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xcd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xce", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xcf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xd9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xda", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xdb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xdc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xdd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xde", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xdf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xe9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xea", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xeb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xec", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xed", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xee", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xef", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xf9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xfa", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xfb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xfc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xfe", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_0xff", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_and", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_xor", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_sel3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_mjr", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"plog_or", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBD1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_10", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_11", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_12", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_13", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_14", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_15", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_16", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_17", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_18", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_19", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_20", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_21", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_22", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_23", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_24", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_25", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_26", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_27", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_28", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_29", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_30", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_31", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_32", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_33", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_34", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_35", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_36", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_37", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_38", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_39", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_40", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_41", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_42", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_43", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_44", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_45", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_46", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_47", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_48", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_49", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_50", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_51", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_52", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_53", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_54", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_55", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_56", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_57", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_58", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_59", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_60", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_61", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_62", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_63", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_64", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_65", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_66", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_67", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_68", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_69", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_70", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_71", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_72", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_73", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_74", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_75", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_76", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_77", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_78", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_79", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_80", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_81", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_82", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_83", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_84", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_85", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_86", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_87", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_88", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_89", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_90", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_91", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_92", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_93", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_94", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_95", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_96", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_97", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_98", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_99", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_100", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_101", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_102", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_103", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_104", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_105", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_106", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_107", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_108", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_109", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_110", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_111", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_112", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_113", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_114", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_115", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_116", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_117", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_118", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_119", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_120", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_121", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_122", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_123", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_124", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_125", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_126", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_127", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x00", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x01", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x02", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x03", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x04", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x05", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x06", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x07", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x08", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x09", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x0f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x10", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x11", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x12", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x13", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x14", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x15", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x16", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x17", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x18", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x19", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x1f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x20", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x21", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x22", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x23", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x24", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x25", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x26", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x27", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x28", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x29", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x2f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x30", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x31", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x32", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x33", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x34", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x35", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x36", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x37", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x38", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x39", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x3f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x40", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x41", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x42", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x43", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x44", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x45", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x46", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x47", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x48", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x49", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x4f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x50", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x51", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x52", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x53", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x54", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x55", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x56", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x57", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x58", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x59", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x5f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x60", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x61", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x62", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x63", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x64", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x65", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x66", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x67", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x68", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x69", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x6f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x70", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x71", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x72", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x73", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x74", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x75", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x76", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x77", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x78", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x79", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x7f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ0};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_128", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_129", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_130", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_131", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_132", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_133", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_134", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_135", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_136", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_137", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_138", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_139", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_140", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_141", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_142", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_143", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_144", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_145", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_146", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_147", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_148", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_149", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_150", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_151", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_152", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_153", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_154", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_155", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_156", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_157", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_158", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_159", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_160", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_161", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_162", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_163", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_164", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_165", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_166", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_167", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_168", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_169", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_170", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_171", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_172", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_173", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_174", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_175", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_176", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_177", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_178", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_179", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_180", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_181", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_182", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_183", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_184", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_185", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_186", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_187", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_188", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_189", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_190", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_191", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_192", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_193", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_194", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_195", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_196", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_197", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_198", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_199", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_200", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_201", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_202", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_203", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_204", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_205", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_206", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_207", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_208", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_209", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_210", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_211", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_212", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_213", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_214", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_215", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_216", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_217", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_218", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_219", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_220", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_221", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_222", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_223", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_224", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_225", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_226", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_227", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_228", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_229", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_230", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_231", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_232", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_233", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_234", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_235", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_236", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_237", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_238", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_239", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_240", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_241", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_242", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_243", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_244", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_245", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_246", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_247", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_248", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_249", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_250", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_251", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_252", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_253", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_254", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_255", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x80", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x81", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x82", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x83", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x84", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x85", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x86", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x87", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x88", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x8, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x89", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x9, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xa, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xb, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xc, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xd, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xe, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x8f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0xf, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x90", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x10, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x91", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x11, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x92", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x12, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x93", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x13, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x94", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x14, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x95", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x15, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x96", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x97", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x17, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x98", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x18, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x99", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x19, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9a", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9b", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9c", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9d", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9e", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0x9f", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x1f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x20, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x21, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x22, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x23, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x24, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x25, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x26, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x27, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x28, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xa9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x29, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xaa", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xab", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xac", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xad", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xae", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xaf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x2f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x30, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x31, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x32, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x33, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x34, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x35, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x36, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x37, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x38, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xb9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x39, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xba", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xbb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xbc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xbd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xbe", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xbf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x3f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x40, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x41, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x42, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x43, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x44, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x45, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x46, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x47, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x48, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xc9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x49, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xca", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xcb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xcc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xcd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xce", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xcf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x4f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x50, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x51, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x52, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x53, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x54, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x55, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x56, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x57, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xd9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x59, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xda", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xdb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xdc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xdd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xde", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xdf", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x5f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x60, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x61, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x62, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x63, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x64, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x65, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x66, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x67, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xe9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x69, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xea", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xeb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xec", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xed", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xee", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xef", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x6f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf0", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x70, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf1", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x71, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf2", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x72, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x73, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf4", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x74, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf5", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x75, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf6", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x76, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf7", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x77, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf8", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x78, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xf9", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x79, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xfa", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7a, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xfb", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7b, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xfc", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7c, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xfd", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7d, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xfe", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_0xff", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7f, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_and", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x0, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_xor", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x16, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_sel3", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x58, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_mjr", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x68, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
if (mcpu >= 5)
{
	static e2k_alopf21_opcode_templ dummy =
		{"qplog_or", parse_alf_args, merge_alopf_simple, ALOPF21, NO_MAS, 0x7e, {1, 1, 0, 1, 1, 0}, ARGS_DDDD, LCMBQ1};
	add_to_insn_table ((e2k_opcode_templ *) &dummy);
}
