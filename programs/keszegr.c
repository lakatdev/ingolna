#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/constants.h"

typedef struct {
    unsigned short type;
    unsigned short argsize;
    unsigned int args;
} instruction;

int* variables;
int** arrays;

instruction* instructions;
char* arguments;

int execution_state = 0;

void execute(instruction*);

/*
    Convert byte pointer to int pointer
*/
int* parse_int_pointer(char* bytes) {
    return (int*)bytes;
}

/*
    Finds the end of the "if" statement
*/
int findEnd() {
    int newifs = 1;
    int seek_position = execution_state;

    while (newifs > 0) {
        seek_position++;

        if (instructions[seek_position].type >= IF_M_NN && instructions[seek_position].type <= IF_ME_VV) {
            newifs++;
        }

        if (instructions[seek_position].type == END) {
            newifs--;
        }
    }
    return seek_position;
}

/*
    Main function
    Reads input file and sends it for execution
*/
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Provide sufficient amount of arguments!\n");
        return 1;
    }

    unsigned int length;
    FILE* fptr = fopen(argv[1], "rb");
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    rewind(fptr);

    char* buffer = malloc(length);
    fread(buffer, length, 1, fptr);
    fclose(fptr);

    int* num_instructions = parse_int_pointer(buffer);
    int* num_vars = parse_int_pointer(&buffer[4]);
    int* num_arrays = parse_int_pointer(&buffer[8]);

    variables = malloc(sizeof(int) * *num_vars);
    arrays = malloc(sizeof(int*) * *num_arrays);

    instructions = malloc(*num_instructions * sizeof(instruction));
    memcpy(instructions, &buffer[sizeof(int) * 3], *num_instructions * sizeof(instruction));

    arguments = malloc(length - sizeof(int) * 3 - *num_instructions * sizeof(instruction));
    memcpy(arguments, &buffer[sizeof(int) * 3 + *num_instructions * sizeof(instruction)], length - sizeof(int) * 3 - *num_instructions * sizeof(instruction));

    for (execution_state = 0; execution_state < *num_instructions; execution_state++) {
        execute(&instructions[execution_state]);
    }

    return 0;
}

/*
    Executes given instruction
*/
void execute(instruction* instruction) {
    switch (instruction->type) {
        case (SET_N): {
            int* var = parse_int_pointer(&arguments[instruction->args]);
            int* val = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*var] = *val;
            break;
        }
        case (SET_V): {
            int* var = parse_int_pointer(&arguments[instruction->args]);
            int* val = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*var] = variables[*val];
            break;
        }
        case (IF_M_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 > *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_M_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 > variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_M_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] > *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_M_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] > variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_L_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 < *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_L_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 < variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_L_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] < *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_L_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] < variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_E_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 == *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_E_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 == variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_E_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] == *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_E_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] == variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_D_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 != *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_D_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 != variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_D_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] != *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_D_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] != variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_LE_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 <= *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_LE_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 <= variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_LE_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] <= *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_LE_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] <= variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_ME_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 >= *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_ME_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(*c1 >= variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_ME_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] >= *c2)) {
                execution_state = findEnd();
            }

            break;
        }
        case (IF_ME_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            if (!(variables[*c1] >= variables[*c2])) {
                execution_state = findEnd();
            }

            break;
        }
        case (ADD_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 + *c3;

            break;
        }
        case (ADD_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 + variables[*c3];

            break;
        }
        case (ADD_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] + *c3;

            break;
        }
        case (ADD_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] + variables[*c3];

            break;
        }
        case (ARRAY_N): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            arrays[*c1] = malloc(sizeof(int) * *c2);

            break;
        }
        case (ARRAY_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            arrays[*c1] = malloc(sizeof(int) * variables[*c2]);

            break;
        }
        case (FREE_ARRAY): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);

            free(arrays[*c1]);

            break;
        }
        case (SUB_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 - *c3;

            break;
        }
        case (SUB_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 - variables[*c3];

            break;
        }
        case (SUB_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] - *c3;

            break;
        }
        case (SUB_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] - variables[*c3];

            break;
        }
        case (MUL_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 * *c3;

            break;
        }
        case (MUL_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 * variables[*c3];

            break;
        }
        case (MUL_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] * *c3;

            break;
        }
        case (MUL_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] * variables[*c3];

            break;
        }
        case (DIV_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 / *c3;

            break;
        }
        case (DIV_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 / variables[*c3];

            break;
        }
        case (DIV_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] / *c3;

            break;
        }
        case (DIV_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] / variables[*c3];

            break;
        }
        case (PRINT_NUM_N): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            printf("%d", *c1);

            break;
        }
        case (PRINT_NUM_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            printf("%d", variables[*c1]);
            
            break;
        }
        case (PRINT_ASCII_N): {
            char* c1 = &arguments[instruction->args];
            printf("%c", *c1);

            break;
        }
        case (PRINT_ASCII_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            printf("%c", variables[*c1]);
            
            break;
        }
        case (PRINT_STRING): {
            char* str = malloc(instruction->argsize + 1);
            memcpy(str, &arguments[instruction->args], instruction->argsize);
            str[instruction->argsize] = '\0';
            printf(str);
            free(str);

            break;
        }
        case (INPUT_NUM): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            scanf("%d", &variables[*c1]);
            break;
        }
        case (INPUT_ASCII): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            scanf(" %c", &variables[*c1]);
            break;
        }
        case (MOD_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 % *c3;

            break;
        }
        case (MOD_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = *c2 % variables[*c3];

            break;
        }
        case (MOD_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] % *c3;

            break;
        }
        case (MOD_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = variables[*c2] % variables[*c3];

            break;
        }
        case (ARRSET_NN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            arrays[*c1][*c2] = *c3;

            break;
        }
        case (ARRSET_NV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            arrays[*c1][*c2] = variables[*c3];

            break;
        }
        case (ARRSET_VN): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            arrays[*c1][variables[*c2]] = *c3;

            break;
        }
        case (ARRSET_VV): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            arrays[*c1][variables[*c2]] = variables[*c3];

            break;
        }
        case (ARRGET_N): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = arrays[*c2][*c3];

            break;
        }
        case (ARRGET_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);
            int* c3 = parse_int_pointer(&arguments[instruction->args + 8]);

            variables[*c1] = arrays[*c2][variables[*c3]];

            break;
        }
        case (INC_N): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*c1] += *c2;
            break;
        }
        case (INC_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*c1] += variables[*c2];
            break;
        }
        case (DEC_N): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*c1] -= *c2;
            break;
        }
        case (DEC_V): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);
            int* c2 = parse_int_pointer(&arguments[instruction->args + 4]);

            variables[*c1] -= variables[*c2];
            break;
        }
        case (JUMP): {
            int* c1 = parse_int_pointer(&arguments[instruction->args]);

            execution_state = *c1 - 1;
            break;
        }
        case (EXEC): {
            char* str = malloc(instruction->argsize + 1);
            memcpy(str, &arguments[instruction->args], instruction->argsize);
            str[instruction->argsize] = '\0';
            system(str);
            free(str);

            break;
        }
    }
}