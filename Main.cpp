# include "std_lib_facilities.h"
# include "token.h"
# include "symbol_table.h"

double expression(Token_stream& ts);
double term(Token_stream& ts);
double factorial(Token_stream& ts);
double primary(Token_stream& ts);
double get_factorial(double val);
void calculate();
void clean_up_mess();

double statement();
double set_variable(bool is_const = false, bool is_declaration = true);

double sqrt_pattern();
double pow_pattern();

void show_help();

Token_stream ts;
Symbol_table names;

int main()
{
    try
    {
        names.declare("pi", 3.1415926535, true);
        names.declare("e", 2.7182818284, true);

        calculate();

        keep_window_open();
    }
    catch (exception& e)
    {
        std::cerr << e.what() << std::endl;
        keep_window_open();

        return 1;
    }
    catch (...)
    {
        std::cerr << "exception" << std::endl;
        keep_window_open();

        return 2;
    }
}

double expression(Token_stream& ts)
{
    double left = term(ts);
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '+':
                {
                    left += term(ts);
                    t = ts.get();
                    break;
                }
            case '-':
                {
                    left -= term(ts);
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double term(Token_stream& ts)
{
    double left = factorial(ts);
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '*':
                {
                    left *= factorial(ts);
                    t = ts.get();
                    break;
                }
            case '/':
                {
                    double d = factorial(ts);

                    if (!d)
                    {
                        error("divide by zero");
                    }

                    left /= d;
                    t = ts.get();
                    break;
                }
            case '%':
                {
                    double d = factorial(ts);

                    if (!d)
                    {
                        error("divide by zero");
                    }

                    left = fmod(left, d);
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double factorial(Token_stream& ts)
{
    double left = primary(ts);
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '!':
                {
                    left = get_factorial(left);
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double primary(Token_stream& ts)
{
    Token t = ts.get();

    switch (t.kind)
    {
        case '(':
            {
                double d = expression(ts);
                t = ts.get();

                if (t.kind != ')')
                {
                    error("')' expected");
                }
                return d;
            }
        case '{':
            {
                double d = expression(ts);
                t = ts.get();

                if (t.kind != '}')
                {
                    error("'}' expected");
                }
                return d;
            }
        case number:
            return t.value;
        case '-':
            return -primary(ts);
        case '+':
            return primary(ts);
        case name:
            return names.get(t.name);
        default:
            error("primary expected");
            return 0;
    }
}

double get_factorial(double val)
{
    int result = 1;

    for (int i = 1; i <= val; ++i)
    {
        result *= i;
    }

    return result;
}

void calculate()
{
    while (std::cin)
    try 
    {
        std::cout << prompt;

        Token t = ts.get();

        while (t.kind == print)
        {
            t = ts.get();
        }

        while (t.kind == lower_help || t.kind == upper_help)
        {
            show_help();
            t = ts.get();
        }

        if (t.kind == quit)
        {
            return;
        }
        
        ts.putback(t);
        std::cout << result << statement() << std::endl;
    }
    catch (exception& e)
    {
        std::cerr << e.what() << std::endl;
        clean_up_mess();
    }
}

void clean_up_mess()
{
    ts.ignore(print);
}

double statement()
{
    Token t = ts.get();

    switch (t.kind)
    {
        case let:
            return set_variable();
        case my_const:
            return set_variable(true);
        case my_sqrt:
            return sqrt_pattern();
        case my_pow:
            return pow_pattern();
        case name:
            {
                char ch;
                std::cin >> ch;
                std::cin.putback(ch);
                
                if (ch == '=')
                {
                    ts.putback(t);
                    return set_variable(false, false);
                }
            }
        default:
            ts.putback(t);
            return expression(ts);
    }
}

double set_variable(bool is_const, bool is_declaration)
{
    Token t = ts.get();

    if (t.kind != name)
    {
        error("name expected in declaration/assignment");
    }

    string var_name = t.name;

    Token t2 = ts.get();

    if (t2.kind != '=')
    {
        error("= missing in declaration/assignment of ", var_name);
    }

    double d = expression(ts);

    if (is_declaration)
    {
        names.declare(var_name, d, is_const);
    }
    else
    {
        names.set(var_name, d);
    }

    return d;
}

double sqrt_pattern()
{
    Token t = ts.get();

    switch (t.kind)
    {
        case '(':
            {
                double d = expression(ts);
                t = ts.get();

                if (t.kind != ')')
                {
                    error("')' expected");
                }

                if (d < 0)
                {
                    error("sqrt by negative");
                }

                return sqrt(d);
            }
        default:
            error("sqrt_pattern expected");
            return 0;
    }
}

double pow_pattern()
{
    Token t = ts.get();

    switch (t.kind)
    {
        case '(':
            {
                double x = expression(ts);
                t = ts.get();

                if (t.kind != ',')
                {
                    error("',' expected");
                }

                double i = expression(ts);
                t = ts.get();

                if (t.kind != ')')
                {
                    error("')' expected");
                }
                return pow(x, i);
            }
        default:
            error("pow_pattern expected");
            return 0;
    }
}

void show_help()
{
    std::cout << "Please discover it by yourself. " << std::endl << prompt;
}
