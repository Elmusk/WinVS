#include <iostream>
#include <array>

template<typename T, size_t S>
class Array
{
public:
	constexpr size_t size() const { return S; }

	T& operator[](size_t index) { return m_data[index]; }
	const T& operator[](size_t index) const { return m_data[index]; }

	T* data() { return m_data; }
	const T* data() const { return m_data; }
private:
	T m_data[S];  
};

int main()
{	
	Array<int, 5> data;

	memset(&data[0], 0, data.size() * sizeof(int));

	data[1] = 8;
	data[4] = 1;

	for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i] << std::endl;
	}

	std::cin.get();
}