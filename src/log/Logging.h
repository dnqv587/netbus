#pragma once
#include <time/Timestamp.h>
#include <base/noncopyable.hpp>
#include <cstring>
#include <cstdint>
#include <string>
#include <sstream>
#include <memory>


//************************************
// class:  SourceFile
// brief:  源文件名处理类
//************************************
class SourceFile
{
 public:
	template <size_t N>
    constexpr explicit SourceFile(const char(&arr)[N])
		:m_data(arr),m_size(N)
	{
		const char* slash = ::strrchr(m_data, '/');
		if (slash)
		{
			m_data = slash + 1;
			m_size -= m_data - arr;
		}
	}

    constexpr explicit SourceFile(const char* FileName)
		:m_data(FileName),m_size(0)
	{
		const char* slash = ::strrchr(m_data, '/');
		if (slash)
		{
			m_data = slash + 1;
			m_size = ::strlen(m_data);
		}
	}

	const char* FileName() const
	{
		return m_data;
	}

	size_t size() const
	{
		return m_size;
	}

 private:
	const char* m_data;//SourceFile名称
	size_t m_size;
};

/// @brief 日志级别
enum class LogLevel : unsigned char
{
	/// TRACE 级别
	TRACE ,
	/// DEBUG 级别
	DEBUG ,
	/// INFO 级别
	INFO ,
	/// WARN 级别
	WARN ,
	/// ERROR 级别
	ERROR ,
	/// FATAL 级别
	FATAL ,
	/// 日志级别个数
	NUM_LOG_LEVELS

};

class LogContext: copyable
{
 public:
    LogContext(SourceFile&& file, int32_t line, std::string&& funcName, time_t elapse, pid_t threadId, uint32_t fiberId, Timestamp time,
               const std::string& threadName, LogLevel level, const std::string& loggerName);

	const SourceFile& getSourceFile() const
	{return m_file;}

	int32_t getLine() const
	{return m_line;}

    const std::string& getFuncName() const
	{return m_funcName;}

	time_t getElapse() const
	{return m_elapse;}

	pid_t getThreadId() const
	{return m_threadId;}

	uint32_t getFiberId() const
	{return m_fiberId;}

	Timestamp getTime() const
	{return m_time;}

    const std::string& getThreadName() const
	{return m_threadName;}

    const std::string& getLoggerName() const
    {return m_loggerName;}

	LogLevel getLogLevel() const
	{return m_level;}

    std::stringstream& getStream()
	{return m_stream;}
 private:
	//文件名
	SourceFile m_file;
	//行号
	int32_t m_line;
	//函数名
	std::string m_funcName;
	//程序启动时间--毫秒
	time_t  m_elapse;
	//线程号
	pid_t m_threadId;
	//协程号
	uint32_t m_fiberId;
	//时间
	Timestamp m_time;
	//线程名
	std::string m_threadName;
    //日志名
    std::string m_loggerName;
	//日志等级
	LogLevel m_level;
	//日志流
	std::stringstream m_stream;
};

class Logger;
class Logging: noncopyable
{
 public:
    using LogStreamPtr=std::shared_ptr<std::stringstream>;

	Logging(Logger& logger, SourceFile file, int32_t line, std::string&& func, LogLevel level);
	~Logging();
	/// @brief 获取日志流
	/// @return
	std::stringstream& getStream()
	{return m_context.getStream();}

    void LogFormat(const char* fmt,...);

 private:
    Logger& m_logger;
	LogContext m_context;
};

