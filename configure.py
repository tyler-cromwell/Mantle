#!/usr/bin/python3

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
  Copyright (C) 2015 Tyler Cromwell <tjc6185@gmail.com>

  This file is part of Ritchie.

  Ritchie is free software: you can redistribute it and/or modify
  it under Version 2 of the terms of the GNU General Public License
  as published by the Free Software Foundation.

  Ritchie is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Ritchie.
  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

import configparser
import os
import re
import subprocess
import sys

FILES = ['grub.cfg', 'include/kernel/version.h', 'make.conf', 'ritchie.conf']
ARCHES = ['amd64']



# Replaces the matched contents of a file with the given string.
# Arguments:
#     filename: The file to open
#     search: The search pattern
#     pattern: The pattern to replace
#     string: The string to replace with
def update(filename, search, pattern, string):
    lines = []

    # Read in the contents
    with open(filename, 'r') as conf:
        lines = conf.readlines()

    # Find the line to edit
    for n, line in enumerate(open(filename)):
        result = re.search(search, line)

        # Replace the matched pattern
        if result != None:
            new = re.sub(pattern, string, line)
            lines[n] = new

    # Writeback
    with open(filename, 'w') as conf:
        conf.writelines(lines)


# Simply runs 'git checkout' on the configuration files
# to restore them to their template form.
#
# This is I don't have to type it myself every time.
def clean():
    for f in FILES[:-1]:
        print('Resetting \''+ f +'\'... ', end='')
        os.system('git checkout '+f)
        print('DONE')


# The main function of this script.
if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print('usage: ./configure.py [arch | cmd]')
        print('    arch = [\'amd64\'')
        print('    cmd = [\'clean\'')
        sys.exit()

    elif sys.argv[1] == 'clean':
        clean()

    elif sys.argv[1] in ARCHES:
        arch = sys.argv[1]
        config = configparser.ConfigParser()
        config.read(FILES[3])

        # Read in the options
        (name, version, codename) = [pair[1] for pair in config.items('Version')]

        # Get latest release tag
        release = subprocess.getoutput('git describe --tags --abbrev=0')

        # Get current branch name
        branch = subprocess.getoutput('git rev-parse --abbrev-ref HEAD')

        if branch == 'master':
            branch = ''
        else:
            branch = '-'+ branch

        # Construct version and binary image strings
        version_string = "\""+ name +" "+ version + branch +" ("+codename+")\""
        image_string = name.lower() +"-v"+ release + branch

        # Update the GRUB configuration file
        print('Updating \''+ FILES[0] +'\'... ', end='')
        update(FILES[0], 'menuentry', r'\".*\"', version_string)
        update(FILES[0], 'multiboot', r'/boot/.*$', '/boot/'+image_string)
        print('DONE')

        # Update kernel version header file
        print('Updating \''+ FILES[1] +'\'... ', end='')
        update(FILES[1], 'PROJECT', r'PROJECT \".*\"', 'PROJECT \"'+ name +'\"')
        update(FILES[1], 'VERSION', r'VERSION \".*\"', 'VERSION \"'+ version + branch +'\"')
        update(FILES[1], 'CODENAME', r'CODENAME \".*\"', 'CODENAME \"'+ codename +'\"')
        print('DONE')

        # Update make.conf file
        print('Updating \''+ FILES[2] +'\'... ', end='')
        update(FILES[2], 'ARCH', r'ARCH = .*', 'ARCH = '+ arch)
        update(FILES[2], 'IMAGE', r'IMAGE = .*', 'IMAGE = '+ image_string)
        print('DONE')

    else:
        print('Architectures:', ARCHES)
        print('Subcommands:', ['clean'])
