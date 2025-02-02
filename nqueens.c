#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Ahmet Refik Şener
*/

static char QUEEN[4] = " Q ";
static char EMPTY[4] = " . ";

/*
@brief a function that examines a given one-dimensional array as a two-dimensional board and checks if the board is valid
for N-Queen problem

@param positions array that contains the queen index positions for each row
@param size length of positions array

@return 1 if board is valid, 0 if it is not
*/
int isValid(int* positions, int size) {
    int i;
    int row1;
    int col1;
    int row2;
    int col2;
    for (i = 0; i < size; i++) {
        row1 = positions[i] / size;
        col1 = positions[i] % size;
        for (int j = i + 1; j < size; j++) {
            row2 = positions[j] / size;
            col2 = positions[j] % size;
            // If another queen on the same row
            if (row1 == row2)
                return 0;
            // If another queen on the same column
            if (col1 == col2)
                return 0;
            // // If another queen on the same diagonal
            if (abs(row1 - row2) == abs(col1 - col2))
                return 0;
        }
    }
    return 1;
}

/*
@brief a function that prints a given one-dimensional array as a two-dimensional board

@param positions array that contains the queen index positions for each row
@param size length of positions array
@param solutionCount variable indicating which solution is printing

@return
*/
void printSolution(int* positions, int size, int solutionCount) {
    int i;
    int j;
    int row;
    int col;
    printf("Solution %d:\n", solutionCount);
    for (i = 0; i < size; i++) {
        row = positions[i] / size;
        col = positions[i] % size;
        for (j = 0; j < size; j++) {
            if (j == col)
                printf(QUEEN);
            else
                printf(EMPTY);
        }
        printf("\n");
    }
    printf("\n");
}

/*
@brief a recursive function that searchs all possible queen placements with brute force and prints valid ones

@param n real size of table (nxn)
@param size length of positions array (n)
@param start the starting index for the next row's queen placement search
@param depth current row number being processed in the recursive search
@param positions array that contains the queen index positions for each row
@param validCount valid solutions found
@param totalCount all positions checked

@return
*/
void bruteForceSearch(int n, int size, int start, int depth, int* positions, int* validCount, long long* totalCount) {
    int i;
    if (depth == size) {
        (*totalCount)++;
        if (isValid(positions, size) == 1) {
            (*validCount)++;
            printSolution(positions, size, *validCount);
        }
        return;
    }

    for (i = start; i <= n - (size - depth); i++) {
        positions[depth] = i;
        bruteForceSearch(n, size, i + 1, depth + 1, positions, validCount, totalCount);
    }
}

/*
@brief a recursive function that places queens knowing that they cannot be on the same row and search valid ones

@param positions array that contains the queen index positions for each row
@param depth current row number being processed in the recursive search
@param size length of positions array (n)
@param validCount valid solutions found
@param totalCount all positions checked

@return
*/
void optimized1Search(int* positions, int depth, int size, int* validCount, long long* totalCount) {
    int i;
    if (depth == size) {
        (*totalCount)++;
        if (isValid(positions, size)) {
            (*validCount)++;
            printSolution(positions, size, *validCount);
        }
        return;
    }

    for (i = 0; i < size; i++) {
        positions[depth] = depth * size + i; // Position of the column
        optimized1Search(positions, depth + 1, size, validCount, totalCount);
    }
}

/*
@brief a recursive function that places queens to the board knowing that they will not be 
in the same row and the same column

@param positions array that contains the queen index positions for each row
@param colsUsed array that indicates which columns are empty and which ones are not
@param depth current row number being processed in the recursive search
@param size length of positions array (n)
@param validCount valid solutions found
@param totalCount all positions checked

@return
*/
void optimized2Search(int* positions, int* colsUsed, int depth, int size, int* validCount, long long* totalCount) {
    int i;
    if (depth == size) {
        (*totalCount)++;
        if (isValid(positions, size)) {
            (*validCount)++;
            printSolution(positions, size, *validCount);
        }
        return;
    }

    for (i = 0; i < size; i++) {
        if (colsUsed[i] == 0) { // If column wasn't used
            positions[depth] = depth * size + i;
            // Mark the column as used
            colsUsed[i] = 1;
            optimized2Search(positions, colsUsed, depth + 1, size, validCount, totalCount);
            // Unmark the column
            colsUsed[i] = 0;
        }
    }
}

/*
@brief a recursive function that places queens to the board knowing that they will not be
in the same row and the same column

@param row current row number being processed in the recursive search
@param size length of positions array (n)
@param positions array that contains the queen index positions for each row
@param colsUsed array that indicates which columns are empty and which ones are not
@param usedDiag1 array that indicates which positive slope diagonals are empty and which ones are not
@param usedDiag2 array that indicates which negative slope diagonals are empty and which ones are not
@param validCount valid solutions found
@param totalCount all positions checked

@return
*/
void backtrackingSearch(int row, int size, int* positions, int* usedCols, int* usedDiag1, int* usedDiag2, int* validCount, long long* totalCount) {
    if (row == size) {
        (*validCount)++;
        printSolution(positions, size, *validCount);
        return;
    }

    for (int col = 0; col < size; col++) {       
        if (!usedCols[col] && !usedDiag1[row - col + size - 1] && !usedDiag2[row + col]) {
            (*totalCount)++;
            positions[row] = col;
            usedCols[col] = usedDiag1[row - col + size - 1] = usedDiag2[row + col] = 1;

            backtrackingSearch(row + 1, size, positions, usedCols, usedDiag1, usedDiag2, validCount, totalCount);

            usedCols[col] = usedDiag1[row - col + size - 1] = usedDiag2[row + col] = 0;
        }
    }
}

