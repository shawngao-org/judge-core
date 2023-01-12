#!/bin/bash

function feedback() {
  # shellcheck disable=SC2181
  if [ $? -eq 0 ]; then
    echo -e "\e[32mOK !"
  else
    echo -e "\e[31mFailed !"
    exit 1
  fi
}

function make_project() {
    echo -e "\e[33mMaking the project..."
    make
    feedback
}

function generate_make_file() {
  echo -e "\e[33mGenerating Makefile..."
  cmake .
  feedback
}

function ubuntu_debian_sys() {
  if dpkg -s gcc >/dev/null 2>&1; then
      echo -e "\e[32mgcc is installed."
  else
      echo -e "\e[33mUpdating apt-get..."
      sudo apt-get update
      feedback
      echo -e "\e[33mInstalling gcc..."
      sudo apt-get install -y gcc
      feedback
  fi

  if dpkg -s cmake >/dev/null 2>&1; then
      echo -e "\e[32mCMake is installed."
  else
      echo -e "\e[33mUpdating apt-get..."
      sudo apt-get update
      feedback
      echo -e "\e[33mInstalling cmake..."
      sudo apt-get install -y cmake
      feedback
  fi

  if dpkg -s libseccomp2 >/dev/null 2>&1; then
      echo -e "\e[32mlibseccomp2 is installed."
  else
      echo -e "\e[33mUpdating apt-get..."
      sudo apt-get update
      feedback
      echo -e "\e[33mInstalling libseccomp2..."
      sudo apt-get install -y libseccomp2
      feedback
  fi

  if dpkg -s libseccomp-dev >/dev/null 2>&1; then
      echo -e "\e[32mlibseccomp-dev is installed."
  else
      echo -e "\e[33mUpdating apt-get..."
      sudo apt-get update
      feedback
      echo -e "\e[33mInstalling libseccomp-dev..."
      sudo apt-get install -y libseccomp-dev
      feedback
  fi
  generate_make_file
  make_project
}

function centos_sys() {
  if rpm -q gcc >/dev/null 2>&1; then
      echo -e "\e[32mgcc is installed."
  else
      echo -e "\e[33mInstalling gcc..."
      sudo yum install -y gcc
      feedback
  fi

  if rpm -q cmake >/dev/null 2>&1; then
      echo -e "\e[32mCMake is installed."
  else
      echo -e "\e[33mInstalling cmake..."
      sudo yum install -y cmake
      feedback
  fi

  if rpm -q libseccomp2 >/dev/null 2>&1; then
      echo -e "\e[32mlibseccomp2 is installed."
  else
      echo -e "\e[33mInstalling libseccomp2..."
      sudo yum install -y libseccomp2
      feedback
  fi

  if rpm -q libseccomp-devel >/dev/null 2>&1; then
      echo -e "\e[32mlibseccomp-devel is installed."
  else
      echo -e "\e[33mInstalling libseccomp-devel..."
      sudo yum install -y libseccomp-devel
      feedback
  fi
  generate_make_file
  make_project
}

name=$(grep -oP '^NAME=\K.+' /etc/os-release)
version=$(grep -oP '^VERSION_ID=\K.+' /etc/os-release)

echo "The distribution you are using is: $name"
echo "Version: $version"

case "$name" in
  *"Ubuntu"*) ubuntu_debian_sys;;
  *"Debian"*) ubuntu_debian_sys;;
  *"CentOS"*) centos_sys;;
  *) echo -e "\e[31mThis is an unsupported distribution.";;
esac
