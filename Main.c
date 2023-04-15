#include <ctype.h> // isdigit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep

char input[50], username[50];
FILE *fp;
int balance = 0;

void check_quit();

void login();

void create();

void dashboard(int);

void deposit();

void withdraw();

int main() {
    START:
    // Clearing the screen first line works on VS Code only while second is for EXE
    printf("\e[1;1H\e[2J");
    // system("cls");
    printf("Welcome To Bank\n");
    printf("Login or Create a new Account\n>> ");
    gets(input);
    strlwr(input);

    check_quit();

    if (!strcmp(input, "login"))
        login();
    else if (!strcmp(input, "create"))
        create();
    else {
        printf("Invald Operation or a typo\n");
        sleep(1);
        goto START;
    }

    return 0;
}

void check_quit() {
    if (!strcmp(input, "q")) {
        fclose(fp);
        exit(0);
    }
}

void login() {
    printf("Enter User Name\n>> ");
    gets(input);

    if (!strcmp(input, "create")) {
        printf("Create Account\n");
        create();
    }

    check_quit();

    fp = fopen(strcat(input, ".txt"), "r");
    if (fp == NULL) {
        printf("Username not Found\n");
        sleep(1);
        login();
    }

    strcpy(username, input);
    // Removing the '.txt' from Username
    int i = strlen(username);
    for (int j = i; j >= i - 4; j--)
        username[j] = 0;

    LOGIN_PASS:
    printf("Enter Password\n>> ");
    gets(input);
    check_quit();
    // Getting Password from File
    i = 0;
    char t, temp[50];
    while ((t = fgetc(fp)) != '\n')
        temp[i++] = t;
    temp[i] = '\0';

    if (strcmp(input, temp) != 0) {
        printf("Incorrect Password\n");
        sleep(1);
        goto LOGIN_PASS;
    }

    memset(temp, 0, strlen(temp));
    // Getting and Calculating the Balance of the User from File
    i = 0;
    while ((t = fgetc(fp)) != EOF)
        if (t == '\n') {
            balance += atoi(temp); // atoi converts string to int
            memset(temp, 0, strlen(temp));
            i = 0;
        }
        else
            temp[i++] = t;

    fclose(fp);
    dashboard(1);
}

void create() {
    printf("Enter User Name\n>> ");
    gets(input);
    check_quit();

    fp = fopen(strcat(input, ".txt"), "w");
    if (fp == NULL) {
        printf("Something Went Wrong. While Creating The File\n");
        sleep(1);
        create();
    }

    printf("Enter Password\n>> ");
    gets(input);
    check_quit();

    fprintf(fp, strcat(input, "\n"));
    fclose(fp);

    printf("Account Created Successfully\nLogin\n");
    login();
}

void dashboard(int first_time) {
    if (first_time) {
        printf("Welcome To Bank\n");
        printf("User : %s\n", username);
    }
    printf("Check Balance, Deposit, Withdraw or logout\n");

    DASHIN:
    printf(">> ");
    gets(input);
    strlwr(input);
    check_quit();

    if (!strcmp(input, "balance")) {
        printf("Balance : %d\n", balance);
        goto DASHIN;
    }
    else if (!strcmp(input, "deposit"))
        deposit();
    else if (!strcmp(input, "withdraw"))
        withdraw();
    else if (!strcmp(input, "logout"))
        main();
    else {
        printf("Invalid Operation\n");
        sleep(1);
        goto DASHIN;
    }
}

void deposit() {
    DEPOIN:
    printf("Enter Amount\n>> ");
    gets(input);
    strlwr(input);
    check_quit();
    // Verifying if the enterned amount is valid(digits only) or not
    int is_digit = 1, i = 0, j = strlen(input);
    while (i < j && is_digit)
        if (!isdigit(input[i++]))
            is_digit = 0;

    if (!is_digit) {
        printf("Invalid Amount\n");
        sleep(1);
        goto DEPOIN;
    }

    fp = fopen(strcat(username, ".txt"), "a");
    // Removing the '.txt' from Username
    i = strlen(username);
    for (j = i; j >= i - 4; j--)
        username[j] = 0;

    if (fp == NULL) {
        printf("Something Went Wrong. While Opening File\n");
        exit(0);
    }

    balance += atoi(input);
    fseek(fp, -1, SEEK_END); // Moving the cursor back one place from end
    // Writing the deposited amount into the file
    j = strlen(input);
    for (i = 0; i < j; i++)
        fputc(input[i], fp);
    fputc('\n', fp);
    fclose(fp);
    dashboard(0);
}

void withdraw() {
    WITHIN:
    printf("Enter Amount\n>> ");
    gets(input);
    strlwr(input);
    check_quit();

    int is_digit = 1, j = 0;
    while (j < strlen(input) && is_digit)
        if (!isdigit(input[j++]))
            is_digit = 0;

    if (!is_digit) {
        printf("Invalid Amount\n");
        sleep(1);
        goto WITHIN;
    }

    if (atoi(input) > balance) {
        printf("Insufficent Balance\n");
        sleep(1);
        goto WITHIN;
    }

    fp = fopen(strcat(username, ".txt"), "a");
    int i = strlen(username);
    for (int j = i; j >= i - 4; j--)
        username[j] = 0;

    if (fp == NULL) {
        printf("Something Went Wrong. While Opening File\n");
        exit(0);
    }

    fseek(fp, -1, SEEK_END);
    balance -= atoi(input);
    fputc('-', fp); // Putting the minus sign to indicate that the amount is withdrawed
    for (i = 0; i < strlen(input); i++)
        fputc(input[i], fp);
    fputc('\n', fp);
    fclose(fp);
    dashboard(0);
}
