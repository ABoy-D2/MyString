#pragma once
#include<iostream>
using  std::cin;
using  std::cout;
constexpr size_t str_alloc = 128; //用于调整string初始内存的大小，若IDE不支持constexpr则可删掉constexpr关键字


/*在对类进行const char*型的赋值时，若内存空间不足，将调用Re_allocate()，但是有些函数如重载  +  运算符时
    采用的是return的递归写法，会导致重新计算const char*的大小而产生不必要的额外时间开销，请自行改为copy构造函数的while写法留作练习
	——懒狗Aboy_D2   */

/*有bug发现请联系 41366065，请大佬指教*/



class Mystring {
public:
	friend std::ostream& operator<<(std::ostream& os, Mystring& m);//T

	friend std::istream& operator>>(std::istream& is, Mystring& m);//T

	static bool str_cmp(Mystring& s1, Mystring& s2);

	static size_t KMP(Mystring& s1, Mystring& s2);
public:

	void Re_allocate();//T ——重新分配内存的allocator_function

	//构造函数群
	Mystring() :ch(new char[str_alloc]), count(0), alloc_num(str_alloc)
	{}//T

	Mystring(const char* s) :ch(new char[str_alloc]), count(0), alloc_num(str_alloc)
	{
		int i = 0; int num = 0;
		for (num = 0; s[num] != NULL && s[num] != '\0'; num++)
		{}
		if (this->alloc_num <= num)//如果原有内存不足以装下s
		{
			while (1)
			{
				if (this->alloc_num <= num) this->Re_allocate();//则重新分配
				else break;
			}
		}
		else {
			for (i = 0; s[i] != NULL && s[i] != '\0'; i++)
			{
				this->ch[i] = s[i];
				this->count++;
			}
		}
	}//T

	Mystring(Mystring& s) :ch(new char[str_alloc]), count(0), alloc_num(str_alloc)
	{
		while (1)
		{
			if (s.size() >= this->alloc_num) this->Re_allocate();
			else break;
		}
		str_assign(s);

	}

public:					//操作符重载群
	char& operator[](int i);

	Mystring& operator+(Mystring& s);

	Mystring& operator+(const char* s);

	bool operator==(Mystring& s)
	{
		return str_cmp(*this, s);
	}//T

public:				//功能函数
	Mystring& str_assign(Mystring& s)
	{
		return *this = (*this) + s;
	}

	Mystring& str_assign(const char* s)
	{
		return *this = (*this) + s;
	}

	void str_change(int i, int j, const char* s)
	{
		if (i <= 0 || j > this->count) cout << "超出字符串索引" << std::endl;

		if (i > j)
		{
			int tem = 0;
			tem = i;
			i = j;
			j = tem;
		}

		size_t the_str_count = 0;
		for (the_str_count = 0; s[the_str_count] != NULL && s[the_str_count] != '\0'; the_str_count++)
		{}

		if (the_str_count < j - i + 1)
		{
			cout << "请使用长度大小一致的字符串" << std::endl;
			return;
		}

		int s_index = 0;
		for (int index = i; index <= j; index++)
		{
			this->ch[index - 1] = s[s_index++];
		}
	}//T ----改变字符串第i个元素到第j个元素为s

	size_t size()
	{
		return count;
	}//T

	bool is_empty()
	{
		if (this->count == 0) return 1;
		else return 0;
	}//T

	bool compare(Mystring& str)
	{
		return this->str_cmp(*this, str);
	}

	bool compare(const char* s)
	{
		Mystring str(s);
		return this->str_cmp(*this, str);
	}

	char* get()
	{
		char* s = new char[this->count];
		for (int i = 0; i < this->count; i++)
		{
			s[i] = this->ch[i];
		}
		s[count] = '\0';
		return s;
	}//返回c风格字符串的char*

	void push_back(const char* s)
	{
		this->str_assign(s);
	}

	void pop_back()
	{
		this->count--;
	}

	void insert(int index, const char* s);//数组的插入(未完成)
	/*鉴于连续数组空间的插入时间复杂度为O(n)
			还请相关大佬指教如何改进string内部结构以支持insert的更快操作*/

public:
	~Mystring()
	{
		if (ch != NULL)
			delete[] ch;
	}//T

private:
	char* ch;
	size_t count;
	size_t alloc_num;
};


void Mystring::Re_allocate()
{
	char* tem_ch = this->ch;
	this->alloc_num += str_alloc;
	this->ch = new char[alloc_num];
	for (int i = 0; i < this->count; i++) ch[i] = tem_ch[i];
	delete[] tem_ch;
}//T


//以下为operator_group
char& Mystring:: operator[](int i)
{
	if (i >= 0 && ((size_t)i) <= this->size())
		return this->ch[i];
	cout << "Warnning:the size is out of the array" << std::endl;
	return this->ch[0];
}

Mystring& Mystring:: operator+(Mystring& s)
{
	size_t new_count = this->count + s.size();
	if (new_count < this->alloc_num)
	{
		int index = 0;
		for (int i = this->count; i < new_count; i++)
			ch[i] = s[index++];
		this->count = new_count;
		return *this;
	}
	else
	{
		Re_allocate();
		return this->operator+(s);
	}
}//T

Mystring& Mystring::operator+(const char* s)
{
	int i = 0;
	for (i = 0; s[i] != NULL && s[i] != '\0'; i++)
	{
	}

	size_t new_count = this->count + i;
	if (new_count < this->alloc_num)
	{
		int index = 0;
		for (int i = this->count; i < new_count; i++)
			ch[i] = s[index++];
		this->count = new_count;
		return *this;
	}
	else
	{
		Re_allocate();
		return this->operator+(s);
	}
}//T



//以下为static_group
std::ostream& operator<<(std::ostream& os, Mystring& m)
{
	for (size_t i = 0; i < m.size(); i++)
	{
		os << m[i];
	}
	return os;
}//T

std::istream& operator>>(std::istream& is, Mystring& m)
{
	char* s = new char[1024];
	is >> s;
	int i = 0;

	m.str_assign(s);
	delete[] s;
	return is;
}//T


bool Mystring::str_cmp(Mystring& s1, Mystring& s2)
{
	if (s1.size() != s2.size()) return 0;
	else {
		for (int i = 0, j = 0; i < s1.size(); i++)
		{
			if (s1[i] != s2[j])
			{
				return 0;
			}
		}
		return 1;
	}
}
