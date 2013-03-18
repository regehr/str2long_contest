#!/usr/bin/perl -w

use strict;

my $CFLAGS = "-m32 -w -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables";

my @comps = (
    "gcc-4.4",
    "gcc-4.5",
    "gcc-4.6",
    "gcc-4.7",
    "/home/regehr/z/compiler-install/gcc-r196703-install/bin/gcc",

    "/home/regehr/z/clang+llvm-3.0-x86_64-linux-Ubuntu-11_10/bin/clang",
    "/home/regehr/z/clang+llvm-3.1-x86_64-linux-ubuntu_12.04/bin/clang",

    "/home/regehr/z/compiler-install/llvm-r177225-install/bin/clang",
    );

my @opts = (
    "-O0",
    "-O1",
    "-O2",
    "-Os",
    "-O3",
    );

my @progs = (
    "john",
    "pascal",
    "toby",
    "bernd_2",
    "francois_2",
    "yolanpa",
    "patrick",
    "daniel_2",
    "phil",
    "mikael_2",
    "ryan",
    "matthew_3",
    "stefan",
    "david_2",
    "mats",
    "peter",
    "thomas",
    "mattias",
    "matthewf_2",
    "kevin",
    "chucky_2",
    "andrew",
    "tordek_2",
    "jeffrey",
    "yang_2",
    "olivier",
    "ken_4",
    "davidl",
    "davidl_2",
    "bastian",
    "ethan_2",
    "renaud",
    "robert_2",
    "till",
    "tennessee",
    "greg",
    "ben",
    "davide",
    "sidney",
    );

foreach my $prog (@progs) {
    print "$prog: ";
    my $smallest = 999999;
    my $small_opt;
    my $small_comp;
    foreach my $comp (@comps) {
	foreach my $opt (@opts) {
	    my $ofile = "${prog}.o";
	    my $cfile = "${prog}.c";
	    my $cmd = "$comp $CFLAGS $opt -c $cfile";
	    system "rm -f $ofile";
	    system $cmd;
	    open INF, "size -A $ofile |" or die;
	    my $found = 0;
	    while (my $line = <INF>) {
		if ($line =~ /\.text\s+([0-9]+)\s/) {
		    my $size = $1;
		    $found = 1;
		    if ($size < $smallest) {
			$smallest = $size;
			$small_opt = $opt;
			$small_comp = $comp;
		    }
		}
	    }
	    die unless ($found);
	}
    }
    print "$small_comp $small_opt = $smallest\n";
}
