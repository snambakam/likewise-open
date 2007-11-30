#
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


sub GetListCount($)
{
    my $listRef = shift || die;
    return $#{@$listRef} + 1;
}


sub GetLastElement($)
{
    my $listRef = shift || die;
    my $lastIndex = $#{@$listRef};

    if ($lastIndex >= 0)
    {
        my $element = $listRef->[$lastIndex];
        die if not ( 'HASH' eq ref($element) );
        return $element;
    }
    else
    {
        return undef;
    }
}


sub TrimWhitespace($)
{
    my $line = shift;
    chomp($line);

    #
    # Trim leading and trailing whitespace
    #

    $line =~ s/^(\s+)//;
    $line =~ s/(\s+)$//;

    return $line;
}


sub CleanupLine($)
{
    my $line = shift;

    $line = TrimWhitespace($line);

    # Remove comments
    $line =~ s/#.*//;

    return $line;
}


sub CreateNode($$$;$)
{
    my $type = shift || die;
    my $name = shift;
    my $originalLine = shift;
    my $lineNumber = shift || 0;

    die if ( not ( ( $type eq 'comment' ) or
                   ( $type eq 'section' ) or
                   ( $type eq 'group' ) or
                   ( $type eq 'value' ) ) );

    if ( $type eq 'comment' )
    {
        die if $name;
    }
    else
    {
        die if not $name;
    }

    return
    {
     parent => undef,
     type => $type,
     name => $name,
     text => [ $originalLine ],
     lineNumber => $lineNumber,
    };
}


sub CreateRootNode(;$)
{
    my $file = shift || '';

    return
    {
     type => 'root',
     file => $file,
     childrenList => [],
     childrenHash => {},
    };
}


sub AddCommentNode($$;$)
{
    my $parent = shift || die;
    my $originalLine = shift;
    my $lineNumber = shift || 0;

    my $node = GetLastElement($parent->{childrenList});
    if ( $node  and ('comment' eq $node->{type}) )
    {
        push(@{$node->{text}}, $originalLine);
    }
    else
    {
        $node = CreateNode('comment', '', $originalLine, $lineNumber);
        AddNode($parent, $node);
    }
}


sub CreateValueNode($$;$$)
{
    my $name = shift || die;
    my $value = shift;
    my $originalLine = shift;
    my $lineNumber = shift;

    if ( not $originalLine )
    {
        $originalLine = "$name = $value";
    }

    my $node = CreateNode('value', $name, $originalLine, $lineNumber);
    $node->{value} = $value;
    return $node;
}


sub CreateGroupNode($;$$)
{
    my $name = shift || die;
    my $originalLine = shift;
    my $lineNumber = shift;

    if ( not $originalLine )
    {
        $originalLine = "$name = {";
    }

    my $node = CreateNode('group', $name, $originalLine, $lineNumber);
    $node->{childrenList} = [];
    $node->{childrenHash} = {};
    $node->{textPost} = [ '}' ]; # replace later

    return $node;
}


sub CreateSectionNode($;$$)
{
    my $name = shift || die;
    my $originalLine = shift;
    my $lineNumber = shift;

    if ( not $originalLine )
    {
        $originalLine = "[$name]";
    }

    my $node = CreateNode('section', $name, $originalLine, $lineNumber);
    $node->{childrenList} = [];
    $node->{childrenHash} = {};

    return $node;
}


sub EndLocation()
{
    return undef;
}


sub IsEndInsertion($)
{
    my $location = shift;

    return ( ( not defined($location) ) or
	     ( $location < 0 ) );
}


sub InsertAt($$;$)
{
    my $list = shift || die;
    my $item = shift;
    my $location = shift;

    if ( IsEndInsertion($location) )
    {
        push(@$list, $item);
    }
    else
    {
        die if $location > $#{@$list};
        splice(@$list, $location, 0, $item);
    }
}


