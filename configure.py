#!/usr/bin/python3

"""Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

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
"""

import configparser
import re

VERSION_H = 'include/kernel/version.h'

"""
Replaces the matched contents of a file with the given string.
Arguments:
    filename: The file to open
    search: The search pattern
    pattern: The pattern to replace
    string: The string to replace with
"""
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

"""
The main function of this script.
"""
if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read('ritchie.conf')

    # Read in the options
    (name, version, codename) = [pair[1] for pair in config.items('Version')]
    (arch, image, device) = [pair[1] for pair in config.items('Build')]

    version_string = "\""+ name +" "+ version +" ("+codename+")\""

    # Update the GRUB configuration file
    print('Updating \'grub.cfg\'... ', end='')
    update('grub.cfg', 'menuentry', r'\".*\"', version_string)
    update('grub.cfg', 'multiboot', r'/boot/.*$', '/boot/'+image)
    print('DONE')

    # Update kernel version header file
    print('Updating \''+ VERSION_H +'\'... ', end='')
    update(VERSION_H, 'PROJECT', r'PROJECT \".*\"', 'PROJECT \"'+ name +'\"')
    update(VERSION_H, 'VERSION', r'VERSION \".*\"', 'VERSION \"'+ version +'\"')
    update(VERSION_H, 'CODENAME', r'CODENAME \".*\"', 'CODENAME \"'+ codename +'\"')
    print('DONE')

    # Update make.conf file
    print('Updating \'make.conf\'... ', end='')
    update('make.conf', 'ARCH', r'ARCH = .*', 'ARCH = '+ arch)
    update('make.conf', 'IMAGE', r'IMAGE = .*', 'IMAGE = '+ image)
    update('make.conf', 'DEVICE', r'DEVICE = .*', 'DEVICE = '+ device)
    print('DONE')
