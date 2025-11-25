#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <cctype>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#define ID_W            4
#define NAME_W          19
#define EMAIL_W         34
#define PHONENUMBER_W   14

using namespace std;

const int MAX_COLUMNS = 50;

void arrangedNames(string Names[], string Emails[], string PhoneNumber[], int Limiter);
void removeCSVRow(string filename, int rowToRemove);
string extractNumber(string phoneNum);
string validateDigitsOnly(string prompt);

int main()
{
    bool Exit = true;
    while (Exit)
    {
        string names[100], emails[100], phoneNumbers[100], lines;
        const int Max = 100;
        string realPhoneNumbers[Max];
        
        // Dynamic column storage
        string columnHeaders[MAX_COLUMNS];
        string columnData[100][MAX_COLUMNS];
        int totalColumns = 3; // Start with 3 columns (Names, Emails, Phone Numbers)
        int counter = 0;

        fstream fileInfo("NameArrangedDone.csv");
        getline(fileInfo, lines);

        if (!fileInfo.is_open())
        {
            cerr << RED << setw(15) << "Error!\n" << RESET;
            return 1;
        }

        // Parse header to get all column names
        stringstream headerStream(lines);
        string headerItem;
        int headerCount = 0;
        while (getline(headerStream, headerItem, ',') && headerCount < MAX_COLUMNS)
        {
            columnHeaders[headerCount] = headerItem;
            headerCount++;
        }
        totalColumns = headerCount;

        cout << CYAN << "Fetching File Data...\n" << RESET;
        this_thread::sleep_for(chrono::seconds(2));
        cout << MAGENTA << "Printing Data\n" << RESET;
        this_thread::sleep_for(chrono::seconds(3));

        // Calculate dynamic table width
        int tableWidth = 2 + ID_W + 2 + 2 + NAME_W + 2 + 2 + EMAIL_W + 2 + 2 + PHONENUMBER_W + 1;
        for (int i = 3; i < totalColumns; i++)
        {
            tableWidth += 16; // 15 for column + 1 for separator
        }
        
        // Print top border
        cout << YELLOW;
        for (int i = 0; i < tableWidth; i++) cout << "-";
        cout << RESET << endl;
        
        cout << left << setw(2) << "|" << BLUE << setw(ID_W) << "No." << RESET << setw(2)
            << "|" << CYAN << setw(NAME_W) << "Names" << RESET << setw(2)
            << "|" << MAGENTA << setw(EMAIL_W) << "Emails" << RESET << setw(2)
            << "|" << GREEN << setw(PHONENUMBER_W) << "Phone Number" << RESET
            << "|";
        
        // Print additional column headers
        for (int i = 3; i < totalColumns; i++)
        {
            cout << setw(15) << columnHeaders[i] << "|";
        }
        cout << "\n";
        
        // Print separator
        cout << YELLOW;
        for (int i = 0; i < tableWidth; i++) cout << "-";
        cout << RESET << endl;
        
        while (getline(fileInfo, lines))
        {
            if (lines.empty()) continue;
            
            stringstream ss(lines);
            string name, email, phonenumber;

            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, phonenumber, ',');

            names[counter] = name;
            emails[counter] = email;
            phoneNumbers[counter] = phonenumber;

            // Read additional columns
            for (int i = 3; i < totalColumns; i++)
            {
                string extraData;
                getline(ss, extraData, ',');
                columnData[counter][i] = extraData;
            }

            string extractedDigit = extractNumber(phonenumber);

            if (extractedDigit.empty())
            {
                cerr << RED << "Data Empty! Row\n" << counter << RESET;
                realPhoneNumbers[counter] = "0";
            }
            
            else
            {
                try
                {
                    realPhoneNumbers[counter] = (extractedDigit);
                }
                catch (const std::exception &e)
                {
                    cerr << RED << "Data Error! Invalid Number format.\n" << RESET;
                    realPhoneNumbers[counter] = "0";
                    return 0;
                }
            }

            counter++;
        }

        for (int i = 0; i < counter; i++)
        {
            cout << left << setw(2) << "|"
                << BLUE << setw(ID_W) << (i + 1) << RESET << setw(2) << "|"
                << CYAN << setw(NAME_W) << names[i] << RESET << setw(2) << "|"
                << MAGENTA << setw(EMAIL_W) << emails[i] << RESET << setw(2) << "|"
                << GREEN << setw(PHONENUMBER_W) << realPhoneNumbers[i] << RESET << "|";
            
            // Print additional column data
            for (int j = 3; j < totalColumns; j++)
            {
                cout << setw(15) << columnData[i][j] << "|";
            }
            cout << "\n";
        }

        // Print bottom border
        cout << YELLOW;
        for (int i = 0; i < tableWidth; i++) cout << "-";
        cout << RESET << endl;
        
        fileInfo.close();

        fstream newInfoFile;
        newInfoFile.open("NameArrangedDone.csv", ios::in | ios::out);

        arrangedNames(names, emails, realPhoneNumbers, counter);

        // Write header with all columns
        newInfoFile << "Names" << ',' << "Emails" << "," << "Phone Numbers";
        for (int i = 3; i < totalColumns; i++)
        {
            newInfoFile << "," << columnHeaders[i];
        }
        newInfoFile << "\n";

        if (newInfoFile.is_open())
        {
            for (int i = 0; i < counter; i++)
            {
                newInfoFile << names[i] << "," << emails[i] << "," << realPhoneNumbers[i];
                for (int j = 3; j < totalColumns; j++)
                {
                    newInfoFile << "," << columnData[i][j];
                }
                newInfoFile << endl;
            }
        }

        char answer; 
        cout << BLUE << "\n(P) to print the new data\n";
        cout << BLUE << "(D) to delete a row(n) in data\n";
        cout << BLUE << "(C) to add another data\n";
        cout << BLUE << "(R) to add a new data column\n";
        cout << BLUE << "(E) to exit\n";
        cout << "Choice: ";
        cin >> answer;
        cout << BLUE << "\n" << RESET;

        char upperChar = toupper(answer);

        if (upperChar == 'P')
        {
            // Calculate dynamic table width for print
            int tableWidth = 2 + ID_W + 2 + 2 + NAME_W + 2 + 2 + EMAIL_W + 2 + 2 + PHONENUMBER_W + 1;
            for (int i = 3; i < totalColumns; i++)
            {
                tableWidth += 16;
            }
            
            // Print top border
            cout << YELLOW;
            for (int i = 0; i < tableWidth; i++) cout << "-";
            cout << RESET << endl;
            
            cout << left << setw(2) << "|" << BLUE << setw(ID_W) << "No." << RESET << setw(2)
                << "|" << CYAN << setw(NAME_W) << "Names" << RESET << setw(2)
                << "|" << MAGENTA << setw(EMAIL_W) << "Emails" << RESET << setw(2)
                << "|" << GREEN << setw(PHONENUMBER_W) << "Phone Number" << RESET
                << "|";
            
            for (int i = 3; i < totalColumns; i++)
            {
                cout << setw(15) << columnHeaders[i] << "|";
            }
            cout << "\n";
            
            // Print separator
            cout << YELLOW;
            for (int i = 0; i < tableWidth; i++) cout << "-";
            cout << RESET << endl;

            for (int i = 0; i < counter; i++)
            {
                cout << left << setw(2) << "|"
                    << BLUE << setw(ID_W) << (i + 1) << RESET << setw(2) << "|"
                    << CYAN << setw(NAME_W) << names[i] << RESET << setw(2) << "|"
                    << MAGENTA << setw(EMAIL_W) << emails[i] << RESET << setw(2) << "|"
                    << GREEN << setw(PHONENUMBER_W) << realPhoneNumbers[i] << RESET << "|";
                
                for (int j = 3; j < totalColumns; j++)
                {
                    cout << setw(15) << columnData[i][j] << "|";
                }
                cout << "\n";
            }

            // Print bottom border
            cout << YELLOW;
            for (int i = 0; i < tableWidth; i++) cout << "-";
            cout << RESET << endl;
        }
            
        else if (upperChar == 'D')
        {
            int rowNumber;
            cout << BLUE << "Enter the row number to delete (1-" << counter << "): ";
            cin >> rowNumber;
            cout << RESET << "\n";

            removeCSVRow("NameArrangedDone.csv", rowNumber);

            cout << CYAN << "Removing Row...\n";
            this_thread::sleep_for(chrono::seconds(2));
            cout << "Row Successfully Removed.\n" << RESET;
        }

        else if (upperChar == 'C')
        {
            cout << BLUE << "Enter the Name, Email and Phone Number\n";

            cin.ignore();

            cout << "Name: ";
            getline(cin, names[counter]);

            cout << "Email: ";
            cin >> emails[counter];

            realPhoneNumbers[counter] = validateDigitsOnly("Phone Number: ");

            // Initialize new row's extra columns to empty
            for (int i = 3; i < totalColumns; i++)
            {
                columnData[counter][i] = "";
            }

            counter++;

            fstream newInfoFile;  
            newInfoFile.open("NameArrangedDone.csv", ios::out);
                
            if (newInfoFile.is_open())
            {
                newInfoFile << "Names" << ',' << "Emails" << "," << "Phone Numbers";
                for (int i = 3; i < totalColumns; i++)
                {
                    newInfoFile << "," << columnHeaders[i];
                }
                newInfoFile << "\n";
                
                for (int i = 0; i < counter; i++)
                {
                    newInfoFile << names[i] << "," << emails[i] << "," << realPhoneNumbers[i];
                    for (int j = 3; j < totalColumns; j++)
                    {
                        newInfoFile << "," << columnData[i][j];
                    }
                    newInfoFile << endl;
                }
            }

            newInfoFile.close();
        }

        else if (upperChar == 'E')
        {
            cout << BLUE << "Existing Program\n\n" << RESET;
            this_thread::sleep_for(chrono::seconds(3));
            Exit = false;
        }

        else if (upperChar == 'R')
        {
            if (totalColumns >= MAX_COLUMNS)
            {
                cerr << RED << "Maximum number of columns reached!\n" << RESET;
            }
            else
            {
                string newColumnName;
                cout << BLUE << "Enter the new column name\n";
                cout << "Column Name: ";
                cin >> newColumnName;
                cout << RESET;

                // Add new column header
                columnHeaders[totalColumns] = newColumnName;

                // Initialize all rows with empty data for new column
                for (int i = 0; i < counter; i++)
                {
                    columnData[i][totalColumns] = "";
                }

                totalColumns++;

                fstream newFile;
                newFile.open("NameArrangedDone.csv", ios::out);

                if (newFile.is_open())
                {
                    // Write header with new column
                    newFile << "Names" << ',' << "Emails" << "," << "Phone Numbers";
                    for (int i = 3; i < totalColumns; i++)
                    {
                        newFile << "," << columnHeaders[i];
                    }
                    newFile << "\n";

                    // Write all data rows with new column
                    for (int i = 0; i < counter; i++)
                    {
                        newFile << names[i] << "," << emails[i] << "," << realPhoneNumbers[i];
                        for (int j = 3; j < totalColumns; j++)
                        {
                            newFile << "," << columnData[i][j];
                        }
                        newFile << endl;
                    }
                }
                
                newFile.close();
                
                cout << GREEN << "Column '" << newColumnName << "' added successfully!\n" << RESET;
            }
        }

        else
        {
            cerr << RED << "Error!\n\n" << RESET;
        }

        newInfoFile.close();
        newInfoFile.open("NameArrangedDone.csv", ios::in | ios::out);

        cout << GREEN << "Saving Data...\n";
        this_thread::sleep_for(chrono::seconds(2));
        cout << GREEN << "Data Successfully Saved.\n";
    }

    this_thread::sleep_for(chrono::seconds(2));
    cout << YELLOW << "Program terminated.\n" << RESET;

    return 0;
}

