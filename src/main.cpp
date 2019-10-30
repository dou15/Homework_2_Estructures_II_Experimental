#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
//#include <L1cache.h>
//#include <debug_utilities.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

/******************************************************************************/
/** Ejecución del programa **/
/** gunzip -c mcf.trace.gz | cache -t < # > -a < # > -l < # > -rp < # > **/
/******************************************************************************/

using namespace std;

/* Helper funtions */
void print_usage ()
{
  printf ("Print print_usage\n");
  exit (0);
}

/******************************************************************************/
/*Toma el string TraceInput, el cual contiene los datos probenientes del trace
y los separa en diferentes string para tomar cada dato por separado.*/
/******************************************************************************/
void TraceData(const string& TraceInput, char c,
           vector<string>& TraceVector) {
   string::size_type i = 0;
   string::size_type j = TraceInput.find(c);

   while (j != string::npos) {
      TraceVector.push_back(TraceInput.substr(i, j-i));
      i = ++j;
      j = TraceInput.find(c, j);

      if (j == string::npos)
         TraceVector.push_back(TraceInput.substr(i, TraceInput.length()));
   }
}

//Convierte la dirección en hexadecimal a binario
void HexAddresstoBin(char Addressinput[], char AddressBinaryOutput[])
{
    int i;
    int size;
    size = strlen(Addressinput);

    for (i = 0; i < size; i++)
    {
        if (Addressinput[i] =='0')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '0';
        }
        else if (Addressinput[i] =='1')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '1';
        }    
        else if (Addressinput[i] =='2')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '0';
        }    
        else if (Addressinput[i] =='3')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '1';
        }    
        else if (Addressinput[i] =='4')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '0';
        }    
        else if (Addressinput[i] =='5')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '1';
        }    
        else if (Addressinput[i] =='6')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '0';
        }    
        else if (Addressinput[i] =='7')
        {
            AddressBinaryOutput[i*4] = '0'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '1';
        }    
        else if (Addressinput[i] =='8')
        {
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '0';
        }
        else if (Addressinput[i] =='9')
        {
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '1';
        }
        else if (Addressinput[i] =='a')
        {    
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '0';
        }
        else if (Addressinput[i] =='b')
        {
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '0'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '1';
        }
        else if (Addressinput[i] =='c')
        {
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '0';
        }
        else if (Addressinput[i] =='d')
        {    
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '0'; AddressBinaryOutput[i*4+3] = '1';
        }
        else if (Addressinput[i] =='e')
        {    
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '0';
        }
        else if (Addressinput[i] =='f')
        {
            AddressBinaryOutput[i*4] = '1'; AddressBinaryOutput[i*4+1] = '1'; AddressBinaryOutput[i*4+2] = '1'; AddressBinaryOutput[i*4+3] = '1';
        }
    }
    AddressBinaryOutput[size*4] = '\0';
}

