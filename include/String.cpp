#include <malloc.h>

#include "String.hpp"

typedef struct _StringBuffer StringBuffer;

struct _StringBuffer
{
	StringBuffer *next;
	StringBuffer *prev;
	char *value;
	size_t len;
	int refs;
};


static size_t string_len(const char *string)
{
	if (string == NULL)
		return 0;

	const char *p = string;
	size_t len = 0;

	while (*p != '\0') 
	{
		len++;
		p++;
	}

	return len;
}

static char* string_dup(const char *string)
{
	size_t len = string_len(string);

	char *res = new char[len + 1];
	
	const char *p = string;
	char *r = res;

	while (*p != '\0') 
	{
		*r = *p;
		p++;
		r++;
	}

	*r = '\0';
	return res;
}

static char* string_cpy(char *dst, const char *src)
{
	char *d = dst;
	const char *s = src;

	while (*s != '\0')
	{
		*d = *s;
		d++;
		s++;
	}

	return d;
}

String::String(const char *string)
{
	if (string == NULL)
	{
		str = NULL;
		size = 0;
		capacity = 0;
	}
	else
	{
		str = string_dup(string);
		size = string_len(string);
		capacity = size + 1;
	}
}

String::String(char c)
{
	if (c == '\0')
	{
		str = NULL;
		size = 0;
	}
	else
	{
		str = new char[2];
		str[0] = c;
		str[1] = '\0';
		size = 1;
		capacity = 2;
	}
}

String::String(const String &src)
{
	if (src.str == NULL)
	{
		str = NULL;
		size = 0;
		capacity = 0;
	}
	else
	{
		str = string_dup(src.str);
		size = src.size;
		capacity = size + 1;
	}
}

String::~String()
{
	if (str != NULL)
		free(str);
}

void String::grow(size_t add)
{
	if (add == 0)
		return;

	if (size + add <= capacity)
		return;

	size_t mincap = capacity + add;
	size_t new_allocated = (mincap >> 3) + (mincap < 9 ? 3 : 6);

	mincap += new_allocated;

	str = (char*)realloc(str, mincap);
	capacity = mincap;
}

int String::compare(const String &b)
{
	const char *pa = str;
	const char *pb = b.str;

	while (1) 
	{
		if (*pa == '\0' && *pb != '\0')
			return -1;

		if (*pa != '\0' && *pb == '\0')
			return 1;

		if (*pa == '\0' && *pb == '\0')
			return 0;

		if (*pa < *pb)
			return -1;
		else if (*pa > *pb)
			return 1;

		pa++;
		pb++;
	}
}

bool String::operator>(const String &b)
{
	if (compare(b) > 0)
		return true;

	return false;
}

bool String::operator>=(const String &b)
{
	if (compare(b) >= 0)
		return true;

	return false;
}

bool String::operator<(const String &b)
{
	if (compare(b) < 0)
		return true;

	return false;
}

bool String::operator<=(const String &b)
{
	if (compare(b) <= 0)
		return true;

	return false;
}

bool String::operator==(const String &b)
{
	if (compare(b) == 0)
		return true;

	return false;
}

bool String::operator!=(const String &b)
{
	if (compare(b) != 0)
		return true;

	return false;
}

String String::operator+(const String &b)
{
	String res;

	res.size = size + b.size;
	res.capacity = res.size + 1;
	res.str = new char[res.capacity];

	string_cpy(res.str, str);
	string_cpy(&res.str[size], b.str);

	return res;
}

String String::operator+(const char *b)
{
	String res;

	size_t b_size = string_len(b);

	res.size = size + b_size;
	res.capacity = res.size + 1;
	res.str = new char[res.capacity];

	string_cpy(res.str, str);
	string_cpy(&res.str[size], b);

	return res;
}

String String::operator+(char b)
{
	String res;

	res.size = size + 1;
	res.capacity = res.size + 1;
	res.str = new char[res.capacity];

	string_cpy(res.str, str);

	res.str[res.size - 1] = b;
	res.str[res.size] = '\0';

	return res;
}

String& String::operator+=(const String &b)
{
	grow(b.size);
	string_cpy(&str[size], b.str);
	size += b.size;

	return *this;
}

String& String::operator+=(const char *b)
{
	size_t b_size = string_len(b);
	grow(b_size);
	string_cpy(&str[size], b);
	size += b_size;

	return *this;
}

String& String::operator+=(const char b)
{
	grow(1);
	str[size] = b;
	str[size + 1] = '\0';
	size += 1;

	return *this;
}

void String::operator=(const String &b)
{
	if (str != NULL)
		free(str);

	size = b.size;
	capacity = b.capacity;
	str = string_dup(b.str);
}

char& String::operator[](size_t idx)
{
	if (idx >= size)
	{
		std::cout << "Index out of bounds!" << std::endl;
		return str[0];
	}

	return str[idx];
}

String String::operator*(unsigned int b)
{
	String res;

	res.size = size * b;
	res.capacity = res.size + 1;
	res.str = new char[res.capacity];

	string_cpy(res.str, str);
	string_cpy(&res.str[size], str);

	return res;
}

String& String::operator*=(unsigned int b)
{
	grow((b - 1) * size);

	size_t start_size = size;

	for (unsigned int i = 0; i < b - 1; i++)
	{
		char tmp = str[size - 1];
		str[size - 1] = '\0';
		string_cpy(&str[size], str);
		str[size - 1] = tmp;
		size += start_size;
		str[size - 1] = tmp;
		str[size] = '\0';
	}

	return *this;
}

const char* String::bare(void)
{
	return str;
}

std::ostream& operator<<(std::ostream &out, const String &str)
{
	out << str.str;
	return out;
}

std::istream& operator>>(std::istream &in, String &str)
{
	int c;
	while ((c = in.get()) && c != ' ' && c != '\n')
	{
		str += c;
	}

	return in;
}
