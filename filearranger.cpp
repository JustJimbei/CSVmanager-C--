#include <iostream>
#include <algorithm>
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


void arrangedNames(string Names[], string Emails[], string PhoneNumber[], int Limiter);
void removeCSVRow(string filename, int rowToRemove);
string extractNumber(string phoneNum);

int main()
{
    
    string names[100], emails[100], phoneNumbers[100], lines;
    const int Max = 100;
    string realPhoneNumbers[Max];
    int counter = 0;

    fstream fileInfo("random_contacts.csv");
    getline(fileInfo, lines);

    if (!fileInfo.is_open())
    {
        cerr << RED << setw(15) << "Error!\n" << RESET;
        return 1;
    }


    cout << CYAN << "Fetching File Data...\n" << RESET;
    this_thread::sleep_for(chrono::seconds(2));
    cout << MAGENTA << "Printing Data\n" << RESET;
    this_thread::sleep_for(chrono::seconds(3));

    cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
    cout << left << setw(2) << "|" << BLUE << setw(ID_W) << "No." << RESET << setw(2)
         << "|" << CYAN << setw(NAME_W) << "Names" << RESET << setw(2)
         << "|" << MAGENTA << setw(EMAIL_W) << "Emails" << RESET << setw(2)
         << "|" << GREEN << setw(PHONENUMBER_W) << "Phone Number" << RESET
         << "|\n";
    cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
    
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
             << GREEN << setw(PHONENUMBER_W) << realPhoneNumbers[i] << RESET << "|\n";
    }

    cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
    
    fileInfo.close();


    fstream newInfoFile;
    newInfoFile.open("NameArrangedDone.csv", ios::in | ios::out);


    arrangedNames(names, emails, realPhoneNumbers, counter);

    newInfoFile << "Names" << ',' << "Emails" << "," << "Phone Numbers\n";


    if (newInfoFile.is_open())
    {
        for (int i = 0; i < counter; i++)
        {
            newInfoFile << names[i] << "," << emails[i] << "," << realPhoneNumbers[i] << endl;
        }
        
    }

    bool Exit = true;
    while (Exit)
    {
        char answer; 
        cout << BLUE << "\n(P) to print the new data\n";
        cout << BLUE << "(R) to remove a row(n) in data\n";
        cout << BLUE << "(C) to add another data\n";
        cout << BLUE << "(E) to exit\n";
        cout << "Choice: ";
        cin >> answer;
        cout << BLUE << "\n" << RESET;

        char upperChar = toupper(answer);

        if (upperChar == 'P')
        {
            cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
            cout << left << setw(2) << "|" << BLUE << setw(ID_W) << "No." << RESET << setw(2)
            << "|" << CYAN << setw(NAME_W) << "Names" << RESET << setw(2)
            << "|" << MAGENTA << setw(EMAIL_W) << "Emails" << RESET << setw(2)
            << "|" << GREEN << setw(PHONENUMBER_W) << "Phone Number" << RESET
            << "|\n";
            cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;

            for (int i = 0; i < counter; i++)
            {
                cout << left << setw(2) << "|"
                    << BLUE << setw(ID_W) << (i + 1) << RESET << setw(2) << "|"
                    << CYAN << setw(NAME_W) << names[i] << RESET << setw(2) << "|"
                    << MAGENTA << setw(EMAIL_W) << emails[i] << RESET << setw(2) << "|"
                    << GREEN << setw(PHONENUMBER_W) << realPhoneNumbers[i] << RESET << "|\n";
            }

            cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
        }
        
        else if (upperChar == 'R')
        {
            int rowNumber;
            cout << BLUE << "Enter the row number to remove (1-" << counter << "): ";
            cin >> rowNumber;
            cout << RESET << "\n";

            removeCSVRow("NameArrangedDone.csv", rowNumber);

            cout << CYAN << "Removing Row...\n";
            this_thread::sleep_for(chrono::seconds(2));
            cout << "Row Successfully Removed.\n" << RESET;
        }

        /*else if (upperChar == 'R')
        {

        }*/

        else if (upperChar == 'E')
        {
            cout << BLUE << "Existing Program\n\n" << RESET;
            this_thread::sleep_for(chrono::seconds(3));
            Exit = false;
        }

        else
        {
            cerr << RED << "Error!\n\n" << RESET;
        }

    }


    newInfoFile.close();
    newInfoFile.open("NameArrangedDone.csv", ios::in | ios::trunc);

    cout << GREEN << "Saving Data...\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << GREEN << "Data Successfully Saved.\n";
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
