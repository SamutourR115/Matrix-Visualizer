#ifndef  MATRIX_FUNCTION_H
#define  MATRIX_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>


#define NB_COL_MAX 3
#define NB_ROW_MAX 3

typedef struct matrix_t Matrix;


/**
 * create_matrix
 * 
 * Constructor to create a matrix dynamically
 * 
 * @pre : rows > 1 & cols > 1
 * @post : m != NULL
 * 
 * @param rows : Numbers of the rows for the matrix
 * @param cols : Numbers of the cols for the matrix
 * 
 * @return : return a dynamic matrix
 */
Matrix *create_matrix(unsigned int rows, unsigned int cols);

/**
 * 
 * free_matrix
 * 
 * Free the memory allocation 
 * 
 * @pre : a ! NULL
 * @post : free the matrix a
 * 
 * @param a : Pointers to the matrix a
 *  
 */
void free_matrix(Matrix *a);

/**
 * set_value  
 * 
 * Set a value in the mtrix m
 * 
 * @pre : m != NULL 
 * @post : change the value in the matix 
 * 
 * @param m : Matrix 
 * @param row : row of the matrix
 * @param col : col of the matrix
 * @param value : value who want to be stock
 * 
 */
void set_value(Matrix *m, unsigned int row, unsigned int col, int value);

/**
 * additionning_matrix
 * 
 * Additionning 2 Matrix an return the result
 * 
 * @pre : a != NULL and b != NULL
 * @post: c != NULL
 * 
 * @param a : Pointers to the matrox a
 * @param b : Pointers to the matrox b
 * 
 * @return : 
 *  -NULL if the creation of matrix fail 
 *  - Matrix c, the result of addition
 *  -1 if matrix a and b are not the same size
 */
Matrix *additionning_matrix(Matrix *a,Matrix *b);

/**
 * multpily_matrix
 * 
 * Multiply 2 matrix together
 * 
 * @pre : a != NULL and b != NULL
 * @post: c != NULL
 * 
 * @param a : Pointers to the matrox a
 * @param b : Pointers to the matrox b
 * 
 * @return : 
 *  -NULL if the creation of matrix fail 
 *  - Matrix c, the result of multiplication
 */
Matrix *multpily_matrix(Matrix *a,Matrix *b);

/**
 * fill_matrix
 * 
 * To fill the matrix in arguments
 * 
 * @pre : a != NULL
 * @post : Fill a
 * 
 * @param a : Pointers to the matrox a
 * 
 * @return : 
 *      -1 Allocations errors
 *      1 All is good
 */
unsigned short fill_matrix(Matrix *a);

/**
 * seeing_matrix
 * 
 * For seeing the matrix in argument
 * 
 * @pre : a != NULL
 * @post : /
 * 
 * @param a : Pointers to the matrox a
 * 
 * @return :
 *      -1 Allocations errors
 *      1 All is good
 */
unsigned short seeing_matrix(Matrix *a);

/**
 * get_cols
 * 
 * Getter for rows
 * 
 * @pre: m != NULL
 * @post: nb rows
 * 
 * @param m : Pointer to the matrix m
 * 
 * 
 * @return : 
 *      0 : Allacations issues
 *      Numbers rows of the matrix m
 */
unsigned int get_rows(Matrix *m);

/**
 * get_cols
 * 
 * Getter for cols
 * 
 * @pre: m != NULL
 * @post: nb cols
 * 
 * @param m : Pointer to the matrix m
 * 
 * 
 * @return : 
 *      0 : Allacations issues
 *      Numbers cols of the matrix m
 */
unsigned int get_cols(Matrix *m);

int get_value(Matrix *m, unsigned int row, unsigned int col);

#endif /* MATRIX_FUNCTION_H */
