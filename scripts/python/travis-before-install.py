#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from utility import make_executable
from utility import DownloadError
from utility import EnvironError
from utility import download
from utility import execute
from utility import getenv
from utility import which
from utility import exit

cmake_script_url = 'http://cmake.org/files/v3.1/cmake-3.1.3-Linux-x86_64.sh'
llvm_key_url = 'http://llvm.org/apt/llvm-snapshot.gpg.key'

repository = {
    '3.3': 'ppa:h-rayflood/llvm',
    '3.4': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.4 main',
    '3.5': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.5 main',
    '3.6': 'deb http://llvm.org/apt/precise llvm-toolchain-precise-3.6 main',
}

# Used to build libcxx when USE_LIBCXX=ON
libcxx_svn = {
    '3.3': 'http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_33/final/',
    '3.4': 'http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_342/final/',
    '3.5': 'http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_350/final/',
    '3.6': 'http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_360/final/'
}

backport = 'ppa:ubuntu-toolchain-r/test'

if __name__ == '__main__':
    print('Checking environment variables...')
    try:
        use_libcxx = getenv('USE_LIBCXX')
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)

    libcxx = use_libcxx == 'ON'
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
        try: download(LLVM_KEY_URL, 'llvm.key')
        except DownloadError as e: exit(e)

        # add llvm key to apt
        print('Adding LLVM Key to APT...')
        execute('sudo', 'apt-key', 'add', 'llvm.key')

        # add the appropriate 'PPA'
        print('Adding Clang APT Repository...')
        execute('sudo', 'add-apt-repository', '--yes', repository[version])

        if libcxx:
            # checkout libcxx if use_libcxx is set to ON
            print('Checking out libcxx...')
            execute('svn', 'co', libcxx_svn[version])
            try: mkdir('libcxx-build')
            except OSError as e: exit(e)

    print('Removing system provided cmake...')
    execute('sudo', 'apt-get', 'purge', 'cmake')

    print('Removing system provided gcc...')
    execute('sudo', 'update-alternatives', '--remove-all', 'gcc')

    print('Removing system provided g++...')
    execute('sudo', 'update-alternatives', '--remove-all', 'g++')

    print('Updating APT...')
    execute('sudo', 'apt-get', 'update')
