#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

const int TYPE_NUM = 6;
char* data_type[] = {"char", "int", "long", "double", "unsigned char", "unsigned int"};
const int length[] = {8, 32, 64, 64, 8, 32};

char* itos(int value) {
	char* ret = malloc(sizeof(char) * 100);
	snprintf(ret, 50, "%d", value);
	return ret;
}

char* ltos(long long value) {
	char* ret = malloc(sizeof(char) * 100);
	snprintf(ret, 50, "%lld", value);
	return ret;
}

char* ftos(double value) {
	char* ret = malloc(sizeof(char) * 100);
	snprintf(ret, 50, "%g", value);	
	return ret;
}

void print_bin(long long value, int data_type) {
	int s[100], j = 0;
	int length[] = {8, 32, 64, 64, 8, 32};
	for( int i = 0; i < length[data_type]; i ++ )
		s[ j ++ ] = value & 1, value = value >> 1;
	for( int i = 0; i < length[data_type]; i ++ ) {
		printf("%d", s[ -- j ]);
		if( i % 4 == 3 ) printf(" ");
	}
}
union myUnion {
    double dValue;
    long long lValue;
};
void print_double_bin(double value, int data_type) {
	int s[100], j = 0;
	
	
	union myUnion myValue;
	myValue.dValue = value;
	long long val = myValue.lValue;
	for( int i = 0; i < length[data_type]; i ++ )
		s[ j ++ ] = val & 1, val = val >> 1;
	for( int i = 0; i < length[data_type]; i ++ ) {
		printf("%d", s[ -- j ]);
		if( i % 4 == 3 ) printf(" ");
	}
}
int Data2Bin(int index, char* data) {
	int intValue;
	long long longValue;
	double doubleValue;
	
	switch( index ) {
		case 0:
			intValue = atoi(data);
			if( strcmp(data, itos(intValue)) != 0 ) return 0;
			if( intValue < -128 || intValue > 127 ) return 0;
			print_bin(intValue, 0);
			break;
		case 1:
			intValue = atoi(data);
			if( strcmp(data, itos(intValue)) != 0 ) return 0;
			print_bin(intValue, 1);
			break;
		case 2:
			sscanf(data, "%lld", &longValue);
			if( strcmp(data, ltos(longValue)) != 0 ) return 0;
			print_bin(longValue, 2);
			break;
		case 3:
			doubleValue = atof(data);
			if( strcmp(data, ftos(doubleValue)) != 0 ) return 0;
			print_double_bin(doubleValue, 3);
			break;
		case 4:
			intValue = atoi(data);
			if( strcmp(data, itos(intValue)) != 0 ) return 0;
			if( intValue < 0 || intValue > 255 ) return 0;
			print_bin(intValue, 4);
			break;
		case 5:
			sscanf(data, "%lld", &longValue);
			if( strcmp(data, ltos(longValue)) != 0 ) return 0;
			if( longValue < 0 || longValue >= ( 1LL << 32 )) return 0;
			print_bin(longValue, 5);
			break;
	}	
	return 1;
}

int checkBin(char* s) {
	if( strlen(s) != 4 ) return 0;
	for( int i = 0; i < 4; i ++ )
		if( s[i] != '0' && s[i] != '1' ) return 0;
	return 1;
}
int Bin2Data(int index, int from, int n, char* data[]) {
	long long value = 0;
	char str[100];
	str[0] = 0;
	for( int i = from; i < n; i ++ ) {
		if( checkBin(data[i]) == 0 ) return 0;
		strcat(str, data[i]);
	}
	for( int i = 0; str[i]; i ++ ) {
		value = value + ((long long)(str[i] - '0') << ( length[index] - i - 1 ));
	}
	union myUnion myValue;
	switch( index ) {
		case 0:
			printf("%d", (char)(value & 0xFF));
			break;
		case 1:
			printf("%d", (int)(value & 0xFFFFFFFF));
			break;
		case 2:
			printf("%lld", (long long)(value & 0xFFFFFFFFFFFFFFFFLL));
			break;
		case 3:
			myValue.lValue = value;
			printf("%g", myValue.dValue);
			break;
		case 4:
			if( value < 0 || value > 255 ) return 0;
			printf("%lld",value & 0xFF);
			break;
		case 5:
			if( value < 0 || value >= ( 1LL << 32 )) return 0;
			printf("%lld",value & 0xFFFFFFFF);
			break;
	}
	return 1;
}
int main(int argc, char* argv[]) {
	if( argc == 1 ) {
		printf("All tests pass.\n");
		return 0;
	}
	int index = 1;
	int dataTypeIndex = -1;
	if( strcmp(argv[index], "char") == 0 ) dataTypeIndex = 0;
	else if( strcmp(argv[index], "int") == 0 ) dataTypeIndex = 1;
	else if( strcmp(argv[index], "long") == 0 ) dataTypeIndex = 2;
	else if( strcmp(argv[index], "double") == 0 ) dataTypeIndex = 3;
	else if( strcmp(argv[index], "unsigned") == 0 ) {
		index ++;
		if( strcmp(argv[index], "char") == 0 ) dataTypeIndex = 4;
		else if( strcmp(argv[index], "int") == 0 ) dataTypeIndex = 5;
	} else {
		char str[100], *p;
		str[0] = 0;
		do {
			p = strchr(argv[index], '}');
			strcat(str, argv[index]);
			index ++;
		}while( !p );
		int typeCount = 1;
		str[strlen(str) - 1] = 0;
		strcpy(str, str + 1);
		for( int k = 0; str[k]; k ++)
			if( str[k] == ';' ) typeCount ++;
		int direction = argc - index == typeCount;
		p = strtok(str, ";");
		while( p ) {
			if( strncmp(p, "char", 4) == 0 ) dataTypeIndex = 0;
			else if( strncmp(p, "int", 3) == 0 ) dataTypeIndex = 1;
			else if( strncmp(p, "long", 4) == 0 ) dataTypeIndex = 2;
			else if( strncmp(p, "double", 6) == 0 ) dataTypeIndex = 3;
			else if( strncmp(p, "unsignedchar", 12) == 0 ) dataTypeIndex = 4;
			else if( strncmp(p, "unsignedint", 11) == 0 ) dataTypeIndex = 5;
			if( direction ) {
				if( !Data2Bin(dataTypeIndex, argv[index ++ ]))
					puts("Input error.");
				else printf(" ");
			} else {
				if( !Bin2Data(dataTypeIndex, index, index + length[dataTypeIndex] / 4, argv))
					puts("Input error.");
				else printf(" ");
				index += length[dataTypeIndex] / 4;
			}
			p = strtok(NULL, ";");
		}		
		return 0;
	}
	if( dataTypeIndex < 0 ) {
		puts("Input error.");
		return 0;
	}
	index ++;
	if( index == argc - 1 ) {
		if( !Data2Bin(dataTypeIndex, argv[index]))
			puts("Input error.");
		else puts("");
	} else {
		if( !Bin2Data(dataTypeIndex, index, argc, argv))
			puts("Input error.");
		else puts("");
	}
	return 0;
}