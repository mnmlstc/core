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
# NOTE: This script is run as part of 'make dist' within the MNMLSTC Core
# build system.

from __future__ import print_function
from contextlib import contextmanager
from argparse import ArgumentDefaultsHelpFormatter
from argparse import ArgumentParser
import subprocess
import shutil
import string
import json
import sys
import os

class LocateError(Exception):
    def __init__ (self, path): self.path = path
    def __str__ (self): return 'Could not locate {}'.format(self.path)

def exit (error): sys.exit(str(error))

@contextmanager
def pushd (directory):
    if not os.path.exists(directory): raise LocateError(directory)
    old = os.getcwd()
    os.chdir(directory)
    yield
    os.chdir(old)

def execute(*command):
    proc = subprocess.Popen(command, universal_newlines=True)
    _, _ = proc.communicate()
    code = proc.returncode
    if not code: return code
    sys.exit(code)

def which (name):
  path, _ = os.path.split(name)
  if path and os.access(path, os.X_OK): return name
  for path in os.environ['PATH'].split(os.pathsep):
    test = os.path.join(path, name)
    if os.access(test, os.X_OK): return test
  raise LocateError(name)

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

def template (path): return string.Template(read(path))
def load (path): return json.loads(read(path))

PKG_ROOT = '_CPack_Packages/Darwin/PKG'
SRC_ROOT = '_CPack_Packages/Darwin/STGZ'

if __name__ == '__main__':
    productbuild = which('productbuild')
    pkgbuild = which('pkgbuild')

    p = parser()
    args = p.parse_args()

    resource = os.path.abspath(args.resource)

    config_file = os.path.join(resource, 'pkg.json')
    try: data = load(config_file)
    except Exception as e: exit(e)

    template_file = os.path.join(resource, data['template'])
    try: distribution = template(template_file)
    except Exception as e: exit(e)

    # Don't care if we error with an exception here
    pkg_identifier = data['identifier']
    pkg_version = data['version']
    pkg_name = '{}-{}'.format(data['package'], pkg_version)

    license_file = os.path.join(resource, data['license'])

    build_dir = os.path.join(os.getcwd(), args.build)
    package_dir = os.path.join(build_dir, 'package')
    distribution_xml = os.path.join(PKG_ROOT, 'distribution.xml')
    resources_dir = os.path.join(PKG_ROOT, 'resources')

    pkgbuild_args = [
        pkgbuild,
        '--quiet',
        '--root', os.path.join(PKG_ROOT, pkg_name),
        '--identifier', pkg_identifier,
        '--install-location', args.prefix,
        '--version', pkg_version,
        os.path.join(PKG_ROOT, '{}.pkg'.format(pkg_name))
    ]

    productbuild_args = [
        productbuild,
        '--quiet',
        '--distribution', distribution_xml,
        '--resource', resources_dir,
        '--package-path', PKG_ROOT,
        '--version', pkg_version,
        os.path.join(package_dir, '{}.pkg'.format(pkg_name))
    ]

    with pushd(build_dir):
        if os.path.exists(PKG_ROOT): shutil.rmtree(PKG_ROOT)
        os.makedirs(PKG_ROOT)
        os.makedirs(resources_dir)
        shutil.copytree(
          os.path.join(SRC_ROOT, pkg_name),
          os.path.join(PKG_ROOT, pkg_name))
        shutil.copy(license_file, resources_dir)
        with open(distribution_xml, 'w') as dist:
            dist.write(distribution.safe_substitute(data))
        execute(*pkgbuild_args)
        execute(*productbuild_args)
