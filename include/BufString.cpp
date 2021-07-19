#include <malloc.h>

#include "BufString.hpp"

struct _StringBuffer
{
	StringBuffer *next;
	StringBuffer *prev;
	char *value;
	size_t len;
	int refs;
};

static StringBuffer *buffers = NULL;

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

int string_cmp(const char *a, const char *b)
{
	const char *pa = a;
	const char *pb = b;

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

static char* string_ncpy(char *dst, const char *src, unsigned int n)
{
	char *d = dst;
	const char *s = src;

	while (*s != '\0' && s != src + n)
	{
		*d = *s;
		d++;
		s++;
	}

	return d;
}

void StringBuffer_free(StringBuffer *&buf)
{
	free(buf->value);

	if (buf->prev != NULL)
		buf->prev->next = buf->next;

	if (buf->next != NULL)
		buf->next->prev = buf->prev;

	free(buf);
	buf = NULL;
}

void StringBuffer_unref(StringBuffer *&buf)
{
	if (buf == NULL)
		return;

	buf->refs--;

	if (buf->refs == 0)
		StringBuffer_free(buf);
}

StringBuffer* StringBuffer_ref(StringBuffer *buf)
{
	if (buf == NULL)
		return NULL;

	buf->refs++;
	return buf;
}

StringBuffer* StringBuffer_get(StringBuffer *&buf, const char *value)
{
	if (buf != NULL)
	{
		StringBuffer *curr = buf;

		while (curr != NULL)
		{
			if (string_cmp(curr->value, value) == 0)
				return StringBuffer_ref(curr);

			curr = curr->next;
		}
	}

	StringBuffer *sb = (StringBuffer*)calloc(1, sizeof(StringBuffer));
	sb->next = NULL;
	sb->prev = NULL;
	sb->len = string_len(value);
	sb->value = string_dup(value);
	sb->refs = 1;

	if (buf != NULL)
	{
		buf->next = sb;
		sb->prev = buf;
	}
	else
		buf = sb;

	return sb;
}

BufString::BufString(const char *string)
{
	if (string == NULL)
		buf = StringBuffer_get(buffers, "");
	else
		buf = StringBuffer_get(buffers, string);
}

BufString::BufString(const char c)
{
	if (c == '\0')
		buf = StringBuffer_get(buffers, "");
	else
	{
		char *str = new char[2];
		str[0] = c;
		str[1] = '\0';

		buf = StringBuffer_get(buffers, str);
	}
}

BufString::~BufString()
{
	StringBuffer_unref(buf);
}

BufString::BufString(const BufString &src)
{
	buf = StringBuffer_ref(src.buf);
}

bool BufString::operator>(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) > 0)
		return true;

	return false;
}

bool BufString::operator>=(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) >= 0)
		return true;

	return false;
}

bool BufString::operator<(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) < 0)
		return true;

	return false;
}

bool BufString::operator<=(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) <= 0)
		return true;

	return false;
}

bool BufString::operator==(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) == 0)
		return true;

	return false;
}

bool BufString::operator!=(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) != 0)
		return true;

	return false;
}

BufString BufString::operator+(const BufString &b)
{
	size_t new_len = buf->len + b.buf->len;
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	string_cpy(&str[buf->len], b.buf->value);

	BufString res(str);
	delete[] str;

	return res;
}

BufString BufString::operator+(const char *b)
{
	size_t new_len = buf->len + string_len(b);
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	string_cpy(&str[buf->len], b);

	BufString res(str);
	delete[] str;

	return res;
}

BufString BufString::operator+(const char b)
{
	size_t new_len = buf->len + 1;
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	str[buf->len] = b;
	str[new_len] = '\0';

	BufString res(str);
	delete[] str;

	return res;
}

BufString& BufString::operator+=(const BufString &b)
{
	size_t new_len = buf->len + b.buf->len;
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	string_cpy(&str[buf->len], b.buf->value);

	if (string_cmp(buf->value, str) != 0)
	{
		StringBuffer_unref(buf);
		buf = StringBuffer_get(buffers, str);
	}

	delete[] str;

	return *this;
}

BufString& BufString::operator+=(const char *b)
{
	size_t new_len = buf->len + string_len(b);
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	string_cpy(&str[buf->len], b);

	if (string_cmp(buf->value, str) != 0)
	{
		StringBuffer_unref(buf);
		buf = StringBuffer_get(buffers, str);
	}

	delete[] str;

	return *this;
}

BufString& BufString::operator+=(const char b)
{
	size_t new_len = buf->len + 1;
	char *str = new char[new_len + 1];

	string_cpy(str, buf->value);
	str[buf->len] = b;
	str[new_len] = '\0';

	if (string_cmp(buf->value, str) != 0)
	{
		StringBuffer_unref(buf);
		buf = StringBuffer_get(buffers, str);
	}

	delete[] str;

	return *this;
}

void BufString::operator=(const BufString &b)
{
	if (string_cmp(buf->value, b.buf->value) != 0)
		StringBuffer_unref(buf);

	buf = StringBuffer_ref(b.buf);
}

BufString BufString::operator*(unsigned int b)
{
	size_t new_len = buf->len * b;
	char *str = new char[new_len + 1];

	for (unsigned int i = 0; i < b; ++i)
	{
		string_ncpy(&str[i * buf->len], buf->value, buf->len);
	}

	BufString res(str);
	delete [] str;

	return res;
}

BufString& BufString::operator*=(unsigned int b)
{
	size_t new_len = buf->len * b;
	char *str = new char[new_len + 1];

	for (unsigned int i = 0; i < b; ++i)
	{
		string_ncpy(&str[i * buf->len], buf->value, buf->len);
	}

	if (string_cmp(buf->value, str) != 0)
	{
		StringBuffer_unref(buf);
		buf = StringBuffer_get(buffers, str);
	}

	delete [] str;

	return *this;
}


void BufString::ElemRef::operator=(const char &a)
{

	char *str = string_dup(bs.buf->value);
	str[idx] = a;

	if (string_cmp(bs.buf->value, str) != 0)
	{
		StringBuffer_unref(bs.buf);
		bs.buf = StringBuffer_get(buffers, str);
	}

	delete [] str;
}

BufString::ElemRef BufString::operator[](size_t idx)
{
	if (idx >= buf->len)
		return BufString::ElemRef(*this, 0);

	return BufString::ElemRef(*this, idx);
}

BufString::operator const char *()
{
	return buf->value;
}

std::ostream& operator<<(std::ostream &out, const BufString &str)
{
	out << str.buf->value;
	return out;
}

void grow(char *&mass, size_t capacity)
{
	size_t new_allocated = (capacity >> 3) + (capacity < 9 ? 3 : 6);
	capacity += new_allocated;
	mass = (char*)realloc(mass, capacity);
}

std::istream& operator>>(std::istream &in,  BufString &bstr)
{
	size_t len = 0;
	size_t capacity = 32;

	char *str = new char[capacity];
	char *p = str;

	int c;

	while ((c = in.get()) && c != ' ' && c != '\n')
	{
		if (len >= capacity)
			grow(str, capacity + 1);

		*p++ = c;
		len++;
	}

	*p = '\0';

	if (string_cmp(bstr.buf->value, str) != 0)
	{
		StringBuffer_unref(bstr.buf);
		bstr.buf = StringBuffer_get(buffers, str);
	}

	delete [] str;

	return in;
}
