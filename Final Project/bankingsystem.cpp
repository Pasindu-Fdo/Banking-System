#include <iostream>
#include <cstdlib> // For Clear Screen
#include <fstream> // File Handling
#include <sstream> // File Handling
#include <string>
#include <cstring> // For strlen
#include <filesystem>
#include <random>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <conio.h>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;
#define RESET_COLOR "\033[32m"
#define RED_COLOR "\033[31m"

vector<vector<string>> adminInfo;
vector<vector<string>> userInfo;
vector<vector<string>> tUserInfo;
vector<vector<string>> fixedInfo;
vector<vector<string>> loanInfo;
vector<vector<string>> pawnInfo;
vector<vector<string>> userAccounts;
vector<vector<string>> Rates;

int adminIndex;

void interestadd();
void Login();
void readData(vector<vector<string>> &arrName, string filename);
void updateData(vector<vector<string>> &arrName, string folderPath);
int GetAdminIndex (string query);
string getPassword();
string createAccNum();
string createInvoice();
string gettime();
void accValidation();
void userMenu();
void accBal();
void accDetails();
void onlineTrans();
void transHistory();
void counterMenu();
void createAcc();
void userDetails();
void withdraw();
void deposit();
void fixdeposit();
void loanMenu();
void app_loan();
void pay_loan();
void his_loan();
void pawningMenu();
void pawning_item();
void gold_payment();
void pawn_his();
void adminMenu();
void create_admin();
void loan_rates();
void pawn_rates();
void loan_admin();
void fixed_admin();
void pawn_admin();
string generate_username();
void edit_rate(string query);

int main() {
    system("color A");
    string accNum;


    while(true) {
        interestadd();
        Login();

    }

    return 0;
}

void interestadd() {
    tm startDate = {};
    string filename = "FixedDeposit/fdInfo.csv";
    fixedInfo.clear();
    fixedInfo.shrink_to_fit();
    readData(fixedInfo, filename);

    for(int i =  1; i < fixedInfo.size(); i++) {
        int year = stoi(fixedInfo[i][0]); // Obtain year, month, date of last payment and assign into a variable
        int month = stoi(fixedInfo[i][1]);
        int day = stoi(fixedInfo[i][2]);
        string accNum = fixedInfo[i][4];

        // If the Fixed deposit time is up deposit the principal amount into the user account
        float remaining_interest = stof(fixedInfo[i][7]); // Convert remaining interest into a float
        if(remaining_interest = 0) {
            filename = "Details/" + accNum + ".csv";
            userInfo.clear();
            userInfo.shrink_to_fit();
            // Import Userdata into the userInfo array
            readData(userInfo, filename);

            float principal = stof(fixedInfo[i][5]);
            float accBal = stof(userInfo[0][8]);
            string time1 = gettime();
            float newBal = accBal + principal;
            userInfo[0][8] = to_string(newBal);
            vector<string> newData = {" " + time1 + " ","FD Completion    ", "+" + to_string(principal)};
            userInfo.push_back(newData);
            updateData(userInfo, filename);
        }

        // Calculate the duration of time after the last interest.
        startDate.tm_year = year - 1900; // year - 1900
        startDate.tm_mon = month - 1;    // month is 0-based
        startDate.tm_mday = day;
        time_t startDateInTimeT = mktime(&startDate);

        time_t currentDate = time(nullptr);
        double durationInSeconds = difftime(currentDate, startDateInTimeT); // Duration in seconds
        double durationInDays = durationInSeconds / (60 * 60 * 24); // Duration in days

        if(durationInDays == 30) { // When the duration of 1 month is conplete add the interest.
            float rem_value = stof(fixedInfo[i][7]); // Convert the string values into float values for calculations.
            float red_value = stof(fixedInfo[i][8]);
            float new_value = rem_value - red_value;
            fixedInfo[i][7] = to_string(new_value); // Update the new value in the fixed Information details after the calculation.

            // Update the current date as the last interest payment date.
            time_t thetime = time(nullptr);
            tm currentDate = *localtime(&thetime);
            fixedInfo[i][0] = to_string(currentDate.tm_year + 1900);
            fixedInfo[i][1] = to_string(currentDate.tm_mon + 1);
            fixedInfo[i][2] = to_string(currentDate.tm_mday);

            filename = "Details/" + accNum + ".csv";
            userInfo.clear();
            userInfo.shrink_to_fit();
            // Import Userdata into the userInfo array.
            readData(userInfo, filename);

            float accBal = stof(userInfo[0][8]); // Convert the string values into float values.
            string time1 = gettime();
            float newBal = accBal + red_value;
            userInfo[0][8] = to_string(newBal); // Update the account balance in the user account.
            // Record the transaction in the user account.
            vector<string> newData = {" " + time1 + " ","FD Interest      ", "+" + to_string(red_value)};
            userInfo.push_back(newData);
            updateData(userInfo, filename);

        } else if(durationInDays > 30) {
            while(durationInDays > 30) { // Looping until the duration is less than 30 days.
                durationInDays -= 30; // Reduce 1 month after a month's interest is added.
                float rem_value = stof(fixedInfo[i][7]);
                float red_value = stof(fixedInfo[i][8]);
                float new_value = rem_value - red_value;
                fixedInfo[i][7] = to_string(new_value); // Update the changes in the fixed Information file.
                int month1 = stoi(fixedInfo[i][1]);
                // If it is the last month instead of increasing month by 1, year should be increased by 1.
                // Also month should be set to 1.
                if(month1 == 12) {
                    int year1 = stoi(fixedInfo[i][0]);
                    fixedInfo[i][0] = to_string(year1 + 1);
                    fixedInfo[i][1] = "1";
                } else {
                    fixedInfo[i][1] = to_string(month1 + 1);
                }

                filename = "Details/" + accNum + ".csv";
                userInfo.clear();
                userInfo.shrink_to_fit();
                // Import Userdata into the userInfo array.
                readData(userInfo, filename);

                float accBal = stof(userInfo[0][8]); // Convert string values into float values.
                string time1 = gettime();
                float newBal = accBal + red_value;
                userInfo[0][8] = to_string(newBal); // Update balance in user accounts.
                // Record the transaction in the user account.
                vector<string> newData = {" " + time1 + " ","FD Interest      ", "+" + to_string(red_value)};
                userInfo.push_back(newData);
                updateData(userInfo, filename);

            }
        }
    }
    filename = "FixedDeposit/fdInfo.csv";
    updateData(fixedInfo, filename);
}

string getPassword() {
    string password;
    char ch;

    cout << "\t\t\t\t" << "Enter password : ";
    // Mask the password with '*'
    while ((ch = _getch()) != 13) { // 13 is the ASCII code for Enter key
        if (ch == 8) { // 8 is the ASCII code for Backspace key
            if (!password.empty()) {
                cout << "\b \b"; // Move the cursor back, erase the character, and move the cursor back again
                password.pop_back();
            }
        } else {
            cout << '*';
            password.push_back(ch);
        }
    }

    cout << endl; // Move to the next line after password input
    return password;
}

int acc_check(string acc_no){
    // Read the Current Account Numbers and Check whether the Account Exists.
    const string filename = "Details/current_users.csv";
    readData(userAccounts, filename);

    for(int i=0; i<userAccounts.size();i++){
        if (userAccounts[i][0]==acc_no){
            return 1;
        }
    }
    return -1;
}

int invoice_check(string acc_no){
    // Read Invoice files and check whether the invoice number Exists.
    vector<vector<string>> userAccounts;
    const string filename = "GoldPawning/Invoices.csv";
    readData(userAccounts, filename);

    for(int i=0; i<userAccounts.size();i++){
        if (userAccounts[i][0] == acc_no){
            return 1;
        }
    }
    return -1;
}

int loan_check(string acc_no){
    // Read the Loan Files and Check whether the Account has a loan.
    const string filename = "Loans/loandetails.csv";
    loanInfo.clear();
    loanInfo.shrink_to_fit();
    readData(loanInfo, filename);

    for(int i=1; i<loanInfo.size();i++){
        if ( loanInfo[i][1] == acc_no) {
            return 1;
        }
    }
    return -1;
}

int GetAdminIndex (string query) {
    // Read the array to find the index & return the index.
    for (int i = 0; i < adminInfo.size(); i++) {
        if (query == adminInfo[i][0]) {
            return i;
        }
    }
    return -1;
}

int GetIndex (string query, vector<vector<string>> &arrName) {
    // Read the array to find the index & return the index.
    for (int i = 0; i < arrName.size(); i++) {
        if (query == arrName[i][1]) {
            return i;
        }
    }
    return -1;
}

string gettime() {
    // Get the current time point
    auto currentTime = chrono::system_clock::now();

    // Convert the current time point to a time_t object
    time_t currentTime_t = chrono::system_clock::to_time_t(currentTime);

    // Convert the time_t object to a local time struct
    struct tm* localTime = localtime(&currentTime_t);

    // Format and display the current time
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    return buffer;
}

void readData(vector<vector<string>> &arrName, string filename) {
    // Open the CSV file
    ifstream file(filename);

    if (!file.is_open()) {
        system("cls");
        cerr << "Wrong Account Number!" << endl << endl;
        Login();
    }

    // Read data from CSV file into a dynamic 2D array

    // Read each line of the file
    string line;
    while (getline(file, line)) {
        vector<string> row;
        istringstream iss(line);
        string value;

        // Read each value separated by commas and store in the array
        while (getline(iss, value, ',')) {
            row.push_back(value);

        }

        // Move to the next row
        arrName.push_back(row);
    }

    // Close the file
    file.close();

}

void updateData(vector<vector<string>> &arrName, string folderPath) {

    ofstream file(folderPath);

    // Write 2D array data to the CSV file
    for (size_t row = 0; row < arrName.size(); ++row) {
        for (size_t col = 0; col < arrName[row].size(); ++col) {
            file << arrName[row][col];

            // Add a comma if it's not the last element in the row
            if (col != arrName[row].size() - 1) {
                file << ",";
            }
        }
        // Start a new line for the next row
        file << "\n";
    }

    // Close the file
    file.close();
}

string createAccNum() {
    random_device rd; // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(0, 9); // Specify the range of the number to be generated

    while(true) {

        // Generate a 10-digit random number and store it in a variable
        string randomNumber = "";
        for (int i = 0; i < 4; ++i) {
            randomNumber = randomNumber +to_string(dis(gen));
        }

        string accNum = "2320934" + randomNumber;
        const string filePath = "Details/" + accNum + ".csv";

        // Try to open the file
        ifstream file (filePath);

        // Check if the file exists
        if (!file) {
            file.close(); // Close the file after checking
            return accNum;
        }
    }
}

