#include "SyntaxHead.h"

#define is_constsym		(Lex::curElmt == "const")
#define is_intsym		(Lex::curElmt == "int")
#define is_charsym		(Lex::curElmt == "char")
#define is_voidsym		(Lex::curElmt == "void")
#define is_mainsym		(Lex::curElmt == "main")
#define is_ifsym		(Lex::curElmt == "if")
#define is_elsesym		(Lex::curElmt == "else")
#define is_forsym		(Lex::curElmt == "for")
#define is_whilesym		(Lex::curElmt == "while")
#define is_scanfsym		(Lex::curElmt == "scanf")
#define is_printfsym	(Lex::curElmt == "printf")
#define is_returnsym	(Lex::curElmt == "return")
#define is_reserved		(is_constsym || is_intsym || is_charsym || is_voidsym || \
						 is_mainsym || is_ifsym || is_elsesym || is_forsym || \
						 is_whilesym || is_scanfsym || is_printfsym || is_returnsym )

#define is_add			(Lex::curElmt == "+")
#define is_sub			(Lex::curElmt == "-")
#define is_mul			(Lex::curElmt == "*")
#define is_div			(Lex::curElmt == "/")
#define is_L_small		(Lex::curElmt == "(")
#define is_R_small		(Lex::curElmt == ")")
#define is_L_mid		(Lex::curElmt == "[")
#define is_R_mid		(Lex::curElmt == "]")
#define is_L_big		(Lex::curElmt == "{")
#define is_R_big		(Lex::curElmt == "}")
#define is_assign		(Lex::curElmt == "=")
#define is_not_eql		(Lex::curElmt == "!=")
#define is_eql			(Lex::curElmt == "==")
#define is_greater		(Lex::curElmt == ">")
#define is_grt_eql		(Lex::curElmt == ">=")
#define is_less			(Lex::curElmt == "<")
#define is_les_eql		(Lex::curElmt == "<=")
#define is_dbl_quote	(Lex::curElmt == "\"")
#define is_sgl_quote	(Lex::curElmt == "'")
#define is_comma		(Lex::curElmt == ",")
#define is_semicolon	(Lex::curElmt == ";") 

#define sent_head_rsvd	(is_ifsym || is_forsym || is_semicolon || is_L_big || \
						 is_whilesym || is_scanfsym || is_printfsym || is_returnsym )
#define is_compare_op	(is_not_eql || is_eql || is_greater || is_grt_eql || is_less || is_les_eql)
#define is_IDEN			(Lex::curCls==Lex::IDEN)
#define is_STRING		(Lex::curCls==Lex::STRING)

std::string curFunc = "";	// "" means global

void skipTill(const std::set<std::string> symbols, const std::set<int> wordType)
{
	//symbol e.g. "," ";"	wordType e.g. Lex::RSVD_WD
	int r = -1;
	while (symbols.count(Lex::curElmt) == 0 && !CODEEOF)
	{
		r = Lex::getsym();
		if (wordType.count(r) == 1)
			break;
	}
}

// about var def
int _int()
{
	int r = Lex::getsym();
	int sign = 1;		// check whether - before num
	if (is_add || is_sub)
	{
		if (is_sub)
		{
			sign = -1;
		}
		r = Lex::getsym();	//unsigned int expected
	}
	if (r != Lex::UNSGN_INT)
	{
		/* TODO: not int after const int */

	}
#if Syn_Out
	std::cout << "int got: " << sign * Lex::curNum << std::endl;
#endif
	return sign * Lex::curNum;
}

char single_char()
{
	int r = Lex::getsym();

	if (r != Lex::SGL_CHARA)
	{
		/* TODO: not int after const int */

	}
	else {
#if Syn_Out
		std::cout << "char got: " << Lex::curElmt[1] << std::endl;
#endif
	}
	return Lex::curElmt[1];
}

