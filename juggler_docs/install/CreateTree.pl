#!/usr/bin/perl

require 5.004;
use lib(".");
use Getopt::Std;
use UniqueID;
getopt('i:o:s');

$xmlFile = $opt_i;
$ouputDir = $opt_o;
$substFile = $opt_s;

print "$#ARGV\n";

sub usage()
{
   warn "Usage: $0 -s <subst file> -i <input file> -o<output dir>\n";
    exit 1;
}

if ( $opt_i eq "" ) 
{
    usage();
}

elsif ( $opt_o eq "" ) 
{
    usage();
}

elsif ( $opt_s eq "" ) 
{
    usage();
}


sub PrintLine($$$);
sub MakeTree($$$$);
sub FindNumCombos($$$$);

use WebFile;	#use vars from Webfile.pl

WebFile::loadSubstFile("$substFile");

open(INFILE, "$xmlFile");

do
{
	$TempLine = <INFILE>;
}until($TempLine =~ m/imageurl/);
$' =~ m/"([\S]+)"/;
$ImageBaseURL = "$1";

seek( INFILE, 0, 0 );

do
{
	$TempLine = <INFILE>;
}until($TempLine =~ m/linkurl/);
$' =~ m/"([\S]+)"/;
$LinkBaseURL = $1;

seek( INFILE, 0, 0 );

do
{
	$TempLine = <INFILE>;
}until($TempLine =~ m/rootfolder/);
$' =~ m/title="([^"]+)"/;
print $1;

seek( INFILE, 0, 0 );

@TreeLevels;
$TreeLevels[0] = $1;

FindNumCombos("INFILE", 0, $1 ,0);

################
for my $i (0 .. $#TreeLevels)
{
	@TreeRecurseArray = ();
         seek( INFILE, 0, 0 );

	@TreeRecurseArray = split/\./, $TreeLevels[$i];
	
	$OutFile = $ouputDir . "/" . $TreeLevels[$i] . ".html";
	print "--File $OutFile\n";
	open(OUTFILE, ">$OutFile");

	print OUTFILE <<END;
<HTML>
<HEAD>
<TITLE>empty<\/TITLE>
</HEAD>
<BODY>

END

	MakeTree("INFILE", 0, 0, 1);

	print OUTFILE <<END;
</BODY>
</HTML>
END

	close OUTFILE;
}
close INFILE;
#######################

sub MakeTree($$$$)
{
	my ($TheInfile, $Level, $j, $ShowLine) = @_;
	my $TempLine;
	my $NewFileName;
	my $FilePath;
	my $FilePathEnv;
	
	$TempLine = <$TheInfile>;
	if($TempLine =~ /\</)
	{
		while(!($TempLine =~ /\>/))
		{
			$TempLine .= <$TheInfile>;
		}
	}
	for(my $i=0; !($TempLine =~/\/rootfolder/) && !($TempLine =~/\/folder/) && defined($TempLine); $i++)
	{
		if($TempLine =~ m/jitfolder/)
		{
			$TempLine =~ m/jit="\$\{[^"]+/;
				$& =~ m/\{[\S]+\}/;
				$FilePath = $';
				$& =~ m/[^\{\}]+/;
				$FilePathEnv = $&;
				$FilePath =~ m/[^\/]+/;
				$FilePath = $&;

			$NewFileName = "INFILE_INCLUDE" . $i . $Level;
			if($FilePathEnv ne "PROGRAMMERREF")
			{
				open($NewFileName, "<$LinkBaseURL/" . $WebFile::path_subst{$FilePathEnv}
					. "\/" . $FilePath);
				$j++;
				MakeTree($NewFileName, $Level, $j, $ShowLine);
				close($NewFileName);
			}
		}
		if($TempLine =~ m/web include/)
		{
			$TempLine =~ m/\$\{[\S]+ /;
				$& =~ m/\{[\S]+}/;
				$FilePath = $';
				$& =~ m/[^\{\}]+/;
				$FilePathEnv = $&;

			$NewFileName = "INFILE_INCLUE" . $i . $Level;
			open($NewFileName, "<$LinkBaseURL/" . $WebFile::path_subst{$FilePathEnv}
				. "\/" . $FilePath);
			MakeTree($NewFileName, $Level, 0, $ShowLine);
		}
		if($TempLine =~ m/<[ ]?item/)
		{
			if($ShowLine == 1){PrintLine($TempLine, $ListString . "["  . $j . "]", $Level);}
			$j++;
		}
		if($TempLine =~ m/<[ ]?folder|< rootfolder/)
		{
			$j++;
			if($ShowLine == 1){PrintLine($TempLine, $ListString . "["  . $j . "]", $Level);}
			$TempLine =~ m/title="([^"]+)"/;
                           my $Temp = $1;
                           $Temp =~ s/\//-/;
                           #print $1 . "---" . $TreeRecurseArray[$Level] . "\n";
                           if($Temp eq $TreeRecurseArray[$Level] && $ShowLine ==1 && $Level <= $#TreeRecurseArray)
			{
				MakeTree($TheInfile, $Level+1, 1, 1);
			}
			else {MakeTree($TheInfile, $Level+1, 1, 0);}
		}
		$TempLine = <$TheInfile>;
		if($TempLine =~ /\</)
		{
			while(!($TempLine =~ /\>/))
			{
				$TempLine .= <$TheInfile>;
			}
		}
	}
}

