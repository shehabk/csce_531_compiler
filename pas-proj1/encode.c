
void back_end_call(char *id, int alignment, unsigned int size)
{
	b_global_decl (id, alignment , size);
	b_skip(size);
}
