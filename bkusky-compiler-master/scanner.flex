%{
#include "tokens.h"
%}
LETTER [a-zA-Z]
DIGIT [0-9]
%%
(" "|\t|\n|\r)		/* skip whitespace */
array			{ return TOKEN_ARRAY; }
auto			{ return TOKEN_AUTO; }
boolean			{ return TOKEN_BOOL; }
char			{ return TOKEN_CHAR; }
else			{ return TOKEN_ELSE; }
false			{ return TOKEN_FALSE; }
for				{ return TOKEN_FOR; }
function		{ return TOKEN_FUNC; }
if				{ return TOKEN_IF; }
integer			{ return TOKEN_INT; }
print			{ return TOKEN_PRINT; }
return			{ return TOKEN_RETURN; }
string			{ return TOKEN_STR; }
true			{ return TOKEN_TRUE; }
void			{ return TOKEN_VOID; }
while			{ return TOKEN_WHILE; }

	/* Comments */
\/\*([^*]|(\*[^\/]))*\*?\*\/ 	
\/\/(.*)\n 						

	/* EOF */

<<EOF>> { return TOKEN_EOF; }

	/* Numbers */
{DIGIT}+	{ return TOKEN_NUMBER; }

	/* Characters */	
:					{ return TOKEN_COLON; }
;					{ return TOKEN_SEMICOLON; }
,					{ return TOKEN_COMMA; }
'(\\.|[^\\])'		{ return TOKEN_CHAR_LIT; }

	/* Expressions */
\+\+			{ return TOKEN_PST_FIX_INC; }
-- 				{ return TOKEN_PST_FIX_DEC; }
!				{ return TOKEN_NOT; }
\^				{ return TOKEN_EXPO; }
\*				{ return TOKEN_MULT; }
\/				{ return TOKEN_DIV; }
%				{ return TOKEN_MOD; }
\+				{ return TOKEN_ADD; }
-				{ return TOKEN_SUB; }
\<				{ return TOKEN_LT; }
\<=				{ return TOKEN_LTE; }
>				{ return TOKEN_GT; }
>=				{ return TOKEN_GTE; }
==				{ return TOKEN_EQ; }
!=				{ return TOKEN_NEQ; }
&&				{ return TOKEN_AND; }
\|\|			{ return TOKEN_OR; }
=				{ return TOKEN_ASSIGN; }
\{				{ return TOKEN_LBRACE; }
\}				{ return TOKEN_RBRACE; }
\[				{ return TOKEN_LBRACKET; }
\]				{ return TOKEN_RBRACKET; }
\(				{ return TOKEN_LPAREN; }
\)				{ return TOKEN_RPAREN; }
\? 				{ return TOKEN_QUESTION; }

	/* Identifiers */
({LETTER}|_)({LETTER}|{DIGIT}|_)*		{ return TOKEN_IDENT; }
	
	/* String Literals */
\"(\\.|[^"\\\n]){0,255}\" 				{ return TOKEN_STR_LIT; }

	/* Errors */
@				{ fprintf(stderr, "scan error: @ is not a valid character\n"); exit(1); }

.	{ return TOKEN_ERROR; }

%%
int yywrap() { return 1; }
