#!/usr/bin/perl -w

use feature qw/say/;
use strict;
use warnings;

my $argc = $#ARGV + 1;

if ($argc > 0) {
    for (my $i = 0; $i < $argc; $i++) {
        my $readme = "$ARGV[$i]/README.md";

        # Create directory
        if (not -e $ARGV[$i]) {
            mkdir $ARGV[$i] or die "$!\n";
            say "Created directory \"$ARGV[$i]\".";
        } else {
            say "Directory \"$ARGV[$i]\" already exists.";
        }

        # Create and Open README.md
        if (not -e "$readme") {
            open FILE, ">$readme" or die "$!\n";

            # Write skeleton info to README.md
            print FILE "# ". ucfirst ($ARGV[$i]) ."\n";
            print FILE "This directory contains\n\n";

            print FILE "## Hierarchy\n";
            print FILE "- `` - Contains\n";

            close FILE;
            say "Created \"$readme\".";
        }
    }
}
else {
    say "No directories given...";
    exit;
}
