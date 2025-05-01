#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STACK_SIZE 100
#define WIDTH 41    // x from -20 to 20
#define HEIGHT 21   // y from -10 to 10
#define MAX_TOKENS 100

typedef struct {
    double data[STACK_SIZE];
    int top;
} Stack;

// Stack operations
void push(Stack* s, double val) {
    s->data[++(s->top)] = val;
}

double pop(Stack* s) {
    return s->data[(s->top)--];
}

// Evaluate a postfix expression for a given x
double evaluate(char* expr[], int len, double x) {
    Stack s;
    s.top = -1;

    for (int i = 0; i < len; i++) {
        if (strcmp(expr[i], "x") == 0) {
            push(&s, x);
        } else if (strcmp(expr[i], "+") == 0) {
            double b = pop(&s), a = pop(&s);
            push(&s, a + b);
        } else if (strcmp(expr[i], "-") == 0) {
            double b = pop(&s), a = pop(&s);
            push(&s, a - b);
        } else if (strcmp(expr[i], "*") == 0) {
            double b = pop(&s), a = pop(&s);
            push(&s, a * b);
        } else if (strcmp(expr[i], "/") == 0) {
            double b = pop(&s), a = pop(&s);
            push(&s, a / b);
        } else {
            push(&s, atof(expr[i])); // constant number
        }
    }

    return pop(&s);
}

// Plotting function
void plot(char* expr[], int len) {
    char graph[HEIGHT][WIDTH];
    int x_axis = HEIGHT / 2;
    int y_axis = WIDTH / 2;

    // Initialize grid
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            graph[i][j] = ' ';

    // Draw axes
    for (int i = 0; i < WIDTH; i++) graph[x_axis][i] = '-';
    for (int i = 0; i < HEIGHT; i++) graph[i][y_axis] = '|';
    graph[x_axis][y_axis] = '+';

    // Plot points
    for (int px = -20; px <= 20; px++) {
        double x = (double)px;
        double y = evaluate(expr, len, x);
        int plot_x = px + y_axis;
        int plot_y = x_axis - (int)(round(y));

        if (plot_x >= 0 && plot_x < WIDTH && plot_y >= 0 && plot_y < HEIGHT)
            graph[plot_y][plot_x] = '*';
    }

    // Print plot
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf("%c", graph[i][j]);
        printf("\n");
    }
}

int main() {
    char* expression[MAX_TOKENS];
    char input[1000];
    int tokenCount = 0;

    printf("Enter postfix expression using space-separated tokens (e.g., x x * 2 +):\n> ");
    fgets(input, sizeof(input), stdin);

    // Tokenize input string
    char* token = strtok(input, " \n");
    while (token != NULL && tokenCount < MAX_TOKENS) {
        expression[tokenCount] = malloc(strlen(token) + 1);
        strcpy(expression[tokenCount], token);
        tokenCount++;
        token = strtok(NULL, " \n");
    }

    printf("\nPlotting the expression using ASCII art:\n\n");
    plot(expression, tokenCount);

    // Free memory
    for (int i = 0; i < tokenCount; i++) {
        free(expression[i]);
    }

    return 0;
}
