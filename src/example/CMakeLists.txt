# Copyright (C) 2016 Marcel Mueller
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.

include_directories(
	${CMAKE_SOURCE_DIR}/include/
)

add_executable(ConsoleHandleExample main.cpp)
target_link_libraries(ConsoleHandleExample ConsoleHandle)
set_target_properties(ConsoleHandleExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)