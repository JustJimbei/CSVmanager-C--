#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

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
        cerr << RED << setw(15) << "Error!" << RESET << endl;
        return 1;
    }


    cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;
    cout << left << setw(2) << "|" << BLUE << setw(ID_W) << "No." << RESET << setw(2)
         << "|" << CYAN << setw(NAME_W) << "Names" << RESET << setw(2)
         << "|" << MAGENTA << setw(EMAIL_W) << "Emails" << RESET << setw(2)
         << "|" << GREEN << setw(PHONENUMBER_W) << "Phone Number" << RESET
         << "|" << endl;
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
            cerr << RED << "Data Empty! Row " << counter << RESET << endl;
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
                cerr << RED << "Data Error! Invalid Number format." << RESET << endl;
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
             << GREEN << setw(PHONENUMBER_W) << realPhoneNumbers[i] << RESET << "|"
             << endl;
    }

    cout << YELLOW << "--------------------------------------------------------------------------------" << RESET << endl;


    fileInfo.close();
    return 0;
}


void arrangedNames(string Names[], string Emails[], string PhoneNumber[], int Limiter)
{
    for (int i = 0; i < Limiter; i++)
    {
        for (int j = 0; j < Limiter - i - 1; j++)
        {
            if (Names[i] > Names[i + 1])
            {
                string tempNames = Names[i];
                Names[i] = Names[i + 1];
                Names[i + 1] = tempNames;

                string tempEmails = Emails[i];
                Emails[i] = Emails[i + 1];
                Emails[i + 1] = tempEmails;

                string tempPhone = PhoneNumber[i];
                PhoneNumber[i] = PhoneNumber[i + 1];
                PhoneNumber[i + 1] = tempPhone; 
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
