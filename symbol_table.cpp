# include "symbol_table.h"

double Symbol_table::get(std::string s)
{
    for (const Variable& v : var_table)
    {
        if (v.name == s)
        {
            return v.value;
        }
    }

    error("get: undefined variable", s);
    return 0;
}

void Symbol_table::set(std::string s, double d)
{
    for (Variable& v : var_table)
    {
        if (v.name == s)
        {
            if (!v.is_const)
            {
                v.value = d;
                return;
            }
            else
            {
                error(v.name, " is const");
            }
        }
    }

    error("set: undefined variable", s);
}

bool Symbol_table::is_declared(std::string var)
{
    for (const Variable& v : var_table)
    {
        if (v.name == var)
        {
            return true;
        }
    }

    return false;
}

double Symbol_table::declare(std::string var, double val, bool is_const)
{
    if (is_declared(var))
    {
        error(var, " declared twice");
    }

    var_table.push_back(Variable(var, val, is_const));

    return val;
}
