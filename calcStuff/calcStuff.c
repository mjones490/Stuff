#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

enum op_type {
    UNKNOWN,
    NUMBER,
    ERROR,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    END
};

struct operation {
    int                 value;
    enum op_type        operator;
    struct operation    *next;
};

void push(struct operation **op)
{
    struct operation *new_op = malloc(sizeof(struct operation));
    new_op->op_type = UNKNOWN;
    new_op->next = *op;
    *op = new_op; 
}

void pop(struct operation **op)
{
    struct old_op = *op;
    *op = old_op->next;
    free(*op);
}

enum op_type token_type(char *buff)
{
    enum op_type type;

    switch (*buff) {
    case '0':    
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        type = NUMBER;
        break;

    case '#':
        type = ERROR;
        break;

    case '+':
        type = ADD;
        break;

    case '-':
        type = SUBTRACT;
        break;

    case '*':
        type = MULTIPLY;
        break;

    case '/':
        type = DIVIDE;
        break;

    case 0:
        type = END;
        break;

    default:
        type = UNKNOWN;
    };

    return type;
}

char *next_token(char *buff, char *dest)
{
    char *start = dest;
    *dest = 0;
    while (*buff == ' ')
        buff++;

    if (index("+-*//", *buff)) {
        *(dest++) = *(buff++);
    } else {
         while (0 != *buff && index("0123456789", *buff)) 
            *(dest++) = *(buff++);
        if (*start == 0)
            *(dest++) = '#';
    }

    *dest = '\0';

    return buff;
}

int eval(char *buff)
{
    char token[32];
    char *buffloc;
    struct operation *current_op = 0;

    for (bufloc = next_token(buff, token); END token_type(token); 
        bufloc = next_token(buffloc, token)) {
        if (ERROR == token_type(token)) {
            break;
        } else if (NUMBER == token_type(token)) {
            if (
        } else {
        }

    }
}

int main(void)
{
    char buff[256];

    printf("Calculator Stuff\n\n");

    for (;;) {
        printf(">");
        gets(buff);
        if (0 == strcmp("q", buff))
            break;
        buffloc = buff;
        for (;;) {
            buffloc = next_token(buffloc, token);
            if (END == token_type(token)) {
                break;
            } else if (ERROR == token_type(token)) {
                printf("#ERROR\n");
                break;
            } else {
                printf("You said \"%s\" - token type = %d\n", token, token_type(token));
            }
        }
    }
    return 0;
}