sub InsertNode($$;$)
{
    my $parent = shift || die;
    my $node = shift || die;
    my $location = shift;

    die if $node->{parent};
    $node->{parent} = $parent;

    my $list = $parent->{childrenList} || die;
    InsertAt($list, $node, $location);

    my $key = $node->{name};
    if ($key)
    {
        die if $node->{type} eq 'comment';

	#
	# If not inserting at end or beginning, force to end.
	#

	if ( !IsEndInsertion($location) and
	     (0 != $location) )
	{
	    $location = EndLocation();
	}

        my $hash = $parent->{childrenHash} || die;
        my $value = $hash->{$key};
        if ( defined($value) )
        {
            my $ref = ref($value);
            if ( $ref eq 'HASH' )
            {
                my $valueList = [ $value ];
                $hash->{$key} = $valueList;
                InsertAt( $valueList, $node, $location );
            }
            elsif ( $ref eq 'ARRAY' )
            {
                my $valueList = $value;
                InsertAt( $valueList, $node, $location );
            }
            else
            {
                # invalid reference
                die;
            }
        }
        else
        {
            $hash->{$key} = $node;
        }
    }
    else
    {
        die if not ($node->{type} eq 'comment');
    }
}


sub AddNode($$)
{
    my $parent = shift || die;
    my $node = shift || die;

    InsertNode($parent, $node);
}


sub DeleteFromList($$)
{
    my $listRef = shift || die;
    my $elementRef = shift || die;

    my $count = GetListCount($listRef);
    for (my $index = 0; $index < $count; $index++)
    {
        my $item = $listRef->[$index];
        if ( $item == $elementRef )
        {
            splice(@$listRef, $index, 1);
            last;
        }
    }
}


sub RemoveNode($)
{
    my $node = shift || die;

    my $key = $node->{name};
    my $parent = $node->{parent};

    die if not $parent;

    my $list = $parent->{childrenList} || die;
    DeleteFromList($list, $node);

    if ($key)
    {
        die if $node->{type} eq 'comment';

        my $hash = $parent->{childrenHash} || die;
        my $value = $hash->{$key};
        if ( defined($value) )
        {
            if ( $value == $node )
            {
                delete $hash->{$key};
            }
            else
            {
                my $ref = ref($value);
                if ( $ref eq 'HASH' )
                {
                    # not in there, so ignore
                }
                elsif ( $ref eq 'ARRAY' )
                {
                    # remove from list
                    # should have at least 2 elements
                    die if (GetListCount($value) < 2);
                    DeleteFromList($value, $node);
                    my $count = GetListCount($value);
                    die if ($count < 1);
                    if ( $count == 1 )
                    {
                        # convert to single hash
                        $hash->{$key} = $value->[0];
                    }
                }
                else
                {
                    # invalid reference
                    die;
                }
            }
        }
    }
    else
    {
        die if not ($node->{type} eq 'comment');
    }

    $node->{parent} = undef;
}


