#!/usr/bin/perl
##
# Expand the speaker notes so that the intermediate 'steps' turn into separate slides.
# This turns the speaker notes which are managed as logical slides, into physical slides.
#
# Run this to generate the expanded speaker notes, one per physical slides:
#   ./expand-speaker-notes.pl
#
# Generate the individual slides with:
#   pyrodev --common --command "dir rougol" --command '^.aif32.present talk/md -save Slides' --config-file present.pyro --config vdu.mode=51
#   pyrodev --common --command "dir rougol" --command '^.aif32.present speaker-notes-expanded/md -save Notes' --config-file present.pyro --config vdu.mode=51
#
# Rename them to PNGs:
#   rename s/,b60/.png/ Slides*,b60 Notes*,b60
#
# Combine each pair of slides together with:
#   for i in (seq 0 118) ; set n (printf '%03i' $i) ; convert -append Slides$n.png Notes$n.png Combined$n.png ; end
#
#

use warnings;
use strict;

my $infile = 'speaker-notes.md';
my $outfile = 'speaker-notes-expanded.md';

open(my $ifh, '<', $infile) or die "Cannot read: $!";
open(my $ofh, '>', $outfile) or die "Cannot write: $!";

my $first_heading = undef;
my $first_physical = undef;
my $nphysical = 0;
my $nlogical = 0;
my @desc;
while (<$ifh>)
{
    chomp;
    if ($_ eq '<step>')
    {
        if (!$first_heading)
        {
            $first_heading = (join(' / ', @desc));
            $first_physical = $nphysical;
        }
        print $ofh "...\n\n---\n...\n\n";
        $nphysical++;
    }
    else
    {
        if ($_ eq '---')
        {
            printf "%3i. %s\n", $first_physical || $nphysical, $first_heading || (join(' / ', @desc));
            $nlogical++;
            $nphysical++;
            @desc = ();
            $first_heading = undef;
            $first_physical = undef;
        }
        if ($_ =~ /^##? (.*)$/)
        {
            push @desc, $1;
        }
        print $ofh "$_\n";
    }
}
printf "%3i. %s\n", $nphysical, (join(' / ', @desc));
print "\n";

print "Logical slides:  $nlogical    (ones you speak for)\n";
print "Physical slides: $nphysical   (including transitions)\n";
