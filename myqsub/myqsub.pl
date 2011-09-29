use warnings;
use strict;
use Data::Dumper;

sub say {
    print shift . "\n";
}
my $qs = <<"EOF"
 Que      User     Request-id  Status      Cpu(  lP x lp)   Memory(     lm x  lP)     Cputime     Elapse(       cp)
 gh10034  a0064164 1520415.nqs RUNNING       0(  64 x  1)       0M(  1800M x  64)    00:00:00   00:00:00( 00:10:00)
 gh10034  a0064164 1520416.nqs RUNNING      16(  64 x  1)     244M(  1800M x  64)    00:01:20   00:00:06( 00:10:00)
EOF
;
my @ret = ();

my $str = `ls`;
my @str = split(/\n/, $qs);

print Dumper [@str];

foreach my $l (@str){
    push ( @ret, $1 ) if $l =~ /.*(\d{7})\.nqs.*/;
}

print Dumper @ret;

sub _dispatch {
    @args = @ARGV;
}

_dispatch();
