#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define max_lenth 32
char name[32];
void PrintTableGraph(int num, char** names, int** table);

void deleteNsort(int num, char*** names, int*** table, int n) {
    num--;
    int** table2 = calloc(num, sizeof(int*));
    for (int i = 0; i < num; i++) {
        table2[i] = calloc(num, sizeof(int));
    }

    char** names2 = calloc(num, sizeof(char*));
    char** names3 = calloc(num, sizeof(char*)); //to print

    int ic = 0, jc = 0;
    for (int i = 0; i < num; i++) {
        if (i == n) { ic = 1; }
        names2[i] = (*names)[i + ic];
        for (int j = 0; j < num; j++) {
            if (j == n) { jc = 1; }
            table2[i][j] = (*table)[i + ic][j + jc];
        }
        jc = 0;
    }

    int* vertex_degree = calloc(num, sizeof(int));

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            vertex_degree[i] += table2[i][j];
            vertex_degree[i] += table2[j][i];
        }
    }

    //bubble sort
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if (vertex_degree[i] < vertex_degree[j]) {
                //                swap(arr, i, j);
                int temp = vertex_degree[j];
                vertex_degree[j] = vertex_degree[i];
                vertex_degree[i] = temp;

                char* temp_name = names2[j];
                names2[j] = names2[i];
                names2[i] = temp_name;

                temp_name = names3[j];
                names3[j] = names3[i];
                names3[i] = temp_name;

                for (int k = 0; k < num; k++) {
                    int temp_table2 = table2[k][j];
                    table2[k][j] = table2[k][i];
                    table2[k][i] = temp_table2;
                }
                int* temp_table = table2[j];
                table2[j] = table2[i];
                table2[i] = temp_table;
            }
        }
    }

    for (int i = 0; i < num; i++) {
        names3[i] = calloc(max_lenth, sizeof(char));
        char temp[32];
        sprintf(temp, "%s (%d)", names2[i], vertex_degree[i]);
        names3[i] = strcat(names3[i], temp);
    }

    PrintTableGraph(num, names3, table2);

    *names = names2;
    *table = table2;
}

void terminal(char graph_mode, int num, char** names, int** table) {
    char string[4000] = { 0 };
    char arrow[3] = "";


    if (graph_mode == 1) {
        strcat(string, "digraph G {");
        strcat(arrow, "->");
    }
    else {
        strcat(string, "graph G {");
        strcat(arrow, "--");
    }

    for (int i = 0; i < num; i++) {
        strcat(string, names[i]);
        strcat(string, "; ");
    }
    strcat(string, name);
    strcat(string, "; ");
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            for (int k = 0; k < table[i][j]; k++) {
                strcat(string, names[i]);
                strcat(string, arrow);
                strcat(string, names[j]);
                strcat(string, "; ");
            }
        }
    }
    strcat(string, "}");
    FILE* fp = fopen("graph.dot", "w");

    if (fp == NULL) {
        printf("Unable to create file\n");
        exit(0);
    }
    fputs(string, fp);
    fclose(fp);

}

void PrintTableGraph(int num, char** names, int** table) {
    printf("\n");
    int graph_check = 1;
    for (int i = 0; i < num; i++) {
        int temp_graph_check = 0;
        for (int j = 0; j < num; j++) {
            if (table[i][j] == 1)
                temp_graph_check = 1;

            if (table[j][i] == 1)
                temp_graph_check = 1;
        }
        if (temp_graph_check == 0)
            graph_check = 0;
    }
    if (graph_check == 0) {
        printf("unrelated graph\n");
    }
    else {
        printf("related graph\n");
    }
    printf("--------------------\n");

    for (int i = 0; i < num; i++) {
        printf("%s | ", names[i]);
        for (int j = 0; j < num; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int mode;

    printf("0: graph\n1: digraph\n");
    scanf("%d", &mode);

    if (mode != 0 && mode != 1)
        exit(0);

    printf("elements: ");
    int amount;
    scanf("%d", &amount);
    getchar(); // to ignore \n
    if (amount <= 0)
        exit(0);

    printf("ribs: ");
    int ribs;
    scanf("%d", &ribs);
    getchar(); // to ignore \n
    if (amount <= 0)
        exit(0);


    char** elemetns = calloc(amount, sizeof(char*));
    for (int i = 0; i < amount; i++) {
        elemetns[i] = calloc(max_lenth + 1, sizeof(char));
    }

    int** graph = calloc(amount, sizeof(int*));
    for (int i = 0; i < amount; i++) {
        graph[i] = calloc(amount, sizeof(int));
    }

    int matrix[amount][ribs];

    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < amount; j++) {
            graph[i][j] = 0;
        }
    }

    printf("Enter names (less than 32 symb)\n");
    for (int i = 0; i < amount; i++) {
        printf("%d: ", i + 1);
        scanf("%s", elemetns[i]);
        for (int j = 0; j < i; j++) {
            if (strcmp(elemetns[i], elemetns[j]) == 0) {
                printf("This name allready exist, enter another\n");
                i--;
            }
        }
    }

    getchar(); //from scanf to ignor \n

    printf("Enter matrix\n");

    for (int i = 0; i < amount; i++) {
        printf("%s\t", elemetns[i]);
        for (int j = 0; j < ribs; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    int error = 0;
    for (int j = 0; j < ribs; j++) {
        int first_element = -1, second_element = -1;
        int temp1 = 0, temp2 = 0;
        for (int i = 0; i < amount; i++) {
            if (matrix[i][j] != 0) {
                if (first_element == -1) {
                    first_element = i;
                    temp1 = matrix[i][j];
                }
                else if (second_element == -1) {
                    second_element = i;
                    temp2 = matrix[i][j];
                }
                else {
                    error = 1;
                    j = ribs;
                    i = amount;
                }
            }
        }
        int O = -1;
        if (mode == 0) { //if graph
            O = 1;
        }
        if (first_element != -1 && second_element == -1 && temp1 == 2 && temp2 == 0) {
            graph[first_element][first_element] += 1;
        }
        else if (first_element != -1 && second_element != -1 && temp1 == 1 && temp2 == O) {
            graph[first_element][second_element] += 1;
        }
        else if (first_element != -1 && second_element != -1 && temp1 == O && temp2 == 1) {
            graph[second_element][first_element] += 1;
        }
        else {
            error = 1;
            j = ribs;
        }
    }
    if (error) {
        printf("matrix not valid\n");
        exit(9);
    }

    PrintTableGraph(amount, elemetns, graph);

    printf("Enter element's name: ");
    int n = 0;
    scanf("%s", name);
    for (int i = 0; i < amount; i++) {
        if (strcmp(name, elemetns[i]) == 0) {
            n = i; i = amount;
        }
    }

    deleteNsort(amount, &elemetns, &graph, n);

    amount--;
  

    terminal(mode, amount, elemetns, graph);

    return 0;
}