string createInvoice() {

    while(true) {
        string accNum;
        string folderPath = "GoldPawning/Invoices.csv";
        userAccounts.clear();
        userAccounts.shrink_to_fit();
        readData(userAccounts, folderPath);
        ifstream file(folderPath);
        if (file.peek() == ifstream::traits_type::eof()) { // Check whether the file is empty.
            accNum = "000001"; // If file is empty accNum is 000001.
        }
        file.close();
        int size = (userAccounts.size()) - 1;
        string last = userAccounts[size][0];
        int value = stoi(last);
        int num1 = value + 1; // Getting the last invoice number and add 1 to the number.

        accNum = "00000" + to_string(num1);
        // checks for no. of digits and adjust the no. of digits of the invoice number to be constant.
        if (abs(num1) >= 10 && abs(num1) <= 99) {
            accNum = "0000" + to_string(num1);
        } else if(abs(num1) >= 100 && abs(num1) <= 999) {
            accNum = "000" + to_string(num1);
        }

        return accNum;
    }
}

void accValidation() {
    string accNum, nicNo;

    while(true) {
        while(true) {
            cout << "\t\t\t\t" << "Enter Account Number: ";
            cin >> accNum;
            // Gets the account no. and check whether in exits using the below function
            int validity = acc_check(accNum);

            if(validity == 1) { // Executes only if the account number exists.
                break;
            } else {
                system("cls");
                cout << "\t\t\t\t" << "Account Not Found! " << endl << endl;
            }
        }
        string filename = "Details/" + accNum + ".csv";

        userInfo.clear();
        userInfo.shrink_to_fit();
        // Import Userdata into the userInfo array
        readData(userInfo, filename);

        cout << "\t\t\t\t" << "Enter NIC Number: ";
        cin >> nicNo;
        // Gets the NIC no. from the user and check whether it matches with the NIC in user Info.
        if(nicNo != userInfo[0][2]) {
            cout << "\t\t\t\t" << "Invalid Login. Please try Again..." << endl << endl;
        } else {
            break;
        }
    }
}

void Login() {
    string accNum, dept;
    string passWord;

    system("cls");

    while(true) {
        cout<<endl<<endl<<endl;
        cout << "\t\t\t\t" << " ____          _   _ _  _______ _   _  _____    _______     _______ _______ ______ __  __ " << endl;
        cout << "\t\t\t\t" << "|  _ \\   /\\   | \\ | | |/ /_   _| \\ | |/ ____|  / ____\\ \\   / / ____|__   __|  ____|  \\/  |" << endl;
        cout << "\t\t\t\t" << "| |_) | /  \\  |  \\| | ' /  | | |  \\| | |  __  | (___  \\ \\_/ / (___    | |  | |__  | \\  / |" << endl;
        cout << "\t\t\t\t" << "|  _ < / /\\ \\ | . ` |  <   | | | . ` | | |_ |  \\___ \\  \\   / \\___ \\   | |  |  __| | |\\/| |" << endl;
        cout << "\t\t\t\t" << "| |_) / ____ \\| |\\  | . \\ _| |_| |\\  | |__| |  ____) |  | |  ____) |  | |  | |____| |  | |" << endl;
        cout << "\t\t\t\t" << "|____/_/    \\_\\_| \\_|_|\\_\\_____|_| \\_|\\_____| |_____/   |_| |_____/   |_|  |______|_|  |_|" << endl<<endl;
        cout << "\t\t\t\t" << " ---------------------------Welcome To Simple Banking System------------------------------" << endl;

        cout << endl << endl << "\t\t\t\t" << "Enter Account Number : ";
        cin >> accNum;

        // Seperating the admin panel and user login
        if(accNum == "0") {
            exit(0);
        } else if(accNum.length() == 3) {
            // Admin Login
            string filename = "login/logindetails.csv";
            string arrName = "adminInfo";

            adminInfo.clear();
            adminInfo.shrink_to_fit();  // Reduces the vector's capacity to fit its size
            // Import login details into the 2D array adminInfo
            readData(adminInfo,filename);
            adminIndex = GetAdminIndex(accNum);
            if (adminIndex == -1) {
                system("cls");
                cout << RED_COLOR << "Access denied!" << RESET_COLOR << "\n\n";
                continue;
            }
            passWord = getPassword();

            // Password Validation
            if (passWord != adminInfo[adminIndex][1]) {
                system("cls");
                cout << RED_COLOR << "Access denied!" << RESET_COLOR << "\n\n";
            } else {
                dept = adminInfo[adminIndex][2];
                // Assigning the respective menu based on the accNum entered
                if(dept == "counter") {
                    counterMenu();
                } else if(dept == "loan") {
                    loanMenu();
                } else {
                    pawningMenu();
                }
                break;
            }

        } else if(accNum.length() == 5) {
            // Admin Login
            string filename = "login/logindetails.csv";
            string arrName = "adminInfo";

            adminInfo.clear();
            adminInfo.shrink_to_fit();  // Reduces the vector's capacity to fit its size
            // Import login details into the 2D array adminInfo
            readData(adminInfo,filename);
            adminIndex = GetAdminIndex(accNum);
            if (adminIndex == -1) {
                system("cls");
                cout << RED_COLOR << "Access denied!" << RESET_COLOR << "\n\n";
                continue;
            }

            passWord = getPassword();

            // Password Validation
            if (passWord != adminInfo[adminIndex][1]) {
                system("cls");
                cout << RED_COLOR << "Access denied!" << RESET_COLOR << "\n\n";
            } else {
                adminMenu();
                break;
            }
        } else {
            int validity = acc_check(accNum);
            if (validity != 1) {
                system("cls");
                cout << "\t\t\t\t" << RED_COLOR << "Account Not Found! " << RESET_COLOR << endl << endl;
                cout << "\t\t\t\t" << "Press any key to continue" << endl;
                _getch();
                Login();
            }

            // User Login
            string filename = "Details/" + accNum + ".csv";
            userInfo.clear();
            userInfo.shrink_to_fit();  // Reduces the vector's capacity to fit its size
            // Import Userdata into the userInfo array
            readData(userInfo, filename);

            passWord = getPassword();

            if (passWord != userInfo[0][7]) {
                system("cls");
                cout << RED_COLOR << "Access denied!" << RESET_COLOR << "\n\n";
            } else {
                userMenu();
                break;
            }

        }
    }
}

void counterMenu() {
    int choice;

    system("cls");

    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl;

    cout << endl << endl << endl << endl << endl;

    cout << "\t\t\t\t" <<  "[1]Create Account" << endl;
    cout << "\t\t\t\t" <<  "[2]Account Details" << endl;
    cout << "\t\t\t\t" <<  "[3]Withdraw Money" << endl;
    cout << "\t\t\t\t" <<  "[4]Deposit Money" << endl;
    cout << "\t\t\t\t" <<  "[5]Fixed Deposit" << endl;
    cout << "\t\t\t\t" <<  "[6]Log Out" << endl;
    cout << endl << endl << endl;
    cout << "\t\t\t\t" <<  "Input : ";
    cin >> choice;

    if(choice == 2 || choice == 3 || choice == 4) {
        accValidation();
    }

    switch (choice) {
        case 1:
            createAcc();
            break;
        case 2:
            userDetails();
            break;
        case 3:
            withdraw();
            break;
        case 4:
            deposit();
            break;
        case 5:
            fixdeposit();
            break;
        case 6:
            Login();
            break;
        case 0:
            exit(0);
            break;
    }
}

void loanMenu() {
    int choice;

    system("cls");

    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl;

    cout << endl << endl << endl << endl << endl;

    cout << "\t\t\t\t\t\t\t\t" << "1. Apply For a Loan" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "2. Loan Payment" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "3. Loan History" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "4. Log Out" << endl;
    cout << endl << endl << endl;
    cout << "\t\t\t\t\t\t\t\t" <<  "Input : ";
    cin >> choice;

    switch (choice){
        case 1:
            app_loan();
            break;

        case 2:
            pay_loan();
            break;

        case 3:
            his_loan();
            break;

        case 4:
            Login();
            break;

        case 0:
            exit(0);
            break;
    }
}

void pawningMenu() {
    int choice;

    system("cls");

    cout << "\t\t\t\t\t"<< " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t"<< "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t"<< "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t"<< "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t"<< "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t"<< "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl;
    cout << endl << endl << endl << endl << endl;


    cout << "\t\t\t\t\t\t\t\t" << "[1]Pawn Gold Items" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "[2]Payments" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "[3]Pawning Information" << endl;
    cout << "\t\t\t\t\t\t\t\t" << "[4]Log Out" << endl;
    cout << endl << endl << endl;
    cout << "\t\t\t\t\t\t\t\t" << "Input: ";
    cin >> choice;

    switch (choice) {
        case 1:
                pawning_item();
            break;
        case 2:
                gold_payment();
            break;
        case 3:
                pawn_his();
            break;
        case 4:
                Login();
            break;
        case 0:
                exit(0);
            break;
    }
}

void userMenu() {
    int choice;
    string filename = userInfo[0][1];
    string folderPath = "Details/" + filename + ".csv";

    system("cls");

    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl;
    cout << "\t\t\t\t\t\t" << "------------------- Welcome " << userInfo[0][0] <<"--------------------" << endl;
    cout << endl << endl << endl << endl << endl;

    cout << "\t\t\t\t\t\t" << "[1]Account Balance" << endl;
    cout << "\t\t\t\t\t\t" << "[2]Account Details" << endl;
    cout << "\t\t\t\t\t\t" << "[3]Transaction History" << endl;
    cout << "\t\t\t\t\t\t" << "[4]Online Transactions" << endl;
    cout << "\t\t\t\t\t\t" << "[5]Log Out" << endl;
    cout << endl << endl << endl;
    cout << "\t\t\t\t\t\t" << "Input: ";
    cin >> choice;

    switch (choice) {
        case 1:
            accBal();
            break;
        case 2:
            accDetails();
            break;
        case 3:
            transHistory();
            break;
        case 4:
            onlineTrans();
            break;
        case 5:
            updateData(userInfo, folderPath);
            userInfo.clear();
            userInfo.shrink_to_fit();
            Login();
            break;
        case 0:
            exit(0);
            break;
    }
}

void accBal() {
    system("cls");

    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << " ---------------------- Account Balance -----------------------" << endl;
    cout << endl << endl << endl << endl << endl;
    // Data is already loaded in the userInfo array
    cout << "\t\t\t\t\t\t" << "Account Name     : " << userInfo[0][0] << endl;
    cout << "\t\t\t\t\t\t" << "Available Balance: " << fixed << setprecision(2) << stof(userInfo[0][8]) << endl; //Round off the balance to 2-decimal places

    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    userMenu();
}

