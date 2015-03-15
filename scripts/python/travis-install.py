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

if __name__ == '__main__':
    print('Checking environment variables...')
    try:
        build_type = getenv('BUILD_TYPE')
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)

    clang = 'clang' in cxx

    print('Installing CMake...')
    execute('sudo', './cmake-amd64.sh', '--skip-license', '--prefix=/usr')

    if clang:
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