void arrangedNames(string Names[], string Emails[], string PhoneNumber[], int Limiter)
{
    for (int i = 0; i < Limiter; i++)
    {
        for (int j = 0; j < Limiter - i - 1; j++)
        {
            if (Names[j] > Names[j + 1])
            {
                string tempNames = Names[j];
                Names[j] = Names[j + 1];
                Names[j + 1] = tempNames;

                string tempEmails = Emails[j];
                Emails[j] = Emails[j + 1];
                Emails[j + 1] = tempEmails;

                string tempPhone = PhoneNumber[j];
                PhoneNumber[j] = PhoneNumber[j + 1];
                PhoneNumber[j + 1] = tempPhone; 
            }
        }
    }
}

void removeCSVRow(string filename, int rowToRemove)
{
    fstream file(filename, ios::in);
    if (!file.is_open())
    {
        cerr << RED << "Error opening file for row removal.\n" << RESET;
        return;
    }

    string content = "";
    string line;
    int currentRow = 0;

    while (getline(file, line))
    {
        currentRow++;
        if (currentRow == rowToRemove + 1) 
            continue;
        content += line + "\n";
    }
    file.close();

    file.open(filename, ios::out | ios::trunc);
    file << content;
    file.close();
}

string extractNumber(string phoneNum)
{
    string Number = "";
    for (char C : phoneNum)
    {
        if (isdigit(C))
        {
            Number = Number + C;
        }
    }
    return Number;
}

string validateDigitsOnly(string prompt)
{
    string userDigit;

    while (true)
    {
        cout << prompt;
        cin >> userDigit;

        bool valid = true;

        for (char C : userDigit)
        {
            if(!isdigit(C))
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            return userDigit;
        }

        cout << RED << "Invalid Input\n" << RESET << BLUE;
    }
}