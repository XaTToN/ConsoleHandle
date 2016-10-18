#include <ConsoleHandle/ConsoleHandle.hpp>

//--------------------------------------------
// ConsoleHandleImpl for Windows based Systems
//--------------------------------------------
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
class ConsoleHandle::ConsoleHandleImpl {
private:
	HANDLE winhandle;
	bool is_fullscreen;
	s2tuple saved_window_size;
	s3tuple font_color;

public:
	ConsoleHandleImpl() :	winhandle(GetStdHandle(STD_OUTPUT_HANDLE)),
							is_fullscreen(false),
							saved_window_size(std::make_tuple(0, 0)),
							font_color(std::make_tuple(0, 0, 0))
	{}

//------------------------------------------------------------
	const bool SetConsoleWindowTitle(const std::string& title) {
		std::string old_title = this->GetConsoleWindowTitle();

		SetConsoleTitle(title.c_str());

		return old_title != this->GetConsoleWindowTitle();
	}

	const std::string GetConsoleWindowTitle() const {
		char WindowTitle[1024];
		memset(WindowTitle, 0, sizeof(WindowTitle));

		GetConsoleTitle(WindowTitle, 1024);
		return WindowTitle;
	}

//------------------------------------------------------------

	const bool SetConsoleWindowSize(const short& x, const short& y) {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		{
			COORD largestSize = GetLargestConsoleWindowSize(this->winhandle);
			if (x > largestSize.X || y > largestSize.Y)
				return false;
		}


		CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
		if (!GetConsoleScreenBufferInfo(this->winhandle, &bufferInfo))
			return false;

		SMALL_RECT& winInfo = bufferInfo.srWindow;
		COORD windowSize = {winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1};

		if (windowSize.X > x || windowSize.Y > y)
		{
			SMALL_RECT info =
			{
				0,
				0,
				x < windowSize.X ? x - 1 : windowSize.X - 1,
				y < windowSize.Y ? y - 1 : windowSize.Y - 1
			};

			if (!SetConsoleWindowInfo(this->winhandle, TRUE, &info))
				return false;
		}

		COORD size = {x, y};
		if (!SetConsoleScreenBufferSize(this->winhandle, size))
			return false;


		SMALL_RECT info = {0, 0, x - 1, y - 1};
		if (!SetConsoleWindowInfo(this->winhandle, TRUE, &info))
			return false;

		return true;
	}