void accDetails() {
    system("cls");

    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << " ---------------------- Account Details -----------------------" << endl;
    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t\t\t" << "Account Name   : " << userInfo[0][0] << endl;
    cout << "\t\t\t\t\t\t" << "Account Number : " << userInfo[0][1] << endl;
    cout << "\t\t\t\t\t\t" << "NIC Number     : " << userInfo[0][2] << endl;
    cout << "\t\t\t\t\t\t" << "Contact Number : " << userInfo[0][3] << endl;
    cout << "\t\t\t\t\t\t" << "Home Address   : " << userInfo[0][4] << "," << endl;
    cout << "\t\t\t\t\t\t" << "                 " << userInfo[0][5] << "," << endl;
    cout << "\t\t\t\t\t\t" << "                 " << userInfo[0][6] << "." << endl;

    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    userMenu();
}

void onlineTrans() {
    system("cls");

    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << " --------------------- Online Transaction ----------------------" << endl;
    cout << endl << endl << endl << endl << endl;
    string accNum;
    char answer;
    int choice, choice1;
    float amount;
    // Converting the string value into an integer
    float accBal = stof(userInfo[0][8]);
    cout << "\t\t\t\t\t\t" << "[1]In-Bank Account Transfers" << endl;
    cout << "\t\t\t\t\t\t" << "[2]Bank Service Payments" << endl;
    cout << endl;
    cout << "\t\t\t\t\t\t" << "Input: ";
    cin >> choice;

    // In-Bank Account transfers option
    if(choice == 1) {
        while(true){
            system("cls");
            cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
            cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
            cout << "\t\t\t\t\t\t" << " --------------------- Inbank Transaction ----------------------" << endl;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t\t\t" << "Enter the Account Number to Deposit: ";
            cin >> accNum;
            string filename = "Details/" + accNum + ".csv";
            //Search the account entered and load the details into a seperate array
            readData(tUserInfo, filename);

            system("cls");
            cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
            cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
            cout << "\t\t\t\t\t\t" << " --------------------- Inbank Transaction ----------------------" << endl;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t\t\t" << "Account Name   : " << tUserInfo[0][0] << endl;
            cout << "\t\t\t\t\t\t" << "Account Number : " << tUserInfo[0][1] << endl;
            cout << endl;
            cout << "\t\t\t\t\t\t" << "Please confirm the account!( Y / N )" << endl;
            cout << "\t\t\t\t\t\t" << "Input: ";
            cin >> answer;

            if(answer == 'Y' || answer == 'y'){ // Looping the process until user confirms the transaction
                break;
            } else {
                continue;
            }
        }
        // Convert the bank balance of the tranferring account into an integer
        float trBal = stof(tUserInfo[0][8]);

        while(true) {
            float newBal = 0;
            float tnewBal = 0;
            system("cls");
            cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
            cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
            cout << "\t\t\t\t\t\t" << " --------------------- Inbank Transaction ----------------------" << endl;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t\t\t" << "Enter the Amount to Transfer: ";
            cin >> amount;

            string time = gettime();

            // Validating the transfer amount and completing the transaction
            if( amount < accBal && amount > 0){
                // Updating the balances of the user account
                newBal = accBal - amount;
                userInfo[0][8] = to_string(newBal);
                vector<string> newData = {" " + time + " ","Transferred      ", "-" + to_string(amount)};
                userInfo.push_back(newData);
                // Updating the balances of the account transferred to
                tnewBal = trBal + amount;
                tUserInfo[0][8] = to_string(tnewBal);
                vector<string> newData1 = {" " + time + " ","Transferred      ", "+" + to_string(amount)};
                tUserInfo.push_back(newData1);

                cout << endl;
                cout << "\t\t\t\t\t\t" << "The amount is Successfully Transferred!\n" << endl;

                break;
            } else {
                continue;
            }
        }

        // Uploading the changed data into the csv files
        const string Ufilename = userInfo[0][1];
        const string UfolderPath = "Details/" + Ufilename + ".csv";
        updateData(userInfo, UfolderPath);
        const string Tfilename = tUserInfo[0][1];
        const string TfolderPath = "Details/" + Tfilename + ".csv";
        updateData(tUserInfo, TfolderPath);
        tUserInfo.clear();
        tUserInfo.shrink_to_fit();  // Reduces the vector's capacity to fit its size

        cout << endl << endl;
        cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
        _getch();
        userMenu();

    } else if(choice == 2) {
        system("cls");
        cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
        cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
        cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
        cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
        cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
        cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
        cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
        cout << endl << endl << endl << endl << endl;
        cout << "\t\t\t\t\t\t" << "[1]Loan Payment" << endl;
        cout << "\t\t\t\t\t\t" << "[2]Gold Loan Payment" << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t" << "Input: ";
        cin >> choice1;

        if(choice1 == 1) {
            float aop, newBal, accBal;
            int validity = loan_check(userInfo[0][1]);
            if(validity == 1){
                system("cls");
                while (true) {
                    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
                    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
                    cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
                    cout << endl << endl << endl << endl << endl;
                    string filename = "Loans/loandetails.csv";
                    loanInfo.clear();
                    loanInfo.shrink_to_fit();
                    readData(loanInfo, filename); // Clearing the arrays and loading data into it.
                    string acc_no = userInfo[0][1];
                    int index_no = GetIndex(acc_no ,loanInfo); // Finding the respective row containing the username entered.
                    accBal = stof(userInfo[0][8]);
                    cout << "\t\t\t\t\t\t" << "Full Name : " << userInfo[0][0] << endl;
                    cout << "\t\t\t\t\t\t" << "Account Number : " << userInfo[0][1] << endl;
                    cout << "\t\t\t\t\t\t" << "Loan Amount : " << fixed << setprecision(2) << stof(loanInfo[index_no][2]) << endl;
                    cout << "\t\t\t\t\t\t" << "Duration : " << loanInfo[index_no][5] << endl;
                    cout << "\t\t\t\t\t\t" << "Monthly Installment : " << fixed << setprecision(2) << stof(loanInfo[index_no][4]) << endl;
                    cout << "\t\t\t\t\t\t" << "Remaining Amount : " << fixed << setprecision(2) << stof(loanInfo[index_no][7]) << endl << endl;
                    cout << "\t\t\t\t\t\t" << "Account Balance : " << fixed << setprecision(2) << stof(userInfo[0][8]) << endl;
                    cout << endl;
                    cout << "\t\t\t\t\t\t" << "Enter the Amount Of Payment :";
                    cin >> aop;

                    if(aop <= stof(loanInfo[index_no][7]) && accBal >= aop) {
                        float remainamount = stof(loanInfo[index_no][7]); // Converting the string into float values and performing calculations
                        float newamount = remainamount - aop;
                        loanInfo[index_no][7] = to_string(newamount);
                        float paid = stof(loanInfo[index_no][6]);
                        float newpaid = paid + aop;
                        loanInfo[index_no][6] = to_string(newpaid); // Updating the files with new values.

                        string time1 = gettime();

                        vector<string> newData = {" " + time1 + " ","Online Loan Payment", "-" + to_string(aop)};
                        userInfo.push_back(newData); // Recording the transaction in user account.
                        newBal = accBal - aop;  // Calculate the new balance.
                        userInfo[0][8] = to_string(newBal);

                        string filename = userInfo[0][1];
                        string folderPath = "Details/" + filename + ".csv";
                        updateData(userInfo, folderPath);

                        folderPath = "Loans/loandetails.csv";
                        updateData(loanInfo, folderPath);
                        loanInfo.clear();
                        loanInfo.shrink_to_fit();

                        system("cls");
                        cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
                        cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                        cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
                        cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                        cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                        cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
                        cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
                        cout << endl << endl << endl << endl << endl;
                        cout << "\t\t\t\t\t\t" << "Payment Successfully Completed!" << endl;
                        cout << endl << endl;
                        cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
                        _getch();
                        userMenu();
                    } else {
                        cout << "\t\t\t\t\t\t" << RED_COLOR << "Invalid Amount!!!" << RESET_COLOR << endl << endl;
                    }
                }

            } else {
                system("cls");
                cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
                cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
                cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
                cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
                cout << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t\t\t" << RED_COLOR << "You don't Have any Loans!" << RESET_COLOR << endl;
                cout << endl << endl;
                cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
                _getch();
                onlineTrans();
            }
        } else if(choice1 == 2) {
            system("cls");
            cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
            cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
            cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
            cout << endl << endl << endl << endl << endl;

            while(true) {
                string invoice_no;
                float payment_amount, payable_amount, newBal, accBal;
                int choice;
                char choice1,choice2;

                cout << "\t\t\t\t\t\t" << "Enter Invoice No : ";
                cin >> invoice_no;

                string folderPath = "GoldPawning/" + invoice_no + ".csv";
                readData(pawnInfo, folderPath);

                //check invoice number
                int validity = invoice_check(invoice_no);

                while(true){
                    if(validity == 1) {

                        cout << "\t\t\t\t\t\t" << "Full Name : " << pawnInfo[0][5] << endl;
                        cout << "\t\t\t\t\t\t" << "NIC Number : " << pawnInfo[0][6] << endl;
                        cout << "\t\t\t\t\t\t" << "Invoice Number : " << pawnInfo[0][4] << endl;
                        cout << endl;
                        cout << "\t\t\t\t\t\t" << "Confirm Details [Y/N] : ";
                        cin >> choice1;

                        if (choice1 == 'Y'|| choice1 == 'y'){
                            system("cls");
                            cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
                            cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                            cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
                            cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                            cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                            cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
                            cout << "\t\t\t\t\t\t" << " ---------------------- Online Payments -----------------------" << endl;
                            cout << endl << endl << endl << endl << endl;
                            string nowtime = gettime();
                            tm startDate = {};

                            int year = stoi(pawnInfo[0][0]);
                            int month = stoi(pawnInfo[0][1]);
                            int day = stoi(pawnInfo[0][2]);

                            startDate.tm_year = year - 1900; // year - 1900
                            startDate.tm_mon = month - 1;    // month is 0-based
                            startDate.tm_mday = day;
                            time_t startDateInTimeT = mktime(&startDate);
                            time_t currentDate = time(nullptr);

                            double durationInSeconds = difftime(currentDate, startDateInTimeT); // Duration in seconds
                            double durationInDays = durationInSeconds / (60 * 60 * 24); // Duration in days

                            float loan_amount = stof(pawnInfo[0][10]);
                            float amountpayable = loan_amount * 0.22 * durationInDays / 365;
                            float newamount = loan_amount + amountpayable - payment_amount;

                            cout << "\t\t\t\t\t\t" << "Number of Pound you pawed : " << fixed << setprecision(2) << stof(pawnInfo[0][11]) << endl;//number of pond
                            cout << "\t\t\t\t\t\t" << "Amount got from Pawning : "<< fixed << setprecision(2) << stof(pawnInfo[0][9]) << endl;//value of gold
                            cout << "\t\t\t\t\t\t" << "Your Total Payable Amount is : " << fixed << setprecision(2) << newamount << endl;//total payable amount (gold value + current interest amount)
                            cout << "\t\t\t\t\t\t" << "Your Current Interest Amount is : " << fixed << setprecision(2) << amountpayable << endl; //auto generated day and calculate interest ((from last day of pay day count) * 22/365)
                            cout << endl;
                            cout << "\t\t\t\t\t\t" << "Account Balance : " << fixed << setprecision(2) << stof(userInfo[0][8]) << endl;
                            cout << endl;
                            cout << "\t\t\t\t\t\t" << "Enter Payment Amount : ";
                            cin >> payment_amount;
                            cout << endl << endl;
                            cout << "\t\t\t\t\t\t" << "Confirm Payment [Y/N]: ";
                            cin >> choice2;

                            if (choice2 == 'Y'|| choice2 == 'y'){

                                float newamount = loan_amount + amountpayable - payment_amount;
                                pawnInfo[0][10] = to_string(newamount);
                                time_t thetime = time(nullptr);
                                tm currentDate1 = *localtime(&thetime);
                                fixedInfo[0][0] = to_string(currentDate1.tm_year + 1900);
                                fixedInfo[0][1] = to_string(currentDate1.tm_mon + 1);
                                fixedInfo[0][2] = to_string(currentDate1.tm_mday);

                                vector<string> newData = {" " + nowtime + " ", "Online Payment", to_string(payment_amount)};
                                pawnInfo.push_back(newData);
                                userInfo.push_back(newData);
                                accBal = stof(userInfo[0][8]);
                                newBal = accBal - payment_amount; // Calculate the new balance.
                                userInfo[0][8] = to_string(newBal);
                                folderPath = "GoldPawning/" + invoice_no + ".csv";
                                updateData(pawnInfo, folderPath);
                                pawnInfo.clear();
                                pawnInfo.shrink_to_fit();
                                string filename = userInfo[0][1];
                                string folderPath = "Details/" + filename + ".csv";
                                updateData(userInfo, folderPath);

                                cout << endl << endl;
                                cout << "\t\t\t\t\t\t" << "Your payment Successfully Completed ....." << endl;
                                cout << "\t\t\t\t\t\t" << "Thank You!" << endl;

                                cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
                                _getch();
                                userMenu();
                                break;

                            }else if (choice2 == 'N'||choice2 == 'n'){
                                onlineTrans();
                                break;
                            }else{
                                cout << "\t\t\t\t\t\t" << RED_COLOR << "Invalid Selection ! Please Input Valid Selection...." << RESET_COLOR << endl << endl;
                            }

                        }
                    } else {

                        cout << "\t\t\t\t\t\t" << RED_COLOR << "Account Number Not Found !" << RESET_COLOR << endl;
                        break;
                    }
                }

            }
        } else {
            cout << "\t\t\t\t\t\t" << RED_COLOR << "Invalid Input" << RESET_COLOR << endl;
            cout << endl << endl;
            cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
            _getch();
            onlineTrans();
        }
    }


}

