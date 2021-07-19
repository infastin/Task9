#ifndef BUFSTRING_HPP_HWDTUSWV
#define BUFSTRING_HPP_HWDTUSWV

#include <stddef.h>
#include <iostream>

typedef struct _StringBuffer StringBuffer;

class BufString
{
public:
	BufString(const char *string = NULL);
	BufString(const char c);
	~BufString();
	BufString(const BufString &src);

	bool operator<(const BufString &b);
	bool operator<=(const BufString &b);
	bool operator>(const BufString &b);
	bool operator>=(const BufString &b);
	bool operator==(const BufString &b);
	bool operator!=(const BufString &b);

	class ElemRef
	{
		public:
			ElemRef(BufString &str, size_t i) : bs(str) { idx = i; };
			void operator=(const char& a);
		private:
			size_t idx;
			BufString &bs;
	};

	ElemRef operator[](size_t idx);

	BufString operator+(const BufString &b);
	BufString operator+(const char *b);
	BufString operator+(const char b);
	BufString& operator+=(const BufString &b);
	BufString& operator+=(const char *b);
	BufString& operator+=(const char b);
	BufString operator*(unsigned int b);
	BufString& operator*=(unsigned int b);
	
	void operator=(const BufString &b);

	operator const char*();

	friend std::ostream& operator<<(std::ostream &out, const BufString &str);
	friend std::istream& operator>>(std::istream &in,  BufString &str);

private:
	StringBuffer *buf;
};

#endif /* end of include guard: BUFSTRING_HPP_HWDTUSWV */
