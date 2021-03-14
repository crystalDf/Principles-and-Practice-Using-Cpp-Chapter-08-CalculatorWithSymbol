# pragma once
# include "std_lib_facilities.h"

const char number = '8';
const char quit = 'q';
const char lower_help = 'h';
const char upper_help = 'H';
const char print = ';';
const std::string prompt = "> ";
const std::string result = "= ";

const char name = 'a';
const char let = 'L';
const std::string declkey = "let";

const char my_sqrt = 'S';
const std::string sqrtkey = "sqrt";

const char my_pow = 'P';
const std::string powkey = "pow";

const char my_const = 'C';
const std::string constkey = "const";

const std::string quitkey = "quit";
const std::string helpkey = "help";

class Token
{
public:
    char kind;
    double value;
    std::string name;

    Token(char ch) : kind { ch } { }
    Token(char ch, double val) : kind { ch }, value { val } { }
    Token(char ch, std::string n) : kind { ch }, name { n } { }
};

class Token_stream
{
public:
    Token_stream(istream& my_cin_istream = std::cin) : full { false }, buffer { ' ' }, my_cin { my_cin_istream } { }
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool full;
    Token buffer;
    istream& my_cin;
};