	const s2tuple GetConsoleWindowSize() const {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(this->winhandle, &csbi);
		
		return std::make_tuple(csbi.srWindow.Right - csbi.srWindow.Left + 1,
			csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
	}

//------------------------------------------------------------

	void ToggleFullScreen() {
		if (!this->is_fullscreen) {
			COORD largestSize = GetLargestConsoleWindowSize(this->winhandle);
			this->saved_window_size = this->GetConsoleWindowSize();
			this->SetConsoleWindowSize(largestSize.X, largestSize.Y);
		}
		else {
			this->SetConsoleWindowSize(std::get<0>(this->saved_window_size),
				std::get<1>(this->saved_window_size));
		}

		this->is_fullscreen = !this->is_fullscreen;

		keybd_event(VK_MENU, 0x36, 0, 0);
		keybd_event(VK_RETURN, 0x1C, 0, 0);
		keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	const bool IsFullScreenEnabled() const {
		return this->is_fullscreen;
	}

//------------------------------------------------------------

	const bool SetConsoleFontSize(const short& x, const short& y) {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		CONSOLE_FONT_INFOEX info{sizeof(CONSOLE_FONT_INFOEX)};
		if (!GetCurrentConsoleFontEx(this->winhandle, false, &info))
			return false;
		info.dwFontSize = COORD{x, y};
		return SetCurrentConsoleFontEx(this->winhandle, false, &info) != 0;
	}

	const s2tuple GetConsoleFontSize() {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return std::make_tuple(0, 0);

		CONSOLE_FONT_INFOEX info{sizeof(CONSOLE_FONT_INFOEX)};
		if (!GetCurrentConsoleFontEx(this->winhandle, false, &info))
			return std::make_tuple(0, 0);
		else 
			return std::make_tuple(info.dwFontSize.X, info.dwFontSize.Y);
	}

//------------------------------------------------------------

	const bool SetFontColor(const short& font, const short& bg) {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		short color_attrib = (bg << 4) + font;

		this->font_color = std::make_tuple(font, bg, color_attrib);

		return SetConsoleTextAttribute(this->winhandle, color_attrib) != 0;
	}

	const s3tuple GetFontColor() {
		return this->font_color;
	}

//------------------------------------------------------------

	const bool SetCursorPosition(const short& x, const short& y) {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		COORD pos = {x, y};
		return SetConsoleCursorPosition(this->winhandle, pos) != 0;
	}

	const s2tuple GetCursorPosition() const {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return std::make_tuple(0, 0);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD pos;

		if(!GetConsoleScreenBufferInfo(this->winhandle, &csbi))
			return std::make_tuple(0, 0);

		return std::make_tuple(pos.X, pos.Y);
	}

//------------------------------------------------------------

	const bool ShowCursor(const bool& state) {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = state;

		return SetConsoleCursorInfo(this->winhandle, &info) != 0;
	}

	const bool IsCursorVisible() const {
		if (this->winhandle == INVALID_HANDLE_VALUE)
			return false;

		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(this->winhandle, &info);
		return info.bVisible != 0;
	}

//------------------------------------------------------------

	void Wait(const int& ms_time) {
		Sleep(ms_time);
	}
};
#endif




//--------------------------------------------
// ConsoleHandle Member Functions
//--------------------------------------------
ConsoleHandle::ConsoleHandle()
{}

ConsoleHandle::~ConsoleHandle() 
{}

/*--------------Set-Functions--------------*/

const bool ConsoleHandle::SetConsoleWindowTitle(const std::string& title) {
	return handle->SetConsoleWindowTitle(title);
}

const bool ConsoleHandle::SetConsoleWindowSize(const short& x, const short& y) {
	return handle->SetConsoleWindowSize(x, y);
}

void ConsoleHandle::ToggleFullScreen() {
	handle->ToggleFullScreen();
}

const bool ConsoleHandle::SetConsoleFontSize(const short& x, const short&y ) {
	return handle->SetConsoleFontSize(x, y);
}

const bool ConsoleHandle::SetFontColor(const short& font, const short& bg) {
	return handle->SetFontColor(font, bg);
}

const bool ConsoleHandle::SetCursorPosition(const short& x, const short& y) {
	return handle->SetCursorPosition(x, y);
}

const bool ConsoleHandle::ShowCursor(const bool& state) {
	return handle->ShowCursor(state);
}

void ConsoleHandle::Wait(const int& ms_time) {
	handle->Wait(ms_time);
}

/*--------------Get-Functions--------------*/

const std::string ConsoleHandle::GetConsoleWindowTitle() {
	return handle->GetConsoleWindowTitle();
}

const s2tuple ConsoleHandle::GetConsoleWindowSize() {
	return handle->GetConsoleWindowSize();
}

const bool ConsoleHandle::IsFullScreenEnabled() {
	return handle->IsFullScreenEnabled();
}

const s2tuple ConsoleHandle::GetConsoleFontSize() {
	return handle->GetConsoleFontSize();
}

const s3tuple ConsoleHandle::GetFontColor() {
	return handle->GetFontColor();
}

const s2tuple ConsoleHandle::GetCursorPosition() {
	return handle->GetCursorPosition();
}

const bool ConsoleHandle::IsCursorVisible() {
	return handle->IsCursorVisible();
}

/*--------------Opaque-Pointer-------------*/
std::unique_ptr<ConsoleHandle::ConsoleHandleImpl> ConsoleHandle::handle(new ConsoleHandle::ConsoleHandleImpl);