/******************************************************************************/
/*Función principal de ejecución del programa, controla la ejecución del
simulador de cache*/
/******************************************************************************/
int main(int argc, char * argv []) {

	//Mensaje inicio programa
  	cout << "-----------------------------------------------------------" << endl;
  printf("El programa simula una memoria cache asociatividad n, tipo write-allocate y write-back, soportando las politicas de reemplazo lru, nru y srriphp.\n");
	cout << "-----------------------------------------------------------" << endl;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Parse argruments */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

unsigned int CacheSize;  		//tamaño de la cache
unsigned int CacheLineSize;	//tamaño de linea(o bloque) de la cache
unsigned int associativity;  //tipo de asociatividad de la cache
string ReplacementPolicy; //Política de reemplazo empleada
													//Puede ser LRU, NRU o SRRIP(HP)
/*
SizeOffset => cantidad de bits del offset
SizeIndex  => cantidad de bits del index
SizaeTag   => cantidad de bits del tag
*/
unsigned int SizeOffset;     
unsigned int SizeIndex;
unsigned int SizeTag;

                
if(argc==9){
	CacheSize = atoi(argv[2]);     //convierte entrada de parámetros de string a
	associativity = atoi(argv[4]); //entero
	CacheLineSize = atoi(argv[6]);
	ReplacementPolicy = argv[8];   //asigna el tipo de política de reemplazo 
  															 //seleccionada
}

if(argc!=9){
	cout << "ERROR, revisar instrucción de ejecución" << endl;
	cout << "gunzip -c mcf.trace.gz | cache -t <tamaño cache KB> -a <tipo asociatividad> -l <tamaño de la linea en bytes> -rp <política de reemplazo>" << endl;
	cout << "ejemplo" << endl;
	cout << "gunzip -c mcf.trace.gz | ./cache -t 16 -a 2 -l 32 -rp lru" << endl;
exit(0);
}

/******************************************************************************/
/* Determinar index, tag cache */
/******************************************************************************/

CacheSize=1024*CacheSize; // tamaño cache bytes

/*
SizeOffset => cantidad de bits del offset
SizeIndex  => cantidad de bits del index
SizaeTag   => cantidad de bits del tag
*/
SizeOffset=log2(CacheLineSize);     
SizeIndex= log2(CacheSize) - (log2(associativity)+log2(CacheLineSize));
SizeTag=32-SizeIndex-SizeOffset;

cout << "offset " << SizeOffset << endl;
cout << "index " << SizeIndex << endl;
cout << "tag " << SizeTag << endl;

/******************************************************************************/
/* Get trace's lines and start your simulation */
/******************************************************************************/

   string TraceInput;
   int accesos=0;
   int OverallMissRate=0;
   int ReadMissRate=0;
   int DirtyEvictions=0;
   int LoadMisses=0;
   int StoreMisses=0;
   int TotalMisses=0;
   int LoadHits=0;
   int StoreHits=0;
   int TotalHits=0;
 
   while(getline(cin,TraceInput)){

   vector<string> TraceVector; 

   //almacena la entrada del trace en string separados
   string SimboloNumeral, LoadStore, AddressHex, IC;

   TraceData(TraceInput, ' ', TraceVector); 
   SimboloNumeral=TraceVector[0];
	 LoadStore=TraceVector[1];
	 AddressHex=TraceVector[2];
	 IC=TraceVector[3];
 
   //Convierte el valor de load/store del trace a un número entero
   bool IntLoadStore = stoi(LoadStore);
   
/******************************************************************************/
/*Convertir Dirección hexadecimal a binario*/
/******************************************************************************/

  char Addressinput[32];
  strcpy(Addressinput,AddressHex.c_str());  
  char AddressBinaryOutput[32];
  
	HexAddresstoBin(Addressinput, AddressBinaryOutput); 
	string AddressBinary = AddressBinaryOutput;
	string StrTag = AddressBinary.substr(0, SizeTag);
  string StrIndex = AddressBinary.substr(SizeTag+1, SizeIndex);
  
/*Convertir index y tag de string a entero*/

unsigned int IntTag = stoi(StrTag, 0, 2);
unsigned int IntIndex = stoi(StrIndex, 0, 2);

//Accesos totales del trace  
accesos=accesos+1;

/*string stprueba;

stprueba="lru";

if((ReplacementPolicy.compare(stprueba)) == 0){
	cout << "son iguales" << endl;
}*/

   } /*****FIN WHILE*****/

   
  /* Print cache configuration */

  /* Print Statistics */
  
  cout << "_________________________________" << endl;
  cout << "Cache parameters" << endl;
  cout << "_________________________________" << endl;
  cout << "Cache Size(KB): " << CacheSize/1024 << endl;
  cout << "Cache Associativity: " << associativity << endl;
  cout << "Cache Block Size(Bytes): " << CacheLineSize << endl; 
  cout << "Replacement Policy: " << ReplacementPolicy << endl;
  cout << "_________________________________" << endl;
  cout << "Simulation results " << endl;
  cout << "_________________________________" << endl;
  cout << "OverallMissRate: " << OverallMissRate << endl;
  cout << "ReadMissRate: " << ReadMissRate << endl;
  cout << "DirtyEvictions: " << DirtyEvictions << endl;
  cout << "LoadMisses: " << LoadMisses << endl;
  cout << "StoreMisses: " << StoreMisses << endl;
  cout << "TotalMisses: " << TotalMisses << endl;
  cout << "LoadHits: " << LoadHits << endl;
  cout << "StoreHits: " << StoreHits << endl;
  cout << "TotalHits: " << TotalHits << endl;
  cout << "_________________________________" << endl;
  cout << "los accesos totales son "<<accesos << endl;
return 0;
}
