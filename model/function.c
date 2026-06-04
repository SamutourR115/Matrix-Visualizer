#include "function.h"

struct matrix_t{
    unsigned int rows;
    unsigned int cols;
    unsigned int **matrix;
};

Matrix *create_matrix(unsigned int rows, unsigned int cols){
    if(rows < 1 || cols < 1) return NULL;
    Matrix *m;
    unsigned int i = 0, j = 0;

    if(!(m = malloc(sizeof(Matrix)))){
        return NULL;
    }

    if(!(m->matrix = malloc(rows * sizeof(unsigned int*)))){
        free(m);
        return NULL;
    }

    for(i = 0; i < rows; i++){
        m->matrix[i] = malloc(cols * sizeof(unsigned int));
        if(!m->matrix[i]){
            for(j = 0; j < i; j++){
                free(m->matrix[j]);
            }
            free(m->matrix);
            free(m);
            return NULL;
        }

        for(j = 0; j < cols; j++){
            m->matrix[i][j] = 0;
        }
    }

    m->cols = cols;
    m->rows = rows;

    return m;
}

void free_matrix(Matrix *a){
    if(a == NULL) return;
    for(unsigned int i = 0; i < a->rows; i++){
            free(a->matrix[i]);
    }
    free(a->matrix);
    free(a);
}


Matrix *additionning_matrix(Matrix *a,Matrix *b){
    if(a == NULL || b == NULL) return NULL;

    if(a->cols != b->cols || a->rows != b->rows) return NULL;

    unsigned int rows = a->rows;
    unsigned int cols = a->cols;

    Matrix *c = create_matrix(rows,cols);

    for(unsigned int i = 0; i < rows;i++){
        for(unsigned int j = 0; j < cols; j++){
            c->matrix[i][j] = a->matrix[i][j] +  b->matrix[i][j];
        }
    }
    return c;
}

Matrix *multpily_matrix(Matrix *a,Matrix *b){
    if(a == NULL || b == NULL) return NULL;

    if(a->cols != b->rows) return NULL;

    unsigned int rows = a->rows;
    unsigned int cols = b->cols;

    Matrix *c = create_matrix(rows,cols);
    if(!c) return NULL;

    for(unsigned int i = 0; i < rows;i++){
        for(unsigned int j = 0; j < cols;j++){
            c->matrix[i][j] = 0;
            for(unsigned int k=0; k < a->cols; k++){
                c->matrix[i][j] += a->matrix[i][k] * b->matrix[k][j];
            }
        }
    }

    return c;
}

unsigned short fill_matrix(Matrix *a){
    if(a == NULL) return 0;
    unsigned int rows = a->rows;
    unsigned int cols = a->cols;

    for(unsigned int i = 0; i < rows; i++){
        for(unsigned int j = 0; j < cols;j++){
            printf("Fill the matrix, you are on the [%d][%d]case:", i+1, j+1);
            scanf("%u",&a->matrix[i][j]);
        }
    }
    return 1;
}

unsigned short seeing_matrix(Matrix *a){
    if(a == NULL) return 0;

    for(unsigned int i = 0; i < a->rows; i++){
        for(unsigned int j = 0; j < a->cols; j++){
            printf(" %u ",a->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return 1;
}