void const_define()
{
	int r;		//ret value from getsym
	bool const_int = false;
	if (is_intsym || is_charsym)	//const int|char
	{
		const_int = is_intsym;	// define const int, then not allow const char
		do
		{
			r = Lex::getsym();
			if (r == Lex::IDEN)
			{
				/* TODO: search in symbolTable
							-> exist in the same layer(function, var, const) -> err
							-> not exist -> enSymbolTable
				*/
				Lex::getsym();
				int int_val = 0;
				char char_val = '0';
				if (is_assign)
				{
					if (const_int)
					{
						int_val = _int();
					}
					else
					{
						char_val = single_char();
					}
				}
				else
				{
					/* TODO: miss '=' after "const int|char x" ,use default val 0 */
				}
#if Syn_Out
				std::cout << "Line: "<< Lex::LineCounter 
					<< " const " << (const_int ? "int " : "char ") << "def" << std::endl;
#endif
				/* TODO: fill in sym table */
				/* TODO: skip until ',' | ';' 
						if meet reservedwords, then 
						break with err"lack semi" */
				/* TODO: if is_comma then getsym(), IDEN expected*/
			}
			else
			{
				/* TODO: miss iden after "const int|char" */
				/* TODO: skip until ';', 
						if meet reservedwords, then 
						break with err"lack semi"*/
			}
		} while (!is_semicolon && !CODEEOF);
	}
	else 
	{
		/* TODO: expect int|char after const  */
		/* TODO: skip until ';'
				if meet reservedwords, then
				break with err"lack semi" */
	}
}

void continuous_var_def(bool inttype)
{	
	int r;
	do
	{	
		r = Lex::getsym();
		//curElmt is IDEN
		if (r == Lex::IDEN)
		{
			Lex::getsym();
			if (is_comma || is_semicolon)
			{
				bool out = is_semicolon;
				/* TODO: fill in sym tab*/
#if Syn_Out
				std::cout << "Line: " << Lex::LineCounter
					<< " continuous var " << (inttype ? "int " : "char ") << "def" << std::endl;
#endif
				if (!out)
					continue;
				else
					return;
			}
			else if (is_L_mid)
			{
				int length = _int();
				length = length > 0 ? length : 1;
				Lex::getsym();		// ']'
				if (!is_R_mid)
				{
					/* TODO: miss R_mid */
					/* skip until ","->continue_var | ";"->continue */
				}
				else
				{
					/* TODO: fill in sym table */
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " continuous array " << (inttype ? "int " : "char ") << "def" << std::endl;
#endif
					Lex::getsym();	// , | ;
				}
			}
			else
			{
				/* TODO: expect , | ; */
				/* skip until , | ; */
			}
		}
		else
		{
			/* TODO: expect IDEN after ',' */
			/* skip until ',' | ';'*/
		}
	} while (!is_semicolon && !CODEEOF);
}

void var_def_complex()
{
	int r;
	if (is_intsym || is_charsym)
	{
		bool ret_int = is_intsym;	// func/var type (int|char)

		r = Lex::getsym();
		if (r == Lex::IDEN)
		{
			std::string key = Lex::curElmt;
			Lex::getsym();
			if (is_semicolon)
			{
				/* TODO: fill in sym table */
#if Syn_Out
				std::cout << "Line: " << Lex::LineCounter
					<< " var " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
				Lex::getsym();
				return;
			}
			else if (is_L_mid)
			{
				int length = _int();
				length = length > 0 ? length : 1;
				Lex::getsym();		// ']'
				if (!is_R_mid)
				{
					/* TODO: miss R_mid */
					/* skip until ","->continue_var | ";"->continue */
				}
				else
				{
					/* TODO: fill in sym table */
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " array " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
					continuous_var_def(ret_int);	//after it, ';'
				}
			}
			else if (is_comma)
			{
				/* TODO: fill in sym table */
#if Syn_Out
				std::cout << "Line: " << Lex::LineCounter
					<< " var " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
				continuous_var_def(ret_int);	//after it, ';'
			}
			else
			{
				/* TODO: invalid word after int|char IDEN */
			}
		}
		else
		{
			/* TODO: IDEN expected */
		}
	}
	else
	{
		/* TODO: no var def */
		
	}
	
}

// about sentence
void value_param(std::string func_name)
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " value_param " << "start" << std::endl;
#endif
	expression();
	while (is_comma)
	{
		expression();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " value_param " << "over" << std::endl;
#endif
}

