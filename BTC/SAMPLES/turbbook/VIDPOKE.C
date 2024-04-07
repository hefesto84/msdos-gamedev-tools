/* --------- vidpoke.c ---------- */

#define VSEG 0xb800

char vdata [] = "What hath Kahn wrought?";

main()
{
	char *vp;
	int v;

	for (v = 0, vp = vdata; *vp; v += 2, vp++)
		poke(VSEG, v, 0x700 | *vp);
}
