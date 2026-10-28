#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 256

int max(int a, int b) {
    return (a > b) ? a : b;
}

void displayTable(int **dp, int rows, int W) {
    printf("\033[H\033[J");

    printf("0/1 KNAPSACK DP TABLE\n\n");

    printf("Item\\Cap |");
    for (int w = 0; w <= W; w++)
        printf("%4d", w);
    printf("\n");

    for (int i = 0; i <= rows; i++) {
        printf("%8d |", i);

        for (int w = 0; w <= W; w++)
            printf("%4d", dp[i][w]);

        printf("\n");
    }

    fflush(stdout);
    usleep(200000); 
}

int knapsack(int n, int W, int wt[], int val[],
             int visualize) {

    int **dp = malloc((n + 1) * sizeof(int *));

    for (int i = 0; i <= n; i++)
        dp[i] = calloc(W + 1, sizeof(int));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {

            if (wt[i - 1] <= w) {
                dp[i][w] = max(
                    dp[i - 1][w],
                    val[i - 1] + dp[i - 1][w - wt[i - 1]]
                );
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }

        if (visualize)
            displayTable(dp, i, W);
    }

    int result = dp[n][W];

    for (int i = 0; i <= n; i++)
        free(dp[i]);

    free(dp);
    return result;
}

int main() {
    FILE *input = fopen("knapsack_data.txt", "r");
    FILE *output = fopen("result.txt", "w");

    if (input == NULL || output == NULL) {
        printf("Error opening files.\n");
        if (input) fclose(input);
        if (output) fclose(output);
        return 1;
    }

    char line[MAX_LINE];
    int test = 0;

    while (fgets(line, sizeof(line), input)) {

        if (strncmp(line, "#Test", 5) == 0) {
            int n, W;

            if (!fgets(line, sizeof(line), input))
                break;

            if (sscanf(line, "%d %d", &n, &W) != 2 ||
                n <= 0 || W < 0) {
                printf("Invalid test case header.\n");
                break;
            }

            int *wt = malloc(n * sizeof(int));
            int *val = malloc(n * sizeof(int));

            if (!wt || !val) {
                printf("Memory allocation failed.\n");
                free(wt);
                free(val);
                break;
            }

            int valid = 1;

            for (int i = 0; i < n; i++) {
                if (!fgets(line, sizeof(line), input) ||
                    sscanf(line, "%d %d", &wt[i], &val[i]) != 2 ||
                    wt[i] < 0) {
                    valid = 0;
                    break;
                }
            }

            if (!valid) {
                printf("Invalid item data.\n");
                free(wt);
                free(val);
                break;
            }

            test++;

            int result = knapsack(n, W, wt, val, 1);

            fprintf(output, "#Test %d (%d %d) %d\n",
                    test, n, W, result);

            printf("\nTest %d: Maximum Value = %d\n",
                   test, result);

            free(wt);
            free(val);
        }
    }

    fclose(input);
    fclose(output);

    printf("\nResults saved in result.txt\n");

    return 0;
}
