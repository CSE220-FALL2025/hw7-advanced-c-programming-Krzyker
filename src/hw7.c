#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {

    if (root == NULL) { //make root
        bst_sf *new = malloc(sizeof(bst_sf));
        new -> mat = mat;
        new -> left_child = NULL;
        new -> right_child = NULL;
        return new;
    } else { //all good
        bst_sf *node = malloc (sizeof(bst_sf));
        node -> mat = mat;
        node -> left_child = NULL;
        node -> right_child = NULL;

        //finding the parent
        bst_sf *parent = root;
        bst_sf *curr = root;
        char nam = mat -> name;

        while (curr != NULL) {
            parent = curr;
            if (nam < curr -> mat->name) {
                curr = curr -> left_child;
            } else {
                curr = curr -> right_child;
            }
        }

        //inserting 
        if (nam < parent->mat->name) {
            parent -> left_child = node;
        } else {
            parent -> right_child = node;
        }
    }

    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root != NULL) {
        bst_sf *curr = root;
        while (curr != NULL) {
            if (name > curr ->mat-> name) {
                curr = curr->right_child;
            } else if (name < curr ->mat-> name) {
                curr = curr->left_child;
            } else {
                return curr -> mat;
            }
        }
    }
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    if (root != NULL) {
        //recursive postorder clear
        free_bst_sf(root-> left_child);
        free_bst_sf(root-> right_child);

        //free matrix and node
        free(root-> mat);
        free(root);
    }
    return;
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    int size = mat1->num_rows * mat1->num_cols;
    matrix_sf *result = malloc(sizeof(matrix_sf) + size * sizeof(int));

    result -> name = 't';
    result -> num_rows = mat1 -> num_rows;
    result -> num_cols = mat1 -> num_cols;

    for( int i = 0; i < size; i++) {
        result->values[i] = mat1->values[i] + mat2->values[i];
    }


    return result;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf *result = malloc(sizeof(matrix_sf) + mat1-> num_rows * mat2-> num_cols * sizeof(int));

    result -> name = 't';
    result -> num_rows = mat1 -> num_rows;
    result -> num_cols = mat2 -> num_cols;

    for (int i = 0; i < mat1-> num_rows; i++) {
        for(int j = 0; j < mat2-> num_cols; j++) {
            int sum = 0;
            for(int k = 0; k < mat1->num_cols; k++) {
                sum += mat1->values[i * mat1-> num_cols + k] * mat2-> values[k * mat2-> num_cols + j];
            }

            result -> values[i * mat2-> num_cols + j] = result;
        }
    }

    return result;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    matrix_sf *result = malloc(sizeof(matrix_sf) + mat-> num_rows * mat-> num_cols * sizeof( int));

    result -> name = 't';
    result -> num_rows = mat -> num_cols;
    result -> num_cols = mat -> num_rows;

    for(int i = 0; i < mat->num_rows; i++) {
        for(int j = 0; j < mat-> num_cols; j++) {
            result->values[j *result->num_cols + i] = mat->values[i*result-> num_cols + j];
        }
    }

    return result;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    //extracting numr and numc
    int NR = 0;
    int NC = 0;

    char *p = expr;
    char *endptr; //for strtol

    //spaces in front
    while(*p == ' ') {
        p++;
    }

    NR = strtol(p, &endptr, 10);
    p = endptr;

    while (*p == ' ') {
        p++;
    }

    NC = strtol(p, &endptr, 10);
    p = endptr;


    matrix_sf *result = malloc(sizeof(matrix_sf) + NR*NC * sizeof(int));
    result -> name = name;
    result -> num_rows = NR;
    result -> num_cols = NC;

    //finding array contents;
    while(*p != '[') {
        p++;
    } p++;

    //reading the array
    for(int i = 0; i < NR; i++) {
        for(int j = 0; j <NC; j++) {
            while(*p == ' ') {
                p++;
            }

            result -> values[i*NC + j] = strtol(p, &endptr, 10);
            p = endptr;
        }

        while (*p == ' ' || *p == ';') {
            p++;
        } //next row
    }

    return result;
}

char* infix2postfix_sf(char *infix) {
    char* post = malloc(strlen(infix) + 1);
    char* stack = malloc(strlen(infix) + 1);

    int post_index = 0;
    char *p = infix;
    int pop = 0;

    while(*p != '\0') {
        if (isspace(*p)) {
            p++;
            continue;
        }

        //if letter
        if (*p >= 'A' && *p <= 'Z') {
            post[post_index] = *p;
            post_index++; p++;

            //transpose check
            while (true) { //while fix for multiple transpose check
                    while (isspace(*p)) {
                        p++;
                    }
                if (*p == '\'') {
                    post[post_index] = *p;
                    post_index++; p++;
                } else {
                    break;
                }
            }
            continue;
        }


        if (*p == '(') {
            stack[pop] = *p;
            pop++; p++;
            continue;
        }

        if (*p == ')') {
            //pop until ( is encountered
            while (stack[pop - 1] != '(') {
                post[post_index] = stack[pop - 1];
                post_index++;
                pop--;
            }

            pop--; 
            p++;

            //transpose check
            while (true) { //whle fix for multiple transpose check
                    while (isspace(*p)) {
                        p++;
                    }
                if (*p == '\'') {
                    post[post_index] = *p;
                    post_index++; p++;
                } else {
                    break;
                }
            }

            continue;
        }

        if (*p == '+') {
            //pop until ( or empty
            while (pop != 0 && stack[pop - 1] != '(') {
                post[post_index] = stack[pop - 1];
                post_index++;
                pop--;
            }
            stack[pop] = *p;
            pop++; p++;
            continue;
        }

        if (*p == '*') {
            //pop until ( or + or empty
            while (pop != 0 && stack[pop - 1] != '(' && stack[pop - 1] != '+') {
                post[post_index] = stack[pop - 1]; //same thing as for +
                post_index++;
                pop--;
            }
            stack[pop] = *p;
            pop++; p++;
            continue;
        }
    }

    //pop remainder
    while (pop > 0) {
            post[post_index] = stack[pop - 1];
            post_index++;
            pop--;
    }
    
    post[post_index] = '\0'; //top it off
    free (stack); //free auxiliary stack

    return post;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char* post = infix2postfix_sf(expr);
    matrix_sf **stack = malloc(sizeof(matrix_sf*) * (strlen(post) + 1));

    int pop = 0;

    for(int i = 0; i < strlen(post); i++) {
        char curr = post[i];
        
    }
}

matrix_sf *execute_script_sf(char *filename) {
   return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
