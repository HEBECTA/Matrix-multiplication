
/* includes */

#include "presettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// for subtractNumbers and readNumberInt functions, also used in main function
#define BUFF_SIZE 255

/* User-defined types */

// Matrix
typedef struct Matrix {

    int height;
    int width;

    char ***elements; // matrix numbers stored as strings
    int **elementSize; // size of characters in each number's string (helps to make faster operations on big numbers)

} Matrix;

/* Function prototypes */

// reads number from the file and stores it as a string in buff, returns size of a string (for reading matrix elements)
int readNumberString(FILE *fp, char * buff);

// reads number from the file and returns it as int (for reading matrix size)
int readNumberInt(FILE *fp);

// dynamically allocates memory and stores there a number from the buffer, returns NULL if memory allocation failed
char *copyNumber(char *buff, int size);

// converts single char to int (used in multiplyNumbers function)
int charIntoInt(char c);


// substracts two numbers which are stored as strings, returns a new dynamically allocated number stored as string, returns NULL if memory allocation failed
char *subtractNumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize);

// adds two numbers which are stored as strings, returns new a dynamically allocated number stored as string, returns NULL if memory allocation failed
char *addnumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize);

// multiples two numbers which are stored as strings, returns a new dynamically allocated number stored as string, returns NULL if memory allocation failed
char *multiplyNumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize);

// multiplies two matrices and returns a new matrix with dynamicvally allocated numbers in it, returns new matrix with height value -1 if memory allocation failed
Matrix matrixMultiplication(Matrix m1, Matrix m2, int *maxNumberSize);

/*
 * Main function
 */
