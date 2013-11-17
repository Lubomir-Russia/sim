#!/usr/bin/env perl
use strict;
use warnings;
my $dir = $ARGV[0];
opendir(DIR, $dir);
chdir($dir);
my @files = grep {-f "$dir/$_"} sort { (stat($a))[10] <=> (stat($b))[10] } (readdir(DIR));
print join("\n",@files);
