#if !defined(CIRCLEQ_INCLUDE)
#define CIRCLEQ_INCLUDE
typedef struct _circq
{
	char * 	Head;
	char * 	Tail;
	char * 	Buffer;
	int	Size;
	int	Flag;
} CircleQ;

CircleQ * 	Qalloc(int size);
void		Qfree(CircleQ * qptr);
void		QinsertChar(CircleQ * qptr, char ch);
char		QpeekChar(CircleQ * qptr);
char 		QreadChar(CircleQ * qptr);
#endif

