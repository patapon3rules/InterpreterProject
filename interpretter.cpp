#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

std::map<std::string, std::string> symbol_table;

bool error()
{
	std::cout << "Syntax Error\n";
	return false;
}

class Lexer
{
    public:
        Lexer(std::string temp): File(temp)
        {
            ofs.open("NOSPACES.txt", std::ofstream::out | std::ofstream::trunc);
            ofs.close();
            ofs2.open("RES_SYM.txt", std::ofstream::out | std::ofstream::trunc);
            ofs2.close();
            ofs3.open("outlex.txt", std::ofstream::out | std::ofstream::trunc);
            ofs3.close();
            inFile.open(File);

            if(!inFile.is_open())
            {
                std::cout << "File could not be opened!\n";
                exit(0);
            }

            while(!inFile.eof())
            {
            inFile.getline(a, 1000, '\n');
            parse(a);
            }
        }

    private:
        std::string File;
        std::ofstream ofs, ofs2, ofs3;
        char a[1000];
        std::ifstream inFile;

        bool isPunctuator(char ch);
        bool validIdentifier(char* str);
        bool isOperator(char ch);
        bool isKeyword(char *str);
        bool isNumber(char* str);
        bool isReal(char* str);
        bool isLiteral(char* str);
        char* subString(char* realStr, int l, int r);
        void parse(char* str);
};

//Code Provided by FavTutor for Lexical Analysis and modified to our needs
bool Lexer::isPunctuator(char ch)					//check if the given character is a punctuator or not
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == '&' || ch == '|' )
        {
            return true;
        }
    return false;
}

bool Lexer::validIdentifier(char* str)						//check if the given identifier is valid or not
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isPunctuator(str[0]) == true)
        {
            return false;
        }									//if first character of string is a digit or a special character, identifier is not valid
    int i,len = strlen(str);
    if (len == 1)
    {
        return true;
    }										//if length is one, validation is already completed, hence return true
    else
    {
        for (i = 1 ; i < len ; i++)						//identifier cannot contain special characters
        {
            if (isPunctuator(str[i]) == true)
            {
                return false;
            }
        }
    }

    return true;
}

bool Lexer::isOperator(char ch)							//check if the given character is an operator or not
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' || ch == '<' ||
        ch == '=' || ch == '|' || ch == '&')
    {
       return true;
    }
    return false;
}

bool Lexer::isKeyword(char *str)						//check if the given substring is a keyword or not
{
    if (!strcmp(str, "if") || !strcmp(str, "else") ||
        !strcmp(str, "while") || !strcmp(str, "do") ||
        !strcmp(str, "break") ||  !strcmp(str, "continue")
        || !strcmp(str, "int") || !strcmp(str, "double")
        || !strcmp(str, "float") || !strcmp(str, "return")
        || !strcmp(str, "char") || !strcmp(str, "case")
        || !strcmp(str, "long") || !strcmp(str, "short")
        || !strcmp(str, "typedef") || !strcmp(str, "switch")
        || !strcmp(str, "unsigned") || !strcmp(str, "void")
        || !strcmp(str, "static") || !strcmp(str, "struct")
        || !strcmp(str, "sizeof") || !strcmp(str,"long")
        || !strcmp(str, "volatile") || !strcmp(str, "typedef")
        || !strcmp(str, "enum") || !strcmp(str, "const")
        || !strcmp(str, "union") || !strcmp(str, "extern")
        || !strcmp(str,"bool") || !strcmp(str,"cout"))
        {
            return true;
        }
    else
    {
       return false;
    }
}

bool Lexer::isNumber(char* str)							//check if the given substring is a number or not
{
    int i, len = strlen(str),numOfDecimal = 0;
    if (len == 0)
    {
        return false;
    }
    for (i = 0 ; i < len ; i++)
    {
        if (numOfDecimal > 1 && str[i] == '.')
        {
            return false;
        }
        else if (numOfDecimal <= 1)
        {
            numOfDecimal++;
        }
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
            {
                return false;
            }
    }
    return true;
}

bool Lexer::isReal(char* str)							//check if the given substring is a real number or not
{
    int i, len = strlen(str),numOfDecimal = 0, count = -1;
    if (len == 0)
    {
        return false;
    }
    for (i = 0 ; i < len ; i++)
    {
        if (numOfDecimal > 1 && str[i] == '.')
        {
            count++;
        }
        else if (numOfDecimal <= 1)
        {
            numOfDecimal++;
        }

        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.'|| (str[i] == '-' && i > 0))
            {
                return false;
            }
        if (count >= 0)
        {
            return false;
        }
    }
    return true;
}

