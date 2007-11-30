
##
## Copyright (C) Centeris Corporation 2004-2007
## Copyright (C) Likewise Software 2007.  
## All rights reserved.
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http:##www.gnu.org/licenses/>.
##


#
# Copyright Centeris Corporation.  All rights reserved.
#

use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin";
use Centeris;

sub main();
exit(main());


#
# Perhaps we only want to mess with options if main option (GSSAPIAuthentication) is not enabled.
#
# ssh_config:
#
# GSSAPIAuthentication no -> yes
# GSSAPIDelegateCredentials no -> yes
# GSSAPIEnableMITMAttack no -> no
#
# sshd_config:
#
# GSSAPIAuthentication no -> yes
# GSSAPICleanupCredentials yes -> yes
# GSSAPIEnableMITMAttack no -> no
#

sub ProcessSshConfigFile($$;$)
{
    my $file = shift || die;
    my $config = shift || die;
    my $opt = shift || {};

    {
        my @keys = keys %$config;
        if ( $#keys < 0 )
        {
            return;
        }
    }

    if ( ! -e $file )
    {
        print "$file does not exist, skipping\n";
        return;
    }

    my $lines = ReadFile($file);
    my $modifyCount = 0;

    foreach my $name (sort keys %$config)
    {
        my $value = $config->{$name} || die;
        my $update = "$name $value";

        my $lineNumber = 0;
        my $found = 0;

        my $lineCount = $#{@$lines} + 1;

        for ($lineNumber = 1; $lineNumber <= $lineCount; $lineNumber++)
        {
            my $line = $lines->[$lineNumber-1];
            #print "$lineNumber: \"$line\"\n";
            chomp($line);

            if ( $line =~ /^\s*$name(\s+(\S+))?\s*$/ )
            {
                if ( $found )
                {
                    $lines->[$lineNumber-1] = "# $line";
                    $modifyCount++;
                }
                else
                {
                    $found = 1;

                    my $got_value = $1 ? $2 : '';

                    if ( ( not $got_value ) or
                         ( not ($got_value eq $value) ) )
                    {
                        $lines->[$lineNumber-1] = $update;
                        $modifyCount++;
                    }
                }
            }
            elsif ( $line =~ /^\s*$name((\s+).*)?$/ )
            {
                $lines->[$lineNumber-1] = "# $line";
                $modifyCount++;
            }
        }

        if ( not $found )
        {
            push(@$lines, $update);
            $modifyCount++;
        }
    }

    if ( $opt->{debug} )
    {
        print "New Config:\n\n";
        foreach my $line (@$lines)
        {
            chomp($line);
            print "$line\n";
        }
    }

    if ( $modifyCount )
    {
	print "Modified $file\n";
        ReplaceFile( $file, $lines ) if not $opt->{test};
    }
    else
    {
	print "No need to modify $file\n";
    }
}


sub GetDesiredSshConfig($$$)
{
    my $type = shift || die;
    my $enable = shift;
    my $testPrefix = shift;

    my $res = undef;

    my $osType = GetOsType($testPrefix);
    my $distroType = GetDistroType($testPrefix);
    my $distroVersion = GetDistroVersion($testPrefix);


    if (   ($osType eq 'SunOS' and $distroVersion eq '5.9') or
           ($osType eq 'Linux' and
               ( 
                   (($distroType eq 'rhel' || $distroType eq 'centos') and $distroVersion lt '4' ) or
                   ($distroType eq 'redhat' and $distroVersion le '9' ) or
                   ($distroType eq 'suse' and $distroVersion le '9' )
               )
           )
       )
    {
        #
        # Do not do GSSAPI changes for systems w/o GSSAPI support in ssh/sshd:
        #
        # - Solaris 9
        # - RedHat 3
        # - CentOS 3
        #

        print "No support for GSSAPI on this platform.\n";
        return {};
    }

    if ( $type eq 'ssh_config' )
    {
        $res = {
                GSSAPIAuthentication => 'yes',
                GSSAPIDelegateCredentials => 'yes', # may want this to stay the same so that user can do option on command-line

                #
                # Revisit GSSAPIEnableMITMAttack later: Some ssh builds do not
                # understand this directive.  Need to tweak per OS.  The
                # default on the OS should do the right thing anyhow.
                #

                # GSSAPIEnableMITMAttack => 'no', # may want to leave alone, but unlikely
               };
    }
    elsif ( $type eq 'sshd_config' )
    {
        $res = {
                GSSAPIAuthentication => $enable ? 'yes' : 'no',
                GSSAPICleanupCredentials => 'yes', # may want to leave alone, but unlikely

                #
                # Revisit GSSAPIEnableMITMAttack later: Some ssh builds do not
                # understand this directive.  Need to tweak per OS.  The
                # default on the OS should do the right thing anyhow.
                #

                # GSSAPIEnableMITMAttack => 'no', # may want to leave alone, but unlikely

                #
                # Making sure that this is always set to 'yes' so that keyboard interactive mode
                # will work correctly. It seems that Fedora Core has this value turned off by
                # default which is breaking our SSH client.
                #

                ChallengeResponseAuthentication => 'yes',

                #
                # UsePAM needs to be set to 'yes' so that we use PAM.
                #

                UsePAM => 'yes',

               };
    }

    if ( $res and
         ( ( $osType eq 'SunOS' ) or
           ( ( $osType eq 'Linux' ) and ( $distroType eq 'suse' ) and ( $distroVersion eq '9.0') ) ) )
    {
        #
        # GSSAPICleanupCredentials is not valid in sshd for:
        # - Solaris
        # - SuSE 9.0
        #

        delete $res->{GSSAPICleanupCredentials};
    }

    if ( $res and
         ( $osType eq 'SunOS' || $osType eq 'AIX' ) )
    {
        #
        # UsePAM is not valid in sshd for:
        # - Solaris and AIX
        #

        delete $res->{UsePAM};
    }

    if ( not defined $res )
    {
        die;
    }

    return $res;
}


sub ModifySshConfig($;$$$)
{
    my $enable = shift;
    my $testPrefix = shift || '';
    my $opt = shift || {};
    my $osType = GetOsType($testPrefix);
    my $client_file = $testPrefix.'/etc/ssh/ssh_config';
    my $server_file = $testPrefix.'/etc/ssh/sshd_config';

    if ( $osType eq 'HP-UX' ) 
    {
	$client_file = $testPrefix.'/opt/ssh/etc/ssh_config';
	$server_file = $testPrefix.'/opt/ssh/etc/sshd_config';
    }

    ProcessSshConfigFile( $client_file,
                          GetDesiredSshConfig('ssh_config', $enable, $testPrefix),
                          $opt );

    ProcessSshConfigFile( $server_file,
                          GetDesiredSshConfig('sshd_config', $enable, $testPrefix),
                          $opt );

    return 0;
};


sub usage()
{
    use Centeris;

    $0 = QuickBaseName($0);
    return <<DATA;
usage: $0 [options] --enable
   or: $0 [options] --disable

  options (case-sensitive):

    --testprefix <prefix>   Prefix for test directory
DATA
}


sub main()
{
    use Centeris;
    use Getopt::Long;

    Getopt::Long::Configure('no_ignore_case', 'no_auto_abbrev') || die;

    my $opt = {};
    my $ok = GetOptions($opt,
			'help|h|?',
                        'testprefix=s',
                        'test',
                        'debug',
                        'enable',
                        'disable',
                       );

    if ( not $ok or
         $opt->{help} or
         not ( $opt->{enable} xor $opt->{disable} ) or
         ( $#ARGV != -1 ) )
    {
	die usage();
    }

    my $prefix = $opt->{testprefix} || '';
    CheckTestPrefix($prefix);

    return ModifySshConfig( $opt->{enable} ? 1 : 0, $prefix, CreateGlobalOpts( $opt ) );
}
