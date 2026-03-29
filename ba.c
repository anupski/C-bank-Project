#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

typedef struct {
    int accountNumber;
    char name[60];
    char password[25];
    double balance;
    char history[1000];
} Account;

Account accounts[MAX_USERS];
int accountCount = 0;


void loadAccounts();
void saveAccounts();
int login();
void createAccount();
void deposit(int userIndex);
void withdraw(int userIndex);
void checkBalance(int userIndex);
void transfer(int userIndex);
void searchAccountByName();
void applyInterest(double rate);
void addTransaction(int userIndex, char details[]);
void viewHistory(int userIndex);
void changePassword(int userIndex);
void deleteAccount();
void listAccounts();
int adminLogin();
void adminMenu();

int main() {
    int choice, userIndex;

    loadAccounts();

    while (1) {
        printf("\n================================================================================\n");
        printf("\n                           -| WELCOME TO ZOL BANK |-                            \n");
        printf("\n================================================================================\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("4. Admin Login\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            createAccount();
            break;

        case 2:
            userIndex = login();
            if (userIndex != -1) {
                int option;
                do {
                    printf("\n========================================================================\n");
                    printf("                      | ACCOUNT DASHBOARD |\n");
                    printf("========================================================================\n");
                    printf("1. Deposit         ||     5. Search Account\n");
                    printf("2. Withdraw        ||     6. Apply Interest\n");
                    printf("3. Check Balance   ||     7. Transaction History\n");
                    printf("4. Transfer        ||     8. Change Password\n");
                    printf("                   ||     9. Logout\n");
                    printf("------------------------------------------------------------------------\n");
                    printf("Enter option: ");
                    scanf("%d", &option);

                    switch (option) {
                        case 1: deposit(userIndex); break;
                        case 2: withdraw(userIndex); break;
                        case 3: checkBalance(userIndex); break;
                        case 4: transfer(userIndex); break;
                        case 5: searchAccountByName(); break;
                        case 6: applyInterest(5.0); break;
                        case 7: viewHistory(userIndex); break;
                        case 8: changePassword(userIndex); break;
                    }

                } while (option != 9);
            }
            break;

        case 3:
            saveAccounts();
            exit(0);

        case 4:
            if (adminLogin()) {
                adminMenu();
            }
            break;
        }
    }
}


void loadAccounts() {
    FILE *fp = fopen("accounts.dat", "rb");
    if (fp != NULL) {
        fread(&accountCount, sizeof(int), 1, fp);
        fread(accounts, sizeof(Account), accountCount, fp);
        fclose(fp);
    }
}

void saveAccounts() {
    FILE *fp = fopen("accounts.dat", "wb");
    fwrite(&accountCount, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(Account), accountCount, fp);
    fclose(fp);
}


void createAccount() {
    if (accountCount >= MAX_USERS) {
        printf("Max accounts reached!\n");
        return;
    }

    Account newAcc;
    newAcc.accountNumber = accountCount + 1001;

    printf("Enter Name: ");
    scanf("%59s", newAcc.name);

    printf("Enter Password: ");
    scanf("%24s", newAcc.password);

    newAcc.balance = 0.0;
    newAcc.history[0] = '\0';

    accounts[accountCount++] = newAcc;
    saveAccounts();

    printf("\n✔ Account created successfully!\n");
    printf("Your Account Number: %d\n", newAcc.accountNumber);
}


int login() {
    int accNo;
    char pass[25];

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    printf("Enter Password: ");
    scanf("%s", pass);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNo &&
            strcmp(accounts[i].password, pass) == 0) {
            printf("\n✔ Login Successful! Welcome %s\n", accounts[i].name);
            return i;
        }
    }

    printf("Invalid credentials!\n");
    return -1;
}


void deposit(int userIndex) {
    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }

    accounts[userIndex].balance += amount;

    char msg[100];
    sprintf(msg, "Deposited: %.2f", amount);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Deposited %.2f\n", amount);
}

