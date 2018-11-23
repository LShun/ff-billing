/******************************************************************************

Name: hiewLongShun.c
Version: 1.2
Description: A program to enable the restaurant to bill their customers quickly, 
             and at the end of the day produce a Daily Sales Report.

*******************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

//constants
#define PRICE_A 10.00
#define PRICE_B 15.00
#define PRICE_C 18.00
#define PRICE_D 24.00
#define TAX 10

//custom data types
typedef struct
{
    int comboA, comboB, comboC, comboD, customers; // variables to track sales for daily sales report
    int nowA, nowB, nowC, nowD; // keeps track of what the present customer order to make cancellation of order possible.
	double gross, sst, net;

}
report;

//function prototypes
void printMenu();
double takeOrders(report *salesReport);
int billing(double *grossTotal, report *salesReport);
int statement(report *daily);
int cancel(report *report);

int main()
{
    //variables
    report *dailyReport = malloc(sizeof(report));
    double gTotal;
    char nextCust, c;
    
    //display information
    //display splash screen
    printf(" ____  ____  ___    ____  __   ____  ____  ____  __    __  ____ \n"
        "/ ___)(  __)/ __)  (  __)/ _\\ / ___)(_  _)(  __)/  \\  /  \\(    \\\n"
        "\\___ \\ ) _)( (__    ) _)/    \\\\___ \\  )(   ) _)(  O )(  O )) D (\n"
        "(____/(__)  \\___)  (__) \\_/\\_/(____/ (__) (__)  \\__/  \\__/(____/\n\n");
    
    //set all elements of dailyReport to 0.
    dailyReport->customers = 0;
    dailyReport->comboA = 0;
    dailyReport->comboB = 0;
    dailyReport->comboC = 0;
    dailyReport->comboD = 0;
	dailyReport->gross = 0;
	dailyReport->sst = 0;
	dailyReport->net = 0;
    
    
    do
    {
        //Print customer number
        dailyReport->customers += 1;
        printf("You are customer: %d\n\n", dailyReport->customers);
        
        //display menu
        printMenu();
        
        //take orders
        gTotal = takeOrders(dailyReport);
    
        //process billing
        billing(&gTotal, dailyReport);

        //display ending screen
        printf("\nThank you, have a nice day and see you later!\n\n");
        
        //ask for next customer
        printf("Next customer? (y/n): ");
        nextCust = getchar();
        while ((c = getchar()) != '\n' && c != EOF);
    }
    while(tolower(nextCust) == 'y');
    
    //print Report
    statement(dailyReport);

	//Display ending message
	printf("\n" "----ENDING----\n");
	printf("~Thank you for using SFC Fast Food's Fast Food Billing System (F2BS)~\n");
	printf("Made by: Hiew LS\n");
	printf("--------------\n");
	system("pause");
	free(dailyReport);
    return 0;
}

void printMenu() //prints menu
{   
    printf("----MENU----\n");
    printf("%-5s %-100s %s\n\n", "Combo", "Contents", "Price (RM)");
	printf("%-5s %-100s %10.2f\n", "A", "Chicken Burger + Potato Wedges (S) +  6 Chicken Nuggets + Coca-cola Drink (L)", PRICE_A);
	printf("%-5s %-100s %10.2f\n", "B", "Marinated Fried Chicken + Chicken Rice + Coleslaw (L) + Pepsi drink (L)", PRICE_B);
	printf("%-5s %-100s %10.2f\n", "C", "2 BBQ Fried Chicken + Chicken Rice + 2 Pepsi drink (S)", PRICE_C);
	printf("%-5s %-100s %10.2f\n", "D", "3 Marinated Fried Chicken + 2 Chicken Rice + 6 Chicken Nuggets + 2 Pepsi drink (S)", PRICE_D);
}

double takeOrders(report *salesReport)
{
    //variables
    char combo, c;
    int qty, *ptr,  *nowPtr;
    double price, cTotal, gTotal = 0;
    
    //initialize now[Combo] in report to 0 - keep track of current order
    salesReport->nowA = 0;
    salesReport->nowB = 0;
    salesReport->nowC = 0;
    salesReport->nowD = 0;
    
    //let cashier know ready to order
    putchar('\n');
    printf("----ORDERING----\n");
    
    //get combo type
    while(1)
    {
        //get combo type
        printf("Combo A,B,C,D (other = exit, X = cancel): ");
        combo = getchar(); 
        while ((c = getchar()) != '\n' && c != EOF);
        
        //set price - nowPtr is used as duplicate entry to track order cancellation
        switch (combo) 
        {
            case 'A':
            {
                price = PRICE_A;
                ptr = &(salesReport->comboA);
                nowPtr = &(salesReport->nowA);
                break;
            }
            case 'B':
            {
                price = PRICE_B;
                ptr = &(salesReport->comboB);
                nowPtr = &(salesReport->nowB);
                break;
            }
            case 'C':
            {
                price = PRICE_C;
                ptr = &(salesReport->comboC);
                nowPtr = &(salesReport->nowC);
                break;
            }
            case 'D':
            {
                price = PRICE_D;
                ptr = &(salesReport->comboD);
                nowPtr = &(salesReport->nowD);
                break;
            }
            case 'X':
            {
                return cancel(salesReport);
            }
            default:
            {
                return(gTotal);
            }
        }
        
        //get quantity
        printf("Quantity\t: "); 
        scanf("%d", &qty);

		//add quantity to pointed combo in daily report
        *nowPtr = *ptr += qty;
        while ((c = getchar()) != '\n' && c != EOF);
        
        //calculate combo total
        cTotal = qty * price;
        
        //display combo total
        printf("\tCombo %c x %d @ RM %8.2f = RM %9.2f\n", combo, qty, price, cTotal);
        
        //add combo total to gross total
        gTotal += cTotal; 
    }
}

int billing(double *grossTotal, report *salesReport)
{
    //variables
    char c;
    double change, netTotal, paid, sstTotal; 
    
    //check if gross is acceptably equal zero
    if (*grossTotal < 0.0001)
    {
        return 0; // no need to calculate, since order is empty
    }
    
    putchar('\n');
    printf("----BILLING----\n");
    
    //calculate SST amount
    sstTotal = *grossTotal * TAX / 100;

    //calculate net total
    netTotal = *grossTotal + sstTotal;
    
    //display gross total
    printf("\tGross total\t= RM %8.2f\n", *grossTotal);

    //display net total
    printf("\tTOTAL PAYABLE\t= RM %8.2f\n", netTotal);
    
    //accept payment
    printf("\tAmount Paid (<-999 = cancel)\t= RM    ");
    scanf("%lf", &paid);
    while ((c = getchar()) != '\n' && c != EOF);
    
    //check if cancel order
    if (paid < -998)
    {
        return cancel(salesReport);
    }
    
    //calculate change
    change = paid - netTotal;
    //display change
    printf("\tChange\t\t= RM\t%1.2f\n", change);

	//accumulate gross total
	salesReport->gross += *grossTotal;

	//accumulate tax total
	salesReport->sst += sstTotal;

	//accumulate net total
	salesReport->net += netTotal;
    
    return 0;
}

int statement(report *daily)
{
    //variables
    char saveFile[35];
    double totalA, totalB, totalC, totalD;
    FILE *fp;
    int totalQty;
    struct tm *timeInfo;
    time_t rawTime;
    
    //process
    
    //calculate combo sales amount
    totalA = daily->comboA * PRICE_A;
    totalB = daily->comboB * PRICE_B;
    totalC = daily->comboC * PRICE_C;
    totalD = daily->comboD * PRICE_D;
    
    //calculate total combo sold
    totalQty = daily->comboA + daily->comboB + daily->comboC + daily->comboD;
    
    //output
    
    //let cashier know report generated
    printf("\n" "----DAILY SALES REPORT----\n");
    
    //print total customers
    printf("Total customers for today: %d\n\n", daily->customers);
    
    //display combo sales 
    printf("Combo Sales For Today\n\n");
    printf("%-10s %-13s %15s\n", "Combo", "Quantity Sold", "Sales Amount");
	printf("  %-8c %13d %15.2f\n", 'A', daily->comboA, totalA);
	printf("  %-8c %13d %15.2f\n", 'B', daily->comboB, totalB);
	printf("  %-8c %13d %15.2f\n", 'C', daily->comboC, totalC);
	printf("  %-8c %13d %15.2f\n", 'D', daily->comboD, totalD);
    
	//display gross total
    putchar('\n');
    printf("%-10s %13s %15s\n", "======", "======", "======");
    printf("%-10s %13d %15.2f\n\n", "TOTALS", totalQty, daily->gross);
    
    printf("\t%-20s = %9.2f\n", "TOTAL SST charges", daily->sst);	//print total SST collected
    printf("\t%-20s = %9.2f\n", "TOTAL RM collected", daily->net);	//print net total
    
    printf("\n" "Do you want to write the daily sales report to a file? (y/n): ");	//ask user if want to write to file
    if (tolower(getchar()) == 'y')
    {
        //get file name in format "Daily-Sales-Report-DD-MM-YYYY"
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        snprintf(saveFile, 35, "Daily-Sales-Report-%d-%d-%d.txt", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
        
        //create/open the save file file
        fp = fopen(saveFile, "a");
        if (fp == NULL)
        {
            printf("File cannot be created, please copy-paste output into a new text file.");
            return 1;
        }
		
		fprintf(fp, "----Daily Sales Report %d-%d-%d----\n\n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);

		//print total customers
		fprintf(fp, "Total customers for today: %d\n\n", daily->customers);

        //print combo sales details
		fprintf(fp, "%-10s %-13s %15s\n", "Combo", "Quantity Sold", "Sales Amount");
		fprintf(fp, "  %-8c %13d %15.2f\n", 'A', daily->comboA, totalA);
		fprintf(fp, "  %-8c %13d %15.2f\n", 'B', daily->comboB, totalB);
		fprintf(fp, "  %-8c %13d %15.2f\n", 'C', daily->comboC, totalC);
		fprintf(fp, "  %-8c %13d %15.2f\n", 'D', daily->comboD, totalD);
    
        fprintf(fp, "%-10s %13s %15s\n"
                    "%-10s %13d %15.2f\n\n", "======", "======", "======", "TOTALS", totalQty, daily->gross);
    
        //print total SST collected
		fprintf(fp, "\t%-20s = %9.2f\n"
					"\t%-20s = %9.2f\n", "TOTAL SST charged", daily->sst, "TOTAL RM collected", daily->net);

		fprintf(fp, "----REPORT END----");
		
		printf("Successfully wrote to: .\\Daily-Sales-Report-%d-%d-%d.txt\n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
		fclose(fp);
    }
	return 0;
}

//cancel order function

int cancel(report *report)
{
    printf("\n" "----Cancellation----\n\n");
    
    //deduct combo's cancelled for current order
    report->comboA -= report->nowA;
    report->comboB -= report->nowB;
    report->comboC -= report->nowC;
    report->comboD -= report->nowD;
    
    //subtract customer count
    report->customers--;
    
    //inform cashier of cancellation
    puts("Order cancelled, customer count - 1.\n");
    
    return 0;
}