void transHistory() {
    system("cls");
    cout << "\t\t\t\t\t\t" << " _    _  _____ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |  | |/ ____|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |  | | (___ | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl ;
    cout << "\t\t\t\t\t\t" << "| |  | |\\___ \\|  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |__| |____) | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << " \\____/|_____/|______|_|  \\_\\    |_|  |_|______|_| \\_|\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << " --------------------- Transaction History ----------------------" << endl;
    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t\t\t" << "Your Account Transaction History Is As Follows; \n" << endl;

    for (int row = 1; row < userInfo.size(); row++) { // Using a for-loop to display all the transactions.
        cout << "\t\t\t\t\t\t";

        if(userInfo[row][1] != "Loan Payment     " ) { // Avoiding the display of loan transactions
            cout << userInfo[row][0] << " | ";
            cout << userInfo[row][1] << " | LKR. ";
            cout << fixed << setprecision(2) << stof(userInfo[row][2]); // Rounds off decimals to 2 digits
        }
        cout << "\n";
    }

    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    userMenu();
}

void createAcc() {
    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t" <<  "-------------------------------- Create Account ----------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;
    string data[2][11];
    float amount;
    string accNum = createAccNum(); // Calling the function to create and return the acc no. & assign to the variable
    data[0][1] = accNum;


    cout << "\t\t\t\t" << "Name : ";
    getline(cin >> ws, data[0][0]);

    bool validId = false;
    do {
        cout << "\t\t\t\t" << "ID No. : ";
        if (cin >> data[0][2]) {
            // Check if input is a positive integer
            if (data[0][2].size() == 12 && all_of(data[0][2].begin(), data[0][2].end(), ::isdigit)) {
                validId = true;
            } else {
                cout << "\t\t\t\t" << RED_COLOR << "Invalid ID No. Please enter a valid integer." << RESET_COLOR << endl;
                cin.clear(); // Clear input buffer to restore cin to a usable state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            }
        } else {
            cout << "\t\t\t\t" << RED_COLOR << "Invalid ID No. Please enter a valid integer." << RESET_COLOR << endl;
            cin.clear(); // Clear input buffer to restore cin to a usable state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    } while (!validId);

    bool validMobile = false;
    do {
        cout << "\t\t\t\t" << "Mobile No. : ";
        if (cin >> data[0][3]) {
            // Check if input is a 10-digit positive integer
            if (data[0][3].size() == 10 && all_of(data[0][3].begin(), data[0][3].end(), ::isdigit)) {
                validMobile = true;
            } else {
                cout << "\t\t\t\t" << RED_COLOR << "Invalid Mobile No. Please enter a 10-digit positive integer." << RESET_COLOR << endl;
                cin.clear(); // Clear input buffer to restore cin to a usable state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            }
        } else {
            cout << "\t\t\t\t" << RED_COLOR << "Invalid Mobile No. Please enter a 10-digit positive integer." << RESET_COLOR << endl;
            cin.clear(); // Clear input buffer to restore cin to a usable state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    } while (!validMobile);

    cout << "\t\t\t\t" << "Address No. : ";
    getline (cin >> ws, data[0][4]);

    cout << "\t\t\t\t" << "Street Name : ";
    getline (cin >> ws, data[0][5]);

    cout << "\t\t\t\t" << "Town : ";
    getline (cin >> ws, data[0][6]);

    bool validPin = false;
    do {
        cout << "\t\t\t\t" << "Pin : ";
        if (cin >> data[0][7]) {
            // Check if input is a positive integer
            if (stoi(data[0][7]) >= 0 && data[0][7].size() == 4) {
                validPin = true;
            } else {
                cout << "\t\t\t\t" << RED_COLOR << "Invalid Pin. Please enter a valid integer." << RESET_COLOR << endl;
                cin.clear(); // Clear input buffer to restore cin to a usable state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            }
        } else {
            cout << "\t\t\t\t" << RED_COLOR << "Invalid Pin. Please enter a valid integer." << RESET_COLOR << endl;
            cin.clear(); // Clear input buffer to restore cin to a usable state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    } while (!validPin);

    while(true) {
        cout << "\t\t\t\t" << "To Complete registration you must deposit a minimum of Rs.2000" << endl;
        cout << "\t\t\t\t" << "Enter Amount to deposit: ";
        cin >> amount;
        if(amount >= 2000 && (!cin.fail())) {
            break;
        } else {
            cout << "\t\t\t\t" << RED_COLOR << "Invalid Amount" << RESET_COLOR << endl;
            cin.clear();
            // Clear the input buffer
            continue;
        }
    }
    string time = gettime();
    string accBal = to_string(amount);
    data[0][8] = accBal;
    data[1][0] = " " + time + " ";
    data[1][1] = "Deposited        ";
    data[1][2] = "+" + accBal;

    // Generate a new CSV file name
    string filename = accNum;
    const string folderPath = "Details/" + filename + ".csv";

    // Open the CSV file for writing
    ofstream file(folderPath);

    // Write each row to the file
    for(int i = 0; i < 2; i++) {
        for (int j = 0; j < 11; j++) {
            file << data[i][j];

            // Add a comma if it's not the last column
            if (j < 9 - 1) {
                file << ",";
            }
        }
        file << endl;
    }

    // Close the file
    file.close();

    filename = "Details/current_users.csv"; // Add the new account number to the current users file
    userAccounts.clear();
    userAccounts.shrink_to_fit();
    readData(userAccounts, filename);
    vector<string> newData = {accNum};
    userAccounts.push_back(newData);
    updateData(userAccounts, filename);
    userAccounts.clear();
    userAccounts.shrink_to_fit();
    cout << endl << endl;
    cout << "\t\t\t\t" << "Account Created Successfully... " << endl;
    cout << "\t\t\t\t" << "Account Number is " << accNum << endl;

    cout << endl << endl;
    cout << "\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    counterMenu();
}

void deposit() {
    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl <<endl;
    cout << "\t\t\t\t" <<  "-------------------------------- Deposit Money -----------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;
    float amount, newBal;
    // Converting the string value into a float for calculations.
    float accBal = stof(userInfo[0][8]);

    while(true) {
        cout << "\t\t\t\t" << "Enter the amount to Deposit: ";
        cin >> amount;

        if( amount > 0 ) {
            string time = gettime();
            newBal = accBal + amount;
            userInfo[0][8] = to_string(newBal);
            vector<string> newData = {" " + time + " ","Deposited        ", "+" + to_string(amount)};
            userInfo.push_back(newData); // Recording the transaction in user account.

            cout << endl;
            cout << "\t\t\t\t" << "The amount is Successfully Deposited!\n" << endl;
            break;
        } else {
            cout << "\t\t\t\t" << "Invaild Amount. Please Try Again..." << endl << endl;
        }
    }
    const string filename = userInfo[0][1];
    const string folderPath = "Details/" + filename + ".csv";
    updateData(userInfo, folderPath);
    cout << endl << endl;
    cout << "\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    counterMenu();
}

void withdraw() {
    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t" <<  "-------------------------------- Withdraw Money --------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;
    float amount, newBal;
    // Converting the string value into an integer
    float accBal = stof(userInfo[0][8]);

    while(true) {
        cout << "\t\t\t\t" << "Enter the amount to Withdraw: ";
        cin >> amount;

        if( amount < accBal && amount > 0 ) {
            string time = gettime();
            newBal = accBal - amount;
            userInfo[0][8] = to_string(newBal);
            vector<string> newData = {" " + time + " ","Withdrawn        ", "-" + to_string(amount)};
            userInfo.push_back(newData); // Recording the transaction in user account.

            cout << endl;
            cout << "\t\t\t\t" << "The amount is Successfully Withdrawn!\n" << endl;
            break;
        } else {
            cout << "\t\t\t\t" << "Invaild Amount. Please Try Again..." << endl << endl;
        }
    }
    const string filename = userInfo[0][1];
    const string folderPath = "Details/" + filename + ".csv";
    updateData(userInfo, folderPath);
    userInfo.clear();
    cout << "\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    counterMenu();

}

void userDetails() {
    int choice1, choice2;
    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t" <<  "-------------------------------- Account Details ----------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;

    while(true) {
        cout << "\t\t\t\t" << "Account Name    : " << userInfo[0][0] << endl;
        cout << "\t\t\t\t" << "Account Number  : " << userInfo[0][1] << endl;
        cout << "\t\t\t\t" << "NIC Number      : " << userInfo[0][2] << endl;
        cout << "\t\t\t\t" << "Contact Number  : " << userInfo[0][3] << endl;
        cout << "\t\t\t\t" << "Home Address    : " << userInfo[0][4] << "," << endl;
        cout << "\t\t\t\t" << "                  " << userInfo[0][5] << "," << endl;
        cout << "\t\t\t\t" << "                  " << userInfo[0][6] << "." << endl;
        cout << "\t\t\t\t" << "Account Balance : " << fixed << setprecision(2) << stof(userInfo[0][8]) << endl;
        cout << endl;
        cout << "\t\t\t\t" << "[1].Edit User Information" << endl;
        cout << "\t\t\t\t" << "[2].Back" << endl;
        cout << endl;
        cout << "\t\t\t\t" << "Input: ";
        cin >> choice1;

        if(choice1 == 1) {
            system("cls");
            cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
            cout << "\t\t\t\t" <<  "----------------------------- Edit User Informations ------------------------------"<<endl;

            cout << endl << endl << endl << endl << endl;
            while(true) {
                cout << "\t\t\t\t"<< "[1]Contact Number " << endl;
                cout << "\t\t\t\t"<< "[2]Home Address   " << endl;
                cout << "\t\t\t\t"<< "[3]Back" << endl;
                cout << endl;
                cout << "\t\t\t\t"<< "Input: ";
                cin >> choice2;

                switch(choice2) {
                    case 1:
                    system("cls");
                    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
                    cout << "\t\t\t\t" <<  "-------------------------------- Edit Contact Number --------------------------------"<<endl;

                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t"<< "Enter New Contact Number: ";
                    cin >> userInfo[0][3];
                    cout << "\t\t\t\t"<< "Data Updated Successfully..." << endl;
                    break;

                    case 2:
                    system("cls");
                    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
                    cout << "\t\t\t\t" <<  "-------------------------------- Edit Home Address ----------------------------------"<<endl;

                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t"<< "Enter New Address: ";
                    getline (cin >> ws, userInfo[0][4]);
                    cout << "\t\t\t\t"<< "                   ";
                    getline (cin, userInfo[0][5]);
                    cout << "\t\t\t\t"<< "                   ";
                    getline (cin, userInfo[0][6]);
                    cout << "\t\t\t\t"<< "Data Updated Successfully..." << endl;
                    break;

                    case 3:
                    userDetails();
                    break;

                    default:
                    system("cls");
                    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl;

                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t"<< "Invalid Input...";
                    continue;
                }

                const string filename = userInfo[0][1];
                const string folderPath = "Details/" + filename + ".csv";
                updateData(userInfo, folderPath);
            }

        } else if(choice1 == 2) {
            counterMenu();
        } else {
            system("cls");
            cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl;

            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t" << "Invalid Input...";
            continue;
        }
    }
}

void fixdeposit() {
    string accNum;
    int choice;
    float principal; //deposit value
    float matureValue,interest,mon_interest; //after mature date value
    char ans;

    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t" <<  "------------------------------------ Fix Deposit ----------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t" << "Enter your account Number : ";
    cin >> accNum;

    system("cls");
    cout << "\t\t\t\t" <<  "  _____ ____  _    _ _   _ _______ ______ _____       __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t" <<  " / ____/ __ \\| |  | | \\ | |__   __|  ____|  __ \\     |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | |  \\| |  | |  | |__  | |__) |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |   | |  | | |  | | . ` |  | |  |  __| |  _  /     | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t" <<  "| |___| |__| | |__| | |\\  |  | |  | |____| | \\ \\     | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t" <<  " \\____ \\____/ \\____/|_| \\_|  |_|  |______|_|  \\_\\    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t" <<  "------------------------------------ Fix Deposit ----------------------------------"<<endl;

    cout << endl << endl << endl << endl << endl;

    string filename = "Details/" + accNum + ".csv";
    // Import Userdata into the fixedInfo array
    userInfo.clear();
    userInfo.shrink_to_fit();
    readData(userInfo, filename); // Load the user details into the userInfo array.
    filename = "FixedDeposit/fdInfo.csv";
    fixedInfo.clear();
    fixedInfo.shrink_to_fit();
    readData(fixedInfo, filename); // Open the fixed  Deposit file and load it into fixedInfo array

    cout << "\t\t\t\t" << "---------------------------------------------------------------" << endl;
    cout << "\t\t\t\t" << "|         Time Duration       |          Intrest Rate          |"<< endl;
    cout << "\t\t\t\t" << "---------------------------------------------------------------|" << endl;
    cout << "\t\t\t\t" << "|           3 Months          |               5%               |" << endl;
    cout << "\t\t\t\t" << "|           6 Months          |               6%               |" << endl;
    cout << "\t\t\t\t" << "|            1 Year           |               8%               |" << endl;
    cout << "\t\t\t\t" << "|            2 Year           |               10%              |" << endl;
    cout << "\t\t\t\t" << "|            5 Year           |               15%              |" << endl;
    cout << "\t\t\t\t" << "---------------------------------------------------------------|" << endl;
    cout << endl;
    cout << "\t\t\t\t" << "Enter principal amount : Rs.";
    cin >> principal;
    cout << endl;
    cout << "\t\t\t\t" << "Select the duration " << endl;
    cout << "\t\t\t\t" << "[1]3 Months" << endl;
    cout << "\t\t\t\t" << "[2]6 Months" << endl;
    cout << "\t\t\t\t" << "[3]1 years" << endl;
    cout << "\t\t\t\t" << "[4]2 years" << endl;
    cout << "\t\t\t\t" << "[5]5 years" << endl;
    cout << "\t\t\t\t" << "Input: ";
    cin >> choice;

    // Generate the interest based on the duration of the fixedDeposit.
    if(choice == 1) {
        interest = (principal * 5/100)/4;
        mon_interest = interest/6;
    } else if(choice == 2) {
        interest = (principal * 6/100)/2;
        mon_interest = interest/12;
    } else if(choice == 3) {
        interest = (principal * 8/100);
        mon_interest = interest/24;
    } else if(choice == 4) {
        interest = (principal * 10/100)*2;
        mon_interest = interest/60;
    }  else if(choice == 5) {
        interest = (principal * 15/100)*5;
        mon_interest = interest/60;
    }
    matureValue = principal + interest;
    cout << "\t\t\t\t" << "Your interest : Rs." << fixed << setprecision(2) << interest << endl;
    cout << "\t\t\t\t" << "After Completion you have : Rs." << fixed << setprecision(2) << matureValue << endl;

    cout << "\t\t\t\t" << "Do you want to Confirm : (Y/N) ";
    cin >> ans;

    if ((ans == 'y')||(ans == 'Y')) {
    // Import the current time and record it in the fixed Deposit file along with other information.
    time_t thetime = time(nullptr);
    tm currentDate = *localtime(&thetime);
    string nowtime = gettime();

    vector<string> newData = {to_string(currentDate.tm_year + 1900),to_string(currentDate.tm_mon + 1),to_string(currentDate.tm_mday),nowtime, accNum, to_string(principal), to_string(interest), to_string(interest), to_string(mon_interest)};
    fixedInfo.push_back(newData);

    const string folderPath = "FixedDeposit/fdInfo.csv";
    updateData(fixedInfo, folderPath); // Update the fixedDeposit Information file and clear the arrays.
    userInfo.clear();
    userInfo.shrink_to_fit();
    fixedInfo.clear();
    fixedInfo.shrink_to_fit();
    cout << "\t\t\t\t" << "Amount deposited Successfully!" << endl;
    cout << endl << endl;
    cout << "\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    counterMenu();
    } else if((ans == 'N') || (ans == 'n')) {
        counterMenu();
    }
}

void app_loan() {
    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "--------------------- Apply For A Loan ----------------------" << endl;

    cout << endl << endl << endl << endl << endl;
    string acc_no;
    int amount,time1;
    char ans;
    float tot_int,installment;
    string folderPath = "Rates/loan.csv";
    Rates.clear();
    Rates.shrink_to_fit();
    readData(Rates, folderPath); // Import the interest rates for respective directory.

    cout << endl;
    cout << "\t\t\t\t\t\t" << "Enter Account Number: ";
    cin >> acc_no;

    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "--------------------- Apply For A Loan ----------------------" << endl;

    cout << endl << endl << endl << endl << endl;

    int validity = acc_check(acc_no); // Check whether the account no. exists & continue only if it exists

    if(validity == 1) {
        cout << endl;
        cout << "\t\t\t\t\t\t" << "                     Interest Rates\n";
        cout << "\t\t\t\t\t\t" << "   ________________________________________________\n";
        cout << "\t\t\t\t\t\t" << "   |    Month      |     Interest Rate (Per Year) |\n";
        cout << "\t\t\t\t\t\t" << "   ------------------------------------------------\n";
        cout << "\t\t\t\t\t\t" << "   |     03        |            " << Rates[2][1] << "%               |\n";
        cout << "\t\t\t\t\t\t" << "   |     06        |            " << Rates[3][1] << "%             |\n";
        cout << "\t\t\t\t\t\t" << "   |     12        |            " << Rates[4][1] << "%               |\n";
        cout << "\t\t\t\t\t\t" << "   |     18        |            " << Rates[5][1] << "%             |\n";
        cout << "\t\t\t\t\t\t" << "   |     24        |            " << Rates[6][1] << "%               |\n";
        cout << "\t\t\t\t\t\t" << "   |     48        |            " << Rates[7][1] << "%             |\n";
        cout << "\t\t\t\t\t\t" << "   ------------------------------------------------\n";
        cout << "\t\t\t\t\t\t" << endl;

        cout << "\t\t\t\t\t\t" << "Enter Loan Amount : ";
        cin >> amount;
        cout << "\t\t\t\t\t\t" << "Enter Time Period : ";
        cin >> time1;

        cout << endl << endl;

        // Display the interest rate and the interest based on the entered amount and time period.
        if( amount > 0 ) {
            if( time1 == 3 ){
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[2][1] << "%" << endl;
                tot_int = amount * (stof(Rates[2][1]) / 100) / 4;

            } else if (time1 == 6) {
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[3][1] << "%" << endl;
                tot_int = amount * (stof(Rates[3][1]) / 100) / 2;

            } else if (time1 == 12) {
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[4][1] << "%" << endl;
                tot_int = amount * (stof(Rates[4][1]) / 100);

            } else if(time1 == 18) {
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[5][1] << "%" << endl;
                tot_int = (amount * (stof(Rates[5][1]) / 100)) * 3 / 2;

            } else if(time1 == 24){
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[6][1] << "%" << endl;
                tot_int = (amount * (stof(Rates[6][1]) / 100)) * 2;

            }else {
                cout << "\t\t\t\t\t\t" << "Interest Rate is " << Rates[7][1] << "%" << endl;
                tot_int = (amount * (stof(Rates[7][1]) / 100)) * 4;

            }
        }

        cout << "\t\t\t\t\t\t" << "Interest For The Period(" << time1 << "months) : " << fixed << setprecision(2) << tot_int << endl;
        cout << "\t\t\t\t\t\t" << "Total Payable Amount : " << fixed << setprecision(2) << (amount + tot_int) << endl;
        installment = (amount + tot_int)/time1;
        cout << "\t\t\t\t\t\t" << "Monthly Installment : " << fixed << setprecision(2) << installment << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t" << "Do you want to Confirm : (Y/N) ";
        cin >> ans;

        if ((ans == 'y')||(ans == 'Y')){

            string filename = "Details/" + acc_no + ".csv";
            userInfo.clear();
            userInfo.shrink_to_fit();
            // Import Userdata into the userInfo array
            readData(userInfo, filename);

            float accBal = stof(userInfo[0][8]);

            string currenttime = gettime();
            float newBal = accBal + amount;
            userInfo[0][8] = to_string(newBal);
            vector<string> newData = {" " + currenttime + " ","Loan Transaction ", "+" + to_string(amount)};
            userInfo.push_back(newData); // Update the bank balance with the loan and record the transaction in the used account.
            updateData(userInfo, filename);
            userInfo.clear();
            userInfo.shrink_to_fit();

            filename = "Loans/loandetails.csv";
            loanInfo.clear();
            loanInfo.shrink_to_fit();
            readData(loanInfo, filename);
            // Update the loans file recording the necessary Information.
            vector<string> newData1 = {" " + currenttime + " ", acc_no , to_string(amount), to_string(tot_int), to_string(installment), to_string(time1), "0", to_string(amount + tot_int)};
            loanInfo.push_back(newData1);
            updateData(loanInfo, filename);
            newData1.clear();
            newData1.shrink_to_fit();

        } else {
            loanMenu();
        }

    } else {
        cout << "\t\t\t\t\t\t" << "Account Number Not Found !";
    }

    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "--------------------- Apply For A Loan ----------------------" << endl;

    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t\t\t" << "Loan Successfully Deposited to the account!" << endl;
    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    loanMenu();

}

void pay_loan() {
    while (true){
        float aop;
        string acc_no;
        system("cls");
        cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
        cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
        cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
        cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
        cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
        cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
        cout << "\t\t\t\t\t\t" << "----------------------- Loan Payments ------------------------" << endl;


        cout << endl << endl << endl << endl << endl;

        cout << "\t\t\t\t\t\t" << "Enter Account Number : ";
        cin >> acc_no;
        cout<<endl<<endl;

        int validity = acc_check(acc_no);

        if(validity == 1) {

            string filename = "Details/" + acc_no + ".csv";
            userInfo.clear();
            userInfo.shrink_to_fit();
            readData(userInfo, filename); // Clearing the arrays and loading data into it.

            filename = "Loans/loandetails.csv";
            loanInfo.clear();
            loanInfo.shrink_to_fit();
            readData(loanInfo, filename); // Clearing the arrays and loading data into it.

            int index_no = GetIndex(acc_no ,loanInfo); // Finding the respective row containing the username entered.

            cout << "\t\t\t\t\t\t" << "Full Name : " << userInfo[0][0] << endl;
            cout << "\t\t\t\t\t\t" << "Account Number : " << userInfo[0][1] << endl;
            cout << "\t\t\t\t\t\t" << "Loan Amount : " << fixed << setprecision(2) << stof(loanInfo[index_no][2]) << endl;
            cout << "\t\t\t\t\t\t" << "Duration : " << loanInfo[index_no][5] << endl;
            cout << "\t\t\t\t\t\t" << "Monthly Installment : " << fixed << setprecision(2) << stof(loanInfo[index_no][4]) << endl;
            cout << "\t\t\t\t\t\t" << "Remaining Amount : " << fixed << setprecision(2) << stof(loanInfo[index_no][7]) << endl;
            cout << endl;
            cout << "\t\t\t\t\t\t" << "Amount Of Payment :";
            cin >> aop;
            float remainamount = stof(loanInfo[index_no][7]); // Converting the string into float values and performing calculations
            float newamount = remainamount - aop;
            loanInfo[index_no][7] = to_string(newamount);
            float paid = stof(loanInfo[index_no][6]);
            float newpaid = paid + aop;
            loanInfo[index_no][6] = to_string(newpaid); // Updating the files with new values.

            string time1 = gettime();

            vector<string> newData = {" " + time1 + " ","Loan Payment     ", "-" + to_string(aop)};
            userInfo.push_back(newData);

            string folderPath = "Details/" + acc_no + ".csv";
            updateData(userInfo, folderPath);
            userInfo.clear();
            userInfo.shrink_to_fit();

            folderPath = "Loans/loandetails.csv";
            updateData(loanInfo, folderPath);
            loanInfo.clear();
            loanInfo.shrink_to_fit();

            break;

        } else {
            cout << "\t\t\t\t\t\t" << "Account Number Not Found !";
        }
    }
    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "----------------------- Loan Payments ------------------------" << endl;

    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t\t\t" << "Payment Successfully Completed!" << endl;
    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    loanMenu();
}

void his_loan() {
    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "-------------------- Loan Payment History ---------------------" << endl;

    cout << endl << endl << endl << endl << endl;
    string accNum;

    cout << "\t\t\t\t\t\t" << "Enter Account Number : ";
    cin >> accNum;

    string filename = "Details/" + accNum + ".csv";
    userInfo.clear();
    userInfo.shrink_to_fit();
    readData(userInfo, filename);

    system("cls");
    cout << "\t\t\t\t\t\t" << " _      ____          _   _      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t\t" << "| |    / __ \\   /\\   | \\ | |    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | | /  \\  |  \\| |    | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |   | |  | |/ /\\ \\ | . ` |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t\t" << "| |___| |__| / ____ \\| |\\  |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t\t" << "|______\\____/_/    \\_\\_| \\_|    |_|  |_|______|_| \\_|\\____/ " << endl<<endl;
    cout << "\t\t\t\t\t\t" << "-------------------- Loan Payment History ---------------------" << endl;

    cout << endl << endl << endl << endl << endl;

    cout << "\t\t\t\t\t\t" << "Your Account Transaction History Is As Follows; \n" << endl;

    for (int row = 1; row < userInfo.size(); row++) { // Using a for loop to output all the transactions.
        if(userInfo[row][1] == "Loan Payment     " ) { // Filtering the transactions and display loan transactions only.
            cout << "\t\t\t\t\t\t";
            cout << userInfo[row][0] << " | ";
            cout << userInfo[row][1] << " | LKR. ";
            cout << fixed << setprecision(2) << stof(userInfo[row][2]) << endl;
        }
    }
    userInfo.clear();
    userInfo.shrink_to_fit();

    cout << endl << endl;
    cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
    _getch();
    loanMenu();
}

void pawning_item() {
    int Karatage;
    float per_pound, payable_value, weight, pre_amount, interest;
    char choice, answer;
    string name, email, NIC, tp_no;
    system("cls");
    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
    cout << "\t\t\t\t\t" << "-------------------------------------- Pawing Item -------------------------------------" << endl;
    cout << endl << endl << endl << endl << endl;

    while (true){

        cout << "\t\t\t\t\t" << "Enter Karatage Value (20,21,22,24) : ";
        cin >> Karatage;

        if (Karatage == 24) {
            per_pound = 122000;
            break;
        } else if(Karatage == 22){
            per_pound = 110000;
            break;
        } else if(Karatage == 21){
            per_pound = 104000;
            break;
        } else if(Karatage == 20){
            per_pound = 99000;
            break;
        } else {
            cout << "\t\t\t\t\t" << "Enter Valid Pound Value :";
        }
    }

    cout << "\t\t\t\t\t" << "Enter the Weight of Gold Item (In Pound) : ";
    cin >> weight;
    cout << endl;
    payable_value = weight * per_pound;
    cout << "\t\t\t\t\t" << "Payable Amount is : Rs." << fixed << setprecision(2) << payable_value << endl;

    do {
        cout << "\t\t\t\t\t" << "Enter Preferred Amount : ";
        cin >> pre_amount;
        cout << endl;

        if (pre_amount <= payable_value) {
            interest = (pre_amount * 22 / 100) / 12;
            cout << "\t\t\t\t\t" << "Anual Interest Rate : 22%" << endl;
            cout << "\t\t\t\t\t" << "Monthly Interest Amount is : Rs." << fixed << setprecision(2) << interest << endl << endl;
            //cout << "Total Amount to payable : Rs." << ((interest) + pre_amount) << endl;

            while (true){
                cout << "\t\t\t\t\t" << "Confirm Pawing [Y/N] : ";
                cin >> choice;
                cout<<endl;
                if (choice == 'Y' || choice == 'y') {
                    cout << "\t\t\t\t\t" << "Enter Customer Name : ";
                    getline( cin >> ws,name );
                    cout << "\t\t\t\t\t" << "Enter NIC Number : ";
                    cin >> NIC;
                    cout << "\t\t\t\t\t" << "Enter Telephone Number : ";
                    cin >> tp_no;
                    cout << "\t\t\t\t\t" << "Enter E-mail : ";
                    cin >> email;
                    cout<<endl;

                    string nowtime = gettime();

                    string invoice = createInvoice();
                    string folderPath = "GoldPawning/" + invoice + ".csv";
                    ofstream file(folderPath);
                    file.close();
                    readData(pawnInfo, folderPath);

                    time_t thetime = time(nullptr);
                    tm currentDate = *localtime(&thetime);

                    vector<string> newData = {to_string(currentDate.tm_year + 1900),to_string(currentDate.tm_mon + 1),to_string(currentDate.tm_mday), nowtime, invoice, name, NIC, tp_no, email, to_string(pre_amount), to_string(pre_amount), to_string(weight)};
                    pawnInfo.push_back(newData);

                    folderPath = "GoldPawning/" + invoice + ".csv";
                    updateData(pawnInfo, folderPath);
                    pawnInfo.clear();
                    pawnInfo.shrink_to_fit();


                    folderPath = "GoldPawning/Invoices.csv";
                    userAccounts.clear();
                    userAccounts.shrink_to_fit();
                    readData(userAccounts, folderPath);

                    vector<string> newData1 = {invoice};
                    userAccounts.push_back(newData1);

                    folderPath = "GoldPawning/Invoices.csv";
                    updateData(userAccounts, folderPath);
                    userAccounts.clear();
                    userAccounts.shrink_to_fit();

                    system("cls");
                    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
                    cout << "\t\t\t\t\t" << "-------------------------------------- Pawing Item -------------------------------------" << endl;
                    cout << endl << endl << endl << endl << endl;
                    cout << endl << endl << endl;
                    cout << "\t\t\t\t\t" << "Pawning Successful"<< endl;

                    cout << "\t\t\t\t\t" << "Press any key to continue" << endl;
                    _getch();
                    pawningMenu();

                    break;

                } else if (choice == 'N' || choice == 'n') {
                    cout << "\t\t\t\t\t" << "[1] New Pawing" << endl;
                    cout << "\t\t\t\t\t" << "[2] Main Menu " << endl;
                    cout << "\t\t\t\t\t" << "Answer : ";
                    cin >> answer;

                    switch (answer){
                    case 1:
                        pawning_item();
                    case 2:
                        pawningMenu();
                    }

                } else {
                    system("cls");
                    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
                    cout << "\t\t\t\t\t" << "-------------------------------------- Pawing Item -------------------------------------" << endl;
                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t\t" << "Enter a Valid Answer" << endl;
                }
            }

        } else {
            system("CLS");
            cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
            cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
            cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
            cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
            cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
            cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
            cout << "\t\t\t\t\t" << "-------------------------------------- Pawing Item -------------------------------------" << endl;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t\t" << "Invalid Amount. Please enter a valid amount." << endl;
        }
    } while (pre_amount > payable_value);

}

void gold_payment(){

    system("cls");
    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
    cout << "\t\t\t\t\t" << "------------------------------------ Payments ---------------------------------------" << endl;
    cout << endl << endl << endl << endl << endl;

    while(true) {
        string invoice_no;
        float payment_amount,payable_amount;
        int choice;
        char choice1,choice2;

        cout << "\t\t\t\t\t" << "Enter Invoice No : ";
        getline(cin >> ws, invoice_no);

        string folderPath = "GoldPawning/" + invoice_no + ".csv";
        readData(pawnInfo, folderPath);

        //check invoice number
        int validity = invoice_check(invoice_no);

        while(true){
            if(validity == 1) {

                cout << "\t\t\t\t\t" << "Full Name : " << pawnInfo[0][5] << endl;
                cout << "\t\t\t\t\t" << "NIC Number : " << pawnInfo[0][6] << endl;
                cout << "\t\t\t\t\t" << "Invoice Number : " << pawnInfo[0][4] << endl;
                cout << endl;
                cout << "\t\t\t\t\t" << "Confirm Details [Y/N] : ";
                cin >> choice1;

                if (choice1 == 'Y'|| choice1 == 'y'){
                    system("cls");
                    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
                    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
                    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
                    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
                    cout << "\t\t\t\t\t" << "-------------------------------------- Payment --------------------------------------" << endl;
                    cout << endl << endl << endl << endl << endl;
                    string nowtime = gettime();
                    tm startDate = {};

                    int year = stoi(pawnInfo[0][0]);
                    int month = stoi(pawnInfo[0][1]);
                    int day = stoi(pawnInfo[0][2]);

                    startDate.tm_year = year - 1900; // year - 1900
                    startDate.tm_mon = month - 1;    // month is 0-based
                    startDate.tm_mday = day;
                    time_t startDateInTimeT = mktime(&startDate);
                    time_t currentDate = time(nullptr);

                    double durationInSeconds = difftime(currentDate, startDateInTimeT); // Duration in seconds
                    double durationInDays = durationInSeconds / (60 * 60 * 24); // Duration in days

                    float loan_amount = stof(pawnInfo[0][10]);
                    float amountpayable = loan_amount * 0.22 * durationInDays / 365;
                    float newamount = loan_amount + amountpayable - payment_amount;

                    cout << "\t\t\t\t\t" << "Number of Pound you pawed : " << fixed << setprecision(2) << stof(pawnInfo[0][11]) << endl;//number of pond
                    cout << "\t\t\t\t\t" << "Amount got from Pawning : "<< fixed << setprecision(2) << stof(pawnInfo[0][9]) << endl;//value of gold
                    cout << "\t\t\t\t\t" << "Your Total Payable Amount is : " << fixed << setprecision(2) << newamount << endl;//total payable amount (gold value + current interest amount)
                    cout << "\t\t\t\t\t" << "Your Current Interest Amount is : " << fixed << setprecision(2) << amountpayable << endl; //auto generated day and calculate interest ((from last day of pay day count) * 22/365)
                    cout << endl;
                    cout << "\t\t\t\t\t" << "Enter Payment Amount : ";
                    cin >> payment_amount;
                    cout << endl << endl;
                    cout << "\t\t\t\t\t" << "Confirm Payment [Y/N]: ";
                    cin >> choice2;

                    if (choice2 == 'Y'|| choice2 == 'y'){

                        float newamount = loan_amount + amountpayable - payment_amount;
                        pawnInfo[0][10] = to_string(newamount);
                        time_t thetime = time(nullptr);
                        tm currentDate1 = *localtime(&thetime);
                        fixedInfo[0][0] = to_string(currentDate1.tm_year + 1900);
                        fixedInfo[0][1] = to_string(currentDate1.tm_mon + 1);
                        fixedInfo[0][2] = to_string(currentDate1.tm_mday);

                        vector<string> newData = {" " + nowtime + " ", " Payment  ", to_string(payment_amount)};
                        pawnInfo.push_back(newData);
                        folderPath = "GoldPawning/" + invoice_no + ".csv";
                        updateData(pawnInfo, folderPath);
                        pawnInfo.clear();
                        pawnInfo.shrink_to_fit();

                        cout << endl << endl;

                        cout << "\t\t\t\t\t" << "Your payment Successfully Completed .....";
                        cout << "\t\t\t\t\t" << "Thank You!" << endl;

                        cout << "\t\t\t\t\t" << "Press any key to continue" << endl;
                        _getch();
                        pawningMenu();
                        break;

                    }else if (choice2 == 'N'||choice2 == 'n'){
                        pawningMenu();
                        break;
                    }else{
                        cout << "\t\t\t\t\t" << "Invalid Selection ! Please Input Valid Selection....";
                    }

                }
            } else {

                cout << "\t\t\t\t\t" << "Account Number Not Found !";
                break;
            }
        }
    }
}

void pawn_his() {
    string invoice_no;
    system("cls");
    cout << "\t\t\t\t\t" << " _____   __          ___   _ _____ _   _  _____      __  __ ______ _   _ _    _ " << endl;
    cout << "\t\t\t\t\t" << "|  __ \\ /\\ \\        / / \\ | |_   _| \\ | |/ ____|    |  \\/  |  ____| \\ | | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |__) /  \\ \\  /\\  / /|  \\| | | | |  \\| | |  __     | \\  / | |__  |  \\| | |  | |" << endl;
    cout << "\t\t\t\t\t" << "|  ___/ /\\ \\ \\/  \\/ / | . ` | | | | . ` | | |_ |    | |\\/| |  __| | . ` | |  | |" << endl;
    cout << "\t\t\t\t\t" << "| |  / ____ \\  /\\  /  | |\\  |_| |_| |\\  | |__| |    | |  | | |____| |\\  | |__| |" << endl;
    cout << "\t\t\t\t\t" << "|_| /_/    \\_\\/  \\/   |_| \\_|_____|_| \\_|\______|    |_|  |_|______|_| \\_|\_____/ " << endl<<endl;
    cout << "\t\t\t\t\t" << "---------------------------------- Payment History -------------------------------------" << endl;
    cout << endl << endl << endl << endl << endl;

    cout << "\t\t\t\t\t" << "Enter Invoice No : ";
    getline(cin >> ws, invoice_no);

    string folderPath = "GoldPawning/" + invoice_no + ".csv";
    readData(pawnInfo, folderPath);

    //check invoice number
    int validity = invoice_check(invoice_no);
    if(validity == 1) {
        cout << endl << endl;
        cout << "\t\t\t\t\t\t" << "Your Payment History Is As Follows; \n" << endl;

        for (int row = 1; row < pawnInfo.size(); row++) {
            cout << "\t\t\t\t\t\t";

            cout << pawnInfo[row][0] << " | ";
            cout << pawnInfo[row][1] << " | LKR. ";
            cout << fixed << setprecision(2) << stof(pawnInfo[row][2]);

            cout << "\n";
        }

        cout << endl << endl;
        cout << "\t\t\t\t\t\t" << "Press any key to continue" << endl;
        _getch();
        pawningMenu();
    }

}

void adminMenu() {
    int choice;
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t  " << "[1]Create Admin Accounts" << endl;
    cout << "\t\t\t\t  " << "[2]Edit Loan Interest rates" << endl;
    cout << "\t\t\t\t  " << "[3]Edit Pawning Interest rates" << endl;
    cout << "\t\t\t\t  " << "[4]View Loan Information" << endl;
    cout << "\t\t\t\t  " << "[5]View Fixed Deposits" << endl;
    cout << "\t\t\t\t  " << "[6]View Pawning Information" << endl;
    cout << "\t\t\t\t  " << "[7]Log Out" << endl;
    cout << endl;
    cout << "\t\t\t\t  " << "Input : ";
    cin >> choice;

    switch(choice) {
        case 1:
            create_admin();
            break;
        case 2:
            loan_rates();
            break;
        case 3:
            pawn_rates();
            break;
        case 4:
            loan_admin();
            break;
        case 5:
            fixed_admin();
            break;
        case 6:
            pawn_admin();
            break;
        case 7:
            Login();
    }
}

void create_admin() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    int choice;
    string acc_no, pin, dept;
    const string filename = "login/logindetails.csv";
    adminInfo.clear();
    adminInfo.shrink_to_fit();
    // Import login details into the 2D array adminInfo
    readData(adminInfo, filename);

    acc_no = generate_username();
    cout << "\t\t\t\t  " << "Account Number is " << acc_no << endl;
    cout << endl;
    cout << "\t\t\t\t  " << "Enter a Pin Number : ";
    cin >> pin;
    cout << endl;
    cout << "\t\t\t\t  " << "Select Department : " << endl;
    cout << "\t\t\t\t  " << "       [1]Counter" << endl;
    cout << "\t\t\t\t  " << "       [2]Loan" << endl;
    cout << "\t\t\t\t  " << "       [3]Pawning" << endl;
    cout << endl;
    cout << "\t\t\t\t  " << "Input : ";
    cin >> choice;
    switch(choice) {
        case 1:
            dept = "counter";
            break;
        case 2:
            dept = "loan";
            break;
        case 3:
            dept = "pawning";
            break;
    }
    vector<string> newData = {acc_no, pin, dept};
    adminInfo.push_back(newData);
    updateData(adminInfo, filename);

    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    cout << "\t\t\t\t  " << "Account Created Successfully!";
    cout << endl << endl;
    cout << "\t\t\t\t  " << "Account Number: " << acc_no << endl;
    cout << "\t\t\t\t  " << "Password      : " << pin << endl;
    cout << "\t\t\t\t  " << "Department    : " << dept;
    cout << endl << endl;
    cout << "\t\t\t\t  " << "Press any key to continue" << endl;
    _getch();
    adminMenu();
}

void loan_rates() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    int choice;
    string folderPath = "Rates/loan.csv";
    Rates.clear();
    Rates.shrink_to_fit();
    readData(Rates, folderPath);

    for (int row = 0; row < Rates.size(); row++) {
        cout << "\t\t\t\t\t\t\t";
        for (int col = 0; col < Rates[row].size(); col++) {

            cout << Rates[row][col];
            if( row != 0 && col == 1) {
                cout << "%";
            } else if( row != 0 && row != 1) {
                cout << "  |  ";
            }
        }
        cout << "\n";
    }
    while(true)  {
        cout << endl << endl;
        cout << "\t\t\t\t  " << "[1]Edit Rates" << endl;
        cout << "\t\t\t\t  " << "[2]Back" << endl;
        cout << endl;
        cout << "\t\t\t\t  " << "Input : ";
        cin >> choice;

        switch(choice) {
            case 1:
                edit_rate("loan");
                break;
            case 2:
                adminMenu();
                break;
            default:
                cout << "\t\t\t\t  " << "Invalid Input!!!";
                continue;
        }
    }

}

void pawn_rates() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    int choice;
    string folderPath = "Rates/fixed.csv";
    Rates.clear();
    Rates.shrink_to_fit();
    readData(Rates, folderPath);

    for (int row = 0; row < Rates.size(); row++) {
        for (int col = 0; col < Rates[row].size(); col++) {

            cout << Rates[row][col];
            if( row != 0 && col == 1) {
                cout << "%";
            } else if( row != 0){
                cout << "  |  ";
            }
        }
        cout << "\n";
    }
    while(true)  {
        cout << endl << endl;
        cout << "\t\t\t\t  " << "[1]Edit Rates" << endl;
        cout << "\t\t\t\t  " << "[2]Back" << endl;
        cout << endl;
        cout << "\t\t\t\t  " << "Input : ";
        cin >> choice;

        switch(choice) {
            case 1:
                edit_rate("fixed");
                break;
            case 2:
                adminMenu();
                break;
            default:
                cout << "\t\t\t\t  " << "Invalid Input!!!";
                continue;
        }
    }
}

void loan_admin() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    string folderPath = "Loans/loandetails.csv";

    ifstream file(folderPath);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }
    // Read and output data rows
    string line;
    while (getline(file, line)) {
        cout << "\t\t";
        // Split the line into columns
        istringstream tokenStream(line);
        string token;
        while (getline(tokenStream, token, ',')) {
            cout << setw(15) << left << token;
        }
        cout << endl << endl;
    }
    // Close the file
    file.close();


    cout << endl << endl;
    cout << "\t\t\t\t  " << "Press any key to continue" << endl;
    _getch();
    adminMenu();
}

void fixed_admin() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    string folderPath = "FixedDeposit/fdInfo.csv";

    ifstream file(folderPath);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }
    // Read and output data rows
    string line;
    while (getline(file, line)) {
        cout << "\t\t";
        // Split the line into columns
        istringstream tokenStream(line);
        string token;
        for (int i = 0; i < 3; ++i) {
            // Skip the first 3 columns
            if (getline(tokenStream, token, ',')) {
                continue;
            }
        }
        for (int i = 3; getline(tokenStream, token, ','); ++i) {
            // Increase the width of the 8th column
            if (i == 8) {
                cout << setw(25) << left << token;
            } else if(i == 3) {
                cout << setw(24) << left << token;
            } else {
                cout << setw(15) << left << token;
            }
        }

        cout << endl << endl;
    }
    // Close the file
    file.close();

    cout << endl << endl;
    cout << "\t\t\t\t  " << "Press any key to continue" << endl;
    _getch();
    adminMenu();
}

void pawn_admin() {
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;
    string folderPath = "GoldPawning/Invoices.csv";
    userAccounts.clear();
    userAccounts.shrink_to_fit();
    readData(userAccounts, folderPath);


    folderPath = "GoldPawning/header.csv";
    ifstream file(folderPath);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }
    string line;
    if (getline(file, line)) {
        cout << "\t  ";
        // Split the line into columns
        istringstream tokenStream(line);
        string token;

        for (int i = 0; i < 3; ++i) {
                // Skip the first 3 columns
                if (getline(tokenStream, token, ',')) {
                    continue;
                }
            }
            for (int i = 3; getline(tokenStream, token, ','); ++i) {
                // Increase the width of the 8th column
                if (i == 8) {
                    cout << setw(25) << left << token;
                } else if(i == 3) {
                    cout << setw(24) << left << token;
                } else {
                    cout << setw(15) << left << token;
                }
            }
        cout << endl << endl;
    }
    file.close();

    for(int i = 0 ; i < userAccounts.size(); i++) {
        string filename = userAccounts[i][0];
        string folderPath = "GoldPawning/" + filename + ".csv";

        ifstream file(folderPath);

        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return;
        }
        // Read and output data rows
        string line;
        if (getline(file, line)) {
            cout << "\t  ";
            // Split the line into columns
            istringstream tokenStream(line);
            string token;
            for (int i = 0; i < 3; ++i) {
                // Skip the first 3 columns
                if (getline(tokenStream, token, ',')) {
                    continue;
                }
            }
            for (int i = 3; getline(tokenStream, token, ','); ++i) {
                // Increase the width of the 8th column
                if (i == 8) {
                    cout << setw(25) << left << token;
                } else if(i == 3) {
                    cout << setw(24) << left << token;
                } else {
                    cout << setw(15) << left << token;
                }
            }
            cout << endl << endl;
        }
        // Close the file
        file.close();
    }

    cout << endl << endl;
    cout << "\t\t\t\t  " << "Press any key to continue" << endl;
    _getch();
    adminMenu();
}

