/*
 Shell.cpp - Interactive shell
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

#include <Arduino.h>

#include "Shell.h"

Shell::Shell(long baud)
{
  Serial.begin(baud);
  firstCommand = NULL;
  lastCommand = NULL;
}

void Shell::add(Command *c)
{
  if(firstCommand == NULL) {
    firstCommand = c;
    lastCommand = c;
  }
  else {
    lastCommand->next = c;
    lastCommand = c;
  }
}

void Shell::run()
{
  String commandString;
  Command *command;
  String parameters;

  while(1) {
    prompt();
    commandString = readCommandString();
    command = getCommand(commandString);
    parameters = getParameters(command, commandString);

    if(command != NULL)
      command->run(parameters);
    Serial.print('\n');
  }
}

void Shell::prompt()
{
  Serial.print("> ");
}

#define MAX_CMD_STR_LEN 32
String Shell::readCommandString()
{
  char buf[MAX_CMD_STR_LEN] = { 0 };
  int buf_index = 0;
  char c;
  while(buf_index < MAX_CMD_STR_LEN-1) {
    if (Serial.available() > 0) {
      c = Serial.read();
      /* backspace */
      if(c == '\b' || c == 127) {
        if(buf_index > 0) {
          Serial.print(c);
          buf_index--;
        }
      }
      /* Exit on <ENTER> key */
      else if(c == '\r' || c == '\n') {
        break;
      }
      else {
        buf[buf_index] = c;
        Serial.print(c);
        buf_index++;
      }
    }
  }
  return String(buf);
}

Command * Shell::getCommand(String commandString)
{
  Command *c = firstCommand;

  while(c != NULL) {
    if(commandString.startsWith(c->getName()))
      return c;
    c = c->next;
  }
  return NULL;
}

String Shell::getParameters(Command *command, String commandString)
{
  String parameters = commandString;
  parameters.remove(0, command->getName().length());
  parameters.trim();
  return parameters;
}