void factor()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " factor " << "start" << std::endl;
#endif
	int r;
	if (is_IDEN)
	{
		std::string name = Lex::curElmt;
		/* TODO: check table*/
		r = Lex::getsym();
		if (is_L_mid)
		{
			Lex::getsym();
			expression();
			if (is_R_mid)
			{
				Lex::getsym();

			}
			else {
				/* TODO: miss ] */
			}
		}
		else if (is_L_small) {
			Lex::getsym();
			value_param(name);
			if (is_R_small)
			{
				Lex::getsym();
			}
			else {
				/* TODO: miss ) */
			}
		}
		else {
			/* TODO: no_param_func / int / char */
			Lex::getsym();
		}
	}
	else if (is_L_small)
	{
		Lex::getsym();
		expression();
		if (is_R_small)
		{
			Lex::getsym();
		}
		else {
			/* TODO: miss ) */
		}
	}
	else{
		/* TODO: to fix  bug */
		//Lex::getsym();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " factor " << "end" << std::endl;
#endif

}

void term()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " term " << "start" << std::endl;
#endif
	factor();
	while (is_mul || is_div)
	{
		Lex::getsym();
		factor();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " term " << "over" << std::endl;
#endif

}

void expression()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " expression " << "start" << std::endl;
#endif
	int r;
	int symbol = 1;	// + | -
	if (is_add || is_sub)
	{
		symbol = is_add ? 1 : -1;
		Lex::getsym();
	}
	term();
	while (is_add || is_sub)
	{
		symbol = is_add ? 1 : -1;
		Lex::getsym();
		term();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " expression " << "over" << std::endl;
#endif

}

void condition()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " conditon " << "start" << std::endl;
#endif

	expression();
	if (is_compare_op)
	{
		Lex::getsym();
		expression();
	}
#if Syn_Out
		std::cout << "Line: " << Lex::LineCounter
		<< " condition " << "end" << std::endl;
#endif

}

void if_sentence()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " if " << "start" << std::endl;
#endif
	int r = Lex::getsym();
	if (is_L_small)
	{
		r = Lex::getsym();
	}
	else
	{
		/* TODO: miss ( */
	}
	condition();
	if (is_R_small)
	{
		Lex::getsym();
	}
	else
	{

	}
	sentence();
	if (is_elsesym)
	{
		sentence();

	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " if " << "overt" << std::endl;
#endif

}

void scanf_sentence()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " scanf " << "start" << std::endl;
#endif

	Lex::getsym();
	if (is_L_small)
	{
		Lex::getsym();
	}
	else
	{
		/* TODO: miss ( */
	}
	if (is_IDEN)
	{

		Lex::getsym();
	}
	while (is_comma)
	{
		Lex::getsym();
		if (is_IDEN)
		{

			Lex::getsym();
		}
	}
#if Syn_Out
		std::cout << "Line: " << Lex::LineCounter
		<< " scanf " << "over" << std::endl;
#endif

}

void printf_sentence()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " printf " << "start" << std::endl;
#endif

	Lex::getsym();
	if (is_L_small)
	{
		Lex::getsym();
	}
	else
	{
		/* TODO: miss ( */
	}
	if (is_STRING)
	{

		Lex::getsym();
		if (is_comma)
		{
			Lex::getsym();
		}
		else
		{
			if (is_R_small)
			{
				Lex::getsym();
			}
			else
			{

			}
#if Syn_Out
			std::cout << "Line: " << Lex::LineCounter
				<< " printf " << "over" << std::endl;
#endif
			return;
		}
	}
	expression();
	/* print */
	if (is_R_small)
	{
		Lex::getsym();
	}
	else
	{

	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " printf " << "over" << std::endl;
#endif

}

void return_sentence()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " return " << "start" << std::endl;
#endif

	Lex::getsym();
	if (is_L_small)
	{
		Lex::getsym();
		expression();
		if (is_R_small)
		{
			Lex::getsym();

		}
		else
		{
			/* TODO: miss ) */
		}
	}
	else
	{
		/* TODO: miss ( */
	}
	
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " return " << "over" << std::endl;
#endif

}

void while_sentence()
{
	Lex::getsym();
	if (is_L_small)
	{
		Lex::getsym();
	}
}

