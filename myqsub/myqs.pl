#!/bin/env/perl
package User;
use strict;
use warnings;
use Data::Dumper;
local $Data::Dumper::Indent = 1;
local $Data::Dumper::Purity = 1;
local $Data::Dumper::Terse = 1;

my @ids;

open ( my $file, ">", ".qs");
my @qs = split(/\n/,`ls -l`);
my $count = 0;
for my $l (@qs){
    if( $count>0 ){
        print $file $count . ": "
    }
    print $file $l . "\n";

    push( @ids, $1 ) if $l =~/(\d\d:\d\d)/;

    $count++;
}

print Dumper [@ids];

