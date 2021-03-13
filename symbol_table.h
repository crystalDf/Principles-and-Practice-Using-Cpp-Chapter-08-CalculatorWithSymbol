# pragma once
# include "std_lib_facilities.h"

class Variable
{
public:
    Variable(std::string var, double val, bool var_is_const = false) : name { var }, value { val }, is_const { var_is_const } { }
    
    std::string name;
    double value;
    bool is_const;
};

class Symbol_table
{
public:
    double get(std::string s);
    void set(std::string s, double d);
    bool is_declared(std::string var);
    double declare(std::string var, double val, bool is_const = false);
private:
    vector<Variable> var_table;
};
