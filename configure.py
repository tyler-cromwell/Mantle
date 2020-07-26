#!/usr/bin/python3

######################################################################
# Copyright (C) 2015-Present Tyler Cromwell <tjc6185@gmail.com>
#
# This file is part of Mantle.
#
# Mantle is free software: you can redistribute it and/or modify
# it under Version 2 of the terms of the GNU General Public License
# as published by the Free Software Foundation.
#
# Mantle is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Mantle.
# If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
######################################################################

import configparser
import getopt
import os
import re
import subprocess
import sys

ARCHES = [
    'amd64'
]

CMDS = [
    'clean'
]

CONF = 'mantle.conf'

FILES = {
    'grub' : 'grub.cfg',
    'make' : 'Makefile'
}


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
    for k in FILES.keys():
        print('Resetting \'', FILES[k], '\'... ', sep='', end='')
        os.system('git checkout '+ FILES[k])
        print('DONE')


# Prints the command usage along with
# supported architectures and commands
def print_usage():
    print('usage: ./configure.py [-a arch] [-c cmd]')
    print('  -h --help\t\tPrints this text')
    print('  -a --arch\t\tThe target architecture')
    print('  -c --cmd \t\tSpecific command')
    print()
    print('Architectures:', ARCHES)
    print('Subcommands:', CMDS)
    sys.exit()


# The main function of this script.
if __name__ == "__main__":
    arch = ''
    cmd = ''

    try:
        short_opts = 'ha:c:'
        long_opts = ['help', 'arch=', 'cmd=']
        opts, args = getopt.getopt(sys.argv[1:], short_opts, long_opts)
    except getopt.GetoptError as error:
        print('Invalid argument: \"', str(error), '\"\n')
        print_usage()

    if len(opts) == 0:
        print_usage()

    for o, a in opts:
        if o == '-h' or o == '--help':
            print_usage()
        elif o == '-a' or o == '--arch':
            arch = a
        elif o == '-c' or o == '--cmd':
            cmd = a

    if cmd == 'clean':
        clean()

    elif arch in ARCHES:
        config = configparser.ConfigParser()
        config.read(CONF)

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
        print('Updating \''+ FILES['grub'] +'\'... ', end='')
        update(FILES['grub'], 'menuentry', r'\".*\"', version_string)
        update(FILES['grub'], 'multiboot', r'/boot/.*$', '/boot/'+image_string)
        print('DONE')

        # Update Makefile
        print('Updating \''+ FILES['make'] +'\'... ', end='')
        update(FILES['make'], 'NAME', r'NAME = .*', 'NAME = '+ name)
        update(FILES['make'], 'VERSION', r'VERSION = .*', 'VERSION = '+ version + branch)
        update(FILES['make'], 'CODENAME', r'CODENAME = .*', 'CODENAME = '+ codename)
        update(FILES['make'], 'IMAGE', r'IMAGE = .*', 'IMAGE = '+ image_string)
        print('DONE')
