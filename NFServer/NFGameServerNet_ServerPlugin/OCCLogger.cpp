﻿#if 0
//logger.cpp
#include "OCCLogger.h"
#include <time.h>
#include <stdarg.h>
#include <direct.h>
#include <vector>
#include <Dbghelp.h>
#include <iostream>
#include <fstream>

#include <sys\timeb.h>

#pragma comment(lib,"Dbghelp.lib")

using std::string;
using std::vector;

namespace LOGGER
{
    OCCLogger::OCCLogger(EnumLogLevel nLogLevel, const std::string strLogPath, const std::string strLogName)
        :m_nLogLevel(nLogLevel),
        m_strLogPath(strLogPath),
        m_strLogName(strLogName)
    {
        //初始化
        m_pFileStream = NULL;
        if (m_strLogPath.empty())
        {
            m_strLogPath = GetAppPathA();
        }
        if (m_strLogPath[m_strLogPath.length() - 1] != '\\')
        {
            m_strLogPath.append("\\");
        }
        //创建文件夹
        MakeSureDirectoryPathExists(m_strLogPath.c_str());
        //创建日志文件
        if (m_strLogName.empty())
        {
            time_t curTime;
            time(&curTime);
            tm tm1;
            localtime_s(&tm1, &curTime);
            //日志的名称如：201601012130.log
            m_strLogName = FormatString("%04d%02d%02d_%02d%02d%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
        }
        m_strLogFilePath = m_strLogPath.append(m_strLogName);

        //以追加的方式打开文件流
        // fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
        m_sOfstream.open(m_strLogFilePath, std::ios::app);

        InitializeCriticalSection(&m_cs);
    }


    //析构函数
    OCCLogger::~OCCLogger()
    {
        //释放临界区
        DeleteCriticalSection(&m_cs);
        m_sOfstream.close();
        //关闭文件流
        /*
        if (m_pFileStream)
        {
            fclose(m_pFileStream);
            m_pFileStream = NULL;
        }
        */
    }

    //文件全路径得到文件名
    const char* OCCLogger::path_file(const char* path, char splitter)
    {
        return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
    }

    //写严重错误信息
    void OCCLogger::TraceFatal(const char* lpcszFormat, ...)
    {
        //判断当前的写日志级别
        if (EnumLogLevel::LogLevel_Fatal > m_nLogLevel)
            return;
        string strResult;
        if (NULL != lpcszFormat)
        {
            va_list marker = NULL;
            va_start(marker, lpcszFormat); //初始化变量参数
            size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
            std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker); //重置变量参数
        }
        if (strResult.empty())
        {
            return;
        }
        string strLog = strFatalPrefix;
        strLog.append(GetTime()).append(strResult);

        //写日志文件
        Trace(strLog);
    }

    //写错误信息
    void OCCLogger::TraceError(const char* lpcszFormat, ...)
    {
        //判断当前的写日志级别
        if (EnumLogLevel::LogLevel_Error > m_nLogLevel)
            return;
        string strResult;
        if (NULL != lpcszFormat)
        {
            va_list marker = NULL;
            va_start(marker, lpcszFormat); //初始化变量参数
            size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
            std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker); //重置变量参数
        }
        if (strResult.empty())
        {
            return;
        }
        string strLog = strErrorPrefix;
        strLog.append(GetTime()).append(strResult);

        //写日志文件
        Trace(strLog);
    }

    //写警告信息
    void OCCLogger::TraceWarning(const char* lpcszFormat, ...)
    {
        //判断当前的写日志级别
        if (EnumLogLevel::LogLevel_Warning > m_nLogLevel)
            return;
        string strResult;
        if (NULL != lpcszFormat)
        {
            va_list marker = NULL;
            va_start(marker, lpcszFormat); //初始化变量参数
            size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
            std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker); //重置变量参数
        }
        if (strResult.empty())
        {
            return;
        }
        string strLog = strWarningPrefix;
        strLog.append(GetTime()).append(strResult);

        //写日志文件
        Trace(strLog);
    }


    //写一般信息
    void OCCLogger::TraceInfo(const char* lpcszFormat, ...)
    {
        std::cout << "enter info trace" << std::endl;
        //判断当前的写日志级别
        if (EnumLogLevel::LogLevel_Info > m_nLogLevel)
            return;
        string strResult;
        if (NULL != lpcszFormat)
        {
            va_list marker = NULL;
            va_start(marker, lpcszFormat); //初始化变量参数
            size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
            std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker); //重置变量参数
        }
        if (strResult.empty())
        {
            return;
        }
        string strLog = strInfoPrefix;
        strLog.append(GetTime()).append(strResult);

        //写日志文件
        std::cout << "before info trace" << std::endl;
        Trace(strLog);
        std::cout << "after info trace" << std::endl;
    }

    //获取系统当前时间
    string OCCLogger::GetTime()
    {
        time_t curTime;
        time(&curTime);
        tm tm1;
        localtime_s(&tm1, &curTime);
        //2016-01-01 21:30:00
        string strTime = FormatString("%04d-%02d-%02d %02d:%02d:%02d ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

        return strTime;
    }

    //改变写日志级别
    void OCCLogger::ChangeLogLevel(EnumLogLevel nLevel)
    {
        m_nLogLevel = nLevel;
    }

    //写文件操作
    void OCCLogger::Trace(const string& strLog)
    {
        try
        {
            //进入临界区
            EnterCriticalSection(&m_cs);
            if (!m_sOfstream.is_open())
            {
                std::cout << "Error opening file" << std::endl;
                LeaveCriticalSection(&m_cs);
                return;
            }
            m_sOfstream << strLog << std::endl;
            /*
            //若文件流没有打开，则重新打开
            if (NULL == m_pFileStream)
            {
                fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
                if (!m_pFileStream)
                {
                    return;
                }
            }
            //写日志信息到文件流
            // std::cout << "write to filestream " << strLog.c_str() << std::endl;
            fprintf(m_pFileStream, "%s\n", strLog.c_str());
            std::cout << "flush filestream " << std::endl;
            */
            fflush(m_pFileStream);
            //离开临界区
            // std::cout << "leave Critical Section " << std::endl;
            LeaveCriticalSection(&m_cs);
        }
        //若发生异常，则先离开临界区，防止死锁
        catch (...)
        {
            // std::cout << "exception" << std::endl;
            LeaveCriticalSection(&m_cs);
        }
    }

    string OCCLogger::GetAppPathA()
    {
        char szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };
        GetModuleFileNameA(NULL, szFilePath, sizeof(szFilePath));
        _splitpath_s(szFilePath, szDrive, szDir, szFileName, szExt);

        string str(szDrive);
        str.append(szDir);
        return str;
    }

    string OCCLogger::FormatString(const char* lpcszFormat, ...)
    {
        string strResult;
        if (NULL != lpcszFormat)
        {
            va_list marker = NULL;
            va_start(marker, lpcszFormat); //初始化变量参数
            size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
            std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker); //重置变量参数
        }
        return strResult;
    }

    long long OCCLogger::GetSystemTime()
    {
        timeb t;
        ftime(&t);
        return t.time * 1000 + t.millitm;
    }
}
#endif
