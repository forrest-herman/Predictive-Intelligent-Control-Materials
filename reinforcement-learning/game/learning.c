#include <stdio.h>
#include <stdlib.h>

// options
// [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15] [16] [17] [18] [19] [20]   -- states
// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [  ] [  ] [  ] [  ] [  ] [  ] [  ] [  ] [10] [10] [-10]  -- reward

// actions
// + 1
// + 2

// int count = 0;

// void move(int *count)
// {
//     float move1Val = reward(*count + 1); // move by 1
//     float move2Val = reward(*count + 2); // move by 2

//     if (move1Val > move2Val)
//     {
//         *count = *count + 1;
//     }
// }

// float reward(int count)
// {
//     float tmp1, tmp2, max;
//     float gamma = pow(.5, count);

//     // opponent moves
//     // count = count + computer();

//     if (count == 20)
//     {
//         return 10;
//     }
//     else if (count == 19)
//     {
//         return 10;
//     }
//     else if (count == 18)
//     {
//         return -10;
//     }
//     else
//     {
//         tmp1 = reward(count + 1);
//         tmp2 = reward(count + 2);
//         if (tmp1 > tmp2)
//             max = tmp1;
//         else
//             max = tmp2;
//         return gamma * max;
//     }
// }

// globals
float valueMap[20] = {};

const float gamma = 0.5;

int R(int n)
{
    switch (n)
    {
    case 20:
        return 10;
    case 18:
        return -10;
    case 19:
        return -10;
    default:
        return 0;
    }
}

int iterations = 1000; // choose

int computer(int count)
{
    if (count % 3 == 1)
        return (1);
    if (count % 3 == 0)
        return (2);
    else
        return (rand() % 2 + 1);
}

// how many for the move?
int move_policy(int state)
{
    if (state == 20)
        return 0;

    printf("computer will move: %i\n", computer(state));
    state = state + computer(state);

    printf("move 1 gives value: %f\n", valueMap[state + 1]);
    printf("move 2 gives value: %f\n", valueMap[state + 2]);
    // find the move that lands us in the cell with highest value
    if (valueMap[state + 1] > valueMap[state + 2])
        return 1;
    printf("move 2 is best\n");
    return 2;
}

void valueIteration(void)
{
    for (int i = 0; i < iterations; i++)
    {
        for (int n = 1; n < sizeof(valueMap) / sizeof(float) + 1; n++)
        {
            printf("--State %i--\n", n);

            valueMap[n] = R(n) + gamma * valueMap[n + move_policy(n)];

            printf("adjusted value of %i: %f\n\n", n, valueMap[n]);
        }
    }
}

int main(void)
{
    valueIteration();

    return 0;
}

// v(n) = R(n) + gamma * v(n + 1)
// iterate through
// alternatively use recursion?

// if (i == 20)
//    reward = 10
// if (i == 19)
//    reward = -10
// if (i == 18)
//    reward = -10
