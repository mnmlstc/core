#!/usr/bin/env python
# -*- coding: utf-8 -*-

# travis-install.py - build script
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

from __future__ import print_function
from subprocess import Popen as process
from subprocess import PIPE
from utility import LocateError
from utility import execute
from utility import getenv
from utility import which
from utility import exit
from os.path import normcase as normalize
from os.path import isdir
from os.path import join
from os import getcwd
from os import pathsep
from os import listdir

def paths ():
    args = ['g++', '-Wp,-v', '-x', 'c++', '-', '-fsyntax-only']
    proc = process(args, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    out, err = proc.communicate()
    paths = []
    for line in out:
        if not isdir(line): continue
        paths.append(normalize(line))
    # we only want the first two
    return paths[:2]

if __name__ == '__main__':
    print('Checking environment variables...')
    try:
        build_type = getenv('BUILD_TYPE')
        use_libcxx = getenv('USE_LIBCXX')
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)

    libcxx = use_libcxx == 'ON'
    clang = 'clang' in cxx

    print('Installing CMake...')
    execute('sudo', './cmake-amd64.sh', '--skip-license', '--prefix=/usr')

    if clang:
        # Needed, so that we can insure
        print('Installing Compiler Dependencies...')
        execute('sudo', 'apt-get', 'install', '-qq', 'g++-4.9')

    print('Installing Compiler...')
    arguments = [
        'sudo',
        'apt-get',
        'install',
        '-qq',
        '{}-{}'.format(cxx, version)
    ]
    execute(*arguments)

    if libcxx:
        current = getcwd()
        arguments = [
            join(current, 'libcxx'), # directory
            '-DCMAKE_INSTALL_PREFIX=/usr',
            '-DLIBCXX_CXX_ABI=libsupc++',
            '-DCMAKE_BUILD_TYPE={}'.format(build_type),
            '-DLIBCXX_LIBSUPCXX_INCLUDE_PATHS={}'.format(';'.join(paths())),
            '-DCMAKE_CXX_COMPILER={}'.format(which('clang++')),
            '-DCMAKE_C_COMPILER={}'.format(which('clang'))
        ]

        print('Building libc++')
        try: cmake = which('cmake')
        except LocateError as e: exit(e)
        arguments.insert(0, cmake)
        execute(*arguments, cwd='libcxx-build')
        execute(cmake, '--build', 'libcxx-build', '--', '-j8')
        execute('sudo', cmake, '--build', 'libcxx-build', '--target', 'install')
