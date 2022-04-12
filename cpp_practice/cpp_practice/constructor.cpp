#include <iostream>
#include <cstring>

class String
{
private:
	int length;
	char* sptr;
	void set_string(const char* s2);

	friend std::ostream& operator<<(std::ostream& os, const String& s)
	{
		return os << s.sptr;
	}

	friend std::istream& operator>>(std::istream& is, String& s)
	{
		return is >> s.sptr;
	}

public:
	String()
	{
		sptr = new char[1];
		sptr[0] = '\0';
		length = 0; 
	}
	String(const char* str)
	{
		sptr = new char[strlen(str) + 1];
		length = strlen(str);
		strcpy_s(sptr, length+1, str);
	}

	const String& operator=(const String& R)
	{
		length = R.length;
		strcpy_s(sptr, length+1, R.sptr);
		return *this;
	}
	
	const String& operator+=(const String& R)
	{
		char* temp = sptr;
		length += R.length;
		sptr = new char[length + 1];
		strcpy_s(sptr, strlen(temp)+1, temp);
		strcat_s(sptr, length+1, R.sptr);
		delete[] temp;
		return *this;
	}

	bool operator==(const String& R) { return strcmp(sptr, R.sptr) == 0; }
	bool operator!=(const String& R) { return !(*this == R); }
	bool operator!() { return length == 0; }
	bool operator<(const String& R) const { return strcmp(sptr, R.sptr) < 0; }
	bool operator>(const String& R) { return R < *this; }
	bool operator>=(const String& R) { return !(*this < R); }
	char& operator[](int subscript) { return sptr[subscript]; }

	~String() { delete[] sptr; }
};

//int main()
//{
//	String s1("happy"), s2("new year"), s3;
//	std::cout << "s1 is " << s1 << "\ns2 is " << s2 << "\ns3 is " << s3;
//	std::cout << "\n比较s2和s1:" << "\ns2==s1结果是 " << (s2 == s1 ? "true" : "false") 
//		<< "\ns2 != s1结果是 " << (s2 != s1 ? "true" : "false") 
//		<< "\ns2 >  s1结果是 " << (s2 > s1 ? "true" : "false") 
//		<< "\ns2 <  s1结果是 " << (s2 < s1 ? "true" : "false") 
//		<< "\ns2 >= s1结果是 " << (s2 >= s1 ? "true" : "false");
//	std::cout << "\n\n测试s3是否为空: ";
//	if (!s3)
//	{
//		std::cout << "s3是空串" << std::endl; //l3
//		std::cout << "把s1赋给s3的结果是：";
//		s3 = s1;
//		std::cout << "s3=" << s3 << "\n"; //l5
//	}
//	std::cout << "s1 += s2 的结果是：s1="; //l6
//	s1 += s2;
//	std::cout << s1; //l7
//
//	std::cout << "\ns1 +=  to you 的结果是："; //l8
//	s1 += " to you";
//	std::cout << "s1 = " << s1 << std::endl; //l9
//	s1[0] = 'h';
//	s1[6] = 'n';
//	s1[10] = 'y';
//	std::cout << "s1 = " << s1 << "\n"; //l10
//	system("pause");
//	return 0;
//}


