#!/bin/bash
cd ../Server/bin/Debug
mono Server.exe >> /dev/null &
echo Please wait...
sleep 7
echo __________________
cd ../../../Shell
ruby Shell.rb

