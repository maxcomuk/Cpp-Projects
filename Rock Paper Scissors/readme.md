# Rock-Paper-Scissors Game Documenation

![License](https://github.com/maxcomuk/Cpp-Projects/blob/main/LICENSE)

## Step 1
- we Include all the necessary libraries we will use from visual studio 2022

```
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
```

## Step 2
- We create 3 constant variables that will represent rock, paper and scissors. The reason i do this, is so that we dont have to worry about spelling issues later on and it can end up being a bit ambigous having writing them again

- One thing i learnt is that we cant use constexpr because std::string is not a literal type therefore the compiler wont know the value at compile time wich ruins the purpose of constexpr and it can even lead to some compilers failing to build because of this, so definatly something we will avoid by simply changing constexpr to a const string
```
const std::string rock = "Rock";
const std::string paper = "Paper";
const std::string scissors = "Scissors";
```

## Step 3
- Before starting this project i scanned through the project example wich was created here --> https://www.geeksforgeeks.org/cpp/rock-paper-scissor-game-in-cpp/ and after scanning through the project example i came to a conclusion that we will need to create a function that will basically generate a random response that we will later compare to get the winner.

- I learnt a nice optimization technique that we can do to skip loads of if statements using an array to peform a lookup later on therefore removing the need to do an if statement for every possible outcome

- Ultimatly all this function does is generate a random number from 0 to 2 (0, 1, 2) and then peforming a quick index lookup through the objects array wich we return the value of to the function that is calling this function eg the main entry function
```
std::string generate_computers_move()
{
	const std::string objects[] = { rock, paper, scissors };

	int randomNum = (rand() % 3);
	return objects[randomNum];
}
```
