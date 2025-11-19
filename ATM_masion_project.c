#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_TRANSACTIONS 100

// Structure to store user account information
struct Account {
    int accountNumber;
    int pin;
    char name[50];
    float balance;
    int transactionCount;
    char transactions[MAX_TRANSACTIONS][100];
};

// Global variables
struct Account accounts[MAX_USERS];
int totalUsers = 0;
int currentUser = -1;

// Function declaration
void loginMenu();
int authenticateUser(int accNum, int pin);
void atmMenu();
void checkBalance();
void depositMoney();
void withdrawMoney();
void transferMoney();
void viewTransactionHistory();
void changePin();
void addTransaction(int userIndex, char *transaction);
void saveToFile();
void loadFromFile();
void createAccount();

int main() {
    int choice;
    
    printf("\n========================================\n");
    printf("   WELCOME TO ATM SIMULATION SYSTEM\n");
    printf("========================================\n");
    
    while (1) {
        
    printf("\n========================================\n");
    printf("           MAIN MENU\n");
    printf("========================================\n");
    printf("1. Login to Account\n");
    printf("2. Create New Account\n");
    printf("3. Exit\n");
    printf("========================================\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                loginMenu();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                printf("\nThank you for using our ATM!\n");
                printf("========================================\n");
                saveToFile();
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    
    return 0;
}


// Login menu and authentication
void loginMenu() {
    int accNum, pin;
    
    printf("\n========================================\n");
    printf("           LOGIN SYSTEM\n");
    printf("========================================\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    printf("Enter PIN: ");
    scanf("%d", &pin);
    
    int userIndex = authenticateUser(accNum, pin);
    
    if (userIndex != -1) {
        currentUser = userIndex;
        printf("\n[SUCCESS] Login Successful!\n");
        printf("Welcome, %s!\n", accounts[currentUser].name);
        atmMenu();
    } else {
        printf("\n[ERROR] Invalid Account Number or PIN!\n");
    }
}

// Authenticate user credentials
int authenticateUser(int accNum, int pin) {
    for (int i = 0; i < totalUsers; i++) {
        if (accounts[i].accountNumber == accNum && accounts[i].pin == pin) {
            return i;
        }
    }
    return -1;
}

// ATM operations menu
void atmMenu() {
    int choice;
    
    while (1) {
        printf("\n========================================\n");
        printf("           ATM MENU\n");
        printf("========================================\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. View Transaction History\n");
        printf("6. Change PIN\n");
        printf("7. Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                checkBalance();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                transferMoney();
                break;
            case 5:
                viewTransactionHistory();
                break;
            case 6:
                changePin();
                break;
            case 7:
                printf("\n[INFO] Logging out...\n");
                saveToFile();
                currentUser = -1;
                return;
            default:
                printf("\n[ERROR] Invalid choice!\n");
        }
    }
}

void checkBalance() {
    printf("\n========================================\n");
    printf("         BALANCE INQUIRY\n");
    printf("========================================\n");
    printf("Account Number: %d\n", accounts[currentUser].accountNumber);
    printf("Account Holder: %s\n", accounts[currentUser].name);
    printf("Current Balance: Rs. %.2f\n", accounts[currentUser].balance);
    printf("========================================\n");
}

void depositMoney() {
    float amount;
    
    printf("\n========================================\n");
    printf("         DEPOSIT MONEY\n");
    printf("========================================\n");
    printf("Current Balance: Rs. %.2f\n", accounts[currentUser].balance);
    printf("Enter amount to deposit: Rs. ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("\n Invalid amount!\n");
        return;
    }
    
    accounts[currentUser].balance += amount;
    
    char transaction[100];
    sprintf(transaction, "Deposited: Rs. %.2f", amount);
    addTransaction(currentUser, transaction);
    
    printf("\nRs. %.2f deposited successfully!\n", amount);
    printf("New Balance: Rs. %.2f\n", accounts[currentUser].balance);
    saveToFile();
}

// Withdraw money
void withdrawMoney() {
    float amount;
    
    printf("\n========================================\n");
    printf("         WITHDRAW MONEY\n");
    printf("========================================\n");
    printf("Current Balance: Rs. %.2f\n", accounts[currentUser].balance);
    printf("Enter amount to withdraw: Rs. ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("\n Invalid amount!\n");
        return;
    }
    
    if (amount > accounts[currentUser].balance) {
        printf("\n Insufficient balance!\n");
        return;
    }
    
    accounts[currentUser].balance -= amount;
    
    char transaction[100];
    sprintf(transaction, "Withdrawn: Rs. %.2f", amount);
    addTransaction(currentUser, transaction);
    
    printf("\n[SUCCESS] Rs. %.2f withdrawn successfully!\n", amount);
    printf("Remaining Balance: Rs. %.2f\n", accounts[currentUser].balance);
    saveToFile();
}

// Transfer money to another account
void transferMoney() {
    int targetAccount;
    float amount;
    int targetIndex = -1;
    
    printf("\n========================================\n");
    printf("         TRANSFER MONEY\n");
    printf("========================================\n");
    printf("Current Balance: Rs. %.2f\n", accounts[currentUser].balance);
    printf("Enter target account number: ");
    scanf("%d", &targetAccount);
    
    // Find  account
    for (int i = 0; i < totalUsers; i++) {
        if (accounts[i].accountNumber == targetAccount) {
            targetIndex = i;
            break;
        }
    }
    
    if (targetIndex == -1) {
        printf("\n[ERROR] Account not found!\n");
        return;
    }
    
    if (targetIndex == currentUser) {
        printf("\n[ERROR] Cannot transfer to same account!\n");
        return;
    }
    
    printf("Transfer to: %s\n", accounts[targetIndex].name);
    printf("Enter amount to transfer: Rs. ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("\n Invalid amount!\n");
        return;
    }
    
    if (amount > accounts[currentUser].balance) {
        printf("\n Insufficient balance!\n");
        return;
    }
    
    accounts[currentUser].balance -= amount;
    accounts[targetIndex].balance += amount;
    
    char transaction1[100], transaction2[100];
    sprintf(transaction1, "Transferred: Rs. %.2f to A/C %d", amount, targetAccount);
    sprintf(transaction2, "Received: Rs. %.2f from A/C %d", amount, accounts[currentUser].accountNumber);
    
    addTransaction(currentUser, transaction1);
    addTransaction(targetIndex, transaction2);
    
    printf("\n[SUCCESS] Rs. %.2f transferred successfully!\n", amount);
    printf("Remaining Balance: Rs. %.2f\n", accounts[currentUser].balance);
    saveToFile();
}

// View transaction history
void viewTransactionHistory() {
    printf("\n========================================\n");
    printf("       TRANSACTION HISTORY\n");
    printf("========================================\n");
    
    if (accounts[currentUser].transactionCount == 0) {
        printf("No transactions yet.\n");
    } else {
        for (int i = 0; i < accounts[currentUser].transactionCount; i++) {
            printf("%d. %s\n", i + 1, accounts[currentUser].transactions[i]);
        }
    }
    printf("========================================\n");
}

// Change PIN
void changePin() {
    int oldPin, newPin, confirmPin;
    
    printf("\n========================================\n");
    printf("           CHANGE PIN\n");
    printf("========================================\n");
    printf("Enter old PIN: ");
    scanf("%d", &oldPin);
    
    if (oldPin != accounts[currentUser].pin) {
        printf("\n Incorrect old PIN!\n");
        return;
    }
    
    printf("Enter new PIN: ");
    scanf("%d", &newPin);
    printf("Confirm new PIN: ");
    scanf("%d", &confirmPin);
    
    if (newPin != confirmPin) {
        printf("\nPINs do not match!\n");
        return;
    }
    
    accounts[currentUser].pin = newPin;
    
    char transaction[100];
    sprintf(transaction, "PIN changed successfully");
    addTransaction(currentUser, transaction);
    
    printf("\n PIN changed successfully!\n");
    saveToFile();
}

// Add transaction to history
void addTransaction(int user, char *transaction) {
    if (accounts[user].transactionCount < MAX_TRANSACTIONS) {
        strcpy(accounts[user].transactions[accounts[user].transactionCount], transaction);
        accounts[user].transactionCount++;
    }
}

// Create new account
void createAccount() {
    if (totalUsers >= MAX_USERS) {
        printf("\nMaximum users reached!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("       CREATE NEW ACCOUNT\n");
    printf("========================================\n");
    
    accounts[totalUsers].accountNumber = 1000 + totalUsers + 1;
    
    printf("Your Account Number: %d\n", accounts[totalUsers].accountNumber);
    printf("Enter your name: ");
    getchar(); // Clear buffer
    fgets(accounts[totalUsers].name, 50, stdin);
    accounts[totalUsers].name[strcspn(accounts[totalUsers].name, "\n")] = 0;
    
    printf("Set your PIN (4 digits): ");
    scanf("%d", &accounts[totalUsers].pin);
    
    printf("Enter initial deposit: Rs. ");
    scanf("%f", &accounts[totalUsers].balance);
    
    if (accounts[totalUsers].balance < 0) {
        printf("\n Invalid amount!\n");
        return;
    }
    
    accounts[totalUsers].transactionCount = 0;
    
    char transaction[100];
    sprintf(transaction, "Account created with Rs. %.2f", accounts[totalUsers].balance);
    addTransaction(totalUsers, transaction);
    
    totalUsers++;
    
    printf("\nAccount created successfully!\n");
    printf("========================================\n");
    saveToFile();
}

// Save data to file
void saveToFile() {
    FILE *file = fopen("atm_data.txt", "w");
    if (file == NULL) {
        printf("\nCould not save data to file!\n");
        return;
    }
    
    fprintf(file, "%d\n", totalUsers);
    
    for (int i = 0; i < totalUsers; i++) {
        fprintf(file, "%d %d %s\n", accounts[i].accountNumber, accounts[i].pin, accounts[i].name);
        fprintf(file, "%.2f %d\n", accounts[i].balance, accounts[i].transactionCount);
        
        for (int j = 0; j < accounts[i].transactionCount; j++) {
            fprintf(file, "%s\n", accounts[i].transactions[j]);
        }
    }
    
    fclose(file);
}