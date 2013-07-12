#!/usr/bin/perl -w

use warnings;
use strict;

use Test::More qw(no_plan);
use FindBin;
use File::Temp;

my $build_root = $ENV{DESTDIR} || "$ENV{HOME}/prefix";
my $zsm = "$build_root/usr/bin/zsyncmake";

my ($small_data_file_fh, $small_data_file) = File::Temp::tempfile();
my ($medium_data_file_fh, $medium_data_file) = File::Temp::tempfile();

print $small_data_file_fh "hello world\n" x 8;
close $small_data_file_fh;

for (1..1024) {
    print $medium_data_file_fh "hello world\n" x 8;
}
close $medium_data_file_fh;

is(`cat $small_data_file|md5sum`,
   "c206bfeb596f75167b404119249403e1  -\n",
   "test that our small setup file was generated properly",);

like( `$zsm -s $small_data_file -o /dev/stdout -u relative/url/path`,
    qr!
    Filename:\s\w+\s+
    MTime:\s\w+, \s \d+ \s \w+ \s \d+ \s [0-9:]+ \s \+\d+ \s+
    Blocksize: \s 2048 \s+
    Length: \s 96 \s+
    Hash-Lengths: \s 1,2,4 \s+
    URL: \s relative/url/path \s+
    SHA-1: \s \w{40}
    !mx,
      "zsyncmake small_data_file");

