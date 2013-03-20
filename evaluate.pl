#!/usr/bin/perl -w

use strict;

my $CFLAGS = "-w -DNDEBUG -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables";

my %comps = (
    "gcc-4.4" => "gcc-4.4",
    "gcc-4.5" => "gcc-4.5",
    "gcc-4.6" => "gcc-4.6",
    "gcc-4.7" => "gcc-4.7",
    "/home/regehr/z/compiler-install/gcc-r196703-install/bin/gcc" => "gcc-trunk",

    "/home/regehr/z/clang+llvm-3.0-x86_64-linux-Ubuntu-11_10/bin/clang" => "clang-3.0",
    "/home/regehr/z/clang+llvm-3.1-x86_64-linux-ubuntu_12.04/bin/clang" => "clang-3.1",
    "/home/regehr/z/clang+llvm-3.2-x86_64-linux-ubuntu-12.04/bin/clang" => "clang-3.2",
    "/home/regehr/z/compiler-install/llvm-r177225-install/bin/clang" => "clang-trunk",

    "icc" => "icc-12.0.5",
    "/opt/intel/bin/icc" => "icc-13.1.0",
    );

my @opts = (
    "-O0",
    "-O1",
    "-O2",
    "-Os",
    "-O3",
    );

my @progs = (
    "pascal",
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
    "davidl",
    "davidl_2",
    "bastian",
    "ethan_2",
    "renaud",
    "robert_2",
    "till",
    "greg",
    "ben",
    "davide",
    "sidney",
    "libc",
    );

my @outfs = ();
foreach my $comp (keys %comps) {
    system "rm -f *.o";
    foreach my $opt (@opts) {
	my $ofiles = "";
	foreach my $prog (@progs) {
	    my $ofile = "${prog}.o";
	    $ofiles .= "$ofile ";
	    my $cfile = "${prog}.c";
	    my $cmd = "$comp $CFLAGS $opt -c $cfile";
	    system $cmd;
	}
	system "$comp $CFLAGS $opt str2long_test.c $ofiles -o str2long_test -lm -lrt";
	my $outf = "speed-$comps{$comp}$opt.txt";
	print "$outf\n";
	push @outfs, $outf;
	system "./str2long_test > $outf";
    }
}

my %fastest;
my %fastest_comp;

foreach my $outf (@outfs) {
    open INF, "<$outf" or die;
    while (my $line = <INF>) {
	if ($line =~ /([0-9\.]+) (.*)/) {
	    my $t = $1;
	    my $who = $2;
	    if (!defined($fastest{$who}) ||
		$t < $fastest{$who}) {
		$fastest{$who} = $t;
		$fastest_comp{$who} = $outf;
	    }	    
	}
    }
    close INF;
}

open OF, "| sort -n >speed.txt" or die;
foreach my $prog (@progs) {
    print OF "$fastest{$prog} $fastest_comp{$prog} $prog\n";
}
close OF;

system "rm -f *.o";

open OF, "| sort -n >size.txt" or die;
foreach my $prog (@progs) {
    my $smallest = 999999;
    my $small_opt;
    my $small_comp;
    my $ofile = "${prog}.o";
    my $cfile = "${prog}.c";
    foreach my $comp (keys %comps) {
	foreach my $opt (@opts) {
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
    print OF "$smallest $small_comp $small_opt $prog\n";
}
close OF;
