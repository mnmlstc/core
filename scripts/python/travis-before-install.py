#!/usr/bin/env python
# -*- coding: utf-8 -*-

# travis-before-install.py - build script
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

from __future__ import print_function
from utility import make_executable
from utility import DownloadError
from utility import EnvironError
from utility import download
from utility import execute
from utility import getenv
from utility import which
from utility import exit
from os import mkdir

cmake_script_url = 'http://cmake.org/files/v3.1/cmake-3.1.3-Linux-x86_64.sh'
llvm_key_url = 'http://llvm.org/apt/llvm-snapshot.gpg.key'

repository = {
    '3.4': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.4 main',
    '3.5': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.5 main',
    '3.6': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.6 main',
}

backport = 'ppa:ubuntu-toolchain-r/test'

if __name__ == '__main__':
    print('Checking environment variables...')
    try:
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)

    clang = 'clang' in cxx

    # download the cmake install script, mark executable
    print('Downloading CMake Installer...')
    try: download(cmake_script_url, 'cmake-amd64.sh')
    except DownloadError as e: exit(e)
    make_executable('cmake-amd64.sh')

    # add gcc to repos
    print('Adding GCC Repository...')
    execute('sudo', 'add-apt-repository', '--yes', backport)

    if clang:
        # download the llvm package key for the repositories
        print('Downloading LLVM Key...')
        try: download(llvm_key_url, 'llvm.key')
        except DownloadError as e: exit(e)

        # add llvm key to apt
        print('Adding LLVM Key to APT...')
        execute('sudo', 'apt-key', 'add', 'llvm.key')

        # add the appropriate 'PPA'
        print('Adding Clang APT Repository...')
        execute('sudo', 'add-apt-repository', '--yes', repository[version])

    print('Removing system provided cmake...')
    execute('sudo', 'apt-get', '-qq', 'purge', 'cmake')

    print('Removing unnecessary packages')
    execute('sudo', 'apt-get', '-qq', 'autoremove')

    print('Updating APT...')
    execute('sudo', 'apt-get', '-qq', 'update')
