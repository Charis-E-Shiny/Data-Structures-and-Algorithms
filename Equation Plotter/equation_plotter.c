#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100
#define WIDTH 70
#define HEIGHT 20

typedef struct {
    int top;
    char items[MAX];
} CharStack;

typedef struct {
    int top;
    double items[MAX];
} DoubleStack;

// Stack for chars (infix to postfix)
void charInit(CharStack *s) { s->top = -1; }
int charIsEmpty(CharStack *s) { return s->top == -1; }
void charPush(CharStack *s, char c) { if (s->top < MAX - 1) s->items[++(s->top)] = c; }
char charPop(CharStack *s) { if (!charIsEmpty(s)) return s->items[(s->top)--]; return '\0'; }
char charPeek(CharStack *s) { if (!charIsEmpty(s)) return s->items[s->top]; return '\0'; }

// Stack for doubles (evaluation)
void doubleInit(DoubleStack *s) { s->top = -1; }
int doubleIsEmpty(DoubleStack *s) { return s->top == -1; }
void doublePush(DoubleStack *s, double v) { if (s->top < MAX - 1) s->items[++(s->top)] = v; }
double doublePop(DoubleStack *s) { if (!doubleIsEmpty(s)) return s->items[(s->top)--]; return 0; }

// Operator precedence
int precedence(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
    }
    return 0;
}

// Check if operator
int isOperator(char c) {
    return c=='+'||c=='-'||c=='*'||c=='/'||c=='^';
}

// Convert infix to postfix (supports single-char vars and digits)
void infixToPostfix(char *infix, char *postfix) {
    CharStack s;
    charInit(&s);
    int i=0, j=0;
    while(infix[i] != '\0') {
        char c = infix[i];
        if (isspace(c)) {
            i++;
            continue; // skip spaces
        }
        if (isalnum(c)) {
            postfix[j++] = c; // single character token
        } else if (c=='(') {
            charPush(&s, c);
        } else if (c==')') {
            while(!charIsEmpty(&s) && charPeek(&s) != '(') {
                postfix[j++] = charPop(&s);
            }
            if(!charIsEmpty(&s)) charPop(&s); // pop '('
        } else if (isOperator(c)) {
            while(!charIsEmpty(&s) && isOperator(charPeek(&s)) &&
                ((precedence(charPeek(&s)) > precedence(c)) ||
                (precedence(charPeek(&s)) == precedence(c) && c != '^'))) {
                postfix[j++] = charPop(&s);
            }
            charPush(&s, c);
        } else {
            // ignore invalid characters
        }
        i++;
    }
    while(!charIsEmpty(&s)) {
        postfix[j++] = charPop(&s);
    }
    postfix[j] = '\0';
}

// Evaluate postfix for given x; postfix contains digits, 'x', and operators
double evaluatePostfix(char *postfix, double xVal) {
    DoubleStack s;
    doubleInit(&s);
    for(int i=0; postfix[i] != '\0'; i++) {
        char c = postfix[i];
        if (isdigit(c)) {
            doublePush(&s, (double)(c - '0'));
        } else if (c == 'x' || c == 'X') {
            doublePush(&s, xVal);
        } else if (isOperator(c)) {
            double op2 = doublePop(&s);
            double op1 = doublePop(&s);
            switch(c) {
                case '+': doublePush(&s, op1 + op2); break;
                case '-': doublePush(&s, op1 - op2); break;
                case '*': doublePush(&s, op1 * op2); break;
                case '/':
                    if (op2 != 0)
                        doublePush(&s, op1 / op2);
                    else
                        doublePush(&s, 0); // avoid division by zero
                    break;
                case '^': doublePush(&s, pow(op1, op2)); break;
            }
        }
    }
    return doublePop(&s);
}

// Plot ascii graph of the function y=f(x)
void plotGraph(char *postfix) {
    double x_min = -10;
    double x_max = 10;
    double y_min = -10;
    double y_max = 10;

    char plot[HEIGHT][WIDTH];

    // Initialize plot with spaces
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            plot[i][j] = ' ';

    // Draw axes
    int x_axis_row = (int)((0 - y_min) / (y_max - y_min) * (HEIGHT-1));
    int y_axis_col = (int)((0 - x_min) / (x_max - x_min) * (WIDTH-1));

    // Clamp axis positions
    if (x_axis_row < 0 || x_axis_row >= HEIGHT) x_axis_row = HEIGHT - 1;
    if (y_axis_col < 0 || y_axis_col >= WIDTH) y_axis_col = 0;

    for (int col = 0; col < WIDTH; col++) {
        plot[x_axis_row][col] = '-';
    }
    for (int row = 0; row < HEIGHT; row++) {
        plot[row][y_axis_col] = '|';
    }
    plot[x_axis_row][y_axis_col] = '+';

    // Calculate and plot function points
    for(int col = 0; col < WIDTH; col++) {
        double x = x_min + (x_max - x_min) * col / (WIDTH - 1);
        double y = evaluatePostfix(postfix, x);
        if (y >= y_min && y <= y_max && !isnan(y) && !isinf(y)) {
            int row = (int)round((y_max - y) / (y_max - y_min) * (HEIGHT - 1));
            if (row >= 0 && row < HEIGHT) {
                if (plot[row][col] == ' ' || plot[row][col] == '-') // don't overwrite axis y marker
                    plot[row][col] = '*'; // mark function point
            }
        }
    }

    // Print the plot
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(plot[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Enter a function of x (e.g. 3*x+2): ");
    if (!fgets(infix, sizeof(infix), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    infix[strcspn(infix, "\r\n")] = 0; // Remove newline characters

    infixToPostfix(infix, postfix);

    printf("Postfix expression: %s\n", postfix);

    plotGraph(postfix);

    return 0;
}
