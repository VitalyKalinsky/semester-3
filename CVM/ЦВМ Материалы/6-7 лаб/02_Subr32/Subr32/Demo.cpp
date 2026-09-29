// Demo.cpp : Defines the entry point for the console application.
//
//#define FC
//#include <iostream>

#ifndef FC
	int Fn(int iI, char iA);
#else
	int __fastcall Fn(int iI, char iA);
#endif
	int iMas[10];
	unsigned int i;

int main() {
	int iMas[10];
	unsigned int i;

	// === Циклический фрагмент на языке высокого уровня ====
	for (i=0;i<9;i++) {			//  Организация цикла - вычисляем элементы массива
		if (i<=3 || i==7 )		//  Условный оператор - 
			iMas[i]=Fn(i,27);	// считаем по этой формуле 
	}	//  Здесь конец цикла
//	std::cout<<iMas[3]+iMas[7];
	return 0;
}
#ifndef FC
 int Fn(int iI, char iA) {
#else
 int __fastcall Fn(int iI, char iA) {
#endif
	int iLocVar;
	iLocVar=(0x0FEDCBA9*iI+iA) | 0xC000000F; // считаем по этой формуле 
	return  iLocVar; 
}

