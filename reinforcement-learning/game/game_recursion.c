/* Some Fun   */
/* Martin v. M. very old code ...  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float valueRecursion(int);

int score; /* Globals */
char in[20];

int user(void)
{
    int i = 0;
    while (i != 1 && i != 2)
    {
        printf("We are at %-2d, add 1 or 2 ? ", score);
        fgets(in, 10, stdin);
        i = atoi(in);
    };
    return (i);
}

int computer(int state)
{
    if (state % 3 == 1)
        return (1);
    if (state % 3 == 0)
        return (2);
    else
        return (rand() % 2 + 1);
}

// int basic_computer(void)
// {
//     int i;
//     if (score % 3 == 1)
//         i = 1;
//     else if (score % 3 == 0)
//         i = 2;
//     else
//         i = rand() % 2 + 1;
//     printf("We are at %-2d Computer adds %d \n", score, i);
//     return (i);
// }

// globals
float valueMap[20] = {};

const float discount = 0.5;

int valueIndexOf(int state)
{
    return state - 1;
}

void printValueMap(void)
{
    // print entire array
    printf("Value Map: ");
    for (int i = 0; i < (sizeof(valueMap) / sizeof(valueMap[0])); i++)
    {
        printf("%.2f,", valueMap[i]);
    }
    printf("\n\n");
}

int R(int n)
{
    switch (n)
    {
    case 18:
        return -100;
    case 19:
        return -100;
    case 20:
        return 100;
    default:
        return 0;
    }
}

int iterations = 7; // choose for now but really use recursion

int intelligentComputer(int state)
{
    // find the move that lands us in the cell with highest value
    int i;
    if (valueMap[valueIndexOf(state) + 1] > valueMap[valueIndexOf(state) + 2])
        i = 1;
    else
        i = 2;
    printf("We are at %-2d Computer adds %d \n", state, i);
    return i;
}

// how many for the move?
int move_policy(int state)
{
    if (state == 20)
        return 0;

    // printf("State %i, computer will move: %i\n", state, computer(state));
    state += computer(state);

    // printf("move 1 gives value: %f\n", valueMap[valueIndexOf(state) + 1]);
    // printf("move 2 gives value: %f\n", valueMap[valueIndexOf(state) + 2]);
    state += intelligentComputer(state);

    return state;
}

void valueIteration(void)
{
    for (int i = 0; i < iterations; i++)
    {
        for (int state = 1; state <= 20; state++)
        {
            // printf("--State %i--\n", n + 1);

            valueMap[valueIndexOf(state)] = R(state) + discount * valueMap[valueIndexOf(move_policy(state))];

            if (state > 15)
            {
                printf("value(state %i) = %i + discount * %f\n\n", state, R(state), valueMap[valueIndexOf(move_policy(state))]);
            }
        }
        // printValueMap();
    }
}

// float valueRecursion(int state)
// {
//     if (state > 20)
//         return 0;

//     // move policy for self and opponent
//     float maxNextValue, opponentMove1_value, opponentMove2_value, move1_value, move2_value;
//     opponentMove1_value = valueRecursion(state + 1);
//     opponentMove2_value = valueRecursion(state + 2);

//     if (opponentMove1_value > opponentMove2_value)
//     {
//         move1_value = valueRecursion(state + 2);
//         move2_value = valueRecursion(state + 3);
//         if (move1_value >= move2_value)
//             maxNextValue = move1_value;
//         else
//             maxNextValue = move2_value;
//     }
//     else
//     {
//         move1_value = valueRecursion(state + 3);
//         move2_value = valueRecursion(state + 4);
//         if (move1_value >= move2_value)
//             maxNextValue = move1_value;
//         else
//             maxNextValue = move2_value;
//     }

//     valueMap[valueIndexOf(state)] = R(state) + discount * maxNextValue;
//     printValueMap();
//     return valueMap[valueIndexOf(state)];
// }

int main(void)
{
    // prepare computer
    // valueRecursion(1);
    valueIteration(); // TODO: write to file
    printValueMap();

    srand(time(NULL));
    int i;
    printf("Who says 20 first! \n \n");
    score = 0;
    i = 0;
    while (i != 1 && i != 2)
    {
        printf("Who goes first: you=1 computer=2 ? ");
        fgets(in, 10, stdin);
        i = atoi(in);
    };
    if (i == 2)
        score = intelligentComputer(score);
    // score = computer(score);
    while (score <= 20)
    {
        score = score + user();
        if (score >= 20)
        {
            printf(" YOU WIN !!\n ");
            break;
        };
        // score = score + computer(score);
        score = score + intelligentComputer(score);
        if (score >= 20)
        {
            printf(" I WIN !! \n ");
            break;
        };
    };
    return (0);
};
