#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GATES 100
#define MAX_INPUTS 60

char** allocate_gate_list() {

    char** gate_list = (char**) malloc (sizeof(char*) * MAX_GATES);

    // Checking if the memory was allocated correctly
    if (!gate_list) {
        printf("Error allocating memory for gate_list\n");
        exit(1);
    }

    for (int i = 0; i < MAX_GATES; i++) {
        gate_list[i] = (char*) malloc (2* sizeof(char) * MAX_INPUTS);

        // If the memory was not allocated correctly, free all the previous memory
        if (!gate_list[i]) {
            for (int j = 0; j < i; j++) {
                free(gate_list[j]);
            }
            free(gate_list);
            printf("Error allocating memory for gate_list[%d]\n", i);
            exit(1);   
        }
    }

    return gate_list;

}

void reading_gate_list(char** gate_list, FILE* input_file, size_t* number_of_gates) {

    char* line = malloc(2 * sizeof(char) * MAX_INPUTS);
    size_t len = 0;

    // Read the newline
    getline(&line, &len, input_file);

    while (getline(&line, &len, input_file) != -1) {
        strcpy(gate_list[*number_of_gates], line);
        (*number_of_gates)++;
    }

}

char* and_gate(int inputs[], size_t len) {

    char* formula = malloc((20 * len + 2) * sizeof(char));
    size_t index = 0;

    formula[index++] = '(';

    for (size_t i = 0; i < len - 1; i++) {
        formula[index++] = '-';
        index += sprintf(&formula[index], "%d|", inputs[i]);
    }

    index += sprintf(&formula[index], "%d", inputs[len - 1]);

    formula[index++] = ')';
    formula[index++] = '&';

    for (size_t i = 0; i < len - 1; i++) {
        index += sprintf(&formula[index], "(%d|-%d)", inputs[i], inputs[len - 1]);
        if (i < len - 2) {
            formula[index++] = '&';
        }
    }

    formula[index] = '\0';

    return formula;
}

char* or_gate(int inputs[], size_t len) {

    char* formula = malloc((20 * len + 2) * sizeof(char));
    size_t index = 0;

    formula[index++] = '(';

    for (size_t i = 0; i < len - 1; i++) {
        index += sprintf(&formula[index], "%d|", inputs[i]);
    }

    index += sprintf(&formula[index], "-%d", inputs[len - 1]);

    formula[index++] = ')';
    formula[index++] = '&';

    for (size_t i = 0; i < len-1; i++) {
        index += sprintf(&formula[index], "(-%d|%d)", inputs[i], inputs[len - 1]);
        if (i < len - 1) {
            formula[index++] = '&';
        }
    }

    formula[index-1] = '\0';

    return formula;
}

char* not_gate(int input, int output) {
    char* not_formula = malloc(MAX_INPUTS * sizeof(char));

    sprintf(not_formula, "(-%d|-%d)&(%d|%d)", input, output, input, output);

    return not_formula;
}

void get_inputs_from_gate(char* gate, int inputs[], int* j) {
    size_t index = 0;

    for (size_t i = 0; i < strlen(gate); i++) {
        if (gate[i] >= '0' && gate[i] <= '9') {
            int num = 0;
            // Keep reading digits until a non-digit character is encountered
            while (gate[i] >= '0' && gate[i] <= '9') {
                num = num * 10 + (int)(gate[i] - '0');
                i++;
            }
            inputs[index] = num;
            index++;
        }
    }
    *j = index;
}

char* cnf_reduction(char** gate_list, size_t number_of_gates) {

     int inputs[MAX_INPUTS];
    int j = 0;

    char* cnf_result = malloc(100 * sizeof(char) * MAX_INPUTS);
    strcpy(cnf_result, "");


    for (int i = 0; i < number_of_gates; i++) {
        get_inputs_from_gate(gate_list[i], inputs, &j);
        switch (gate_list[i][0])
        {
        case 'N':
            strcat(cnf_result, not_gate(inputs[0], inputs[1]));
            cnf_result[strlen(cnf_result)] = '&';
            break;
        case 'O':
            strcat(cnf_result, or_gate(inputs, j));
            cnf_result[strlen(cnf_result)] = '&';
            break;
        case 'A':
            strcat(cnf_result, and_gate(inputs, j));
            cnf_result[strlen(cnf_result)] = '&';
            break;   
        default:
            break;
        }
    }
    cnf_result[strlen(cnf_result)-1] = '\0';

    return cnf_result;
    
}

int count_clauses(char* cnf_result) {
    int clauses = 0;

    for (size_t i = 0; i < strlen(cnf_result); i++) {
        if (cnf_result[i] == '(') {
            clauses++;
        }
    }

    return clauses;
}

void write_literals(FILE* output_file, char* cnf_result) {
    size_t i = 0;

    while (cnf_result[i] != '\0') {
        if (cnf_result[i] == '(') {
            i++;
            while (cnf_result[i] != ')') {
                if (cnf_result[i] == '|') {
                    i++;
                }
                int literal = 0;

                // Handle negative literals
                if (cnf_result[i] == '-') {
                    i++;
                    fprintf(output_file, "-");
                }
                    while (cnf_result[i] >= '0' && cnf_result[i] <= '9') {
                        literal = literal * 10 + (cnf_result[i] - '0');
                        i++;
                    }
                    fprintf(output_file, "%d ", literal);
                
                while (cnf_result[i] == ' ' || cnf_result[i] == '|') {
                    i++;
                }
            }
            fprintf(output_file, "0\n");
        } else {
            i++;
        }
    }
}

void destroy_gate_list(char*** gate_list, size_t number_of_gates) {
    for (size_t i = 0; i < number_of_gates; i++) {
        free((*gate_list)[i]);
    }
    free(*gate_list);
}


int main(int argc, char *argv[]) {

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");

    char** gate_list = allocate_gate_list();

    size_t number_of_inputs = 0, last_gate = 0, number_of_gates = 0;

    fscanf(input_file, "%zu", &number_of_inputs);
    fscanf(input_file, "%zu", &last_gate);

    reading_gate_list(gate_list, input_file, &number_of_gates);

    char* cnf_result = cnf_reduction(gate_list, number_of_gates);
    printf("%s\n", cnf_result);

    int number_of_clauses = count_clauses(cnf_result);

    // Writing in the output file the convention
    fprintf(output_file, "p cnf ");
    fprintf(output_file, "%zu %d\n", last_gate, number_of_clauses + 1);
    
    // Write the literals
    write_literals(output_file, cnf_result);

    // Write last_gate and 0
    fprintf(output_file, "%zu 0\n", last_gate);

    // Freeing the memory
    destroy_gate_list(&gate_list, number_of_gates);
    free(cnf_result);

    fclose(input_file);
    fclose(output_file);

}