void withdraw(int userIndex) {
    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0 || amount > accounts[userIndex].balance) {
        printf("Invalid or insufficient balance!\n");
        return;
    }

    accounts[userIndex].balance -= amount;

    char msg[100];
    sprintf(msg, "Withdrawn: %.2f", amount);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Withdrawn %.2f\n", amount);
}

void transfer(int userIndex) {
    int targetAcc;
    double amount;

    printf("Enter recipient account: ");
    scanf("%d", &targetAcc);

    if (accounts[userIndex].accountNumber == targetAcc) {
        printf("Cannot transfer to your own account!\n");
        return;
    }

    printf("Enter amount: ");
    scanf("%lf", &amount);

    int targetIndex = -1;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == targetAcc) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("Account not found!\n");
        return;
    }

    if (amount <= 0 || amount > accounts[userIndex].balance) {
        printf("Invalid or insufficient balance!\n");
        return;
    }

    accounts[userIndex].balance -= amount;
    accounts[targetIndex].balance += amount;

    char msg[100];
    sprintf(msg, "Transferred %.2f to %d", amount, targetAcc);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Transfer successful!\n");
}


void checkBalance(int userIndex) {
    printf("\nCurrent Balance: %.2f\n", accounts[userIndex].balance);
}

void searchAccountByName() {
    char name[50];
    printf("Enter name: ");
    scanf("%s", name);

    int found = 0;

    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            printf("Account No: %d | Balance: %.2f\n",
                   accounts[i].accountNumber,
                   accounts[i].balance);
            found = 1;
        }
    }

    if (!found) printf("No account found!\n");
}

void applyInterest(double rate) {
    for (int i = 0; i < accountCount; i++) {
        accounts[i].balance += accounts[i].balance * rate / 100.0;
    }

    saveAccounts();
    printf("Interest applied: %.2f%%\n", rate);
}


void addTransaction(int userIndex, char details[]) {
    if (strlen(accounts[userIndex].history) + strlen(details) < 990) {
        strcat(accounts[userIndex].history, details);
        strcat(accounts[userIndex].history, "\n");
    }
}

void viewHistory(int userIndex) {
    printf("\n------------| Transaction History |------------\n");
    if (strlen(accounts[userIndex].history) == 0) {
        printf("No transactions yet.\n");
    } else {
        printf("%s", accounts[userIndex].history);
    }
}


void changePassword(int userIndex) {
    char oldPass[25], newPass[25];

    printf("Enter old password: ");
    scanf("%s", oldPass);

    if (strcmp(oldPass, accounts[userIndex].password) != 0) {
        printf("Incorrect password!\n");
        return;
    }

    printf("Enter new password: ");
    scanf("%s", newPass);

    strcpy(accounts[userIndex].password, newPass);
    saveAccounts();

    printf("Password changed successfully!\n");
}


int adminLogin() {
    char user[20], pass[20];

    printf("Admin Username: ");
    scanf("%s", user);
    printf("Admin Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        return 1;
    }

    printf("Invalid admin credentials!\n");
    return 0;
}

void listAccounts() {
    printf("\n================ ALL ACCOUNTS ================\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Acc No: %d | Name: %s | Balance: %.2f\n",
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].balance);
    }
}

void deleteAccount() {
    int accNo;
    printf("Enter account number to delete: ");
    scanf("%d", &accNo);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNo) {
            for (int j = i; j < accountCount - 1; j++) {
                accounts[j] = accounts[j + 1];
            }

            accountCount--;
            saveAccounts();

            printf("Account deleted successfully!\n");
            return;
        }
    }

    printf("Account not found!\n");
}

void adminMenu() {
    int choice;

    do {
        printf("\n==================================================\n");
        printf("                | ADMIN CONTROL PANEL |\n");
        printf("==================================================\n");
        printf("1. View All Accounts\n");
        printf("2. Apply Interest\n");
        printf("3. Delete Account\n");
        printf("4. Exit\n");
        printf("--------------------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: listAccounts(); break;
            case 2: applyInterest(5.0); break;
            case 3: deleteAccount(); break;
        }

    } while (choice != 4);
}