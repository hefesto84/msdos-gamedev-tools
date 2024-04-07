struct point {
	int h, v;
};

struct rect {
		int left, top, right, bot;
};

struct bitmap {
	struct rect bounds;
	int bytes;
	void far *pntr;
};

extern void pset_verb(), preset_verb(), and_verb(), and_not_verb(),
or_verb(), or_not_verb(), xor_verb();


void blit(struct bitmap *src, struct bitmap *dest, void (*style)());
