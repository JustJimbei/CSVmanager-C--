#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
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
#define AGE_W            6

using namespace std;

const int MAX_COLUMNS = 50;

void arrangedNames(vector <string> &Names, vector <string> &Emails, vector <string> &PhoneNumber, int low, int high);
void removeCSVRow(string filename, int rowToRemove);
string extractNumber(string phoneNum);
string validateDigitsOnly(string prompt);

int main()
{
    bool Exit = true;
    while (Exit)
    {
        vector <string> names;
        vector <string> emails;
        vector <string> phoneNumbers;
        string lines;

        vector <string> realPhoneNumbers;
        
        
        string columnHeaders[MAX_COLUMNS];

        vector <vector <string>> columnData;

        int totalColumns = 3; 
        int counter = 0;

        fstream fileInfo("NameArrangedDone.csv");
        
        if (!fileInfo.is_open())
        {
            cerr << RED << setw(15) << "Error!\n" << RESET;
            return 1;
        }
        
        getline(fileInfo, lines);
        
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

        
        int tableWidth = 2 + ID_W + 2 + 2 + NAME_W + 2 + 2 + EMAIL_W + 2 + 2 + PHONENUMBER_W + 1;
        for (int i = 3; i < totalColumns; i++)
        {
            tableWidth += 16; 
        }
        
        
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
            if (columnHeaders[i] == "Ages")
                cout << YELLOW << setw(AGE_W) << columnHeaders[i] << RESET << "|";
            else if (columnHeaders[i] == "Sex")
                cout << CYAN << setw(8) << columnHeaders[i] << RESET << "|";
            else
                cout << MAGENTA << setw(15) << columnHeaders[i] << RESET << "|";
        }
        cout << "\n";
        
        
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

            // names[counter] = name;
            names.push_back(name);
            // emails[counter] = email;
            emails.push_back(email);
            // phoneNumbers[counter] = phonenumber; 
            phoneNumbers.push_back(phonenumber);
            

            vector <string> rowExtras(MAX_COLUMNS);

            for (int i = 3; i < totalColumns; i++)
            {
                string extraData;
                getline(ss, extraData, ',');
                rowExtras[i] = extraData;
            }
            columnData.push_back(rowExtras);

            string extractedDigit = extractNumber(phonenumber);

            if (extractedDigit.empty())
            {
                cerr << RED << "Data Empty! Row\n" << counter << RESET;
                realPhoneNumbers.push_back("0");
            }
            
            else
            {
                try
                {
                    realPhoneNumbers.push_back(extractedDigit);
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
            
            
            for (int j = 3; j < totalColumns; j++)
            {
                if (columnHeaders[j] == "Ages")
                {
                    cout << YELLOW << setw(AGE_W) << columnData[i][j] << RESET << "|";
                }
                else if (columnHeaders[j] == "Sex")
                {
                    cout << CYAN << setw(8) << columnData[i][j] << RESET << "|";
                }
                else
                {
                    cout << MAGENTA << setw(15) << columnData[i][j] << RESET << "|";
                }
            }
            cout << "\n";
        }

        
        cout << YELLOW;
        for (int i = 0; i < tableWidth; i++) cout << "-";
        cout << RESET << endl;
        
        fileInfo.close();

        arrangedNames(names, emails, realPhoneNumbers, 0, counter - 1); // having trouble with this part bro


        fstream newInfoFile;
        newInfoFile.open("NameArrangedDone.csv", ios::in | ios::trunc);

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
            
            int tableWidth = 2 + ID_W + 2 + 2 + NAME_W + 2 + 2 + EMAIL_W + 2 + 2 + PHONENUMBER_W + 1;
            for (int i = 3; i < totalColumns; i++)
            {
                tableWidth += 16;
            }
            
            
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

            // changed this part to dynamic(maybe we can change it to linkedlist nextime)
            string newName, newEmail;

            cout << "Name: ";
            getline(cin, newName);

            cout << "Email: ";
            cin >> newEmail;

            string newPhone = validateDigitsOnly("Phone Number: ");

            // we need to change to linkedlist nextime bro(Hate it)
            names.push_back(newName);
            emails.push_back(newEmail);
            realPhoneNumbers.push_back(newPhone);

            //Ill handle this part if transfer to linkedlist
            vector <string> emptyRow(MAX_COLUMNS, "");
            columnData.push_back(emptyRow);
            
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

                
                columnHeaders[totalColumns] = newColumnName;

                
                for (int i = 0; i < counter; i++)
                {
                    columnData[i][totalColumns] = "";
                }

                totalColumns++;

                fstream newFile;
                newFile.open("NameArrangedDone.csv", ios::out);

                if (newFile.is_open())
                {
                    
                    newFile << "Names" << ',' << "Emails" << "," << "Phone Numbers";
                    for (int i = 3; i < totalColumns; i++)
                    {
                        newFile << "," << columnHeaders[i];
                    }
                    newFile << "\n";

                    
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

// Ill change it to quicksort cause bubble is trash
void arrangedNames(vector <string> &Names, vector <string> &Emails, vector <string> &PhoneNumber, int low, int high)
{

    if (low < high)
    {
        string pivot = Names[high];
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (Names[j] <= pivot)
            {
                i++;
                swap(Names[i], Names[j]);
                swap(Emails[i], Emails[j]);
                swap(PhoneNumber[i], PhoneNumber[j]);
            }
        }

        swap(Names[i + 1], Names[high]);
        swap(Emails[i + 1], Emails[high]);
        swap(PhoneNumber[i + 1], PhoneNumber[high]);

        int pivotIndex = i + 1;
        
        arrangedNames(Names, Emails, PhoneNumber, low, pivotIndex - 1);
        arrangedNames(Names, Emails, PhoneNumber, pivotIndex + 1, high);
    }
    
    /* for (int i = 0; i < Limiter; i++)
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
    } */
   
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