sub ParseKrb5ConfFile($;$)
    # Note that krb5.conf has ordering as described in
    # http://mailman.mit.edu/pipermail/krbdev/2005-November/003901.html
{
    my $lines = shift || die;
    my $file = shift;

    my $root = CreateRootNode($file);

    my $parent = $root;
    my $lineNumber = 0;
    foreach my $originalLine (@$lines)
    {
        # note that the iteration can modify the list if you modify the
        # element, so make sure that we do not modify the iteration variable

        my $line = $originalLine;
        $lineNumber++;
        $line = CleanupLine($line);

        if ( $line =~ /^$/ )
        {
            #
            # a comment
            #

            AddCommentNode($parent, $originalLine);
        }
        elsif ( $line =~ /^\[\s*(.+)\s*\]$/ )
        {
            #
            # a section
            #

            my $name = $1;

            if ( $parent != $root )
            {
                my $type = $parent->{type};

                if ( 'section' eq $type )
                {
                    # this is ok, we will do a new section
                    $parent = $root;
                }
                elsif ( 'group' eq $type )
                {
                    my $groupName = $parent->{name};
                    die GetFileParseMessage("ERROR: Cannot start section \"$name\" while in the middle of group \"$groupName\"",
                                            $file, $lineNumber, $originalLine);
                }
                else
                {
                    die GetFileParseMessage("ERROR: Unexpected parent of type \"$type\" when starting section \"$name\"",
                                            $file, $lineNumber, $originalLine);
                }
            }

            die if not ('root' eq $parent->{type});

            if ( $parent->{childrenHash}->{$name} )
            {
                die GetFileParseMessage("ERROR: Section \"$name\" already defined",
                                        $file, $lineNumber, $originalLine);
            }

            my $node = CreateSectionNode($name, $originalLine, $lineNumber);
            AddNode($parent, $node);
            $parent = $node;
        }
        elsif ( $parent == $root )
        {
            #
            # If the parent is the root and this is not a section, we treat it
            # a comment.

            die if $parent != $root;

            #
            # Note that this means that there most not be any sections.
            #

            die if keys %{$parent->{childrenHash}};

            #
            #
            #

            die if GetListCount($parent) > 1;

            #
            # anything before the first section is a comment
            #

            AddCommentNode($parent, $originalLine);
        }
        elsif ( $line =~ /^(\S+)\s*=\s*{$/ )
        {
            #
            # a group relation
            #

            my $name = $1;

            die if not ( ( $parent->{type} eq 'section' ) or ( $parent->{type} eq 'group' ) );

            my $node = CreateGroupNode($name, $originalLine, $lineNumber);
            delete $node->{textPost};
            AddNode($parent, $node);
            $parent = $node;
        }
        elsif ( $line =~ /^(\S+)\s*=\s*([^{].*)?$/ )
        {
            #
            # a value relation
            #

            my $name = $1;
            my $value = $2 ? $2 : '';

            my $node = CreateValueNode($name, $value, $originalLine, $lineNumber);
            AddNode($parent, $node);
        }
        elsif ( $line =~ /}/ )
        {
            #
            # terminate a group
            #

            #
            # In particular, we are terminating the parent.
            #

            if ( not ( $parent->{type} eq 'group' ) )
            {
                die GetFileParseMessage("ERROR: Trying to terminate something that is not a group",
                                        $file, $lineNumber, $originalLine);
            }

            die if $parent->{textPost};

            $parent->{textPost} = [ $originalLine ];

            $parent = $parent->{parent};
        }
        else
        {
            die GetFileParseMessage("ERROR: Could not parse text", $file, $lineNumber, $originalLine);
        }
    }

    return $root;
}


sub GetNodeLevel($)
{
    my $node = shift || die;
    my $level = -1;

    #
    # We start at -1 because sections are level 0.
    #

    while ( defined($node->{parent}))
    {
        $node = $node->{parent};
        $level++;
    }

    #
    # Let's make the root be 0 too.
    #

    if ( $level < 0 )
    {
        $level = 0
    }

    return $level;
}


sub GetIndentedLines($$)
{
    my $lines = shift || die;
    my $level = shift;

    my @result = ();

    my $justSpaces = 1;
    foreach my $line (@$lines)
    {
        if ( $line =~ /[^ \t\n]/ )
        {
            $justSpaces = 0;
            last;
        }
    }

    if ( $justSpaces )
    {
        push(@result, @$lines);
    }
    else
    {
        map {
            my $line = ('    ' x $level).$_;
            push(@result, $line);
        } @{$lines};
    }

    return \@result;
}


sub UnparseKrb5ConfFile($);
sub UnparseKrb5ConfFile($)
{
    my $node = shift || die;

    my $lines = [];

    if ( $node->{text} )
    {
        if ( $node->{lineNumber} )
        {
            push(@$lines, @{$node->{text}});
        }
        else
        {
            push(@$lines, @{GetIndentedLines($node->{text}, GetNodeLevel($node))});
        }
    }

    if ( $node->{childrenList} )
    {
        foreach my $child (@{$node->{childrenList}})
        {
            my $childLines = UnparseKrb5ConfFile($child);
            push(@$lines, @$childLines);
        }
    }

    if ( $node->{textPost} )
    {
        if ( $node->{lineNumber} )
        {
            push(@$lines, @{$node->{textPost}});
        }
        else
        {
            push(@$lines, @{GetIndentedLines($node->{textPost}, GetNodeLevel($node))});
        }
    }

    return $lines;
}


sub GetEscapedDomainName($)
{
    my $domain = shift || die;
    $domain = uc($domain);
    $domain =~ s/\./\\\./g;
    return $domain;
}


sub GetShortDomainName($)
{
    my $domain = shift || die;
    $domain = uc($domain);
    my @components = split(/\./, $domain);
    return $components[0];
}


sub GetAuthToLocalValueString($;$)
{
    my $domain = shift || die;
    my $shortDomain = shift || '';

    $domain = uc($domain);
    $shortDomain = uc($shortDomain);

    if ( not $shortDomain )
    {
        $shortDomain = GetShortDomainName($domain);
    }

    my $escapedDomain = GetEscapedDomainName($domain);

    #
    # DOMAIN.EXAMPLE.COM --> RULE:[1:$0\$1](^DOMAIN\.EXAMPLE\.COM\\.*)s/^DOMAIN\.EXAMPLE\.COM/DOMAIN/
    #
    return 'RULE:[1:$0\$1](^'.$escapedDomain.'\\\\.*)s/^'.$escapedDomain.'/'.$shortDomain.'/';
}


sub GetMappingsValueString($;$)
{
    my $domain = shift || die;
    my $shortDomain = shift || '';

    $domain = uc($domain);
    $shortDomain = uc($shortDomain);

    if ( not $shortDomain )
    {
        $shortDomain = GetShortDomainName($domain);
    }

    #
    # DOMAIN.EXAMPLE.COM --> DOMAIN\\(.*) $1@DOMAIN.EXAMPLE.COM
    #

    return $shortDomain.'\\\\(.*) $1@'.$domain;
}


sub GetReverseMappingsValueString($;$)
{
    my $domain = shift || die;
    my $shortDomain = shift || '';

    $domain = uc($domain);
    $shortDomain = uc($shortDomain);

    if ( not $shortDomain )
    {
        $shortDomain = GetShortDomainName($domain);
    }

    #
    # DOMAIN.EXAMPLE.COM --> (.*)@DOMAIN.EXAMPLE.COM DOMAIN\\$1
    #

    $domain = GetEscapedDomainName($domain);

    return '(.*)@'.$domain.' '.$shortDomain.'\\$1';
}


sub GetChildNode($$)
{
    my $node = shift || die;
    my $name = shift || die;

    die if not $node->{childrenHash};

    return $node->{childrenHash}->{$name};
}


sub GetFileParseMessageFromNode($$)
{
    my $message = shift || die;
    my $node = shift || die;

    my $file;
    my $tempNode = $node;
    while ( not (defined($tempNode->{file})) and
            defined($tempNode->{parent}) )
    {
        $tempNode = $tempNode->{parent};
    }
    $file = $tempNode->{file} ? $tempNode->{file} : '';

    return GetFileParseMessage( $message, $file, $node->{lineNumber}, $node->{text}->[0] );
}


sub AddNodeBeforeComment($$)
{
    my $parent = shift || die;
    my $node = shift || die;

    my $list = $parent->{childrenList};
    my $last = GetLastElement($list);
    if ( $last and $last->{type} eq 'comment' )
    {
        my $location = GetListCount($list) - 1;
        InsertNode( $parent, $node, $location );
    }
    else
    {
        AddNode( $parent, $node );
    }
}


sub EnsureSectionNode($$$)
{
    my $parent = shift || die;
    my $name = shift || die;
    my $nodeRef = shift || die;

    die if defined($$nodeRef);
    die if not ($parent->{type} eq 'root');

    my $modifyCount = 0;

    my $node = GetChildNode( $parent, $name );
    if ( not $node )
    {
        $node = CreateSectionNode( $name );
        AddNodeBeforeComment( $parent, $node );
        $modifyCount++;
    }
    else
    {
        die if not ( $name eq $node->{name} );
        my $type = $node->{type};

        if ( not ( $type eq 'section' ) )
        {
            die GetFileParseMessageFromNode( "ERROR: Invalid type \"$type\" for \"$name\"", $node );
        }
    }

    $$nodeRef = $node;

    return $modifyCount;
}

sub EnsureGroupNode($$$)
{
    my $parent = shift || die;
    my $name = shift || die;
    my $nodeRef = shift || die;

    die if defined($$nodeRef);

    my $modifyCount = 0;

    my $node = GetChildNode( $parent, $name );
    if ( not $node )
    {
        $node = CreateGroupNode( $name );
        AddNodeBeforeComment( $parent, $node );
        $modifyCount++;
    }
    else
    {
        die if not ( $name eq $node->{name} );
        my $type = $node->{type};

        if ( not ( $type eq 'group' ) )
        {
            die GetFileParseMessageFromNode( "ERROR: Invalid type \"$type\" for \"$name\"", $node );
        }
    }

    $$nodeRef = $node;

    return $modifyCount;
}


sub EnsureNoChildNode($$)
{
    my $parent = shift || die;
    my $name = shift || die;

    my $modifyCount = 0;

    my $item = GetChildNode( $parent, $name );
    while ( $item )
    {
        if ( ref($item) eq 'HASH' )
        {
            my $node = $item;
            RemoveNode( $node );
            $modifyCount++;
            last;
        }
        elsif ( ref($item) eq 'ARRAY' )
        {
            my $node = $item->[0];
            RemoveNode( $node );
            $modifyCount++;
            $item = GetChildNode( $parent, $name );
            next;
        }
        else
        {
            die;
        }
    }

    return $modifyCount;
}


sub EnsureSingleValueNode($$$)
{
    my $parent = shift || die;
    my $name = shift || die;
    my $value = shift;

    my $modifyCount = 0;

    my $newNode = CreateValueNode( $name, $value );

    my $item = GetChildNode( $parent, $name );
    if ( not $item )
    {
        AddNodeBeforeComment( $parent, $newNode );
        $modifyCount++;
    }
    elsif ( ref($item) eq 'HASH' )
    {
        if ( $item->{value} eq $value )
        {
            # done
        }
        else
        {
            print GetFileParseMessageFromNode( "INFO: Removing \"$name\" directive before adding", $item );
            RemoveNode( $item );
            $modifyCount++;
            AddNodeBeforeComment( $parent, $newNode );
            $modifyCount++;
        }
    }
    elsif ( ref($item) eq 'ARRAY' )
    {
        my $count = GetListCount( $item );
        die if ($count < 2);

        if ( not ($item->[0]->{value} eq $value) )
        {
            InsertNode( $parent, $newNode, 0 );
            $modifyCount++;
        }

        while ( ref($item) eq 'ARRAY' )
        {
            my $node = $item->[1];
            print GetFileParseMessageFromNode( "INFO: Removing \"$name\" directive after adding", $node );
            RemoveNode( $node );
            $modifyCount++;
            $item = GetChildNode( $parent, $name );
        }
    }
    else
    {
        die;
    }

    return $modifyCount;
}


sub EnsureMultiValueNode($$$)
{
    my $parent = shift || die;
    my $name = shift || die;
    my $valueList = shift || die;

    my $modifyCount = 0;

    die if not (ref($valueList) eq 'ARRAY');

    my $newNodes = [];
    foreach my $value (@$valueList)
    {
        push(@$newNodes, CreateValueNode($name, $value));
    }

    $modifyCount += EnsureNoChildNode( $parent, $name );
    foreach my $newNode (@$newNodes)
    {
        AddNodeBeforeComment( $parent, $newNode );
        $modifyCount++;
    }

    return $modifyCount;
}


sub LeaveDomain($)
{
    my $root = shift || die;

    my $modifyCount = 0;

    my $libDefaultsSectionNode = undef;

    $modifyCount += EnsureSectionNode( $root, 'libdefaults', \$libDefaultsSectionNode );
    $modifyCount += EnsureNoChildNode( $libDefaultsSectionNode, 'default_realm' );

    return $modifyCount;
}


sub SetupRealm($$;$$)
{
    my $root = shift || die;
    my $domain = shift || die;
    my $shortDomain = shift || '';
    my $trusts = shift || 0;

    $domain = uc($domain);
    $shortDomain = uc($shortDomain);

    my $modifyCount = 0;

    my $realmsSectionNode = undef;
    my $realmGroupNode = undef;

    $modifyCount += EnsureSectionNode( $root, 'realms', \$realmsSectionNode );
    $modifyCount += EnsureGroupNode( $realmsSectionNode, $domain, \$realmGroupNode );

    my $list = [ GetAuthToLocalValueString($domain, $shortDomain) ];
    if ($trusts)
    {
        foreach my $shortTrust (keys %$trusts)
        {
            if ( not ($shortTrust eq $shortDomain ) )
            {
                push(@$list, GetAuthToLocalValueString($trusts->{$shortTrust}, $shortTrust));
            }
        }
        push(@$list, 'DEFAULT');
    }

    $modifyCount += EnsureMultiValueNode( $realmGroupNode, 'auth_to_local', $list );

    return $modifyCount;
}


sub GetCommandOutputLines($)
{
    my $command = shift || die;
    my $lines = [];
    @$lines = `$command`;
    my $error = ($? == -1) ? $? : ($? >> 8);
    if (($? == -1) || ($? >> 8) != 0)
    {
        return undef;
    }

    map {
        chomp($_);
    } @$lines;

    return $lines;
}


sub GatherTrusts($)
{
    my $root = shift || die;
    my $trusts = {};

    my @dirs = qw(/opt/likewise-open/bin);
    my $file = 'lwinet';
    my $lwiNetCommand = 'lwinet ads trusts';
    my $lwiInfoCommand = 'lwiinfo --details -m';

    my $dir;
    my $lines = undef;
    foreach my $d (@dirs)
    {
        if ( -x "$d/$file" )
        {
            $dir = $d;
            last;
        }
    }

    if (not $dir)
    {
        die "Could not find $file\n";
    }

    my $cmdLine;

    $cmdLine = "$dir/$lwiNetCommand";
    $lines = GetCommandOutputLines($cmdLine);
    if (not defined($lines))
    {
        die "Error running: $cmdLine\n";
    }

    foreach my $line (@$lines)
    {
        chomp($line);
        if ($line =~ /^\s*\d+\s+(\S+)\s+.*\s+(\S+)\s*$/)
        {
            my $shortDomain = uc($1);
            my $domain = uc($2);
            print "[$shortDomain] <-> [$domain]\n";
            die if not $shortDomain;
            die if not $domain;
            $trusts->{$shortDomain} = $domain;
        }
        else
        {
            die "Bad output line from $file: $line\n";
        }
    }

    $cmdLine = "$dir/$lwiInfoCommand";
    $lines = GetCommandOutputLines($cmdLine);
    if (not defined($lines))
    {
        print "Error running: $cmdLine\n";
    }
    else
    {
        foreach my $line (@$lines)
        {
            chomp($line);
            if ($line =~ /^([^,]*),([^,]*),[^,]*$/)
            {
                my $shortDomain = uc(TrimWhitespace($1));
                my $domain = uc(TrimWhitespace($2));
                if ( $domain and not $trusts->{$shortDomain} )
                {
                    print "[$shortDomain] <-> [$domain]\n";
                    die if not $shortDomain;
                    $trusts->{$shortDomain} = $domain;
                }
            } else {
                die "Bad output line from $file: $line\n";
            }
        }
    }

    return $trusts;
}


sub JoinDomain($$;$$)
{
    my $root = shift || die;
    my $domain = shift || die;
    my $shortDomain = shift || '';
    my $doTrusts = shift || 0;

    $domain = uc($domain);
    $shortDomain = uc($shortDomain);

    my $modifyCount = 0;

    my $libDefaultsSectionNode = undef;

    $modifyCount += EnsureSectionNode( $root, 'libdefaults', \$libDefaultsSectionNode );

    #
    # Ideally, we should probably merge encryption types.
    #

    my $encTypes = 'DES-CBC-CRC DES-CBC-MD5 RC4-HMAC';

    $modifyCount += EnsureSingleValueNode( $libDefaultsSectionNode, 'default_realm', $domain );
    $modifyCount += EnsureSingleValueNode( $libDefaultsSectionNode, 'default_tgs_enctypes', $encTypes );
    $modifyCount += EnsureSingleValueNode( $libDefaultsSectionNode, 'default_tkt_enctypes', $encTypes );
    $modifyCount += EnsureSingleValueNode( $libDefaultsSectionNode, 'preferred_enctypes', $encTypes );
    $modifyCount += EnsureSingleValueNode( $libDefaultsSectionNode, 'dns_lookup_kdc', 'true' );

    my $trusts = undef;
    if ( $doTrusts )
    {
        $trusts = GatherTrusts($root);
    }

    $modifyCount += SetupRealm( $root, $domain, $shortDomain, $trusts );

    #
    # appdefaults
    #

    #
    # Note that the app using the appdefaults value is likely using the
    # krb5_appdefault API, which, as of krb5 1.4, can only get strings and
    # integers and does not handle multi-value relations.  This means that an
    # app using the krb5_appdefault API canot handle multi-value relations
    # (which would be possible if the application were using the profile API
    # as krb5 does internally do handle multi-value items like kdc
    # directives).
    #

    my $appDefaultsSectionNode = undef;
    my $pamGroupNode = undef;
    my $httpdGroupNode = undef;

    $modifyCount += EnsureSectionNode( $root, 'appdefaults', \$appDefaultsSectionNode );
    $modifyCount += EnsureGroupNode( $appDefaultsSectionNode, 'pam', \$pamGroupNode );
    $modifyCount += EnsureGroupNode( $appDefaultsSectionNode, 'httpd', \$httpdGroupNode );

    #
    # pam appdefaults
    #

    $modifyCount += EnsureSingleValueNode( $pamGroupNode, 'mappings', GetMappingsValueString( $domain, $shortDomain ) );
    $modifyCount += EnsureSingleValueNode( $pamGroupNode, 'forwardable', 'true' );
    $modifyCount += EnsureSingleValueNode( $pamGroupNode, 'validate', 'true' );

    #
    # http appdefaults - for modified mod_auth_krb (mod_auth_krb_centeris)
    #

    $modifyCount += EnsureSingleValueNode( $httpdGroupNode, 'mappings', GetMappingsValueString( $domain, $shortDomain ) );
    $modifyCount += EnsureSingleValueNode( $httpdGroupNode, 'reverse_mappings', GetReverseMappingsValueString( $domain, $shortDomain ) );

    return $modifyCount;
}


sub DumpConfig($)
{
    my $root = shift || die;

    my $lines = UnparseKrb5ConfFile( $root );
    foreach my $line (@$lines)
    {
        chomp($line);
        print("$line\n");
    }
}


sub LinkEtcKrb5($$)
{
    my $prefix = shift || '';
    my $file = shift || die;

    if ( -d $prefix.'/etc/krb5' )
    {
        my $link = $prefix.'/etc/krb5/'.$file;
        my $target = '../'.$file;
        my $modifiedSymlink = ReplaceSymlink($link, $target);
        if ( $modifiedSymlink )
        {
            print "Modified symlink $link\n";
        }
    }
}


sub usage()
{
    $0 = QuickBaseName($0);
    return <<DATA;
usage: $0 [options] --join <domain>
   or: $0 [options] --leave

  options (case-sensitive):

    --testprefix <prefix>   Prefix for test directory
    --short <shortName>     Short domain name (only valid with --join)
    --trusts                Include information about domain trusts (which
                            requires a running auth daemon).
DATA
}


sub main()
{
    use Getopt::Long;

    Getopt::Long::Configure('no_ignore_case', 'no_auto_abbrev') || die;

    my $opt = {};
    my $ok = GetOptions($opt,
                        'help|h|?',
                        'testprefix=s',
                        'test',
                        'debug',
                        'leave',
                        'join=s',
                        'short=s',
                        'trusts',
                       );

    if ( not $ok or
         $opt->{help} or
         not ( $opt->{join} xor $opt->{leave} ) or
         ( $opt->{short} and (not $opt->{join}) ) or
         ( $#ARGV != -1 ) )
    {
        die usage();
    }

    my $prefix = $opt->{testprefix} || '';
    CheckTestPrefix($prefix);

    my $file = $prefix.'/etc/krb5.conf';

    # drop in a minimal /etc/krb5.conf
    my $lines = [];
    if ( -r $file)
    {
	$lines = ReadFile($file);
    }

    my $root = ParseKrb5ConfFile( $lines, $file );

    if ( $opt->{debug} )
    {
        print "Initial File:\n\n";
        DumpConfig( $root );
        print "\n";
    }

    print "***MODIFYING***\n\n" if $opt->{debug};

    my $modifyCount = 0;

    if ( $opt->{join} )
    {
        $modifyCount = JoinDomain( $root, $opt->{join}, $opt->{short}, $opt->{trusts} );
        LinkEtcKrb5($prefix, "krb5.conf");
        LinkEtcKrb5($prefix, "krb5.keytab");
    }
    else
    {
        $modifyCount = LeaveDomain( $root );
    }

    my $newLines = UnparseKrb5ConfFile( $root );
    if ( $modifyCount )
    {
        my $isDifferent = CompareLines( $lines, $newLines );
        if ( not $isDifferent )
        {
            $modifyCount = 0;
        }
    }

    print "MODIFIED ($modifyCount)\n\n" if $opt->{debug} and $modifyCount;

    if ( $opt->{debug} )
    {
        print "Post Modify File:\n\n";
        DumpConfig( $root );
        print "\n";
    }

    if ( $modifyCount )
    {
        print "Modified $file\n";
        ReplaceFile( $file, $newLines ) if ( not $opt->{test} );
    }
    else
    {
        print "No need to modify $file\n";
    }

    return 0;
}
