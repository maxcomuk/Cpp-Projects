# Scientific-Calculator Documentation

### What is a scientific calculator and what is the difference from a normal calculator?
A scientific calculator allows multiplication, division, addition and subtracting just like a normal calculator as well as following BIDMAS order. however a scientific calculator also gives you extra functionalities such as sin, cos, tan and log, which is not supported by a normal calculator.

The goal of this project is to support normal calculator functions while also including scientific calculator functions such as sin, cos, tan and log.

## Step 1: Including Libraries
```
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <cmath>
```

## Step 2: Defining Enums & Struct Data
We must first create a struct (named Token) to give each character from the user's equation a type and a value. Essentially, we are defining if a character is an operator (like +) or a number. We need to differentiate between the types of characters we are dealing with—whether it is a parenthesis, a number, etc. This is why we also create an enum named TokenType containing all possible character types.

In theory, we use the Token struct as a template to store each character's value and type in a vector inside our calculator class below. The main goal is to define the character type so we can separate it later into lists of numbers and operators. Note: when I say operators I mean +, -, *, /, and ().
```
enum tokenType
{
	NUMBER,
	OPERATOR,
	FUNCTION,
	PARENTHESIS
};

struct token
{
	tokenType type;
	std::string value;
};
```
