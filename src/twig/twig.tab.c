/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 2 "twig.y"

#include "common.h"
#include "code.h"
#include "sym.h"
#include "machine.h"
#include <stdarg.h>

#define UNDEFINED -1
#define GIVENUP	-2

int	yyparse(void);

int debug_flag = 0;
int Dflag = 0;
int tflag = 0;
int sflag = 0;
int line_xref_flag = 0;
int ntrees = 0;
int nerrors = 0;
int fatalerrors = 0;
int tree_lineno;
FILE *outfile;
FILE *symfile;
Code *epilogue;

SymbolEntry ErrorSymbol;


/* Line 268 of yacc.c  */
#line 100 "twig.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ERROR = 258,
     K_NODE = 259,
     K_LABEL = 260,
     K_PROLOGUE = 261,
     K_CONST = 262,
     K_INSERT = 263,
     K_COST = 264,
     K_ACTION = 265,
     ID = 266,
     NUMBER = 267,
     CBLOCK = 268
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 29 "twig.y"

	Node *y_nodep;
	SymbolEntry *y_symp;
	Code *y_code;
	int y_int;



/* Line 293 of yacc.c  */
#line 158 "twig.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 170 "twig.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   104

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNRULES -- Number of states.  */
#define YYNSTATES  98

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      16,    17,    18,     2,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,    14,
       2,    15,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    11,    15,    19,    23,    27,
      31,    36,    41,    45,    49,    53,    56,    58,    61,    64,
      66,    69,    72,    74,    77,    80,    85,    89,    93,    94,
      97,    99,   102,   106,   108,   110,   113,   116,   118,   120,
     124,   128,   135,   138,   141,   144,   145,   147,   149,   150,
     152,   153,   159,   161,   165,   167,   169
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      22,     0,    -1,    23,    34,    -1,    23,    24,    -1,    24,
      -1,     4,    27,    14,    -1,     4,    26,    14,    -1,     7,
      31,    14,    -1,     5,    25,    14,    -1,     6,    13,    14,
      -1,     9,    11,    13,    14,    -1,    10,    11,    13,    14,
      -1,     6,     1,    14,    -1,    10,     1,    14,    -1,     9,
       1,    14,    -1,    25,    11,    -1,    11,    -1,    25,     1,
      -1,    26,    29,    -1,    29,    -1,    26,     1,    -1,    27,
      28,    -1,    28,    -1,    27,     1,    -1,    11,    30,    -1,
      11,    30,    15,    33,    -1,    16,    33,    17,    -1,    16,
      18,    17,    -1,    -1,    31,    32,    -1,    32,    -1,    31,
       1,    -1,    11,    15,    33,    -1,    12,    -1,    11,    -1,
      34,    36,    -1,    34,    35,    -1,    35,    -1,    36,    -1,
       8,    13,    14,    -1,     8,     1,    14,    -1,    39,    19,
      40,    38,    37,    14,    -1,     1,    14,    -1,    15,    13,
      -1,    15,    11,    -1,    -1,    13,    -1,    11,    -1,    -1,
      11,    -1,    -1,    11,    41,    16,    42,    17,    -1,    11,
      -1,    42,    20,    40,    -1,    40,    -1,     1,    -1,    42,
       1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    48,    48,    51,    52,    54,    56,    62,    64,    66,
      68,    72,    76,    77,    78,    89,    95,    96,   102,   108,
     109,   112,   116,   117,   119,   127,   135,   136,   137,   140,
     144,   145,   148,   155,   156,   164,   165,   166,   167,   170,
     171,   175,   187,   189,   192,   198,   200,   204,   210,   213,
     224,   224,   238,   249,   257,   258,   259
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ERROR", "K_NODE", "K_LABEL",
  "K_PROLOGUE", "K_CONST", "K_INSERT", "K_COST", "K_ACTION", "ID",
  "NUMBER", "CBLOCK", "';'", "'='", "'('", "')'", "'*'", "':'", "','",
  "$accept", "pattern_spec", "declarations", "decl", "id_list",
  "assoc_list2", "assoc_list", "assoc", "assoc2", "arity_spec",
  "const_list", "const_def", "constant", "patternsOrInserts", "insert",
  "pattern", "action", "cost", "label", "tree", "$@1", "tree_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    59,    61,    40,    41,    42,    58,
      44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    21,    22,    23,    23,    24,    24,    24,    24,    24,
      24,    24,    24,    24,    24,    25,    25,    25,    26,    26,
      26,    27,    27,    27,    28,    29,    30,    30,    30,    31,
      31,    31,    32,    33,    33,    34,    34,    34,    34,    35,
      35,    36,    36,    37,    37,    37,    38,    38,    38,    39,
      41,    40,    40,    42,    42,    42,    42
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     3,     3,     3,     3,     3,
       4,     4,     3,     3,     3,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     4,     3,     3,     0,     2,
       1,     2,     3,     1,     1,     2,     2,     1,     1,     3,
       3,     6,     2,     2,     2,     0,     1,     1,     0,     1,
       0,     5,     1,     3,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
      28,     0,     0,    22,    19,    16,     0,     0,     0,     0,
       0,    30,     0,     0,     0,     0,     1,     0,     0,    49,
       3,     0,    37,    38,     0,     0,    24,    20,    28,     6,
      18,    23,    28,     5,    21,    17,    15,     8,    12,     9,
       0,    31,     7,    29,    14,     0,    13,     0,    42,     0,
       0,    36,    35,     0,    34,    33,     0,     0,     0,     0,
      24,    32,    10,    11,    40,    39,    52,    48,    27,    26,
      25,     0,    47,    46,    45,     0,     0,     0,    55,    54,
       0,    44,    43,    41,    56,    51,     0,    53
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,     8,     9,    16,    11,    12,    13,    14,    36,
      20,    21,    67,    31,    32,    33,    87,    84,    34,    77,
      81,    90
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const yytype_int8 yypact[] =
{
      48,    -4,    26,     8,    45,    23,    39,    59,    38,   -56,
      44,     0,     4,   -56,   -56,   -56,    11,    52,    53,    49,
      12,   -56,    54,    56,    57,    60,   -56,    58,    16,   -56,
     -56,    27,   -56,   -56,    46,    -8,    55,   -56,    44,   -56,
     -56,   -56,    44,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
      51,   -56,   -56,   -56,   -56,    61,   -56,    62,   -56,    63,
      64,   -56,   -56,    68,   -56,   -56,    65,    66,    51,    55,
     -56,   -56,   -56,   -56,   -56,   -56,    69,    -5,   -56,   -56,
     -56,    70,   -56,   -56,    72,    50,    20,    67,   -56,   -56,
      -1,   -56,   -56,   -56,   -56,   -56,    68,   -56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -56,    76,   -56,   -56,   -56,    77,    79,    -6,
     -56,    71,   -48,   -56,    43,    73,   -56,   -56,   -56,   -55,
     -56,   -56
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -51
static const yytype_int8 yytable[] =
{
      94,    37,    71,    64,    65,    41,    82,    10,    83,    17,
      66,    38,    45,    51,    39,    42,    95,    59,    43,    96,
      80,    18,    46,    19,    22,    47,    52,    -2,    27,    60,
      89,    91,    69,    92,    23,    28,    70,    15,    29,    27,
      24,    97,     1,     2,     3,     4,    28,     5,     6,    29,
      25,    88,     1,     2,     3,     4,    19,     5,     6,    26,
      35,    76,    64,    65,    50,    63,    48,    49,    54,    55,
      68,    56,    58,    57,    61,    72,    73,    74,    75,    76,
       0,    93,    78,    79,    30,   -50,    85,    86,     0,    44,
      40,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-56))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       1,     1,    50,    11,    12,     1,    11,    11,    13,     1,
      18,    11,     1,     1,    14,    11,    17,     1,    14,    20,
      68,    13,    11,    11,     1,    14,    14,     0,     1,    13,
      85,    11,    38,    13,    11,     8,    42,    11,    11,     1,
       1,    96,     4,     5,     6,     7,     8,     9,    10,    11,
      11,     1,     4,     5,     6,     7,    11,     9,    10,     0,
      16,    11,    11,    12,    15,    19,    14,    14,    14,    13,
      15,    14,    14,    13,    31,    14,    14,    14,    14,    11,
      -1,    14,    17,    17,     8,    16,    16,    15,    -1,    12,
      11,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     6,     7,     9,    10,    22,    23,    24,
      11,    26,    27,    28,    29,    11,    25,     1,    13,    11,
      31,    32,     1,    11,     1,    11,     0,     1,     8,    11,
      24,    34,    35,    36,    39,    16,    30,     1,    11,    14,
      29,     1,    11,    14,    28,     1,    11,    14,    14,    14,
      15,     1,    14,    32,    14,    13,    14,    13,    14,     1,
      13,    35,    36,    19,    11,    12,    18,    33,    15,    30,
      30,    33,    14,    14,    14,    14,    11,    40,    17,    17,
      33,    41,    11,    13,    38,    16,    15,    37,     1,    40,
      42,    11,    13,    14,     1,    17,    20,    40
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 49 "twig.y"
    { if (nerrors==0) machine_build(); }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 54 "twig.y"
    { SymbolEnterList ((yyvsp[(2) - (3)].y_symp), A_NODE); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 57 "twig.y"
    {
			SymbolEnterList((yyvsp[(2) - (3)].y_symp), A_NODE);
			SymbolCheckNodeValues();
		}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 62 "twig.y"
    { SymbolEnterList ((yyvsp[(2) - (3)].y_symp), A_CONST); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 64 "twig.y"
    { SymbolEnterList ((yyvsp[(2) - (3)].y_symp), A_LABEL); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 66 "twig.y"
    { CodeWrite(outfile, (yyvsp[(2) - (3)].y_code)); CodeFreeBlock((yyvsp[(2) - (3)].y_code)); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 69 "twig.y"
    { (yyvsp[(2) - (4)].y_symp)->sd.ca.code = (yyvsp[(3) - (4)].y_code); (yyvsp[(2) - (4)].y_symp)->sd.ca.assoc = NULL;
		 SymbolEnter ((yyvsp[(2) - (4)].y_symp), A_COST); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 73 "twig.y"
    { (yyvsp[(2) - (4)].y_symp)->sd.ca.code = (yyvsp[(3) - (4)].y_code); (yyvsp[(2) - (4)].y_symp)->sd.ca.assoc = NULL;
		 SymbolEnter ((yyvsp[(2) - (4)].y_symp), A_ACTION); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 89 "twig.y"
    {
				if(CheckIsUndefined((yyvsp[(2) - (2)].y_symp))) {
					(yyvsp[(2) - (2)].y_symp)->next = (yyvsp[(1) - (2)].y_symp);
					(yyval.y_symp) = (yyvsp[(2) - (2)].y_symp);
				} else (yyval.y_symp) = (yyvsp[(1) - (2)].y_symp);
			}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 95 "twig.y"
    { if(CheckIsUndefined((yyvsp[(1) - (1)].y_symp))) (yyval.y_symp) = (yyvsp[(1) - (1)].y_symp); else (yyval.y_symp) = NULL; }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 103 "twig.y"
    {
			if((yyvsp[(2) - (2)].y_symp)->attr==A_ERROR)
				(yyval.y_symp) = (yyvsp[(1) - (2)].y_symp);
			else { (yyvsp[(2) - (2)].y_symp)->next = (yyvsp[(1) - (2)].y_symp); (yyval.y_symp) = (yyvsp[(2) - (2)].y_symp); }
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 108 "twig.y"
    { (yyval.y_symp) = (yyvsp[(1) - (1)].y_symp)->attr==A_ERROR ? NULL : (yyvsp[(1) - (1)].y_symp); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 112 "twig.y"
    { 
					if((yyvsp[(2) - (2)].y_symp)->attr==A_ERROR) (yyval.y_symp) = (yyvsp[(1) - (2)].y_symp);
			        	else { (yyvsp[(2) - (2)].y_symp)->next = (yyvsp[(1) - (2)].y_symp); (yyval.y_symp) = (yyvsp[(2) - (2)].y_symp); }
				}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 116 "twig.y"
    { (yyval.y_symp) = (yyvsp[(1) - (1)].y_symp)->attr==A_ERROR ? NULL : (yyvsp[(1) - (1)].y_symp); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 120 "twig.y"
    {
			if (CheckIsUndefined((yyvsp[(1) - (2)].y_symp))) {
				(yyvsp[(1) - (2)].y_symp)->sd.arity = (yyvsp[(2) - (2)].y_int); (yyval.y_symp) = (yyvsp[(1) - (2)].y_symp);
			} else (yyval.y_symp) = &ErrorSymbol;
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 127 "twig.y"
    {
				if(CheckIsUndefined((yyvsp[(1) - (4)].y_symp))) {
					(yyvsp[(1) - (4)].y_symp)->unique = (yyvsp[(4) - (4)].y_int); (yyvsp[(1) - (4)].y_symp)->sd.arity = (yyvsp[(2) - (4)].y_int);
					(yyval.y_symp) = (yyvsp[(1) - (4)].y_symp);
				} else (yyval.y_symp) = &ErrorSymbol;
			}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 135 "twig.y"
    { (yyval.y_int) = (yyvsp[(2) - (3)].y_int); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 136 "twig.y"
    { (yyval.y_int)=GIVENUP; }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 137 "twig.y"
    { (yyval.y_int) = UNDEFINED; }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 140 "twig.y"
    {
				if ((yyvsp[(2) - (2)].y_symp)->attr==A_ERROR) (yyval.y_symp) = (yyvsp[(1) - (2)].y_symp);
				else { (yyvsp[(2) - (2)].y_symp)->next = (yyvsp[(1) - (2)].y_symp); (yyval.y_symp) = (yyvsp[(2) - (2)].y_symp); }
			}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 144 "twig.y"
    { (yyval.y_symp) = (yyvsp[(1) - (1)].y_symp)->attr==A_ERROR ? NULL : (yyvsp[(1) - (1)].y_symp); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 148 "twig.y"
    {
				if(CheckIsUndefined((yyvsp[(1) - (3)].y_symp))) {
					(yyvsp[(1) - (3)].y_symp)->sd.cvalue = (yyvsp[(3) - (3)].y_int); (yyval.y_symp) = (yyvsp[(1) - (3)].y_symp);
				} else (yyval.y_symp) = &ErrorSymbol;
		}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 156 "twig.y"
    {
		if(!CheckIsDefined((yyvsp[(1) - (1)].y_symp))) (yyval.y_int) = UNDEFINED;
		else if((yyvsp[(1) - (1)].y_symp)->attr!=A_CONST) {
			sem_error("non-constant id used");
			(yyval.y_int) = -1;
		} else (yyval.y_int) = (yyvsp[(1) - (1)].y_symp)->sd.cvalue;
	}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 170 "twig.y"
    { epilogue = CodeAppend(epilogue, (yyvsp[(2) - (3)].y_code)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 175 "twig.y"
    {
		    if ((yyvsp[(1) - (6)].y_symp)->attr==A_ERROR) {
			error(0, "\"label: tree\" pair ignored");
			TreeFree((yyvsp[(3) - (6)].y_nodep));
		    } else {
			if(nerrors==0)
				cgotofn(SymbolEnterTreeIntoLabel((yyvsp[(1) - (6)].y_symp),
					(yyvsp[(3) - (6)].y_nodep), (yyvsp[(4) - (6)].y_symp), (yyvsp[(5) - (6)].y_symp), tree_lineno));
			if(debug_flag&DB_TREE)
				TreePrint((yyvsp[(3) - (6)].y_nodep), 1);
		    }
		}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 189 "twig.y"
    { SymbolEntry *sp = SymbolAllocate (SymbolGenUnique());
		sp->sd.ca.code = (yyvsp[(2) - (2)].y_code); sp->sd.ca.assoc = NULL;
		SymbolEnter(sp, A_ACTION); (yyval.y_symp) = sp; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 192 "twig.y"
    { if(CheckIsDefined((yyvsp[(2) - (2)].y_symp))) {
			if ((yyvsp[(2) - (2)].y_symp)->attr!=A_ACTION) {
				sem_error ("non action id: %s", (yyvsp[(2) - (2)].y_symp)->name);
				(yyval.y_symp) = &ErrorSymbol;
			} else (yyval.y_symp) = (yyvsp[(2) - (2)].y_symp);
		} else (yyval.y_symp) = &ErrorSymbol; }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 198 "twig.y"
    { (yyval.y_symp) = NULL;}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 200 "twig.y"
    { SymbolEntry *sp = SymbolAllocate (SymbolGenUnique());
		sp->sd.ca.code = (yyvsp[(1) - (1)].y_code); sp->sd.ca.assoc = NULL;
		SymbolEnter (sp, A_COST); (yyval.y_symp) = sp;
		}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 204 "twig.y"
    { if (CheckIsDefined((yyvsp[(1) - (1)].y_symp))) {
			if ((yyvsp[(1) - (1)].y_symp)->attr!=A_COST) {
				sem_error ("non cost id: %s", (yyvsp[(1) - (1)].y_symp)->name);
				(yyval.y_symp) = &ErrorSymbol;
			} else (yyval.y_symp) = (yyvsp[(1) - (1)].y_symp);
		} else (yyval.y_symp) = &ErrorSymbol; }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 210 "twig.y"
    { (yyval.y_symp) = NULL; }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 213 "twig.y"
    {
		tree_lineno = yyline;	/* record line no */
	        if(!CheckIsDefined((yyvsp[(1) - (1)].y_symp)))
			(yyvsp[(1) - (1)].y_symp)->attr = A_ERROR;
		else if(!is_label((yyvsp[(1) - (1)].y_symp))) {
			sem_error("non label id: %s", (yyvsp[(1) - (1)].y_symp)->name);
			(yyvsp[(1) - (1)].y_symp)->attr = A_ERROR;
		}
		(yyval.y_symp) = (yyvsp[(1) - (1)].y_symp);
	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 224 "twig.y"
    {CheckIsNodeOrPred((yyvsp[(1) - (1)].y_symp));}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 225 "twig.y"
    {
		int count;
		Node *ap; 
		/* check the arity of the node */
		for(count=0, ap = (yyvsp[(4) - (5)].y_nodep); ap!=NULL; ap=ap->siblings) count++;
		switch((yyvsp[(1) - (5)].y_symp)->attr) {
			case A_NODE:
				set_arity((yyvsp[(1) - (5)].y_symp), &(yyvsp[(1) - (5)].y_symp)->sd.arity, count);
				break;
		}

		(yyval.y_nodep) = TreeBuild ((yyvsp[(1) - (5)].y_symp), (yyvsp[(4) - (5)].y_nodep));
	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 239 "twig.y"
    {
		CheckIsDefined((yyvsp[(1) - (1)].y_symp));
		switch ((yyvsp[(1) - (1)].y_symp)->attr) {
			case A_NODE:
				set_arity((yyvsp[(1) - (1)].y_symp), &(yyvsp[(1) - (1)].y_symp)->sd.arity, 0);
				break;
		}
		(yyval.y_nodep) = TreeBuild ((yyvsp[(1) - (1)].y_symp), NULL);
	}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 249 "twig.y"
    {
			/*
			 * build sibling list in reverse order TreeBuild will
			 * put it right later.
			 */
			(yyvsp[(3) - (3)].y_nodep)->siblings = (yyvsp[(1) - (3)].y_nodep);
			(yyval.y_nodep) = (yyvsp[(3) - (3)].y_nodep);
		}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 257 "twig.y"
    { (yyvsp[(1) - (1)].y_nodep)->siblings = NULL; (yyval.y_nodep) = (yyvsp[(1) - (1)].y_nodep); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 258 "twig.y"
    { (yyval.y_nodep) = NULL; }
    break;



/* Line 1806 of yacc.c  */
#line 1842 "twig.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 261 "twig.y"


static char *process_suffix(char *);

void
set_arity(SymbolEntry *symp, int *arityp, int count)
{
	if(*arityp!=GIVENUP) {
		if (*arityp==UNDEFINED)
			*arityp = count;
		else if (*arityp!=count) {
			sem_error("inconsistent arity for %s", symp->name);
			*arityp = GIVENUP;
		}
	}
}

int
is_node(SymbolEntry *symp)
{
	return(symp->attr==A_NODE);
}

int
is_label(SymbolEntry *symp)
{
	return(symp->attr==A_LABEL);
}

void
CheckIsNodeOrPred(SymbolEntry *symp)
{
	if (symp->attr==A_ERROR)
		return;
	if (symp->attr!=A_NODE)
		sem_error ("non-node identifier %s", symp->name);
}

int
CheckIsUndefined(SymbolEntry *symp)
{
	if (symp->attr==A_ERROR)
		return(0);
	if (symp->attr!=A_UNDEFINED) {
		sem_error ("multiple declaration: %s", symp->name);
		return(0);
	} else return(1);
}

int
CheckIsDefined(SymbolEntry *symp)
{
	if (symp->attr==A_ERROR)
		return(0);
	if (symp->attr==A_UNDEFINED) {
		sem_error ("undefined identifier: %s", symp->name);
		symp->attr=A_ERROR;
		return(0);
	} else return(1);
}

void
yyerror(char *fmt, ...)
{
	va_list va;

	fprintf(stderr, "line %d: ", yyline);
	va_start(va, fmt);
	vfprintf (stderr, fmt, va);
	va_end(va);
	if (token_buffer[0]!=0)
		fprintf(stderr, " at \"%s\"\n", token_buffer);
}

void
yyerror2 (char *fmt, ...)
{
	va_list va;

	fprintf(stderr, "line %d: ", yyline);
	va_start(va, fmt);
	vfprintf (stderr, fmt, va);
	va_end(va);
	putchar('\n');
}

char *cmdnam;
char *inFileName;
char *outFileName;
char prefixFile[100];
static char usage[] = "usage: mt [ASC] [-d] file";
#define USAGE	error(1, usage)
char *prefix_base = "walker";
char *suffix = "stdio";

void set_arity(SymbolEntry *symp, int *arityp, int count);
void CheckIsNodeOrPred(SymbolEntry *symp);

void
main(int argc, char **argv)
{
	char *cp;

	cmdnam = argv[0];
	argv++;
	inFileName = NULL;

	while(--argc > 0) {
		if (*argv[0]=='-')
			switch(argv[0][1]) {
			/* to see what each of these flags mean...
			 * see common.h */
			case 'd': {
				char *cp;
				for(cp = &argv[0][2]; *cp!='\0'; cp++) 
					switch(*cp) {
					case 'l': debug_flag |= DB_LEX; break;
					case 'm': debug_flag |= DB_MACH; break;
					case 's': debug_flag |= DB_SYM; break;
					case 't': debug_flag |= DB_TREE; break;
					case 'M': debug_flag |= DB_MEM;	break;
					}
				}
				break;
			case 'D': Dflag++;		break;
			case 't': tflag++;		break;
			case 's': sflag++;		break;
			case 'A': suffix = "ansi";	break;
			case 'S': suffix = "stdio";	break;
			case 'C': suffix = "libc";	break;
			case 'w': suffix = process_suffix(&argv[0][2]); break;
			case 'l': prefix_base = &argv[0][2]; break;
			case 'x': line_xref_flag++; break;
			default:
				USAGE;
			}
		else inFileName = argv[0];
		argv++;
	}
	if(inFileName==NULL)
		USAGE;

	fin = fopen(inFileName, "r");	
	if(fin==NULL)
		error(1, "can't open %s", inFileName);
	if((cp=strrchr(inFileName, '.'))!=NULL && strcmp(cp,".mt")==0) {
		if ((outfile = fopen("walker.c" , "w"))==NULL)
			error(1, "can't write %s", outFileName);
		if ((symfile = fopen("symbols.h", "w"))==NULL)
			error(1, "can't write %s", outFileName);
	} else error(1, "input file must have suffix .mt");

	ErrorSymbol.attr = A_LABEL;
	TreeInit();
	SymbolInit();
	LexInit();
	MachInit();
	yyparse();

	SymbolDump();
	if(nerrors == 0) {
		if(!tflag) {
			FILE *prefix;
			char c;
 			sprintf(prefixFile, "%s.%s", prefix_base, suffix);
			prefix = fopen(prefixFile, "r");
			assert(prefix!=NULL);
			if(Dflag)
				fputs("#define DEBUG 1\n", outfile);
			if(line_xref_flag)
				fputs("#define LINE_XREF 1\n", outfile);
			fprintf(outfile,"# line 1 \"%s\"\n", prefixFile);
			while((c=getc(prefix))!=EOF) putc(c, outfile);
		}
		MachineGen();
		SymbolGenerateWalkerCode();
		CodeWrite(outfile, epilogue);
		CodeFreeBlock(epilogue);
	}

	cleanup(0);
}

void
cleanup(int retcode)
{
	lexCleanup();
	if(retcode==0) {
		SymbolFinish();
	}
	exits(retcode ? "errors" : 0);
}

void
error(int rc, char *fmt, ...)
{
	va_list va;

	fprintf(stderr, "%s: ", cmdnam);
	va_start(va, fmt);
	vfprintf (stderr, fmt, va);
	va_end(va);
	putc ('\n', stderr);
	if(rc)
		exits("errors");
}

void
sem_error(char *fmt, ...)
{
	va_list va;

	fprintf (stderr, "line %d: ", yyline);
	va_start(va, fmt);
	vfprintf (stderr, fmt, va);
	va_end(va);
	putc('\n', stderr);
fflush(stderr);
	nerrors++;
	fatalerrors++;
}

void
sem_error2(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vfprintf (stderr, fmt, va);
	va_end(va);
	putc('\n', stderr);
fflush(stderr);
	nerrors++;
}

static char *
process_suffix(char *suf)
{
	extern int gen_table2;
	if(strcmp(suf,"exper")==0) {
		/* experimental walker */
		/* expect this to change alot */
		gen_table2++;
	}
	return(suf);
}

void *
Malloc(unsigned long n)
{
	void *p;

	p = malloc(n);
	if(!p)
		error(1, "out of memory");
	memset(p, 0, n);
	return p;
}