bool Lexer::isLiteral(char* str)						//check if the given identifier is valid or not
{
    int i = 0,len = strlen(str);
    if(str[i] == '\"' && str[len-1] == '\"')
    {
        return true;
    }

    return false;
}

char* Lexer::subString(char* realStr, int l, int r)				//extract the required substring from the main string
{
    int i;

    char* str = (char*) malloc(sizeof(char) * (r - l + 2));

    for (i = l; i <= r; i++)
    {
        str[i - l] = realStr[i];
        str[r - l + 1] = '\0';
    }
    return str;
}

void Lexer::parse(char* str)						//parse the expression
{
    std::ofstream output, key, outlex;
    std::string temptext = "none";
    output.open("NOSPACES.txt", std::ios_base::app);
    if(!output.is_open())
    {
        std::cout << "File could not be opened!\n";
        exit(0);
    }

    key.open("RES_SYM.txt", std::ios_base::app);
    if(!key.is_open())
    {
        std::cout << "File could not be opened!\n";
        exit(0);
    }

    outlex.open("outlex.txt", std::ios_base::app);
    if(!outlex.is_open())
    {
        std::cout << "File could not be opened!\n";
        exit(0);
    }

    int left = 0, right = 0;
    int len = strlen(str);
    while (right <= len && left <= right)
    {
        if (isPunctuator(str[right]) == false)			//if character is a digit or an alphabet
        {
            right++;
        }

        if (isPunctuator(str[right]) == true && left == right)		//if character is a punctuator
        {
            if (isOperator(str[right]) == true)
            {

                output << str[right];
                key << str[right];
                outlex << str[right];

                if((str[right] == '<') && (str[right+1] == '<'))
                {
                    right++;
                    output << str[right];
                    key << str[right];
                    outlex << str[right];
                }
                key <<" IS AN OPERATOR\n";
                outlex << ',';
            }
            else if(str[right] != ' ' && str[right] != '\n')
            {
                output << str[right];
                key<< str[right] <<" IS AN PUNCTUATOR\n";
                outlex << str[right] << ',';
            }
            right++;
            left = right;
        }
        else if (isPunctuator(str[right]) == true && left != right || (right == len && left != right)) 			//check if parsed substring is a keyword or identifier or number
        {
            char* sub = subString(str, left, right - 1);   //extract substring

            if(int(sub[0]) == 0)
            {
                output.close();
                if (output.is_open())
                {
                    std::cout << "Stream could not close!\n";
                }
                key.close();
                if (key.is_open())
                {
                    std::cout << "Stream could not close!\n";
                }
                outlex.close();
                if (outlex.is_open())
                {
                    std::cout << "Stream could not close!\n";
                }
                return;
            }

            if (isKeyword(sub) == true)
            {
                output << sub;
                key << sub <<" IS A RESERVED KEYWORD\n";
                outlex << sub << ',';

                if(strcmp(sub, "int") || strcmp(sub,"double"))
                {
                    temptext = sub;
                }
                else
                {
                    temptext = "none";
                }
            }
            else if (isNumber(sub) == true)
            {
                output << sub;
                key<< sub <<" IS A NUMBER\n";
                outlex << "num,";
            }
            else if (isReal(sub) == true)
            {
                output << sub;
                key<< sub <<" IS A REAL NUMBER\n";
                outlex << "real,";
            }
            else if (isLiteral(sub) == true)
            {
                output << sub;
                key << sub << " IS A LITERAL\n";
                outlex << "literal,";
            }
            else if (validIdentifier(sub) == true && isPunctuator(str[right - 1]) == false)
            {
                output << sub;
                key<< sub <<" IS A VALID IDENTIFIER\n";
                outlex << sub << ",";

                if(temptext != "none")
                {
                    symbol_table.emplace(sub, temptext);
                    temptext = "none";
                }
            }
            else if (validIdentifier(sub) == false && isPunctuator(str[right - 1]) == false)
            {
                key<< sub <<" IS NOT A VALID IDENTIFIER\n";
                std::cout << "Lexical Error\n";
                return;
            }

            left = right;
        }
    }
    return;
}

