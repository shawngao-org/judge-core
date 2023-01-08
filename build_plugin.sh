#!/bin/bash

read -p "Enter your plugin resource code file name (eg. filename1): " plugin_name
echo -e "\e[33mCompiling with gcc..."
eval "gcc -c -fpic $plugin_name"".c"" -lseccomp"
if [ $? -ne 0 ]; then
  echo -e "\e[31mCompile failed."
  exit 1
fi
echo -e "\e[32mCompiled"
echo -e "\e[33mBuilding plugin with gcc..."
eval "gcc -shared -lc -lseccomp -o $plugin_name"".so"" $plugin_name"".o"""
if [ $? -ne 0 ]; then
  echo -e "\e[31mBuild failed."
  exit 1
fi
echo -e "\e[32mBuild finished."