void sentence()
{
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " sentence " << "start" << std::endl;
#endif
	if (is_ifsym)
	{
		if_sentence();
	}
	else if (is_whilesym)
	{

	}
	else if (is_forsym)
	{

	}
	else if (is_L_big)
	{
		sentence_list();
	}
	else if (is_IDEN)
	{
		std::string name = Lex::curElmt;
		Lex::getsym();
		if (is_semicolon)
		{
			Lex::getsym();
		}
		else if (is_L_small)
		{
			value_param(name);
			if (is_R_small)
			{
				Lex::getsym();
			}
			else 
			{

			}
			if (is_semicolon)
			{
				Lex::getsym();
			}
			else
			{
				/* TODO: miss ; */
			}
		}
		else if (is_L_mid)
		{
			Lex::getsym();
			expression();
			if (is_R_mid)
			{
				Lex::getsym();
			}
			else
			{
			
			}
			if (is_assign)
			{
				Lex::getsym();
			}
			else
			{

			}
			expression();
			if (is_semicolon)
			{
				Lex::getsym();
			}
		}
		else if (is_assign)
		{
			Lex::getsym();
			expression();
			if (is_semicolon)
			{
				Lex::getsym();
			}
		}
		else
		{
			/* TODO: miss ; */
		}
	}
	else if (is_scanfsym)
	{
		scanf_sentence();
	}
	else if (is_printfsym)
	{
		printf_sentence();
	}
	else if (is_semicolon)
	{
		Lex::getsym();
	}
	else if (is_returnsym)
	{
		return_sentence();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " sentence " << "over" << std::endl;
#endif

}

void sentence_list()
{
	//int r = Lex::curCls;
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " sentence_list " << "start" << std::endl;
#endif
	while (sent_head_rsvd || is_IDEN)
	{
		sentence();
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " sentence_list " << "end" << std::endl;
#endif
}

void complex_sentence()
{
	int r;
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " complex_sentence " << "end" << std::endl;
#endif
	do
	{
		r = Lex::getsym();
		if (is_constsym)
		{
			Lex::getsym();
			const_define();	//after it, curElmt = ";" bad->rsvd_wd
		}
		else
		{
			break;	
		}
	} while (is_semicolon && !CODEEOF);
	if (is_semicolon)
	{
		r = Lex::getsym();		// int | char 
	}
	do
	{
		var_def_complex();
		if (is_semicolon)
		{
			r = Lex::getsym();		// int | char 
		}
	} while ((is_intsym || is_charsym) && !CODEEOF);
	
	if (is_semicolon)
	{
		r = Lex::getsym();		// sentence list start
	}
	sentence_list();		// before: new word, after: "}"
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " complex_sentence " << "end" << std::endl;
#endif

}

//about func def
void param_list(std::string func_name)
{
	int r;
	do
	{
		r = Lex::getsym();
		if (is_intsym || is_charsym)
		{
			bool int_para = is_intsym;
			r = Lex::getsym();
			if (r == Lex::IDEN)
			{
				std::string IDEN_name = Lex::curElmt;
#if Syn_Out
				std::cout << "Line: " << Lex::LineCounter << " Function "
					<< func_name << " param: " << (int_para ? "int " : "char ") << IDEN_name << std::endl;
#endif
				/* TODO: enSymTable func_name->param */
				Lex::getsym();
			}
			else
			{
				/* TODO: invalid IDEN after int|char */
			}
			
		}
		else
		{
			/* TODO: invalid param type */

		}
	} while (!is_comma && !is_R_small && !CODEEOF);
}

void param_func_def_piece(std::string func_name)
{
	param_list(func_name);	//after it, curElmt = non-comma word
	if (is_R_small)
	{
		Lex::getsym();
		if (is_L_big)
		{
			complex_sentence();
			if (!is_R_big)
			{
				/* TODO: no } after func def */
			}
#if Syn_Out
			std::cout << "Line: " << Lex::LineCounter
				<< " param Function " << "def piece" << std::endl;
#endif
		}
		else
		{
			/* TODO: no { */
		}
	}
	else
	{
		/* TODO: no ) */
	}
}

void no_param_func_def_piece(std::string func_name)
{
	complex_sentence();
	if (!is_R_big)
	{
		/* TODO: no } after func def */
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " non-param Function " << "def piece" << std::endl;
#endif
}

void main_piece()
{
	int r = Lex::getsym();
	if (is_L_small)
	{
		Lex::getsym();
		if (is_R_small)
		{
			Lex::getsym();
			if (is_L_big)
			{
				complex_sentence();
			}
			else
			{
				/* TODO: miss {*/
			}
		}
		else
		{
			/* TODO: miss ) */
		}
	}
	else
	{
		/* TODO: miss ( */
	}
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " main piece over" << std::endl;
#endif
}

void Syn::program()
{
	int r;
	r = Lex::getsym();
	while (is_constsym && !CODEEOF)
	{
		Lex::getsym();
		const_define();		//out: curE = ";" / reserved
		if (is_constsym)
		{
			continue;	// miss ;
		}
		if (is_reserved)
		{
			break;
		}
		else if (is_semicolon)
		{
			Lex::getsym();	//const expected
		}
		else 
		{
			/* TODO: invalid reserved word */
		}		
	}
	//tricky time
	/* int|char -> IDEN ->  [ -> var array
							, -> var next
							; -> var end
					ban var	( -> para_func_def 
					ban var { -> no_para_func_def 
	   void -> main -> out:main_part
	  ban var  IDEN ->  ( -> para_func_def 
						{ -> no_para_func_def */
	bool in_func_def = false;
	while (!CODEEOF) {
		if (is_intsym || is_charsym)
		{
			bool ret_int = is_intsym;	// func/var type (int|char)

			r = Lex::getsym();
			if (r == Lex::IDEN)
			{
				std::string key = Lex::curElmt;
				Lex::getsym();
				if (is_semicolon && !in_func_def)
				{
					/* TODO: fill in sym table */
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " var " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
					Lex::getsym();
					continue;
				}
				else if (is_L_mid && !in_func_def)
				{
					int length = _int();
					length = length > 0 ? length : 1;
					Lex::getsym();		// ']'
					if (!is_R_mid)
					{
						/* TODO: miss R_mid */
						/* skip until ","->continue_var | ";"->continue */
					}
					else
					{
						/* TODO: fill in sym table */
#if Syn_Out
						std::cout << "Line: " << Lex::LineCounter
							<< " array " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
						Lex::getsym();	// , | ;
					}
				}
				else if (is_comma)
				{
					/* TODO: fill in sym table */
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " var " << (ret_int ? "int " : "char ") << "def" << std::endl;
#endif
					continuous_var_def(ret_int);
				}
				else if (is_L_small)
				{
					in_func_def = true;
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " para-function " << (ret_int ? "int " : "char ") << std::endl;
#endif
					/* TODO: fill in sym table (func) */
					param_func_def_piece(key);


				}
				else if (is_L_big)
				{
					in_func_def = true;
#if Syn_Out
					std::cout << "Line: " << Lex::LineCounter
						<< " non-para-function " << (ret_int ? "int " : "char ") << std::endl;
#endif
					/* TODO: fill in sym table (func) */
					no_param_func_def_piece(key);
				}
				else
				{
					/* TODO: invalid word after int|char IDEB */
				}
			}
			else
			{
				/* TODO: IDEN expected */
			}
		}
		else if (is_voidsym)
		{
			in_func_def = true;
			r = Lex::getsym();
			if (r == Lex::IDEN)
			{

			}
			else if(is_mainsym)
			{
#if Syn_Out
				std::cout << "Line: " << Lex::LineCounter
					<< " main start" << std::endl;
#endif
				break;		// to main
			}
			else
			{
				/* TODO: miss main/iden */
				
			}
		}
		else
		{
			/* TODO: int/char/void expected*/
			skipTill({ "int", "char", "void" }, { Lex::RSVD_WD });

		}
	}
	assert(is_mainsym);
	main_piece();
#if Syn_Out
	std::cout << "Line: " << Lex::LineCounter
		<< " main over" << std::endl;
#endif
}

#if Syn_Out
int main(int argc, char** argv)
{
	std::string code_path(argv[1]);
	//../Docs/16231246_test.txt
	Lex::code_file.open(code_path, std::ifstream::in);
	std::ofstream f_out("../Docs/16231246_Syn_out.txt", std::ios::trunc | std::ofstream::ate);
	int r;
	if (!Lex::code_file || !f_out)
	{
		std::cout << "open failed";
		return -1;
	}
	unsigned long cnt = 0;
	Syn::program();
	if (Lex::code_file.is_open())
	{
		Lex::code_file.close();
	}
	return 0;
}
#endif