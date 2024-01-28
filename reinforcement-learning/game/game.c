/* Some Fun   */
/* Martin v. M. very old code ...  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// globals
int score;
char in[20];

float valueMap[21] = {};
const float discount = 0.5;
int iterations = 200; // choose

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

int intelligentComputer(int state)
{
    // find the move that lands us in the cell with highest value
    int i;
    if (valueMap[state + 1] > valueMap[state + 2])
        i = 1; // move 1 is better than move 2
    else
        i = 2; // move 2 is better than move 1

    printf("We are at %-2d Computer adds %d \n", state, i);
    return i;
}

// how many for the move?
int move_policy(int state)
{
    // printf("State %i, computer will move: %i\n", state, computer(state));
    state += computer(state);

    // printf("move 1 gives value: %f\n", valueMap[state + 1]);
    // printf("move 2 gives value: %f\n", valueMap[state + 2]);
    state += intelligentComputer(state);

    return state;
}

void printValueMap(void)
{
    // print entire array

    FILE *f_out = fopen("VALUES", "w");
    fprintf(f_out, "State\tValue\tMove Policy\n");

    printf("Value Map: ");
    for (int state = 0; state < (sizeof(valueMap) / sizeof(valueMap[0])); state++)
    {
        printf("%f,", valueMap[state]);
        fprintf(f_out, "%i\t\t%4.3f\t\t%i\n", state, valueMap[state], intelligentComputer(state));
    }
    printf("\n\n");
    fclose(f_out);
}

void valueIteration(void)
{
    for (int i = 0; i < iterations; i++)
    {
        for (int state = 0; state <= 20; state++)
        {
            valueMap[state] = R(state) + discount * valueMap[move_policy(state)];
            // printf("value(state %i) = %i + discount * %f\n\n", state, R(state), valueMap[move_policy(state)]);
        }
        // printValueMap();
    }
}

int main(void)
{
    // prepare computer
    valueIteration();
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
        score = score + intelligentComputer(score);
        if (score >= 20)
        {
            printf(" I WIN !! \n ");
            break;
        };
    };
    return (0);
};
