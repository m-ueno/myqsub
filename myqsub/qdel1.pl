#!/bin/env perl
package User;
use strict;
use warnings;
use Data::Dumper;
local $Data::Dumper::Indent = 1;
local $Data::Dumper::Purity = 1;
local $Data::Dumper::Terse = 1;

my @ids;

sub get_ids {
    my @qs = split(/\n/,`qs`);
    return if $qs[0] == /no jobs/;
    for my $l (@qs) {
        push( @ids, $1 ) if $l =~/(\d+)\.nqs/;
    }
}

sub main {
    get_ids();
    print Dumper [@ids];
    die if not @ids;
    my $i = pop(@ids);
    `qdel -k $i`;
}

main();
