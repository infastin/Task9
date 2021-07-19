#ifndef STRING_HPP_7XZCHU3V
#define STRING_HPP_7XZCHU3V

#include <stddef.h>
#include <iostream>

class String
{
public:
	String(const char *string = NULL);
	String(char c);
	~String();
	String(const String &src);
	
	bool operator<(const String &b);
	bool operator<=(const String &b);
	bool operator>(const String &b);
	bool operator>=(const String &b);
	bool operator==(const String &b);
	bool operator!=(const String &b);

	void operator=(const String &b);

	char& operator[](size_t idx);

	String operator+(const String &b);
	String operator+(const char *b);
	String operator+(char b);

	String operator*(unsigned int b);
	String& operator*=(unsigned int b);

	String& operator+=(const String &b);
	String& operator+=(const char *b);
	String& operator+=(char b);

	friend std::ostream& operator<<(std::ostream &out, const String &str);
	friend std::istream& operator>>(std::istream &in,  String &str);

	const char* bare(void);

private:
	int compare(const String &b);
	void grow(size_t add);
	char *str;
	size_t size;
	size_t capacity;
};

#endif /* end of include guard: STRING_HPP_7XZCHU3V */
