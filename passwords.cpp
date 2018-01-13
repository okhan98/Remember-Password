#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

#include <cstdlib>
#include <cctype>

using namespace std;

struct Account
{
  string name;
  string password;
  Account* next;
};

void menu();
void table();
string toLowerCase(string aCppString);
bool addAccount(Account*&, Account*&);
void removeAccount(Account*&, Account*&);
void listAccount(Account*);
string encode(string); 
string decode(string);
void deallocateNodes(Account*);

int main()
{
  
  // Variable Declaration
  Account* firstAccount = 0;   
  Account* lastAccount = 0;
  string buf;
  char choice;
  
  // Serializing Up
  ifstream fin;
  fin.open("a.txt");
  if (fin.good())
  {
    while(true)
    {
      getline(fin, buf); // could be the first attribute OR the "end-of-file" marker.
      if (buf == "EOF") break;

      // use buf as the first attribute
      Account* temp = new Account;
      temp->name = decode(buf);
      
      // read the remaining attributes
      getline(fin,temp->password);
      temp->password = decode(temp->password);
      // link to the end of the list
      temp->next = 0;
      if (lastAccount) lastAccount->next = temp;
      else firstAccount = temp;
      lastAccount = temp;
    }
  }
  fin.close();
  
  // Main Loop
  while(true)
  {
    menu();
    cout << "\nYour Choice: ";
    cin >> choice;
    cin.ignore(1000,10);
    choice = toupper(choice);
    
    if(choice == 'A')
    {
      addAccount(firstAccount,lastAccount);
    }
    
    if(choice == 'E')
    {
      removeAccount(firstAccount, lastAccount);
    }
    
    if(choice == 'L')
    {
      listAccount(firstAccount);
    }
    
    if(choice == 'Q')
    {
      break;
    }
  }
  
  // Serializing down
  ofstream fout;
  fout.open("a.txt");
  for (Account* p = firstAccount; p; p = p->next)
  {
    // output each attribute on its own line
    fout << encode(p->name) << endl;
    fout << encode(p->password) << endl;
  }
  
  fout << "EOF\n"; // mark the end of the list
  fout.close();
  
  // Deallocates nodes
  deallocateNodes(firstAccount);
}

void menu()
{
  cout << "MENU" << endl;
  cout << "A Add a password" << endl;
  cout << "E rEmove a password" << endl;
  cout << "L List all passwords" << endl;
  cout << "Q Quit" << endl;
}

void table()
{
  cout << "#  Account              Password" << endl;
  cout << "-- -------------------- --------" << endl;
}

string toLowerCase(string aCppString)
{
  int n = aCppString.length( );
  for (int i = 0; i < n; i++)
  aCppString[i] = tolower(aCppString[i]);
  return aCppString;
}

bool addAccount(Account*& head, Account*& tail) 
{   
  string buf;   
  cout << "Enter the website[q to quit]: ";   
  getline(cin, buf);   
  
  if (buf == "q") return true;
  
  // #1 create a node   
  Account* a = new Account;
  
  // #2 input the attrbutes   
  a->name = buf;
  cout << "Enter the password for " << a->name << ": ";
  getline(cin, buf);
  a->password = buf;
  
  cout << endl;
  
  // #3   
  Account* p, *prev; 
  // step 4 will need to know the "previous node"   
  for (p = head, prev = 0; p; prev = p, p = p->next)     
  if (toLowerCase(a->name) < toLowerCase(p->name)) break; 


  
  // #4   
  a->next = p;  // places the node at p after the new node -- works even if p is zero   
  if (prev) prev->next = a;  // places the new node after the node at prev, if there is one...   
  else head = a;  // ...or makes it the new head if prev is zero   if (p == 0) tail = a; // in case there's a tail pointer
  
  return false; 
}

void removeAccount(Account*& head, Account*& tail)
{
   // Counting Accounts
      string buf;
      int index = 1;
      int count = 0;
       for(Account* p = head; p; p = p->next)
         count++;
      
      // Asking User Which Account to remove 
      int accountNum;
      cout << "Which password do you want to remove [1-" << count << "]: ";
      cin >> buf; accountNum = atoi(buf.c_str());
      cout << endl;
      
      Account* p, *prev; // declare above loop so that these survive below the loop
      for (p = head, prev = 0; p; prev = p, p = p->next, index++)
      if (index == accountNum) break;
    
     if (prev) prev->next = p->next; // skips over the node at p
     else head = p->next; // there's a new head, possibly zero
     delete p; // we're done with this node
     if (p == tail) tail = prev; // that's the 1st if...
}

void listAccount(Account* head)
{      
  table();
  int index = 1;
  for (Account* p = head; p; p = p->next,index++)
  {
    cout.width(2);
    cout << index;
    cout.width(21);
    cout << p->name;
    cout.width(9);
    cout << p->password << endl;
  }
  cout << endl;
}

string encode(string aCppString)
{
  int n = aCppString.length();
  for (int i = 0; i < n; aCppString[i]++, i++);
  return aCppString;
}

string decode(string aCppString)
{
  int n = aCppString.length();
  for (int i = 0; i < n; aCppString[i]--, i++);
  return aCppString;
}

void deallocateNodes(Account* head)
{
  while (head) // keep going as long as the list is not empty
  {
    Account* p = head; // hold onto head node...
    head = head->next; // ...because we're linking over it to its next
    delete p; // now deallocate the old head
  }
}