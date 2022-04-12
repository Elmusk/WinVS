#include <iostream>

class Log final
{
public:
	enum LogLevel
	{
		log_level_error = 0, log_level_warning, log_level_info    
	};

private:
    LogLevel m_log_level = log_level_warning;

public: 
	void set_level(LogLevel level)
	{
		m_log_level = level;
	}

	void error(const char* message)
	{
        if (m_log_level >= log_level_error)
            std::cout << "[ERROR]: " << message << std::endl;
	}
	void warn(const char* message)
	{
        if (m_log_level >= log_level_warning)
            std::cout << "[WARNING]: " << message << std::endl;
	}
	void info(const char* message)
	{
        if (m_log_level >= log_level_info)
            std::cout << "[INFO]: " << message << std::endl;
	}
};

static int s_variable = 10;

void function()
{

}

//int main()
//{
//	Log log;
//	log.set_level(Log::log_level_warning);
//	log.error("hello");
//	log.warn("hello");
//	log.info("hello");
//	
//	std::cin.get();
//}
