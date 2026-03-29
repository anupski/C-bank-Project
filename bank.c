#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

typedef struct {
    int accountNumber;
    char name[60];
    char password[25];
    double balance;
    double loanBalance;
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
void takeLoan(int userIndex);
void repayLoan(int userIndex);

int main() {
    int choice, userIndex;

    loadAccounts();

    while (1) {
        printf("\n================================================================================\n");
        printf("                           -| WELCOME TO ZOL BANK |-                            \n");
        printf("================================================================================\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("4. Admin Login\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;

            case 2:
                userIndex = login();
                if (userIndex != -1) {
                    int option;
                    do {
                        printf("\n========================================================================\n");
                        printf("                      | ACCOUNT DASHBOARD |\n");
                        printf("========================================================================\n");
                        printf("1. Deposit         ||     6. Apply Interest\n");
                        printf("2. Withdraw        ||     7. Transaction History\n");
                        printf("3. Check Balance   ||     8. Change Password\n");
                        printf("4. Transfer        ||     9. Take Loan\n");
                        printf("5. Search Account  ||    10. Repay Loan\n");
                        printf("                   ||    11. Logout\n");
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
                            case 9: takeLoan(userIndex); break;
                            case 10: repayLoan(userIndex); break;
                            case 11: printf("Logging out...\n"); break;
                            default: printf("Invalid option!\n");
                        }

                    } while (option != 11);
                }
                break;

            case 3:
                saveAccounts();
                printf("Exiting... Goodbye!\n");
                exit(0);

            case 4:
                if (adminLogin()) adminMenu();
                break;

            default:
                printf("Invalid choice!\n");
        }
    }
}


void loadAccounts() {
    FILE *fp = fopen("accounts.dat", "rb");
    if (fp) {
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
        printf("Maximum accounts reached!\n");
        return;
    }

    Account newAcc;
    newAcc.accountNumber = accountCount + 1001;

    printf("Enter Name: ");
    scanf(" %[^\n]s", newAcc.name);  

    printf("Enter Password: ");
    scanf("%s", newAcc.password);

    newAcc.balance = 0;
    newAcc.loanBalance = 0;
    newAcc.history[0] = '\0';

    accounts[accountCount++] = newAcc;
    saveAccounts();

    printf("\nAccount created successfully!\n");
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
    printf("\nInvalid credentials!\n");
    return -1;
}


void deposit(int userIndex) {
    double amount;
    printf("Enter amount to deposit: ");
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
    printf("Deposit successful!\n");
}


void withdraw(int userIndex) {
    double amount;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0 || amount > accounts[userIndex].balance) {
        printf("Invalid amount or insufficient funds!\n");
        return;
    }

    accounts[userIndex].balance -= amount;

    char msg[100];
    sprintf(msg, "Withdrawn: %.2f", amount);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Withdrawal successful!\n");
}


void transfer(int userIndex) {
    int acc;
    double amount;

    printf("Enter account number to transfer to: ");
    scanf("%d", &acc);
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    int idx = -1;
    for (int i = 0; i < accountCount; i++)
        if (accounts[i].accountNumber == acc) idx = i;

    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }

    if (amount <= 0 || amount > accounts[userIndex].balance) {
        printf("Invalid amount or insufficient funds!\n");
        return;
    }

    accounts[userIndex].balance -= amount;
    accounts[idx].balance += amount;

    char msg[100];
    sprintf(msg, "Transferred %.2f to account %d", amount, acc);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Transfer successful!\n");
}


void searchAccountByName() {
    char name[50];
    printf("Enter name to search: ");
    scanf(" %[^\n]s", name);

    int found = 0;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            printf("Account No: %d | Balance: %.2f\n",
                   accounts[i].accountNumber,
                   accounts[i].balance);
            found = 1;
        }
    }

    if (!found) printf("No account found with that name.\n");
}


void applyInterest(double rate) {
    for (int i = 0; i < accountCount; i++)
        accounts[i].balance += accounts[i].balance * rate / 100;

    saveAccounts();
    printf("Interest applied at %.2f%% rate.\n", rate);
}


void takeLoan(int userIndex) {
    double amount;
    printf("Enter loan amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid loan amount!\n");
        return;
    }

    accounts[userIndex].loanBalance += amount;
    accounts[userIndex].balance += amount;

    char msg[100];
    sprintf(msg, "Loan Taken: %.2f", amount);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Loan granted successfully!\n");
}


void repayLoan(int userIndex) {
    double amount;
    printf("Enter repayment amount: ");
    scanf("%lf", &amount);

    if (amount <= 0 || amount > accounts[userIndex].balance) {
        printf("Invalid amount or insufficient funds!\n");
        return;
    }

    if (amount > accounts[userIndex].loanBalance)
        amount = accounts[userIndex].loanBalance;

    accounts[userIndex].balance -= amount;
    accounts[userIndex].loanBalance -= amount;

    char msg[100];
    sprintf(msg, "Loan Repaid: %.2f", amount);
    addTransaction(userIndex, msg);

    saveAccounts();
    printf("Loan repayment successful!\n");
}


void checkBalance(int userIndex) {
    printf("\nBalance: %.2f\n", accounts[userIndex].balance);
    printf("Loan Due: %.2f\n", accounts[userIndex].loanBalance);
}


void addTransaction(int userIndex, char details[]) {
    if (strlen(accounts[userIndex].history) + strlen(details) + 2 < 1000) {
        strcat(accounts[userIndex].history, details);
        strcat(accounts[userIndex].history, "\n");
    }
}


void viewHistory(int userIndex) {
    printf("\n--- Transaction History ---\n%s", accounts[userIndex].history);
}


void changePassword(int userIndex) {
    char oldp[25], newp[25];
    printf("Enter current password: ");
    scanf("%s", oldp);

    if (strcmp(oldp, accounts[userIndex].password) != 0) {
        printf("Incorrect password!\n");
        return;
    }

    printf("Enter new password: ");
    scanf("%s", newp);
    strcpy(accounts[userIndex].password, newp);
    saveAccounts();
    printf("Password changed successfully!\n");
}


int adminLogin() {
    char u[20], p[20];
    printf("Enter admin username: ");
    scanf("%s", u);
    printf("Enter admin password: ");
    scanf("%s", p);

    if (strcmp(u, "admin") == 0 && strcmp(p, "1234") == 0) {
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Invalid admin credentials!\n");
        return 0;
    }
}


void listAccounts() {
    printf("\n--- All Accounts ---\n");
    for (int i = 0; i < accountCount; i++)
        printf("Account: %d | Name: %s | Balance: %.2f | Loan: %.2f\n",
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].balance,
               accounts[i].loanBalance);
}


void deleteAccount() {
    int acc;
    printf("Enter account number to delete: ");
    scanf("%d", &acc);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == acc) {
            for (int j = i; j < accountCount - 1; j++)
                accounts[j] = accounts[j + 1];

            accountCount--;
            saveAccounts();
            printf("Account deleted successfully!\n");
            return;
        }
    }
    printf("Account not found!\n");
}


void adminMenu() {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. List Accounts\n");
        printf("2. Apply Interest\n");
        printf("3. Delete Account\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: listAccounts(); break;
            case 2: applyInterest(5.0); break;
            case 3: deleteAccount(); break;
            case 4: printf("Logging out of admin...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (ch != 4);
}