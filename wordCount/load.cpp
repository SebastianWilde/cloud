#include <iostream>
#include <fstream>
using namespace std;

int main () {

     string saveload;
     
     cout << "Welcome to the program!\n" << endl;
     cout << "---------------------------" << endl;
     cout << "| save - Save to a file   |" << endl;
     cout << "| load - Load from a file |" << endl;
     cout << "---------------------------\n" << endl;
     
     getline(cin,saveload);

     if(saveload == "load") {

          ifstream loadFile;

          loadFile.open ("data2.txt", ifstream::in);

          cout << "The file contained: ";
          
          while (loadFile.good()){
                
               cout << (char) loadFile.get();

          }
          
          cout << "hye" << endl;
          
        loadFile.close();
  
     }
     
     if (saveload == "save") {
     
       string textToSave;
       cout << "Enter the string you want saved: " << endl;
       getline(cin,textToSave);

       ofstream saveFile ("Save.txt");

       saveFile << textToSave;

       saveFile.close();    
              
     }
     
     return 0;
}
