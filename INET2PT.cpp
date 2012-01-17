// INET2PT.cpp
//
// Route converter for INET to PT LIN files
// Author: Andrew Rohne arohne@oki.org Licensed open source 2012 GPLv3
//
// Version 1.0

#include "stdafx.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

class tRoute{
public:
	int Mode;
	int Line;
	int Company;
	int AMHeadway;
	int MDHeadway;
	int PMHeadway;
	int RG; // <~ This is put into USERN2
	string LongName;
	bool OneWay;
	bool Circular;
	vector <int> nodeList;
	tRoute(){
		Mode=0;
		Line=0;
		Company=0;
		AMHeadway=0;
		MDHeadway=0;
		PMHeadway=0;
		RG=0;
		LongName="";
		OneWay=false;
		Circular=false;
		nodeList.clear();
	};
	~tRoute(){
		Mode=0;
		Line=0;
		Company=0;
		AMHeadway=0;
		MDHeadway=0;
		PMHeadway=0;
		RG=0;
		LongName="";
		OneWay=false;
		Circular=false;
		nodeList.clear();
	};
	void clear(){
		Mode=0;
		Line=0;
		Company=0;
		AMHeadway=0;
		MDHeadway=0;
		PMHeadway=0;
		RG=0;
		LongName="";
		OneWay=false;
		Circular=false;
		nodeList.clear();
	};
};

vector <tRoute> transitRoutes;

void splitN(tRoute &routeObj, string NLine)
{
	char* Ns;
	Ns=strtok((char*)NLine.c_str(),",&");
	while(Ns != NULL){
		routeObj.nodeList.push_back(-1*stoi(Ns));
		Ns=strtok(NULL,",&");
	}
}

bool compareTransit(tRoute routeObj, int period)
{
	for(unsigned int t=0;t<transitRoutes.size();t++)
	{
		if(transitRoutes[t].Mode==routeObj.Mode && transitRoutes[t].Line==routeObj.Line){
			//check nodes - they may be different; ignore stop/non-stop, as differences there might be in error
			if(transitRoutes[t].nodeList.size()!=routeObj.nodeList.size())
				return false;
			else
				for(unsigned int n=0;n<transitRoutes[t].nodeList.size();n++)
					if(transitRoutes[t].nodeList[n]!=routeObj.nodeList[n])
						return false;
			if(period=0)
				transitRoutes[t].MDHeadway=routeObj.MDHeadway;
			else
				transitRoutes[t].PMHeadway=routeObj.PMHeadway;
			return true;
		}
	}
	return false;
}			

