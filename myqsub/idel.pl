package Idel;
use strict;
use warnings;
use Data::Dumper;
local $Data::Dumper::Indent = 1;
local $Data::Dumper::Purity = 1;
local $Data::Dumper::Terse = 1;

my @ids;

sub get_ids {
    my @qs = split(/\n/,`qs`);
    shift @qs;
    die "nojobs" if not @qs;
    my $count = 0;
    for my $l (@qs) {
        print $count . ": ";
        print $l . "\n";

        push( @ids, $1 ) if $l =~/(\d+)\.nqs/;

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
    print "\nreally? ";
    if(chomp(my $in =  <STDIN>) eq 'y' || 'Y' || "\n"){
        print "EXECUTE!\n";
				printf ("arg:%i, ids[num]:%s", $num, $ids[$num]);
        print `qdel -k $ids[$in]`;
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
