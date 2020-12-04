/*
    This program will:
        Read two data files
        Print their data to the screen
        Process the data
        Print a report to the screen and write a report to a file
        Update the products in the inventory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 25
#define MAX_LINE_ITEMS 200
#define MAX_READ_CHARS 200

typedef struct{
    char product_code[10];
    char description[50];
    double price;
    int quantity;
} product_t;

typedef struct{
    int invoice_id;
    char product_code[10];
    double price;
    int quantity;
    double total;
} line_item_t;

int read_products(product_t pa[], char filename[]);
void print_product(product_t p);
void print_products(product_t pa[], int pa_count);
int read_line_items(line_item_t la[], char filename[]);
void print_line_item(line_item_t l);
void print_line_items(line_item_t la[], int line_items);
void zero_quantities(int quantity_sums[], int pa_count);
void zero_totals(double total_sums[], int pa_count);
void calc_report(product_t pa[], int pa_count, line_item_t la[], int la_count, double total_sums[], int quantity_sums[]);
void print_report(product_t pa[], int pa_count, double total_sums[], int quantity_sums[]);
void write_report(product_t pa[], int pa_count, double total_sums[], int quantity_sums[]);
void update_products(product_t pa[], int pa_count, int quantity_sums[]);
void write_products(product_t pa[], int pa_count, char filename[]);

int main()
{
    product_t pa[MAX_PRODUCTS];
    int pa_count = read_products(pa, "products.csv");
    printf("%d products read.\n", pa_count);
    print_products(pa, pa_count);

    printf("\n");

    line_item_t la[MAX_LINE_ITEMS];
    int la_count = read_line_items(la, "lineItems.csv");
    printf("%d line items read.\n", la_count);
    print_line_items(la, la_count);

    printf("\n");

    int quantity_sums[pa_count];
    zero_quantities(quantity_sums, pa_count);
    double total_sums[pa_count];
    zero_totals(total_sums, pa_count);
    calc_report(pa, pa_count, la, la_count, total_sums, quantity_sums);
    print_report(pa, pa_count, total_sums, quantity_sums);
    write_report(pa, pa_count, total_sums, quantity_sums);

    printf("\n");

    update_products(pa, pa_count, quantity_sums);
    printf("Updated products\n");
    print_products(pa, pa_count);
    write_products(pa, pa_count, "newProducts2.txt");

    return 0;
}

// Counts the number of products and reads the data in the file
int read_products(product_t pa[], char filename[])
{
    FILE *file = fopen(filename, "r");
    char buffer[MAX_READ_CHARS];
    int count = 0;
    int i = 0;
    if(file != NULL)
        while(fgets(buffer, MAX_READ_CHARS, file) != NULL)
        {
            strcpy(pa[i].product_code, strtok(buffer, ","));
            strcpy(pa[i].description, strtok(NULL, ","));
            pa[i].price = atof(strtok(NULL, ","));
            pa[i].quantity = atoi(strtok(NULL, ","));
            i++;
            count++;
        }
    else
    {
        perror("File not found");
        exit(1);
    }
    fclose(file);
    return count;
}

// Prints a product to the screen
void print_product(product_t p)
{
    printf("%-6s %-50s %10.2f %10d\n", p.product_code, p.description, p.price, p.quantity);
}

// Prints all products from file to the screen
void print_products(product_t pa[], int pa_count)
{
    printf("%-6s %-50s %10s %10s\n", "[CODE]", "[DESCRIPTION]", "[PRICE]", "[QUANTITY]");
    for(int i=0; i<pa_count; i++)
            print_product(pa[i]);
}

// Counts the number of lint items and reads the data in the file
int read_line_items(line_item_t la[], char filename[])
{
    FILE *file = fopen(filename, "r");
    char buffer[MAX_READ_CHARS];
    int count = 0;
    int i = 0;
    if(file != NULL)
        while(fgets(buffer, MAX_READ_CHARS, file) != NULL)
        {
            la[i].invoice_id = atoi(strtok(buffer, ","));
            strcpy(la[i].product_code, strtok(NULL, ","));
            la[i].price = atof(strtok(NULL, ","));
            la[i].quantity = atoi(strtok(NULL, ","));
            la[i].total = atof(strtok(NULL, ","));
            i++;
            count++;
        }
    else
    {
        perror("File not found");
        exit(2);
    }
    fclose(file);
    return count;
}

// Prints a line item to the screen
void print_line_item(line_item_t l)
{
    printf("%-10d %-10s %10.2f %15d %10.2f\n", l.invoice_id, l.product_code, l.price, l.quantity, l.total);
}

// Prints all line items to the screen
void print_line_items(line_item_t la[], int line_items)
{
    printf("%-10s %-10s %10s %15s %10s\n", "[INVOICE]", "[CODE]", "[PRICE]", "[QUANTITY]", "[TOTAL]");
    for(int i=0; i<line_items; i++)
            print_line_item(la[i]);
}

// Writes zeros (type int) to all elements in quantity_sums[]
void zero_quantities(int quantity_sums[], int pa_count)
{
    for(int i=0; i<pa_count; i++)
        quantity_sums[i] = 0;
}

// Writes zeros (type double) to all elements in total_sums[]
void zero_totals(double total_sums[], int pa_count)
{
    for(int i=0; i<pa_count; i++)
        total_sums[i] = 0.0;
}

// Sums up the total revenue and quantity of items purchased
void calc_report(product_t pa[], int pa_count, line_item_t la[], int la_count, double total_sums[], int quantity_sums[])
{
    for(int i=0; i<la_count; i++)
        for(int j=0; j<pa_count; j++)
            if(strcmp(la[i].product_code, pa[j].product_code) == 0)
            {
                total_sums[j] += pa[j].price * la[i].quantity;
                quantity_sums[j] += la[i].quantity;
            }
}

// Prints sales report to the screen
void print_report(product_t pa[], int pa_count, double total_sums[], int quantity_sums[])
{
    int tot_quantity = 0;
    double tot_total = 0.0;

    printf("Product Sales Report\n%-10s %10s %10s\n", "[Code]", "[Quantity]", "[Total]");

    for(int i=0; i<pa_count; i++)
    {
        printf("%-10s %10d %10.2f\n", pa[i].product_code, quantity_sums[i], total_sums[i]);
        tot_quantity += quantity_sums[i];
        tot_total += total_sums[i];
    }

    printf("\n%-20s %15d\n%-20s %10.2f\n", "Total items sold:", tot_quantity, "Total revenue from sales:", tot_total);
}

// Writes sales report to a file
void write_report(product_t pa[], int pa_count, double total_sums[], int quantity_sums[])
{
    int tot_quantity = 0;
    double tot_total = 0.0;

    FILE *file = fopen("report2.txt", "w");

    if(file != NULL)
    {
        fprintf(file, "Product Sales Report\n%-10s %10s %10s\n", "[Code]", "[Quantity]", "[Total]");

        for(int i=0; i<pa_count; i++)
        {
            fprintf(file, "%-10s %10d %10.2f\n", pa[i].product_code, quantity_sums[i], total_sums[i]);
            tot_quantity += quantity_sums[i];
            tot_total += total_sums[i];
        }

        fprintf(file, "\n%-20s %15d\n%-20s %10.2f\n", "Total items sold:", tot_quantity, "Total revenue from sales:", tot_total);
    }
    else
    {
        perror("File failed to open");
        exit(3);
    }

    fclose(file);
}

// Updates the product list with the line item data
void update_products(product_t pa[], int pa_count, int quantity_sums[])
{
    for(int i=0; i<pa_count; i++)
        pa[i].quantity -= quantity_sums[i];
}

//Writes updated products to a file
void write_products(product_t pa[], int pa_count, char filename[])
{
    FILE *file = fopen(filename, "w");

    if(file != NULL)
    {
        fprintf(file, "Updated products\n%-6s %-50s %10s %10s\n", "[CODE]", "[DESCRIPTION]", "[PRICE]", "[QUANTITY]");
        for(int i=0; i<pa_count; i++)
        {
            fprintf(file, "%-6s %-50s %10.2f %10d\n", pa[i].product_code, pa[i].description, pa[i].price, pa[i].quantity);
        }
    }
    else
    {
        perror("File failed to open");
        exit(4);
    }

    fclose(file);
}