int main(int argc, char *argv[]) {

    // wrong number which describes matrix size
    ErrorCode error = -1;

    // checks if input of arguments are correct
    if ( argc != 2){

        fprintf(stderr, invalid_arg_num_str);
        return EXIT_ARGS;
    }
        
    char *file = argv[1];

    FILE *fp;
    
    fp = fopen(file, "r");

    if (fp == NULL){

        fprintf(stderr, open_infile_err_str, file);
        return EXIT_IO;
    }
        
    // initialize 1 matrix
    Matrix m1;

    // read 1 matrix size
    m1.height = readNumberInt(fp);
    m1.width = readNumberInt(fp);

    if ( m1.height == -1 || m1.width == -1){

        fprintf(stderr, unknown_error_str, error);
        return error;
    }

    m1.elements = (char ***)malloc(sizeof(char **)*m1.height);

    m1.elementSize = (int **) malloc(sizeof(int *)*m1.height);

    if ( m1.elements == NULL  || m1.elementSize == NULL ){

        fprintf(stderr, out_of_mem_str);
        return EXIT_OUT_OF_MEM;
    }

    // initialize 2 matrix
    Matrix m2;

    // read 2 matrix size
    m2.height = readNumberInt(fp);
    m2.width = readNumberInt(fp);

    if ( m2.height == -1 || m2.width == -1){

        fprintf(stderr, unknown_error_str, error);
        return error;
    }

    if ( m1.width != m2.height ){

        free(m1.elements);
        free(m1.elementSize);

        fprintf(stderr, incompatible_dim_str);
        return EXIT_INCOMPATIBLE_DIM;
    }

    m2.elements = (char ***) malloc(sizeof(char **)*m2.height);

    m2.elementSize = (int **) malloc(sizeof(int *)*m2.height);

    if ( m2.elements == NULL  || m2.elementSize == NULL ){

        fprintf(stderr, out_of_mem_str);
        return EXIT_OUT_OF_MEM;
    }

    // read numbers from the file and write them to the matrixes
    char buff[BUFF_SIZE];
    int size;

    // read 1 matrix from the file
    for (int y = 0; y < m1.height; ++y)
    {
        m1.elements[y] = (char **) malloc(sizeof(char **)*m1.width);

        m1.elementSize[y] = (int *) malloc(sizeof(int)*m1.width);

        if ( m1.elements[y] == NULL  || m1.elementSize[y] == NULL ){

            fprintf(stderr, out_of_mem_str);
            return EXIT_OUT_OF_MEM;
        }
        
        // loop while there is a new line, till the first number
        while ((size = readNumberString(fp, buff)) == -4);

        if ( size > MAX_LENGTH ){

                fprintf(stderr, number_too_big_str, size);
                return EXIT_NUMBER_TOO_BIG;
            }

            else if ( size == -2 ){

                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -3 ){

                fprintf(stderr, leading_zero_str);
                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -1 ){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            m1.elements[y][0] = (char *) malloc(sizeof(char *)*size);

            m1.elements[y][0] = copyNumber(buff, size);

            if ( m1.elements[y][0] == NULL ){

                fprintf(stderr, out_of_mem_str);
                return EXIT_OUT_OF_MEM;
            }

            m1.elementSize[y][0] = size;

        for (int x = 1; x < m1.width; ++x){

            if ((size = readNumberString(fp, buff)) == -4){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            else if ( size > MAX_LENGTH ){

                fprintf(stderr, number_too_big_str, size);
                return EXIT_NUMBER_TOO_BIG;
            }

            else if ( size == -2 ){

                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -3 ){

                fprintf(stderr, leading_zero_str);
                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -1 ){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            m1.elements[y][x] = (char *) malloc(sizeof(char *)*size);

            m1.elements[y][x] = copyNumber(buff, size);

            if ( m1.elements[y][x] == NULL ){

                fprintf(stderr, out_of_mem_str);
                return EXIT_OUT_OF_MEM;
            }

            m1.elementSize[y][x] = size;
        }
    }

    // read 2 matrix from the file
    for (int y = 0; y < m2.height; ++y)
    {
        m2.elements[y] = (char **) malloc(sizeof(char **)*m2.width);

        m2.elementSize[y] = (int *) malloc(sizeof(int)*m2.width);

        if ( m2.elementSize[y] == NULL  || m2.elements[y] == NULL ){

            fprintf(stderr, out_of_mem_str);
            return EXIT_OUT_OF_MEM;
        }

        // loop while there is a new line, till the first number
        while ((size = readNumberString(fp, buff)) == -4);

        if ( size > MAX_LENGTH ){

                fprintf(stderr, number_too_big_str, size);
                return EXIT_NUMBER_TOO_BIG;
            }

            else if ( size == -2 ){

                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -3 ){

                fprintf(stderr, leading_zero_str);
                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -1 ){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            m2.elements[y][0] = (char *) malloc(sizeof(char *)*size);

            m2.elements[y][0] = copyNumber(buff, size);

            if ( m2.elements[y][0] == NULL ){

                fprintf(stderr, out_of_mem_str);
                return EXIT_OUT_OF_MEM;
            }

            m2.elementSize[y][0] = size;

        for (int x = 1; x < m2.width; ++x){

            // loop while there is a new line, till the first number
            if ((size = readNumberString(fp, buff)) == -4){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            else if ( size > MAX_LENGTH ){

                fprintf(stderr, number_too_big_str, size);
                return EXIT_NUMBER_TOO_BIG;
            }

            else if ( size == -2 ){

                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -3 ){

                fprintf(stderr, leading_zero_str);
                fprintf(stderr, invalid_number_str);
                return EXIT_INVALID_NUMBER;
            }

            else if ( size == -1 ){

                fprintf(stderr, invalid_matrix_str);
                return EXIT_INVALID_MATRIX;
            }

            m2.elements[y][x] = (char *) malloc(sizeof(char *)*size);

            m2.elements[y][x] = copyNumber(buff, size);

            if ( m2.elements[y][x] == NULL ){

                fprintf(stderr, out_of_mem_str);
                return EXIT_OUT_OF_MEM;
            }

            m2.elementSize[y][x] = size;
        }
    }

    // create 3 matrix and store in it multiplication of the 1 and 2 matrices
    Matrix m3;

    int maxNmbSize;

    m3 = matrixMultiplication(m1, m2, &maxNmbSize);

    if ( m3.height == -1 ){

        fprintf(stderr, out_of_mem_str);
        return EXIT_OUT_OF_MEM;
    }

    // free dynamically allocated memory

    for (int y = 0; y < m1.height; ++y)
    {
        for (int x = 0; x < m1.width; ++x){

            free(m1.elements[y][x]);
        }

        free(m1.elements[y]);
        free(m1.elementSize[y]);
    }

    free(m1.elements);
    free(m1.elementSize);

    for (int y = 0; y < m2.height; ++y)
    {
        for (int x = 0; x < m2.width; ++x){

            free(m2.elements[y][x]);
        }

        free(m2.elements[y]);
        free(m2.elementSize[y]);
    }

    free(m2.elements);
    free(m2.elementSize);

    // print 3 matrix
    for (int y = 0; y < m3.height; ++y)
    {
        for (int x = 0; x < m3.width; ++x){

            printf(" ");

            for(int i = 0; i < maxNmbSize - m3.elementSize[y][x]; ++i)
                printf(" ");

            printf("%s", m3.elements[y][x]);
        }

        printf("\n");
    }

    // free memory from m3 matrix

    for (int y = 0; y < m3.height; ++y)
    {
        for (int x = 0; x < m3.width; ++x){

            free(m3.elements[y][x]);
        }

        free(m3.elements[y]);
        free(m3.elementSize[y]);
    }

    free(m3.elements);
    free(m3.elementSize);

    if (fclose(fp) == EOF ){

        fprintf(stderr, close_file_err_str);
        return EXIT_IO;
    }

    return EXIT_OK;
}

// ************************************************************************************************************************************
// ************************************************************************************************************************************
// ************************************************************************************************************************************

Matrix matrixMultiplication(Matrix m1, Matrix m2, int *maxNumberSize){

    Matrix answ;

    answ.height = m1.height;
    answ.width = m2.width;

    answ.elements = (char ***)malloc(sizeof(char **)*m1.height);
    answ.elementSize = (int **)malloc(sizeof(int *)*m1.height);

    if ( answ.elements == NULL || answ.elementSize == NULL ){

            answ.height = -1;
            return answ;
        }

    int maxNmbSize = 0;

    for (int y = 0; y < m1.height; ++y){

        answ.elements[y] = (char **)malloc(sizeof(char *)*m2.width);
        answ.elementSize[y] = (int *)malloc(sizeof(int)*m2.width);

        if ( answ.elements[y] == NULL || answ.elementSize[y] == NULL ){

            answ.height = -1;
            return answ;
        }

        for (int x = 0; x < m2.width; ++x){

            int prevNmbSize;

            char *prevNmb = multiplyNumbers(m1.elements[y][0], m1.elementSize[y][0], m2.elements[0][x], m2.elementSize[0][x], &prevNmbSize);

            if ( prevNmb == NULL ){

                answ.height = -1;
                return answ;
            }

            for (int i = 1; i < m1.width; ++i){

                int nmbSize; 

                char *nmb = multiplyNumbers(m1.elements[y][i], m1.elementSize[y][i], m2.elements[i][x], m2.elementSize[i][x], &nmbSize);

                if ( nmb == NULL ){

                    answ.height = -1;
                    return answ;
                }

                char *tmp = prevNmb;

                prevNmb = addnumbers(prevNmb, prevNmbSize, nmb, nmbSize, &prevNmbSize);

                if ( prevNmb == NULL ){

                    answ.height = -1;
                    return answ;
                }

                if ( prevNmbSize > maxNmbSize )
                    maxNmbSize = prevNmbSize;

                free(tmp);
            }

            answ.elements[y][x] = prevNmb;
            answ.elementSize[y][x] = prevNmbSize;
        }
    }

    *maxNumberSize = maxNmbSize;

    return answ;
}


// ************************************************************************************************************************************
char *subtractNumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize){

    // exmpl: n1 - - n2 = n1 + n2, call addNumbers function
    if ( n1[0] != '-' && n2[0] == '-' ){

        return addnumbers(n1, n1Size, &n2[1], n2Size-1, answerSize);
    }

    // exmpl: - n1 - n2 = - (n1 + n2), call addNumbers function
    else if ( n1[0] == '-' && n2[0] != '-' ){

        return addnumbers(n1, n1Size, n2, n2Size, answerSize);
    }

    char answBuff[BUFF_SIZE];

    char *longerNumber = n1;
    char *shorterNumber = n2;

    bool answNegative = false;

    if ( n1[0] == '-'){
     
        --n1Size;
        longerNumber = &longerNumber[1];
    }

    if ( n2[0] == '-'){

        --n2Size;
        shorterNumber = &shorterNumber[1];
    }

    int maxSize = n1Size;
    int minSize = n2Size;

    if (maxSize < n2Size ){

        maxSize = n2Size;
        minSize = n1Size;
        char *temp = longerNumber;
        longerNumber = shorterNumber;
        shorterNumber = temp;

        answNegative = true;
    }

    // number n1 and n2 are equal
    else if ( n1Size == n2Size ){

        bool equal = true;

        for (int i = 0; i < maxSize; ++i){

            if ( longerNumber[i] < shorterNumber[i] ){

                char *temp = longerNumber;
                longerNumber = shorterNumber;
                shorterNumber = temp;

                equal = false;

                answNegative = true;
          
                break;
            }

            else if ( longerNumber[i] > shorterNumber[i]){

                equal = false;

                break;
            }
        }

        if ( equal ){

            char *answ = malloc(sizeof(char)*2);

            if ( answ == NULL )
                return NULL;

            answ[0] = '0';
            answ[1] = '\0';

            *answerSize = 1;

            return answ;
        }
    }

    bool reminder = false;

    int answSize;

    // substract numbers, write answer to the buffer
    for (int i = 0; i < minSize; ++i){

        int result = longerNumber[maxSize-i-1] - shorterNumber[minSize-i-1] + '0';

        if (reminder){

            --result; 
            reminder = false;
        }
            
        if ( result < '0' ){

            result += '9' - '0' + 1;
            reminder = true;
        }

        if ( result != '0' )
            answSize = i + 1;
        


        answBuff[BUFF_SIZE-i-1] = result;
    }

    // rewrite remaining longer number values to the buffer
    for ( int i = minSize; i < maxSize; ++i){

        int result = longerNumber[maxSize-i-1];

        if ( reminder ){

            --result;
            reminder = false;
        }

        if ( result < '0' ){

            result += '9' - '0' + 1;
            reminder = true;
        }

        if ( result != '0' )
            answSize = i + 1;
        

        answBuff[BUFF_SIZE-i-1] = result;
    }

    char *answ;

    // if answer number is negative allocate enough memory to store '-' sign
    if ( answNegative ){

        answ = malloc(sizeof(char)*(answSize+2)); // +2 for '\0' and for '-'

        if ( answ == NULL )
            return NULL;
        
        // write answer number from the buff to the a new dynamically allocated memory
        for ( int i = 0; i < answSize; ++i){

            answ[answSize-i] = answBuff[BUFF_SIZE-i-1];
        }

        answ[0] = '-';
        answ[answSize+1] = '\0';
        *answerSize = answSize+1;
    }

    // if answer number is not negative allocate enough memory to store without '-' sign
    else{

        answ = malloc(sizeof(char)*(answSize+1)); // +1 for '\0'

        if ( answ == NULL )
            return NULL;
        
        // write answer number from the buff to the a new dynamically allocated memory
        for ( int i = 0; i < answSize; ++i){

            answ[answSize-i-1] = answBuff[BUFF_SIZE-i-1];
        }

        answ[answSize] = '\0';
        *answerSize = answSize;
    }

    return answ;
}


// ************************************************************************************************************************************
char *addnumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize){

    // if n2 is '0', just return n1
    if ( n2[0] == '0'){

        char *answ = malloc(sizeof(char)*n1Size+1);

        if ( answ == NULL )
            return NULL;

        for (int i = 0; i < n1Size; ++i){

            answ[i] = n1[i];
        }

        answ[n1Size] = '\0';

        *answerSize = n1Size;

        return answ;
    }

    // if n1 is '0', just return n2
    else if ( n1[0] == '0' ){

        char *answ = malloc(sizeof(char)*n2Size+1);

        if ( answ == NULL )
            return NULL;

        for (int i = 0; i < n2Size; ++i){

            answ[i] = n2[i];
        }

        answ[n2Size] = '\0';

        *answerSize = n2Size;

        return answ;
    }

    // exmpl: - n1 + n2 = n2 - n1, call subtractNumbers function
    if ( n1[0] == '-' && n2[0] != '-' ){

        return subtractNumbers(n2, n2Size, &n1[1], n1Size-1, answerSize);
    }

    // exmpl: n1 + - n2 = n1 - n2, call subtractNumbers function
    else if ( n1[0] != '-' && n2[0] == '-' ){

        return subtractNumbers(n1, n1Size, &n2[1], n2Size-1, answerSize);
    }

    char answBuff[BUFF_SIZE];

    bool answNegative = false;

    char *longerNumber = n1;
    char *shorterNumber = n2;

    // exmpl: - n1 + - n2 = - (n1 + n2); n1 + n2 = n3, later just add '-' sign before n3
    if ( n1[0] == '-' && n2[0] == '-' ){

        answNegative = true;
        longerNumber = &n1[1];
        shorterNumber = &n2[1];
        --n1Size;
        --n2Size;
    }

    int maxSize = n1Size;
    int minSize = n2Size;

    if ( maxSize < n2Size){

        minSize = n1Size;
        maxSize = n2Size;

        char *temp = longerNumber;
        longerNumber = shorterNumber;
        shorterNumber = temp;
    }

    bool reminder = false;

    // add n1 and n2 numbers
    for (int i = 0; i < minSize; ++i){

        answBuff[BUFF_SIZE-1-i] = longerNumber[maxSize-1-i] + shorterNumber[minSize-1-i];

        if (reminder)
            ++answBuff[BUFF_SIZE-1-i];

        answBuff[BUFF_SIZE-1-i] %= 58;

        reminder = true;

        if ( answBuff[BUFF_SIZE-1-i] < '0'){

            answBuff[BUFF_SIZE-1-i] += 10;

            reminder = false;
        }
    }

    // rewrite digits from the longernumber to the answer buffer
    for (int i = minSize; i < maxSize; ++i){

        answBuff[BUFF_SIZE-1-i] = longerNumber[maxSize-1-i];

        if ( reminder ){

            ++answBuff[BUFF_SIZE-1-i];

            reminder = false;
        }

        if ( answBuff[BUFF_SIZE-1-i] > '9'){

            answBuff[BUFF_SIZE-1-i] = '0';

            reminder = true;
        }
    }

    int answSize = maxSize;

    if (reminder){

        ++answSize;
        answBuff[BUFF_SIZE-answSize] = '1';
    }

    char *answ;

    int k = answSize;

    if ( answNegative ){

        answ =  malloc(sizeof(char)*answSize+2); // +2 for '-' sign and '\0'

        if ( answ == NULL )
            return NULL;

        answ[0] = '-';

        answ[answSize+1]= '\0';

        k = answSize+1;

        *answerSize = answSize+1;
    }

    else{

        answ =  malloc(sizeof(char)*answSize+1); // +1 for '\0'

        if ( answ == NULL )
            return NULL;

        answ[answSize]= '\0';

        *answerSize = answSize;
    }

    // write answer number from buffer to a new dynamically allocated memory
    for (int i = 0; i < answSize; ++i){

            answ[k-1-i] = answBuff[BUFF_SIZE-1-i];
    }

    return answ;
}


// ************************************************************************************************************************************
char *multiplyNumbers(char *n1, int n1Size, char *n2, int n2Size, int *answerSize){

    // if one of the numbers are 0, answer is also zero
    if ( n1[0] == '0' || n2[0] == '0'){

        char *answ = malloc(sizeof(char)*2);

        if ( answ == NULL )
            return NULL;
        
        answ[0] = '0';
        answ[1] = '\0';
        *answerSize = 1;
        return answ;
    }

    char answBuff[BUFF_SIZE];

    bool answNegative = false;

    char *shorterNumber = n1;
    char *longerNumber = n2;
    
    if ( n1[0] == '-'){
     
        --n1Size;
        answNegative = true;
        shorterNumber = &shorterNumber[1];
    }

    if ( n2[0] == '-'){

        --n2Size;
        longerNumber = &longerNumber[1];

        if ( answNegative)
            answNegative = false;
        
        else
            answNegative = true;
    }

    int minSize = n1Size;
    int maxSize = n2Size;

    if (minSize > n2Size){

        minSize = n2Size;
        maxSize = n1Size;

        char *temp = shorterNumber;
        shorterNumber = longerNumber;
        longerNumber = temp;
    }

    for (int i = 0; i < maxSize+minSize; ++i)
            answBuff[i] = '0';

    int reminder;

    //      C A L C U L A T I O N
    for (int i = maxSize-1; i >= 0; --i){

        reminder = 0;

        for (int k = minSize-1; k >= 0; --k){

            int a = (charIntoInt(shorterNumber[k]) * charIntoInt(longerNumber[i]) + reminder + charIntoInt(answBuff[(maxSize+minSize-2)-i-k]));

            answBuff[(maxSize+minSize-2)-i-k] = (a % 10) + '0';

            reminder = (a / 10);
        }

        answBuff[(maxSize+minSize-1)-i] = reminder + '0';
    }

    int answSize;

    if ( reminder > 0){
        answSize = maxSize + minSize;
        answBuff[maxSize+minSize] = '\0';}
    
    else{
        answSize = maxSize + minSize-1;
        answBuff[maxSize+minSize-1] = '\0';}

    char *answ;
    int k = 0;
    if (answNegative){

        answ = malloc(sizeof(char)*answSize+2); // +2, sign '-' and '\0'

        if ( answ == NULL )
            return NULL;
        
        answ[0] = '-';
        k = 1;
        answ[answSize+1] = '\0';

        *answerSize = answSize+1;
    }
        
    else {

        answ = malloc(sizeof(char)*answSize+1); // +1 for '\0'

        if ( answ == NULL )
            return NULL;

        answ[answSize] = '\0';

        *answerSize = answSize;
    }

    // write answer number from buffer to dynamically allocated memory
    for ( int i = answSize-1; i >= 0; --i){
        answ[k] = answBuff[i];
        ++k;
    }

    return answ;
}


// ************************************************************************************************************************************
int readNumberString(FILE *fp, char *buff){

    char c;
    int i = 0;

    while ( (c = getc(fp)) != EOF ){

        if ( i == 0 && ((c >= '1' && c <= '9') || c == '-')){

            buff[i] = c;
            ++i;
            continue;
        }

        else if ( i == 0 && c == '0'){

            buff[i] = c;
            ++i;
            continue;
        }

        else if ( (c == ' ' || c == 9) && i == 0){

            continue;
        }

        else if ( i > 0 && (c >= '0' && c <= '9') ){

            if ( buff[0] == '0' )
                return -3;

            buff[i] = c;
            ++i;
            continue;
        }

        else if ( i > 0 && (c == ' ' || c == 9)){

            buff[i] = '\0';

            return i;
        }

        else if ( i == 1 && c == 10 && buff[0] != '-'){

            buff[i] = '\0';

            return i;
        }

        else if ( i > 0 && c == 10){

            buff[i] = '\0';

            return i;
        }

        // if new line
        else if ( c == 10)
            return -4;

        else
            return -2;
    }

    buff[i] = '\0';

    return -1;
}


// ************************************************************************************************************************************
int readNumberInt(FILE *fp){

    char buff[255];

    int result = readNumberString(fp, buff);

    if ( result > 0){

        if ( buff[0] == '-' )
            return -1;

        int dimension = 1;

        if ( result > 1 ){

            for (int i = 0; i < result-1; ++i)
                dimension *= 10;
        }
        
        int answ = 0, i = 0;

        while ( dimension > 0){

            answ += dimension * (buff[i] - 48);
            ++i;
            dimension /= 10;
        }

        return answ;
    }

    return -1;
}


// ************************************************************************************************************************************
char *copyNumber(char *buff, int size){

    char *answ = malloc(sizeof(char)*size);

    if ( answ == NULL )
        return NULL;

    for(int i = 0; i < size; ++i){

        answ[i] = buff[i];
    }

    return answ;
}


// ************************************************************************************************************************************
int charIntoInt(char c){

    switch (c)
    {
    case '0':
        return 0;
        break;
    
    case '1':
        return 1;
        break;
    
    case '2':
        return 2;
        break;

    case '3':
        return 3;
        break;

    case '4':
        return 4;
        break;

    case '5':
        return 5;
        break;

    case '6':
        return 6;
        break;

    case '7':
        return 7;
        break;

    case '8':
        return 8;
        break;
    
    case '9':
        return 9;
        break;
    
    default:
    
        return -1;
        break;
    }
}
