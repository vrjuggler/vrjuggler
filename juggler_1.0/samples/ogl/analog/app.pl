#!/usr/local/bin/perl

# Kevin's app runner Perl script....

use Getopt::Std;

print "args: -1 (1con) -6 (6con) -s (simulator) -a (auditorium)\nmake sure your VJ_BASE_DIR is set!\n";

getopts('16as');

$app = "analog";

$HOME = "/home/users/kevn";
$VJ_BASE_DIR = $ENV{'VJ_BASE_DIR'};

#sim
$simconfig = "$VJ_BASE_DIR/share/Data/configFiles/sim.base.config ";
$simconfig .= "$VJ_BASE_DIR/share/Data/configFiles/sim.displays.config ";
$simconfig .= "$VJ_BASE_DIR/share/Data/configFiles/sim.wand.mixin.config ";
$simconfig .= "$VJ_BASE_DIR/share/Data/configFiles/sim.analog.mixin.config";

#aud
$audconfig = "$VJ_BASE_DIR/share/Data/configFiles/sim.base.config ";
$audconfig .= "$VJ_BASE_DIR/share/Data/configFiles/sim.wand.mixin.config ";
$audconfig .= "/home/vr/vjconfig/auditorium/auditorium.displays-x2.1con.config";

#audsim
$audsimconfig = "$VJ_BASE_DIR/share/Data/configFiles/sim.base.config ";
$audsimconfig .= "$VJ_BASE_DIR/share/Data/configFiles/sim.wand.mixin.config ";
$audsimconfig .= "/home/vr/vjconfig/auditorium/sim.aud.config";

#1con
$oneconconfig = "/home/vr/vjconfig/c6/C6.perf-1con.config";

#6con
$sixconconfig = "/home/vr/vjconfig/c6/C6.perf-6con.config";

if ($opt_1)
{
   print "Running 1 console mode\n";
   system( "$app $oneconconfig" );
}
elsif ($opt_6)
{
   print "Running 6 console mode\n";
   system( "$app $sixconconfig" );
}
elsif ($opt_a && $opt_s)
{
   print "Running auditorium simulator\n";
   system( "$app $audsimconfig" );
}
elsif ($opt_a)
{
   print "Running auditorium\n$app $audconfig\n";
   system( "$app $audconfig" );
}
elsif ($opt_s)
{
   print "Running simulator\n$app $simconfig\n";
   system( "$app $simconfig" );
}