int main() {
    // Declare all variables
    int i;
    int j;
    int size; 
    clock_t start;
    clock_t end;
    double timeTaken;
    int mode = 0;  
    int bruteForceValidCount = 0;
    int optimized1ValidCount = 0;
    int optimized2ValidCount = 0;
    int backtrackValidCount = 0;
    double totalTime = 0;
    long long int allCount = 0;
    int* positions;
    int* usedColumns;
    int* usedDiag1;
    int* usedDiag2;
    long long int* possibilities;
    double* times;

    // Get size from user
    do {
        printf("Enter the size (cannot be chosen above 15): ");
        scanf("%d", &size);
    } while (size > 15 || size < 0);

    // Initialize size-needed variables
    possibilities = (long long int*)malloc(4 * sizeof(long long int));
    times = (double*)malloc(3 * sizeof(double));
    positions = (int*)malloc(size * sizeof(int));
    usedColumns = (int*)calloc(size, sizeof(int));
    usedDiag1 = (int*)calloc((2 * size - 1), sizeof(int));
    usedDiag2 = (int*)calloc((2 * size - 1), sizeof(int));
    for (i = 0; i < size; i++) {
        usedColumns[i] = 0;
    }
    for (i = 0; i < (2 * size - 1); i++) {
        usedDiag1[i] = 0;
        usedDiag2[i] = 0;
    }

    // Print mode options and get one from user
    printf("\nMODES\n");
    printf("0. All Search Modes Together\n");
    printf("1. Brute Force Search\n");
    printf("2. Optimized 1 Search\n");
    printf("3. Optimized 2 Search\n");
    printf("4. Backtracking\n\n");
    printf("Choose a mode: ");
    scanf("%d", &mode);
    printf("\n");
    if (mode < 0 || mode > 4) {
        mode = 0;
    }

    // BRUTE FORCE
    if (mode == 1) {
        start = clock();
        bruteForceSearch(size * size, size, 0, 0, positions, &bruteForceValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("FOR N = %d\n", size);
        printf("1. BRUTE FORCE: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", allCount, bruteForceValidCount, timeTaken);
    }
    // OPTIMIZED_1
    else if (mode == 2) {
        start = clock();
        optimized1Search(positions, 0, size, &optimized1ValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("FOR N = %d\n", size);
        printf("2. OPTIMIZED_1: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", allCount, optimized1ValidCount, timeTaken);
    }
    // OPTIMIZED_2
    else if (mode == 3) {
        start = clock();
        optimized2Search(positions, usedColumns, 0, size, &optimized2ValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("FOR N = %d\n", size);
        printf("3. OPTIMIZED_2: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", allCount, optimized2ValidCount, timeTaken);
    }
    // BACKTRACKING
    else if (mode == 4) {
        start = clock();
        backtrackingSearch(0, size, positions, usedColumns, usedDiag1, usedDiag2, &backtrackValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("FOR N = %d\n", size);
        printf("4. BACKTRACKING: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", allCount, backtrackValidCount, timeTaken);
    }
    // ALL OF THEM
    else {
        // Brute force
        printf("BRUTE FORCE\n");
        start = clock();
        bruteForceSearch(size * size, size, 0, 0, positions, &bruteForceValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        possibilities[0] = allCount;
        times[0] = timeTaken;
        allCount = 0;
        printf("\n\n");
        // Optimized 1
        printf("OPTIMIZED_1\n");
        start = clock();
        optimized1Search(positions, 0, size, &optimized1ValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        possibilities[1] = allCount;
        times[1] = timeTaken;
        allCount = 0;
        printf("\n\n");
        // Optimized 2
        printf("OPTIMIZED_2\n");
        start = clock();
        optimized2Search(positions, usedColumns, 0, size, &optimized2ValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        possibilities[2] = allCount;
        times[2] = timeTaken;
        allCount = 0;
        printf("\n\n");
        // Backtracking
        printf("BACKTRACKING\n");
        start = clock();
        backtrackingSearch(0, size, positions, usedColumns, usedDiag1, usedDiag2, &backtrackValidCount, &allCount);
        end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        totalTime = times[0] + times[1] + times[2] + times[3];
        printf("\n\n");
        printf("RESULTS FOR N = %d\n", size);
        printf("1. BRUTE FORCE: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", possibilities[0], bruteForceValidCount, times[0]);
        printf("2. OPTIMIZED_1: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", possibilities[1], optimized1ValidCount, times[1]);
        printf("3. OPTIMIZED_2: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", possibilities[2], optimized2ValidCount, times[2]);
        printf("4. BACKTRACKING: Out of %lld checked placements in this mode, %d valid ones were found within %.2f seconds.\n", allCount, backtrackValidCount, timeTaken);
        printf("All operations completed in %.2f seconds.\n", totalTime);
    }

    // Free memory
    free(usedColumns);
    free(positions);
    free(possibilities);
    free(times);

    // Prevent the console from shutting down so that the result can be seen
    system("pause");

    return 0;
}
