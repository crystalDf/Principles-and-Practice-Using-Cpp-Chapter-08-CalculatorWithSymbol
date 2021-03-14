# include "token.h"

void Token_stream::putback(Token t)
{
    if (full)
    {
        error("putback() into a full buffer");
    }

    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    ch = my_cin.get();

    switch (ch)
    {
        case print:
        case '(':
        case ')':
        case '{':
        case '}':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '!':
        case '=':
        case ',':
            return Token { ch };
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                my_cin.putback(ch);
                double val;
                my_cin >> val;
                return Token { number, val };
            }
        case '#':
            return Token { let };
        case '\n':
            std::cout << "print" << std::endl << prompt;
            return get();
        default:
            {
                if (isspace(ch))
                {
                    return get();
                }

                if (isalpha(ch) || ('_' == ch))
                {
                    string s;

                    while (true)
                    {
                        s += ch;

                        if (!my_cin.get(ch) || (!isalnum(ch) && ('_' != ch)))
                        {
                            break;
                        }
                    }

                    my_cin.putback(ch);

                    if (s == declkey)
                    {
                        return Token { let };
                    }
                    if (s == sqrtkey)
                    {
                        return Token { my_sqrt };
                    }
                    if (s == powkey)
                    {
                        return Token { my_pow };
                    }
                    if (s == constkey)
                    {
                        return Token { my_const };
                    }
                    if (s == quitkey || (s.size() == 1 && (s[0] == quit)))
                    {
                        return Token { quit };
                    }
                    if (s == helpkey || (s.size() == 1 && (s[0] == lower_help || s[0] == upper_help)))
                    {
                        return Token { lower_help };
                    }

                    return Token { name, s };
                }

                error("Bad token");
                return Token { ' ' };
            }
    }
}

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind)
    {
        full = false;
        return;
    }

    full = false;

    char ch;
    while (my_cin >> ch)
    {
        if (ch == c)
        {
            return;
        }
    }
}
