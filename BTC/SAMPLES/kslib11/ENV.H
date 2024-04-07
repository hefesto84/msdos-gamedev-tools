typedef struct {
	char	miscl[22];
	uint	par_seg;
	char	misc2[20];
	uint	env_seg;
} PSP;

typedef struct {
	char	status;						/* Block is in chain or not			*/
	uint	owner_psp;					/* PSP segment of block's owner		*/
	uint	size;						/* Size (in paragraphs) of block	*/
} MCB;

void	fndenvp(void);
int		setenvp(char *,char *);
int		delenvp(char *);