void processAM(string filename){
	ifstream amStream;
	amStream.open(filename);
	string line;
	int status=0;
	tRoute tmp;
	if(amStream.is_open()){
		while(amStream.good()){
			getline(amStream,line);
			if(line.length()>0){
				if(line.compare(1,1,"*")==0)
					goto skip;

				switch(status){

				case 0: //begin collection

					if(line.find("M=")!=string::npos)
						tmp.Mode=stoi(line.substr(line.find("M=")+2,(line.find(",",line.find("M="))-line.find("M=")))-8); //NOTE THE -8 HERE!

					if(line.find("L=")!=string::npos)
						tmp.Line=stoi(line.substr(line.find("L=")+2,(line.find(",",line.find("L="))-line.find("L="))));

					if(line.find("C=")!=string::npos)
						tmp.Company=stoi(line.substr(line.find("C=")+2,(line.find(",",line.find("C="))-line.find("C="))));

					if(line.find("H=")!=string::npos)
						tmp.AMHeadway=stoi(line.substr(line.find("H=")+2,(line.find(",",line.find("H="))-line.find("H="))));

					if(line.find("RG=")!=string::npos)
						tmp.RG=stoi(line.substr(line.find("RG=")+3,(line.find(",",line.find("RG="))-line.find("RG="))));

					if(line.find("ONEWAY=")!=string::npos){
						if(line.compare(line.find("ONEWAY=")+7,1,"T")==0)
							tmp.OneWay=true;							
					}
					if(line.find("CIRCULAR=")!=string::npos){
						if(line.compare(line.find("CIRCULAR=")+9,1,"T")==0)
							tmp.Circular=true;							
					}

					if(line.find("ID=")!=string::npos){
						string tmpLN=line.substr(line.find("ID=")+4,(line.find(",",line.find("ID="))-line.find("ID=")-5));
						tmp.LongName=tmpLN.substr(0,tmpLN.find_last_not_of(" ")+1);
					}

					if(line.find("N=")!=string::npos){
						string tmpN=line.substr(line.find("N=")+2,line.length());
						status=1;
						splitN(tmp,tmpN);
					}

					if(line.find("&END")!=string::npos){
						transitRoutes.push_back(tmp);
						tmp.clear();
						status=0;
					}

					break;
				case 1: //collecting
					if(line.find("&END")!=string::npos){
						string tmpN=line.substr(1,line.find("&END"));
						splitN(tmp,tmpN);
						transitRoutes.push_back(tmp);
						tmp.clear();

						status=0;
					}else{
						splitN(tmp,line);
					}
					break;
				} //end switch
skip:;
			} // end if(line.length()>0)
		} //end while(amstream.good)
	} //end if(amstream.open)
}

void processMDPM(string filename,int period){
	//period=0 = MD, period=1 = PM
	ifstream mdStream;
	mdStream.open(filename);
	string line;
	int status=0;
	tRoute tmp;
	if(mdStream.is_open()){
		while(mdStream.good()){
			getline(mdStream,line);
			if(line.length()>0){
				if(line.compare(1,1,"*")==0)
					goto skip;
				switch(status){

				case 0: //begin collection

					if(line.find("M=")!=string::npos)
						tmp.Mode=stoi(line.substr(line.find("M=")+2,(line.find(",",line.find("M="))-line.find("M="))));

					if(line.find("L=")!=string::npos)
						tmp.Line=stoi(line.substr(line.find("L=")+2,(line.find(",",line.find("L="))-line.find("L="))));

					if(line.find("C=")!=string::npos)
						tmp.Company=stoi(line.substr(line.find("C=")+2,(line.find(",",line.find("C="))-line.find("C="))));

					if(line.find("H=")!=string::npos){
						if(period==0)
							tmp.MDHeadway=stoi(line.substr(line.find("H=")+2,(line.find(",",line.find("H="))-line.find("H="))));
						else
							tmp.PMHeadway=stoi(line.substr(line.find("H=")+2,(line.find(",",line.find("H="))-line.find("H="))));
					}

					if(line.find("RG=")!=string::npos)
						tmp.RG=stoi(line.substr(line.find("RG=")+3,(line.find(",",line.find("RG="))-line.find("RG="))));

					if(line.find("ONEWAY=")!=string::npos){
						if(line.compare(line.find("ONEWAY=")+7,1,"T")==0)
							tmp.OneWay=true;							
					}
					if(line.find("CIRCULAR=")!=string::npos){
						if(line.compare(line.find("CIRCULAR=")+9,1,"T")==0)
							tmp.Circular=true;							
					}

					if(line.find("ID=")!=string::npos){
						string tmpLN=line.substr(line.find("ID=")+4,(line.find(",",line.find("ID="))-line.find("ID=")-5));
						tmp.LongName=tmpLN.substr(0,tmpLN.find_last_not_of(" ")+1);
					}

					if(line.find("N=")!=string::npos){
						string tmpN=line.substr(line.find("N=")+2,line.length());
						status=1;
						splitN(tmp,tmpN);
					}

					if(line.find("&END")!=string::npos){
						if(!compareTransit(tmp,period))
							transitRoutes.push_back(tmp);
						tmp.clear();
						status=0;
					}

					break;
				case 1: //collecting
					if(line.find("&END")!=string::npos){
						string tmpN=line.substr(1,line.find("&END"));
						splitN(tmp,tmpN);
						if(!compareTransit(tmp,period))
							transitRoutes.push_back(tmp);

						tmp.clear();

						status=0;
					}else{
						splitN(tmp,line);
					}
					break;
				} //end switch
			} //line len
		} //while
	} //isopen

skip:;
}

