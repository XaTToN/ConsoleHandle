#ifndef __CONSOLE_HANDLE_HPP__
#define __CONSOLE_HANDLE_HPP__

#include <string>
#include <tuple>
#include <memory>

typedef std::tuple<short, short> s2tuple;
typedef std::tuple<short, short, short> s3tuple;

class ConsoleHandle {
private:
	ConsoleHandle(const ConsoleHandle&) = delete;
	ConsoleHandle();
	~ConsoleHandle();
public:

/*--------------Set-Functions--------------*/
	static const bool			SetConsoleWindowTitle(const std::string& title);
	static const bool			SetConsoleWindowSize(const short& x, const short& y);
	static void					ToggleFullScreen();
	static const bool			SetConsoleFontSize(const short& x, const short& y);
	static const bool			SetFontColor(const short& font, const short& bg);
	static const bool			SetCursorPosition(const short& x, const short& y);
	static const bool			ShowCursor(const bool& state);
	static void					Wait(const int& ms_time);

/*--------------Get-Functions--------------*/
	static const std::string	GetConsoleWindowTitle();
	static const s2tuple		GetConsoleWindowSize();
	static const bool			IsFullScreenEnabled();
	static const s2tuple		GetConsoleFontSize();
	static const s3tuple		GetFontColor();
	static const s2tuple		GetCursorPosition();
	static const bool			IsCursorVisible();

/*--------------Opaque-Pointer-------------*/
private:
	class ConsoleHandleImpl;
	static std::unique_ptr<ConsoleHandleImpl> handle;
};

#endif //__CONSOLE_HANDLE_HPP__