string generate_username() {
    while(true) {
        string accNum;
        string folderPath = "login/logindetails.csv";
        userAccounts.clear();
        userAccounts.shrink_to_fit();
        readData(userAccounts, folderPath);
        int size = (userAccounts.size()) - 1;
        string last = userAccounts[size][0];
        int value = stoi(last);
        int num1 = value + 1;

        accNum = "00" + to_string(num1);

        if (abs(num1) >= 10 && abs(num1) <= 99) {
            accNum = "0" + to_string(num1);
        } else if(abs(num1) >= 100 && abs(num1) <= 999) {
            accNum = to_string(num1);
        }

        return accNum;

    }
}

void edit_rate(string query) {
    int choice;
    string newrate;
    system("cls");
    cout << "\t\t\t\t  " << "           _____  __  __ _____ _   _      __  __ ______ _   _ _    _ \n";
    cout << "\t\t\t\t  " << "     /\\   |  __ \\|  \\/  |_   _| \\ | |    |  \\/  |  ____| \\ | | |  | |\n";
    cout << "\t\t\t\t  " << "    /  \\  | |  | | \\  / | | | |  \\| |    | \\  / | |__  |  \\| | |  | |\n";
    cout << "\t\t\t\t  " << "   / /\\ \\ | |  | | |\\/| | | | | . ` |    | |\\/| |  __| | . ` | |  | |\n";
    cout << "\t\t\t\t  " << "  / ____ \\| |__| | |  | |_| |_| |\\  |    | |  | | |____| |\\  | |__| |\n";
    cout << "\t\t\t\t  " << " /_/    \\_\\_____/|_|  |_|_____|_| \\_|    |_|  |_|______|_| \\_|\\____/\n";
    cout << endl << endl << endl << endl << endl;

    while(true) {
        cout << "\t\t\t\t  " << "[1] 6 Months" << endl;
        cout << "\t\t\t\t  " << "[2] 12 Months" << endl;
        cout << "\t\t\t\t  " << "[3] 18 Months" << endl;
        cout << "\t\t\t\t  " << "[4] 24 Months" << endl;
        cout << "\t\t\t\t  " << "[5] 48 Months" << endl;
        cout << "\t\t\t\t  " << "[6]Back" << endl;
        cout << endl << endl;
        cout << "\t\t\t\t  " << "Input : ";
        cin >> choice;
        cout << endl;

        if(choice <= 5 && choice > 0) {
            cout << "\t\t\t\t  " << "Enter New Rate : ";
            cin >> newrate;
            Rates[choice + 1][1] = newrate;

            string folderPath = "Rates/" + query +".csv";
            updateData(Rates, folderPath);
            Rates.clear();
            Rates.shrink_to_fit();

            cout << endl << endl;
            cout << "\t\t\t\t  " << "Rate changed Successfully!" << endl;
            cout << endl;
            cout << "\t\t\t\t  " << "Press any key to continue" << endl;
            _getch();
            if (query == "loan") {
                loan_rates();
            } else {
                pawn_rates();
            }
        } else if(choice == 6) {
            if (query == "loan") {
                loan_rates();
            } else {
                pawn_rates();
            }
        } else {
            continue;
        }
    }
}

