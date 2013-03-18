HEX	[0-9A-Fa-f]
OCTAL	[0-7]
DECIMAL	[0-9]
LETTER	[A-Za-z/_]

%{
#include "xpal.h"
#include "parser.tab.h"

int	yyback(int *, int);
int	yylook(void);

char cbuf[MAXPIN];
int clen;

#undef input
#undef unput

%}

%START normal string
%option noyywrap

%%

<normal>"0x"{HEX}+	{ return(_HEX); }
<normal>"0"{OCTAL}+	{ return(_OCTAL); }
<normal>{DECIMAL}+	{ return(_DECIMAL); }
<normal>"0b"[01]+	{ return(_BINARY); }

<normal>\"		{ BEGIN string; clen=0; }
<string>\"		{ 	cbuf[clen]='\0';
		  		BEGIN normal;
		  		return(_STRING);
			}
<string>\n		;
<string>" "		;
<string>"\\\""		{ cbuf[clen++] = '"'; }
<string>"\\\\"		{ cbuf[clen++] = '\\'; }
<string>.		{ cbuf[clen++] = yytext[0];
			  if(clen>=MAXPIN-1)
				{yyerror("string too long",cbuf); BEGIN normal;
			  }
			}

<normal>%.*$		;
<normal>\n		;
<normal>" "		;
<normal>\t		;

<normal>{DECIMAL}+{LETTER}+{DECIMAL}+	{ return(_PART); }

<normal>array		return(_ARRAY);
<normal>inputs		return(_INPUTS);
<normal>outputs		return(_OUTPUTS);
<normal>set		return(_SET);
<normal>reset		return(_RESET);
<normal>clock		return(_CLOCK);
<normal>macrocell	return(_MACROCELL);
<normal>enabled		return(_ENABLED);
<normal>enables		return(_ENABLES);
<normal>clocked		return(_CLOCKED);
<normal>inverted	return(_INVERTED);
<normal>internal	return(_INTERNAL);
<normal>external	return(_EXTERNAL);
<normal>input		return(_INPUT);
<normal>output		return(_OUTPUT);
<normal>fuses		return(_FUSES);
<normal>complement"+"	return(_COMPLEMENTPLUS);
<normal>complement"-"	return(_COMPLEMENTMINUS);
<normal>pins		return(_PINS);
<normal>offset		return(_OFFSET);
<normal>declare		return(_DECLARE);
<normal>supply		return(_VCC);
<normal>ground		return(_GND);
<normal>package		return(_PACKAGE);
<normal>invert		return(_INVERT);
<normal>enable		return(_ENABLE);
<normal>not		return(_NOT);

<normal>{LETTER}({LETTER}|{DECIMAL})*	return(_ID);
<normal>.		return(yytext[0]);

%%
