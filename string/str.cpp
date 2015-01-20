#include <string>
#include <iostream>

using namespace std ;

void trueFalse(int x)
{
  cout << (x? "True": "False") << endl;
}

int main()
{
   string S1="ABC";
   string S2="ABC";
   string S3="DEF";
   string S4; //This specifies an empty initial-controlled sequence.
   char CP1[]="abc";
   char CP2[]="DEF";
   char *CP3 = NULL;

   cout << "S1 is " << S1 << endl;
   cout << "S2 is " << S2 << endl;
   cout << "S3 is " << S3 << endl;
   cout << "S4 is" << S4 << endl;
   cout << "CP1 is " << CP1 << endl;
   cout << "CP2 is " << CP2 << endl;

   cout << "S1==CP1 returned ";
   trueFalse(S1==CP1);  // False    (calls function 1)

   cout << "S1==CP2 returned ";
   trueFalse(S1==CP2);  // False   (calls function 1)

   cout << "CP1==S1 returned ";
   trueFalse(CP1==S1);  // False    (calls function 2)

   cout << "CP2==S1 returned ";
   trueFalse(CP2==S1);  // False   (calls function 2)

   cout << "S1==S2 returned ";
   trueFalse(S1==S2);   // True    (calls function 3)

   cout << "S1==S3 returned ";
   trueFalse(S1==S3);   // False   (calls function 3)

   cout << "S1==S4 returned ";
   trueFalse(S1==S4);   // False   (calls function 3)

   // Following use of the operator will cause the program to
   // crash since CP3 is NULL.
   cout << "S1==CP3 returned ";
   trueFalse(S1==CP3);
}