string itos(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}

void writePTOut(string filename)
{
	unsigned int col=70; //this is the max column width
	ofstream outFile;
	outFile.open(filename);
	outFile << ";;<<PT>><<LINE>>;;" << endl;
	string outLine="";
	for(unsigned int t=0;t<transitRoutes.size();t++)
	{
		// NOTE: READ THIS!!!!!!!!!
		// The faresystem and vehicletypes are hard coded.  This is a really bad practice, but there is no equivalent in INET to these.
		// YOU WANT TO CHANGE THIS, either globally or via Cube.
		
		outLine="LINE NAME=\"M"+itos(transitRoutes[t].Mode)+"L"+itos(transitRoutes[t].Line)+"\", "+"MODE="+itos(transitRoutes[t].Mode)+ ", "+
			"FARESYSTEM=7, VEHICLETYPE=1, "+"LONGNAME=\""+transitRoutes[t].LongName+"\", "+"HEADWAY[1]="+itos(transitRoutes[t].AMHeadway)+", "+
			"HEADWAY[2]="+itos(transitRoutes[t].MDHeadway)+", HEADWAY[3]="+itos(transitRoutes[t].PMHeadway)+", ";
		if(transitRoutes[t].OneWay)
			outLine+="ONEWAY=T, ";
		if(transitRoutes[t].Circular)
			outLine+="CIRCULAR=T, ";
		outLine+="OPERATOR="+itos(transitRoutes[t].Company)+", N=";
		
		for(unsigned int n=0;n<transitRoutes[t].nodeList.size();n++)
		{
			outLine+=itos(transitRoutes[t].nodeList[n]);
			if(n<transitRoutes[t].nodeList.size()-1)
				outLine+=", ";
		}
		
		//split the line into col-sized pieces
		vector <unsigned int> comma;
		while(outLine.length()>col){
			for(unsigned int sp=0;sp<=outLine.length();sp++)
			{
				if(outLine.substr(sp,1).compare(",")==0)
					comma.push_back(sp);
			}
			for(unsigned int c=0;c<comma.size();c++){
				if(comma[c]>col){
					outFile << outLine.substr(0,comma[c-1]+2) << endl;
					outLine="     "+outLine.substr(comma[c-1]+2,outLine.length());
					break;
				}
				if(c==comma.size()-1){
					outFile << outLine.substr(0,comma[c-1]+2) << endl;
					outLine="     "+outLine.substr(comma[c-1]+2,outLine.length());
				}
			}
			comma.clear();
		}
		outFile << outLine << endl;
	}
	outFile.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
	if(argc<3){
		printf("\nINET Route to PT LIN converter\n");
		printf("Written by Andrew Rohne, OKI Regional Council, 2012.  Licensed under GPLv3\n\n");
		printf("Usage: \n     INET2PT <output LIN file> <am transit file> [<md transit file> <pm transit file>]\n\n");
		return 0;
	}
	if(argc>=3){
		printf("Processing AM...\n");
		processAM((string)argv[2]);
		printf("Completed AM.\n\n");
	}
	if(argc>=4){
		printf("Processing MD...\n");
		processMDPM((string)argv[3],0);
		printf("Completed MD.\n\n");
	}
	if(argc>=5){
		printf("Processing PM...\n");
		processMDPM((string)argv[4],1);
		printf("Completed PM.\n\n");
	}
	printf("Writing output...\n");
	writePTOut((string)argv[1]);
	printf("Completed!\n\n");

	return 0;
}

