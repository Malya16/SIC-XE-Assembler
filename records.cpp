#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void parseLine(string &line,string&label,string&instruction,string&operand)
{
    int l=line.length();
    string temp="";
    bool foundLabel=false;
    for(int i=0;i<l;i++)
    {
        if(line[i]==' '&&!foundLabel&&label.empty())
        {
            label=temp;
            temp="";
            foundLabel=true;
            continue;
        }
        else if(line[i]==' '&&instruction.empty())
        {
            instruction=temp;
            temp="";
            continue;
        }
        else if(line[i]!=' ')
        temp+=line[i];
    }
    operand=temp;
}

string formatAddress(int address,int width) {
    stringstream ss;
    ss<<setw(width)<<setfill('0')<<hex<<uppercase<<address;
    return ss.str();
}

void createHeaderRecord(ofstream& outputFile,string programName,int startAddress,int programLength) 
{
    outputFile<<"H "<<setw(6)<<left<<programName.substr(0,6)<<formatAddress(startAddress,6)<<' '<<formatAddress(programLength,6)<<endl;
}

void createEndRecord(ofstream &outputFile,int startAddress) {
    outputFile<<"E "<<formatAddress(startAddress,6)<<endl;
}

void createTextRecords(ofstream &outputFile,ifstream &inputFile,int startAddress) {
    string line;
    int currentAddress = startAddress;
    string textRecord;
    int recordLength = 0;
    
    while(getline(inputFile,line)) 
    {
        if(line.substr(0,4)=="RESW")
        {
            if(recordLength>0) 
            {
                outputFile<<"T "<<formatAddress(currentAddress,6)<<' '<<formatAddress(recordLength/2,2)<<' '<<textRecord<<endl;
                currentAddress+=recordLength/2;
                textRecord="";
                recordLength=0;
            }
            string operand=line.substr(5);
            currentAddress+=stoi(operand)*3;
        }
        else if(line.substr(0,4)=="RESB")
        {
            if(recordLength>0) 
            {
                outputFile<<"T "<<formatAddress(currentAddress,6)<<' '<<formatAddress(recordLength/2,2)<<' '<<textRecord<<endl;
                currentAddress+=recordLength/2;
                textRecord="";
                recordLength=0;
            }
            string operand=line.substr(5);
            currentAddress+=stoi(operand);
        }
        else if(recordLength+line.length()>60)
        {
            outputFile<<"T "<<formatAddress(currentAddress,6)<<' '<<formatAddress(recordLength/2,2)<<' '<<textRecord<<endl;
            currentAddress+=recordLength/2;
            textRecord="";
            recordLength=0;
        }
        if(line.substr(0,4)!="RESW"&&line.substr(0,4)!="RESB")
        {
            recordLength+=line.length();
            textRecord+=line+' ';
        }
    }
    if(recordLength>0) {
        outputFile<<"T "<<formatAddress(currentAddress,6)<<' '<<formatAddress(recordLength/2,2)<<' '<<textRecord<<endl;
    }
}

int find_length(ifstream &inputFile,int&start_add)
{
    int loc_ctr=0;
    string line;
    bool started=false;
    while(getline(inputFile,line))
    {
        string label="",instruction="",operand="";
        parseLine(line,label,instruction,operand);
        operand.erase(operand.find_last_not_of(" \n\r\t")+1);
        instruction.erase(instruction.find_last_not_of(" \n\r\t")+1);
        if(instruction=="END")
        break;
        if(instruction=="START")
        {
            start_add=stoi(operand,nullptr,16);
            loc_ctr=start_add;
            started=true;
            continue;
        }
        if(!(instruction=="WORD"||instruction=="RESW"||instruction=="RESB"||instruction=="BYTE"))
        loc_ctr+=3;
        else if(instruction=="RESW")
        loc_ctr+=3*stoi(operand);
        else if(instruction=="RESB")
        loc_ctr+=stoi(operand);
        else if(instruction=="WORD")
        loc_ctr+=3;
        else if(instruction=="BYTE")
        {
            if(operand[0]=='C')
            loc_ctr+=operand.length()-3;
            else if(operand[0]=='X')
            loc_ctr+=(operand.length()-3)/2;
        }
    }
    return loc_ctr;
}
int main() {
    ifstream inputFile("Input.txt");
    ifstream objFile("Output.obj");
    ofstream outputFile("Records.obj");
    if (!inputFile||!objFile||!outputFile) {
        cout<<"Error opening file!"<<endl;
        return 1;
    }
    string programName,startAddrStr,startKeyword;
    int startAddress,programLength;
    inputFile>>programName>>startKeyword>>startAddrStr;
    startAddress=stoi(startAddrStr,nullptr,16);
    string line;

    programLength=find_length(inputFile,startAddress);  

    createHeaderRecord(outputFile,programName,startAddress,programLength);

    createTextRecords(outputFile,objFile,startAddress);

    createEndRecord(outputFile,startAddress);

    inputFile.close();
    objFile.close();
    outputFile.close();

    return 0;
}