sub FindNumCombos($$$$)
{
	my ($TheInfile, $Level, $FileNum, $j) = @_;
	my $TempLine;
	my $NewFileName;
	my $FilePath;
	my $FilePathEnv;

	$TempLine = <$TheInfile>;
	if($TempLine =~ /\</)
	{
		while(!($TempLine =~ /\>/))
		{
			$TempLine .= <$TheInfile>;
		}
	}

	for(my $i=0; !($TempLine =~/\/rootfolder/) && !($TempLine =~/\/folder/) && defined($TempLine); $i++)
	{
		if($TempLine =~ m/jitfolder/)
		{
			$TempLine =~ m/jit="\$\{[^"]+/;
				$& =~ m/\{[\S]+\}/;
				$FilePath = $';
				$& =~ m/[^\{\}]+/;
				$FilePathEnv = $&;
				$FilePath =~ m/[^\/]+/;
				$FilePath = $&;

			$NewFileName = "INFILE_INCLUDE" . $i . $Level;
			if($FilePathEnv ne "PROGRAMMERREF")
			{
				open($NewFileName, "<$LinkBaseURL/" . $WebFile::path_subst{$FilePathEnv}
					. "\/" . $FilePath);
				$j++;
				FindNumCombos($NewFileName, $Level, $FileNum, $j-1);
				close($NewFileName);
			}
		}
		if($TempLine =~ m/web include/)
		{
			$TempLine =~ m/\$\{[\S]+ /;
				$& =~ m/\{[\S]+}/;
				$FilePath = $';
				$& =~ m/[^\{\}]+/;
				$FilePathEnv = $&;

			$NewFileName = "INFILE_INCLUE" . $i . $Level;
			open($NewFileName, "<$LinkBaseURL/" . $WebFile::path_subst{$FilePathEnv}
				. "\/" . $FilePath);
			FindNumCombos($NewFileName, $Level, $FileNum, 0);
			close($NewFileName);
		}
		if($TempLine =~ m/<[ ]?item/)
		{
			$j++;# title="StartPage";
		}
		if($TempLine =~ m/<[ ]?folder|[ ]?rootfolder/ && $Level>0)
		{
			$j++;
                           $TempLine =~ m/title="([^"]+)"/;
                           my $Temp = $1;
                           $Temp =~ s/\//-/;
			$TreeLevels[$#TreeLevels+1] = $FileNum . "." . $Temp;
			FindNumCombos($TheInfile, $Level + 1, $FileNum . "." . $Temp , 0);
		}
		if($TempLine =~ m/<[ ]?rootfolder/ && $Level==0)
		{
			FindNumCombos($TheInfile, $Level + 1, $FileNum, 0);
		}
		$TempLine = <$TheInfile>;
		if($TempLine =~ /\</)
		{
			while(!($TempLine =~ /\>/))
			{
				$TempLine .= <$TheInfile>;
			}
		}
	}
}

sub PrintLine($$$)
{
	my ($TL, $ListString, $Level) = @_;
	my $TempImageSRC;
	my $TempName;
	my $TempLink;
	if($TL =~ m/link="([\S]+)"/)
	{
		$TempLink = $1;
	}
	if($TL =~ m/image="([\S]+)"/)
	{
		$TempImageSRC = $ImageBaseURL . $1;
	}
	if($TL =~ m/title="([^"]+)"/)
	{
		$TempName = $1;
	}

	if($TempImageSRC eq $TempOverImageSRC){ $TempOverImageSRC = "";}
	
	if($Level != 0)
	{
	print OUTFILE "<IMG SRC=\"" . $WebFile::path_subst{'IMAGES'} . "/clear.dot.gif\" HEIGHT=1 WIDTH="
		 . ($Level * 16) . ">";
	}
	#if($TL =~ /<[ ]?folder|<[ ]?rootfolder/ && $Level>0)
	#	{print OUTFILE "Folder";}
	#if($TL =~ /<[ ]?folder|<[ ]?rootfolder/ && $Level>0)
	#	{print OUTFILE "<A HREF=\"\" onClick=\"return false;\" ><IMG SRC=\"../Images/ExpandPlus.gif\" HEIGHT=16 WIDTH=11 BORDER=0></A>\n";}
	print OUTFILE "<A HREF=\"" . $TempLink . "\" onClick=\"return false;\">"
		. "<IMG SRC=\"" . $TempImageSRC . "\" HEIGHT=16 WIDTH=16 BORDER=0>\n"
		. $TempName . "</A><BR>\n\n";
}
