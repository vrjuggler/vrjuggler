package UniqueID;

require 5.004;
require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(filePathToUniqeID uniqueIdToFilePath);


sub filePathToUniqeID($$) 
{
   my $filepath = shift;
   my $unique = shift;
   
   $$unique = "$filepath";
   $$unique =~ s/\//--slash--/gis;
   $$unique =~ s/:/--colon--/gis;
}

sub uniqueIdToFilePath($$) 
{
   my $unique = shift;
   my $filepath = shift;
   
   $$filepath = "$unique";
   $$filepath =~ s/--slash--/\//gis;
   $$filepath =~ s/--colon--/:/gis;
}
