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


    cout << CYAN << "\nFetching File Data...\n" << RESET;
    this_thread::sleep_for(chrono::seconds(2));
    cout << MAGENTA << "\nPrinting Data\n" << RESET;
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
    newInfoFile.open("NameArrangedDone.csv", ios::out);


    arrangedNames(names, emails, realPhoneNumbers, counter);

    newInfoFile << "Names" << ',' << "Emails" << "," << "Phone Numbers\n";


    if (newInfoFile.is_open())
    {
        for (int i = 0; i < counter; i++)
        {
            newInfoFile << names[i] << "," << emails[i] << "," << realPhoneNumbers[i] << endl;
        }
        
    }

    newInfoFile.close();

    char answer; 
    cout << BLUE << "\nWould like to print the Arranged Data(y/n)? ";
    cin >> answer;
    cout << BLUE << "\n" << RESET;

    char upperChar = toupper(answer);

    if (upperChar == 'Y')
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
        
        cout << GREEN << "Saving Data...\n\n";
        this_thread::sleep_for(chrono::seconds(2));
        cout << GREEN << "Data Successfully Saved.\n\n";
        this_thread::sleep_for(chrono::seconds(2));
    }
    
    else if (upperChar == 'N')
    {
        cout << BLUE << "Existing Program\n\n" << RESET;
        this_thread::sleep_for(chrono::seconds(3));
    }

    else
    {
        cerr << RED << "Error!\n\n" << RESET;
    }


    cout << YELLOW << "Program terminated.\n\n" << RESET;

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