int main(int argc, char **argv)
{
    if (std::string(argv[1]) == "integer" && argc < 2)
    {
        std::cout << "Argument not found. Execute program following this syntax: ./lex [filename]\n";
        exit(0);
    }

    Lexer lexer(argv[1]);

    std::vector<std::string> tokens;
	std::string temp;

	//iterator
	std::vector<std::string>::iterator i;

	//error
	bool flag = true;
    std::ifstream inFile;

    inFile.open("outlex.txt");
    if(!inFile.is_open())
    {
        std::cout << "File could not be opened!\n";
        exit(0);
    }

    while(std::getline(inFile, temp, ','))
    {
		tokens.push_back(temp);
    }

    for (i = tokens.begin(); i != tokens.end(); ++i)
	{
        //instantiating a variable
		if(*i == "int" || *i == "double")
		{
			++i;
            if (symbol_table.count(*i) != 1)
            {
                flag = error();
            }
            else if (symbol_table.at(*i) == "int" || symbol_table.at(*i) == "double")
            {
                temp = *i;
            }
            else
            {
                flag = error();
            }
			++i;
			if(*i == "=" || *i == ";")
			{
                if(*i == ";")
                {
                }
                else
                {
                    while(true)
                    {
                        ++i;
                        if (symbol_table.count(*i) != 1)
                        {
                            if(*i == "num" && symbol_table.at(temp) != "int")
                            {
                                flag = error();
                            }
                            if(*i == "real" && symbol_table.at(temp) == "double")
                            {
                                flag = error();
                            }
                        }
                        else
                        {
                            if(symbol_table.at(*i) == "real" && symbol_table.at(temp) == "int")
                            {
                                flag = error();
                            }
                        }
                        ++i;
                        if(*i != ";" && *i != "+" && *i != "-" && *i != "*" && *i != "/")
                        {
                            flag = error();
                        }
                        if(*i == ";")
                        {
                            break;
                        }
                    }
                }
			}
		}
		else if(*i == "output")
		{
            ++i;
            while(true)
            {
                while(true)
                {
                   if(*i != "<<")
                    {
                        flag = error();
                    }
                    ++i;
                    if(*i != "literal" && symbol_table.count(*i) != 1)
                    {
                        flag = error();
                    }
                    ++i;
                    if(*i == ";" || *i == "+")
                    {
                        break;
                    }
                }
                while(true)
                {
                    if(*i == "<<" || *i == ";")
                    {
                        break;
                    }
                    if(*i != "+")
                    {
                        flag = error();
                    }
                    ++i;
                    if(symbol_table.count(*i) != 1 || *i == "literal")
                    {
                        flag = error();
                    }
                    ++i;
                }
                if(*i == ";")
                {
                    break;
                }
            }
		}
		else if(*i == "if")
		{
			++i;
			if(*i != "(")
			{
				flag = error();
			}
			++i;
			if(symbol_table.count(*i) != 1)
			{
				flag = error();
			}
			++i;
			if(*i != "<")
			{
				flag = error();
			}
			++i;
			if(*i != "num")
			{
				flag = error();
			}
			++i;
			if(*i != ")")
			{
				flag = error();
			}
			++i;
			if(*i != "{")
			{
				flag = error();
			}
			++i;
			if(*i != "output")
			{
				flag = error();
			}
			++i;
			if(*i != "<<")
			{
				flag = error();
			}
			++i;
			if(*i == "literal" || symbol_table.count(*i) != 1)
			{
				flag = error();
			}
			++i;
			if(*i != ";")
			{
				flag = error();
			}
			++i;
			if(*i != "}")
			{
				flag = error();
			}
		}
		else if (symbol_table.count(*i) != 0)
        {
            if (symbol_table.at(*i) == "int" || symbol_table.at(*i) == "double")
            {
                temp = *i;
            }
            else
            {
                flag = error();
            }
			++i;
			if(*i == "=" || *i == ";")
			{
                if(*i == ";")
                {
                }
                else
                {
                    while(true)
                    {
                        ++i;
                        if (symbol_table.count(*i) != 1)
                        {
                            if(*i == "num" && symbol_table.at(temp) != "int")
                            {
                                flag = error();
                            }
                            if(*i == "real" && symbol_table.at(temp) == "int")
                            {
                                flag = error();
                            }
                        }
                        else
                        {
                            if(symbol_table.at(*i) == "double" && symbol_table.at(temp) == "int")
                            {
                                flag = error();
                            }
                        }
                        ++i;
                        if(*i != ";" && *i != "+" && *i != "-" && *i != "*" && *i != "/")
                        {
                            flag = error();
                        }
                        if(*i == ";")
                        {
                            break;
                        }
                    }
                }
            }
        }
	}

	if(flag)
	{
		std::cout << "No Errors";
	}

    exit(0);
}
