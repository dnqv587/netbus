#include <log/Logging.h>
#include <time/Timestamp.h>
#include <thread/CurrentThread.h>
#include <thread/ProcessInfo.h>
#include <log/Logger.h>
#include <stdarg.h>

LogContext::LogContext(SourceFile&& file, int32_t line, std::string&& funcName, time_t elapse, pid_t threadId, uint32_t fiberId,
                       Timestamp time, const std::string &threadName, LogLevel level, const std::string& loggerName )
                       :m_file(std::move(file)),m_line(line),m_funcName(funcName),m_elapse(elapse),m_threadId(threadId),m_fiberId(fiberId),m_time(time),
                       m_threadName(threadName),m_level(level),m_loggerName(loggerName)
{
}
Logging::Logging(Logger& logger, SourceFile file, int32_t line, std::string&& func, LogLevel level)
:m_logger(logger),
m_context(std::move(file), line, std::move(func), 0, CurrentThread::tid(), 0, Timestamp::now(), CurrentThread::ThreadName(), level, m_logger.getName())
{

}
Logging::~Logging()
{
    if(!m_logger.m_format)
    {
        m_logger.setFormatter(std::make_shared<LogFormatter>("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }
    LogStreamPtr logStream=m_logger.getFormatter()->Format(m_context);
    m_logger.logging(std::move(logStream),m_context.getLogLevel());
}

void Logging::LogFormat(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    char* buf= nullptr;
    int n=::vasprintf(&buf,fmt,args);
    if(n > 0)
    {
        m_context.getStream()<<std::string(buf,n);
        ::free(buf);
    }
    va_end(args);
}
