package Utils;

use strict qw(vars);
use vars qw(@ISA @EXPORT);

@ISA = qw(Exporter);

require Exporter;

@EXPORT = qw(initOutputDebug outputDebug);

my $USE_DEBUG_OUTPUT = 0;

sub initOutputDebug ($) {
    $USE_DEBUG_OUTPUT = shift;
}

sub outputDebug ($) {
    print "$_[0]" if $USE_DEBUG_OUTPUT;
}

1;
