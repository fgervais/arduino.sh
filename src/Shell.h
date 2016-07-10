/*
  Shell.h - Interactive shell
  Copyright (c) 2016 François Gervais. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Shell_h
#define Shell_h

class Command
{
public:
  Command(String name, uint8_t (*function)(void *data)) {
    this->name = name;
    this->function = function;
    next = NULL;
  };
  String getName() { return name; };
  void run(void *parameters) { function(parameters); };
  Command *next;
private:
  String name;
  uint8_t (*function)(void *parameters);
};

class Shell
{
public:
  Shell(long baud);
  void add(Command *c);
  void run();
private:
  Command *firstCommand;
  Command *lastCommand;
   
  void prompt();
  String readCommandString();
  Command * getCommand(String commandString);
};

#endif
