Hshtab*		addctl(Hshtab *, char);
void		assgn(Hshtab *, Value, int);
Value		btog(Value);
Node*		build(int, Node *, Node *);
void		cdl(void);
void		checkdefns(void);
void		checkfields(void);
void		checkio(Hshtab *, int);
void		clearcache(void);
void		clearmem(void);
Hshtab*		comname(Hshtab *);
Minterm *	compile(int);
void		compsw(Node *, int);
void		dclbit(Hshtab *, Value);
Value		eval(Node *);
int		eval1(void);
int 		finddepend(Hshtab *);
Value		flone(Value);
Value		frone(Value);
void		labels(void);
Value		loadf(Hshtab *);
Hshtab*		lookup(char *, int);
int		makdigit(int, int);
void		out(Node *);
void		plaout(Node *);
void		poline(int);
void		printmt(Minterm *, Minterm *);
int		ql(int);
void		romout(Node *);
int		scanseq(int, int, int);
void		setctl(Hshtab *, char);
void		setdcused(Hshtab *, Node *);
int		setinput(void);
void		setiused(Hshtab *, int);
void		setoused(Hshtab *, Node *);
void		storef(Hshtab *, Value, int);
int		subseq(int, int, int);
Value		take(Value, Value);
void		treedep(Node *);
void		treeprint(Node *);
Minterm*	tree_walk(int, unsigned int, int, Node*, Node*, Minterm*, void *, void *);
int		vconst(Node *);
Value		vexec(Value, int);
void		yyerror(char *, ...);
int		yylex(void);
int		yyparse(void);
void		yyundef(Hshtab *);
void		yywarn(char *, ...);