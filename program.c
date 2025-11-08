#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

void createAccount();
void showAllAccounts();
void searchAccount();
void depositMoney();
void withdrawMoney();
void transferMoney();
void clearAllAccounts();
int login();

int main() {
    int choice;

    printf("\n-----------------------------\n");
    printf("   BANK MANAGEMENT SYSTEM\n");
    printf("-----------------------------\n");

    if (!login()) {
        printf("\nAccess Denied! Exiting program.\n");
        return 0;
    }

    while (1) {
        printf("\n-----------------------------");
        printf("\n1. Create New Account");
        printf("\n2. Show All Accounts");
        printf("\n3. Search Account");
        printf("\n4. Deposit Money");
        printf("\n5. Withdraw Money");
        printf("\n6. Transfer Money");
        printf("\n7. Clear All Accounts");
        printf("\n8. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: showAllAccounts(); break;
            case 3: searchAccount(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: transferMoney(); break;
            case 7: clearAllAccounts(); break;
            case 8: 
                printf("\nExiting... Thank you!\n");
                exit(0);
            default: printf("\nInvalid choice! Try again.\n");
        }
    }
    return 0;
}

int login() {
    char username[20], password[20];
    int attempts = 0;

    while (attempts < 3) {
        printf("\nEnter Username: ");
        scanf("%s", username);
        printf("Enter Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
            printf("\n Login Successful! Welcome, %s.\n", username);
            return 1;
        } else {
            printf("\n Invalid Username or Password! Try again.\n");
            attempts++;
        }
    }
    return 0;
}

void createAccount() {
    FILE *file = fopen("bank_records.txt", "ab");
    struct Account acc;

    if (file == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accountNumber);
    printf("Enter Name: ");
    scanf("%s", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("\n Account created successfully!\n");
}

void showAllAccounts() {
    FILE *file = fopen("bank_records.txt", "rb");
    struct Account acc;

    if (file == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\n----- All Accounts -----\n");
    while (fread(&acc, sizeof(acc), 1, file)) {
        printf("\nAccount No: %d", acc.accountNumber);
        printf("\nName: %s", acc.name);
        printf("\nBalance: ₹%.2f\n", acc.balance);
    }

    fclose(file);
}

void searchAccount() {
    FILE *file = fopen("bank_records.txt", "rb");
    struct Account acc;
    int accNum, found = 0;

    if (file == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\nEnter Account Number to Search: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNum) {
            printf("\n Account Found!\n");
            printf("Account No: %d\n", acc.accountNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: ₹%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found)
        printf("\n Account not found!\n");
}

void depositMoney() {
    FILE *file = fopen("bank_records.txt", "rb+");
    struct Account acc;
    int accNum, found = 0;
    float amount;

    if (file == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);
    printf("Enter Amount to Deposit: ₹");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNum) {
            acc.balance += amount;
            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            printf("\n Deposit Successful! New Balance: ₹%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found)
        printf("\n Account not found!\n");
}

void withdrawMoney() {
    FILE *file = fopen("bank_records.txt", "rb+");
    struct Account acc;
    int accNum, found = 0;
    float amount;

    if (file == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);
    printf("Enter Amount to Withdraw: ₹");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNum) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                printf("\n Withdrawal Successful! Remaining Balance: ₹%.2f\n", acc.balance);
            } else {
                printf("\n Insufficient Balance!\n");
            }
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found)
        printf("\n Account not found!\n");
}

void transferMoney() {
    FILE *file = fopen("bank_records.txt", "rb+");
    struct Account accounts[100];
    int total = 0, senderAcc, receiverAcc, senderIndex = -1, receiverIndex = -1;
    float amount;

    if (file == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    while (fread(&accounts[total], sizeof(struct Account), 1, file))
        total++;
    fclose(file);

    printf("\nEnter Sender Account Number: ");
    scanf("%d", &senderAcc);
    printf("Enter Receiver Account Number: ");
    scanf("%d", &receiverAcc);
    printf("Enter Amount to Transfer: ₹");
    scanf("%f", &amount);

    for (int i = 0; i < total; i++) {
        if (accounts[i].accountNumber == senderAcc)
            senderIndex = i;
        if (accounts[i].accountNumber == receiverAcc)
            receiverIndex = i;
    }

    if (senderIndex == -1 || receiverIndex == -1) {
        printf("\n Invalid Account Number(s)!\n");
        return;
    }

    if (accounts[senderIndex].balance < amount) {
        printf("\n Insufficient Balance!\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    file = fopen("bank_records.txt", "wb");
    for (int i = 0; i < total; i++)
        fwrite(&accounts[i], sizeof(struct Account), 1, file);
    fclose(file);

    printf("\n ₹%.2f Transferred Successfully!\n", amount);
}

void clearAllAccounts() {
    char confirm;
    printf("\nAre you sure you want to delete all account records? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        remove("bank_records.txt");
        printf("\n All account records have been cleared.\n");
    } else {
        printf("\nOperation cancelled.\n");
    }
}
