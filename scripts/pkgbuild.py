#!/usr/bin/env python
# -*- coding: utf-8 -*-
# pkgbuild.py - generate .pkg files on OS X
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

# This file exists until CPack gets support for pkg files on OS X once more

# Example usage from 'root' project directory:
# python scripts/pkgbuild.py -r package -b build-clang
# From inside build directory:
# python ../scripts/pkgbuild.py -r ../package -b .
# NOTE: This script *MUST* be run after running 'make package'

from __future__ import print_function
from argparse import ArgumentDefaultsHelpFormatter
from argparse import ArgumentParser
from utility import execute
from utility import which
from utility import pushd
from utility import exit
from os.path import abspath
from os.path import join
from string import Template
from shutil import copytree
from shutil import rmtree
from shutil import copy
from json import loads
from sys import exit as error
from os import makedirs
from os import getcwd

# sets up all our program arguments
# placed here for easy reading
def parser ():
    p = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    add = p.add_argument
    add('-r', '--resource', help='resource directory', required=True, type=str)
    add('-b', '--build', help='cmake build directory', required=True, type=str)

    add('--prefix', help='installation prefix', default='/usr/local', type=str)
    return p

def read (path):
    with open(path) as f: return f.read()

def template (path): return Template(read(path))
def load (path): return loads(read(path))

PKG_ROOT = '_CPack_Packages/Darwin/PKG'
SRC_ROOT = '_CPack_Packages/Darwin/STGZ'

if __name__ == '__main__':
    productbuild = which('productbuild')
    pkgbuild = which('pkgbuild')

    p = parser()
    args = p.parse_args()

    config_file = join(abspath(args.resource), 'pkg.json')
    try: data = load(config_file)
    except Exception as e: exit(e)

    template_file = join(abspath(args.resource), data['template'])
    try: distribution = template(template_file)
    except Exception as e: exit(e)

    # Don't care if we error with an exception here
    pkg_identifier = data['identifier']
    pkg_version = data['version']
    pkg_name = '{}-{}'.format(data['package'], pkg_version)

    license_file = join(abspath(args.resource), data['license'])

    build_dir = join(getcwd(), args.build)
    package_dir = join(build_dir, 'package')
    distribution_xml = join(PKG_ROOT, 'distribution.xml')
    resources_dir = join(PKG_ROOT, 'resources')

    pkgbuild_args = [
        pkgbuild,
        '--quiet',
        '--root', join(PKG_ROOT, pkg_name),
        '--identifier', pkg_identifier,
        '--install-location', args.prefix,
        '--version', pkg_version,
        join(PKG_ROOT, '{}.pkg'.format(pkg_name))
    ]

    productbuild_args = [
        productbuild,
        '--quiet',
        '--distribution', distribution_xml,
        '--resource', resources_dir,
        '--package-path', PKG_ROOT,
        '--version', pkg_version,
        join(package_dir, '{}.pkg'.format(pkg_name))
    ]

    with pushd(build_dir):
        rmtree(PKG_ROOT)
        makedirs(PKG_ROOT)
        makedirs(resources_dir)
        copytree(join(SRC_ROOT, pkg_name), join(PKG_ROOT, pkg_name))
        copy(license_file, resources_dir)
        with open(distribution_xml, 'w') as dist:
            dist.write(distribution.safe_substitute(data))
        execute(*pkgbuild_args)
        execute(*productbuild_args)
    #execute(*productbuild_args)
