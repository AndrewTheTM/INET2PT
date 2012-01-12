// INET2PT.cpp
//
// Route converter for INET to PT LIN files
// Author: Andrew Rohne arohne@oki.org Licensed open source 2012 GPLv3


#include "stdafx.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>

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
	if(NLine.length()<15)
		int a=1;
	char* Ns;
	Ns=strtok((char*)NLine.c_str(),",&");
	while(Ns != NULL){
		routeObj.nodeList.push_back(-1*stoi(Ns));
		Ns=strtok(NULL,",&");
	}
}
		


int _tmain(int argc, _TCHAR* argv[])
{
	if(argc<2){
		printf("\nINET Route to PT LIN converter\n");
		printf("Written by Andrew Rohne, OKI Regional Council, 2012.  Licensed under GPLv3\n\n");
		printf("Usage: \n     INET2PT <am transit file> [<md transit file> <pm transit file>]\n\n");
	}
	if(argc>=2){
		printf("Processing AM...\n");
		ifstream amStream;
		amStream.open((string)argv[1]);
		string line;
		int status=0;
		tRoute tmp;
		if(amStream.is_open()){
			while(amStream.good()){
				getline(amStream,line);
				if(line.length()>0){
				if(line.compare(1,1,"*")==0)
					goto skip;

				if(line.length()<10)
					int a=1;

				switch(status){

				case 0: //begin collection

					if(line.find("M=")!=string::npos)
						tmp.Mode=stoi(line.substr(line.find("M=")+2,(line.find(",",line.find("M="))-line.find("M="))));

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

					if(line.find("ID=")!=string::npos)
						tmp.LongName=line.substr(line.find("ID=")+3,(line.find(",",line.find("ID="))-line.find("ID=")));

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
	} //end if(argc>=2)
	printf("Completed AM.\n\n");
	if(argc>=3){




	return 0;
}

