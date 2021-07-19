#include <iostream>

#include "String.hpp"
#include "BufString.hpp"

int main()
{
	BufString str1("Hello World!");
	BufString str2("Hello World!");

	BufString str3 = str1 + str2;
	BufString str4("Hello World!");
	str4 = str4 + str4;

	BufString str5("123");

	BufString str6 = str5;
	str6 *= 2;

	std::cout << str1 << std::endl;
	std::cout << str2 << std::endl;
	std::cout << str3 << std::endl;
	std::cout << str4 << std::endl;
	std::cout << str6 << std::endl;

	BufString::ElemRef test = str5[0];

	std::cout << str5 << std::endl;

	test = 'A';

	std::cout << str5 << std::endl;

	const char *brr = str5;

	std::cout << brr << std::endl;
}
