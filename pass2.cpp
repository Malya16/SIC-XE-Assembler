#include<iostream>
#include<vector>
#include <string>
#include<fstream>
#include <iomanip>
#include<unordered_map>
#include<algorithm>

using namespace std;

unordered_map<string, string>OPTAB={
    {"ADD","18"},    // Add
    {"AND","40"},    // And
    {"COMP","28"},   // Compare
    {"DIV","24"},    // Divide
    {"J","3C"},      // Jump
    {"JEQ","30"},    // Jump if equal to zero
    {"JGT","34"},    // Jump if greater than
    {"JLT","38"},    // Jump if less than
    {"JSUB","48"},   // Jump to subroutine
    {"LDA","00"},    // Load accumulator
    {"LDB","68"},    // Load B register
    {"LDCH","50"},   // Load character
    {"LDF","70"},    // Load F register
    {"LDL","08"},    // Load L register
    {"LDS","6C"},    // Load S register
    {"LDT","74"},    // Load T register
    {"LDX","04"},    // Load X register
    {"MUL","20"},    // Multiply
    {"OR","44"},     // Or
    {"RD","D8"},     // Read
    {"RSUB","4C"},   // Return from subroutine
    {"STA","0C"},    // Store accumulator
    {"STB","78"},    // Store B register
    {"STCH","54"},   // Store character
    {"STF","80"},    // Store F register
    {"STL","14"},    // Store L register
    {"STS","7C"},    // Store S register
    {"STSW","E8"},   // Store status word
    {"STT","84"},    // Store T register
    {"STX","10"},    // Store X register
    {"SUB","1C"},    // Subtract
    {"TD","E0"},     // Test device
    {"TIX","2C"},    // Test and increment index
    {"WD","DC"}      // Write
};

void create_symtab(vector<pair<string,int>>&symtab)
{
    ifstream inFile("Symtab.txt");
    string label,add;
    while(inFile>>label>>add)
    {
        symtab.push_back({label,stoi(add,nullptr,16)});
    }
    inFile.close();
}


int findAddress(vector<pair<string, int>> symtab, string operand) {
    operand.erase(operand.find_last_not_of(" \n\r\t")+1);
    auto it=find_if(symtab.begin(),symtab.end(),[&operand](const pair<string,int>& x) 
    {
        return x.first==operand;
    });
    if(it!=symtab.end()) {
        return it->second;
    }
    return -1;
}

void parseLine(string &line,string &label,string &instruction,string &operand)
{
    int l=line.length(),i=0;
    while(i<l&&line[i]!=' ')
    {
        label+=line[i];
        i++;
    }
    while(i<l&&line[i]==' ')
    i++;
    while(i<l&&line[i]!=' ')
    {
        instruction+=line[i];
        i++;
    }
    while(i<l&&line[i]==' ')
    i++;
    while(i<l)
    {
        operand+=line[i];
        i++;
    }
}

void pass2(ifstream &inputFile,vector<pair<string,int>> &symtab,int &e)
{
    string line;
    int loc_ctr=0;
    ofstream outputFile("Output.obj");
    if (!outputFile) {
        cerr<<"Error opening output file\n"<<endl;
        return ;
    }
    while(getline(inputFile,line))
    {
        string label,instruction,operand;
        parseLine(line,label,instruction,operand);
        operand.erase(operand.find_last_not_of(" \n\r\t")+1);
        instruction.erase(instruction.find_last_not_of(" \n\r\t")+1);
        if(instruction=="RSUB")
        {
            outputFile<<"4C0000"<<endl;
            continue;
        }
        if(instruction=="START")
        {
            loc_ctr=stoi(operand);
            continue;
        }
        if(instruction=="END")
        break;
        if(OPTAB.find(instruction)!=OPTAB.end())
        {
            string opcode=OPTAB[instruction];
            bool i=false;
            int l=operand.length(),add;
            if(operand.length()>2&&operand[l-1]=='X'&&operand[l-2]==',')
            {
                i=true;
                operand=operand.substr(0,operand.size()-2);
            }
            add=findAddress(symtab,operand);
            if(add==-1)
            {
                e=1;
                cout<<operand<<endl;
                return;
            }
            if(i)
            add|=0x8000;
            outputFile<<opcode<<setw(4)<<setfill('0')<<hex<<uppercase<<add<<endl;
        }
        else if(instruction=="WORD")
        outputFile<<setw(6)<<setfill('0')<<hex<<uppercase<<stoi(operand)<<endl;
        else if(instruction=="BYTE")
        {
            if(operand[0]=='X')
            outputFile<<operand.substr(2,operand.size()-3)<<endl;
            else if(operand[0]=='C')
            {
                for (int j=2;j<operand.size()-1;j++)
                {
                    char currentChar=operand[j];
                    outputFile<<setw(2)<<setfill('0')<<hex<<uppercase<<(int)(unsigned char)currentChar<<endl;
                }
            }
        }
        else if(instruction=="RESW")
        outputFile<<"RESW_"<<operand<<endl;
        else if(instruction=="RESB")
        outputFile<<"RESB_"<<operand<<endl;
        else
        {
            e=1;
            cout<<"Unidentified Symbol:"<<instruction<<endl;
            return ;
        }
    }
}

int main()
{
    ifstream inputFile("Input.txt");
    int e=0;
    if(!inputFile)
    {
        cout<<"Error in opening File\n";
        return 1;
    }
    vector<pair<string,int>>symtab;
    create_symtab(symtab);
    pass2(inputFile,symtab,e);
    if(e==1)
    {
        cout<<"ERROR\n";
        return 1;
    }
    inputFile.close();
    return 0;
}