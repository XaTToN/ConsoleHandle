/*
* <ConsoleHandle>
* Copyright (C) 2016 Marcel Mueller
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <ConsoleHandle/ConsoleHandle.hpp>

#include <iostream>
#include <conio.h>
#include <iomanip>

int main() {
	setlocale(LC_ALL, "");

	ConsoleHandle::SetConsoleWindowTitle("Hello World");
	ConsoleHandle::SetConsoleFontSize(12, 16);
	ConsoleHandle::ShowCursor(false);
	ConsoleHandle::ToggleFullScreen();

	ConsoleHandle::Wait(250);

	int x, y;
	std::tie(x, y) = ConsoleHandle::GetConsoleWindowSize();
	std::cout << "Resolution: " << x << "x" << y << std::endl;

	std::cout << "Press f to toggle fullscreen, ESC to continue" << std::endl;

	bool exit = false;
	while (!exit) {
		if (_kbhit()) {
			char c = _getch();
			switch (c) {
				case 27:	exit = true;			 break;
				case 'f':	ConsoleHandle::ToggleFullScreen(); ConsoleHandle::Wait(50); std::cout << "Toggled!" << std::endl;
			}
		}
	}

	ConsoleHandle::SetCursorPosition(0, 0);

	unsigned char colorBackground = 0x00;
	unsigned char colorForeground = 0x00;

	std::cout << std::setbase(16);

	for (colorBackground = 0; colorBackground < 16; colorBackground++) {
		for (colorForeground = 0; colorForeground < 16; colorForeground++) {

			//set front and back colors
			ConsoleHandle::SetFontColor(colorForeground, colorBackground);

			//output the color code
			std::cout << std::setfill('0')
				<< std::setw(2)
				<< std::uppercase
				<< static_cast<int>(std::get<2>(ConsoleHandle::GetFontColor()))
				<< ' ';                       
		}
		std::cout << '\n';
	}

	std::cin.get();
}
