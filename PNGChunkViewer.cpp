#include <iostream>
#include <fstream>

using namespace std;

union char_int{
	char a[4];
	int b;
};
#ifdef __linux__
void getCorrectByteOrder(char*,const size_t size){
}
#else
void getCorrectByteOrder(char *a,const size_t size){
	for(int i=0;i<size/2;i++){
		char tmp;
		tmp = a[i];
		a[i] = a[size-i-1];
		a[size-i-1] = tmp;
	}
}
#endif

string typeToHigher(char *a);

int main(int argc,char **argv){
	string fileName;
	bool showIDAT = true;
	if(argc < 2){
		cout << "file path";
		cin >>fileName;
		cout << "show IDAT chunks? y/n";
		char tmp;
		do{
			cin >> tmp;
		}while(tmp !='y' && tmp!='n');
		if(tmp == 'n')showIDAT = false;
	}
	else{
		fileName = argv[1];
	}
	ifstream file(fileName,ios_base::binary);
	unsigned char pngSignature[8] = {137,80,78,71,13,10,26,10};
	char fileSignature[8];
	file.read(fileSignature,8);
	
	for(int i=0;i<8;i++){
		if(pngSignature[i] != (unsigned char)fileSignature[i]){
			cout << "png signature incorrect" << endl;
			return 1;
		}
	}
	while(true){
		char_int chunkSize;
		file.read(chunkSize.a,4);
		getCorrectByteOrder(chunkSize.a,4);
		char chunkType[4];
		file.read(chunkType,4);
		if(typeToHigher(chunkType) != "IDAT" && (argc>2 || !showIDAT)){
		cout << "chunk type: " << typeToHigher(chunkType) << endl;
		cout << "chunk size: " << chunkSize.b << endl;
		cout << "--------------------------" << endl;
		}
		else if(argc<=2 && showIDAT){
		cout << "chunk type: " << typeToHigher(chunkType) << endl;
		cout << "chunk size: " << chunkSize.b << endl;
		cout << "--------------------------" << endl;
		}
		if(typeToHigher(chunkType) == "IEND"){
			return 1;
		}
		char *chunkData = (char*)malloc(chunkSize.b);
		file.read(chunkData,chunkSize.b);
		delete(chunkData);
		chunkData = nullptr;
		char crc[4];
		file.read(crc,4);

	}	
}



string typeToHigher(char *a){
	string b;
	for(int i=0;i<4;i++){
		if(*a+i >= 'a' && *a+i <= 'z'){
			b+= *(a+i)+('A'-'a');
			continue;
		}
		b+= *(a+i);
	}
	return b;
}
		
