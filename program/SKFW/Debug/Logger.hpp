#pragma once
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdarg>
#include <mutex>
#include<iostream>

class Logger {
public:
    enum class Level { Info, Warn, Error };

    /// <summary>
    /// 初期化
    /// ログファイルのクリア
    /// </summary>
    static void Initialize() {
#if _DEBUG
        std::lock_guard<std::mutex> lock(m_mtx);
        std::ofstream ofs(m_logFileName, std::ios::trunc); // ここでファイルをクリア
        ofs << "[" << GetCurrentTimeString() << "] [INFO] ログ出力の初期化 .\n";
#endif // _DEBUG
    }

    /// <summary>
	/// 標準のログ出力
    /// </summary>
    /// <param name="fmt"></param>
    /// <param name=""></param>
    static void Info(const char* fmt, ...) {
#if _DEBUG
        va_list args;
        va_start(args, fmt);
        Log(Level::Info, fmt, args);
        va_end(args);
#endif // _DEBUG
    }

    /// <summary>
	/// 警告ログ出力
	/// 重大なエラーではないが注意が必要な場合に使用
    /// </summary>
    /// <param name="fmt"></param>
    /// <param name=""></param>
    static void Warn(const char* fmt, ...) {
#if _DEBUG
        va_list args;
        va_start(args, fmt);
        Log(Level::Warn, fmt, args);
        va_end(args);
#endif // _DEBUG
    }

    /// <summary>
	/// 重大なエラーログ出力
	/// 落とす処理は含まないが、即時対応が必要な場合に使用
    /// </summary>
    /// <param name="fmt"></param>
    /// <param name=""></param>
    static void Error(const char* fmt, ...) {
#if _DEBUG
        va_list args;
        va_start(args, fmt);
        Log(Level::Error, fmt, args);
        va_end(args);
#endif // _DEBUG
    }

private:
    /// <summary>
	/// ログファイルに書き込む
    /// </summary>
    /// <param name="level"></param>
    /// <param name="fmt"></param>
    /// <param name="args"></param>
    static void Log(Level level, const char* fmt, va_list args) {
#if _DEBUG
        std::lock_guard<std::mutex> lock(m_mtx);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, args);

        std::ofstream logFile(m_logFileName, std::ios::app);
        if (!logFile) return;

        logFile << "[" << GetCurrentTimeString() << "] "
            << "[" << LevelToString(level) << "] "
            << buffer << "\n";

        std::cout << buffer << std::endl;
#endif // _DEBUG
    }

    /// <summary>
	/// 今の時間を文字列で取得
    /// </summary>
    /// <returns></returns>
    static std::string GetCurrentTimeString() {
#if _DEBUG
        std::time_t t = std::time(nullptr);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
#endif // _DEBUG
        return std::string();
    }

    /// <summary>
	/// 今のログレベルを文字列で取得
    /// </summary>
    /// <param name="level"></param>
    /// <returns></returns>
    static const char* LevelToString(Level level) {
#if _DEBUG
        switch (level) {
        case Level::Info:  return "INFO";
        case Level::Warn:  return "WARN";
        case Level::Error: return "ERROR";
        }
        return "UNKNOWN";
#endif // _DEBUG
    }

    static inline std::string m_logFileName = "Log/log.txt";
    static inline std::mutex m_mtx;
};
