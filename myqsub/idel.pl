package Idel;
use strict;
use warnings;
use Data::Dumper;
local $Data::Dumper::Indent = 1;
local $Data::Dumper::Purity = 1;
local $Data::Dumper::Terse = 1;

my @ids;

sub get_ids {
    my @qs = split(/\n/,`ls -l`);
    shift @qs;
    my $count = 0;
    for my $l (@qs) {
        print $count . ": ";
        print $l . "\n";

        push( @ids, $1 ) if $l =~/(\d\d:\d\d)/;

        $count++;
    }
}

sub input {
    chomp ( my $in = <STDIN> );
    @ARGV = split(/\s/, $in);
}


sub myqdel {
    my $num = shift;
    printf ("arg:%i, ids[num]:%s", $num, $ids[$num]);
    print "\nreally?\n";
    if(chomp(my $in =  <STDIN>) eq 'y' || 'Y' || "\n"){
        printf ("EXECUTE! arg:%i, ids[num]:%s", $num, $ids[$num]);
    }
}

sub main {
    get_ids();
    input() if not $ARGV[0];
    foreach my $i (@ARGV){
        myqdel($i);
    }
}

main();
