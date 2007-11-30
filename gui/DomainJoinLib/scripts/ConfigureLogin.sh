#!/bin/sh
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

LC_MESSAGES=C; export LC_MESSAGES

SCRIPT_DIR=/opt/likewise-open/bin

alias_replacement()
{
    # Simulates the alias builtin function. It does this by creating a function
    # with the name of what should be aliased. So if it was run like this:
    #   alias_replacement myecho='echo my'
    # Then the alias would be emulated like this:
    #   myecho()
    #   {
    #      echo my "$@"
    #   }
    if [ "$#" -ne 1 ]; then
        echo "alias takes 1 argument"
        return 1
    fi
    # This function is passed something like abc=xyz . The name variable gets
    # set to abc, and value gets set to xyz .
    name="`expr "$1" : '^\(.*\)='`"
    value="`expr "$1" : '.*=\(.*\)$'`"
    eval "$name() { $value \"\$@\"; }"
}

alias aliastest=echo
type aliastest 1>/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
    # This platform doesn't have a working alias. It needs to be replaced. This
    # is primarily for Solaris.
    alias()
    {
        alias_replacement "$@"
    }
fi

type printf 1>/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
    # Usually printf is a shell built in, but on HPUX it is a program located
    # at /bin/printf. During system startup and shutdown the path is only
    # /sbin, so we need to manually find printf
    if [ -x /bin/printf ]; then
        alias printf=/bin/printf
    else
        echo "WARNING: unable to find printf program"
    fi
fi

# echo_replacement emulates echo for all platforms using printf. printf is a
# shell builtin that exists on all platforms.
echo_replacement()
{
    if [ "$1" = "-n" ]; then
        shift;
        printf %s "$*"
    else
        printf %s\\n "$*"
    fi
}

# 'echo -n' works with bash, but not with sh on Solaris, HPUX, and AIX.
if [ "`echo -n`" = "-n" ]; then
    alias echo=echo_replacement
fi

seq_replacement()
{
    FIRST=1
    INCREMENT=1
    case "$#" in
        0)
            echo too few arguments
            return 1
            ;;
        1)
            LAST="$1"
            ;;
        2)
            FIRST="$1"
            LAST="$2"
            ;;
        3)
            FIRST="$1"
            INCREMENT="$2"
            LAST="$3"
            ;;
        *)
            echo too many arguments
            return 1
            ;;
    esac
    i="$FIRST"
    while [ "$i" -le "$LAST" ]; do
        echo "$i"
        i="`expr "$i" + "$INCREMENT"`"
    done
    return 0;
}

# seq doesn't exist on HPUX
type seq 2>/dev/null 1>/dev/null
if [ $? -ne 0 ]; then
    alias seq=seq_replacement
fi

Help()
{
    echo "usage: $0 <enable> | <disable>"
}

GetOsType()
{
    perl -I${SCRIPT_DIR} -MCenteris -e 'print GetOsType';
}

GetDistroType()
{
    perl -I${SCRIPT_DIR} -MCenteris -e 'print GetDistroType';
}

GetDistroVersion()
{
    perl -I${SCRIPT_DIR} -MCenteris -e 'print GetDistroVersion';
}

GracefulSSHDRestart()
{
# can't just restart sshd, 'cause that would tear down
# the remote installer session, so we need to SIGHUP just
# the parent sshd process. However, on SuSE that file
# is called /var/run/sshd.init.pid, and on RedHat it is
# /var/run/sshd.pid...|
    result=0
    files=`ls /var/run/sshd*.pid 2> /dev/null`
    shouldBeRunning=0
    if [ -n "$files" ]; then
        for pidfile in "$files"; do
            if [ -f "$pidfile" ]; then
                shouldBeRunning=1
                kill -HUP `cat $pidfile` || result=$?
            fi
        done
        if [ $shouldBeRunning -ne 0 ]; then
            # sshd on SuSE 9.0 has a bug that causes it to die on a SIGHUP.
            # So try starting sshd in that case.
            if [ `GetDistroType` = "suse" ]; then
                if [ `GetDistroVersion` = "9.0" ]; then
                    /etc/init.d/sshd start > /dev/null 2>&1
                fi
            fi
        fi
    else
        echo "sshd does not appear to be running"
    fi
    # Ignore kill errors because .pid files may have been orphaned.
    return 0
}

FixGdmLoginWithSpaces()
{
    PreSessScript="/etc/X11/gdm/PreSession/Default"

    if [ -f $PreSessScript ]; then
        if [ ! -f ${PreSessScript}.lwidentity.orig ]; then
            cp -p $PreSessScript ${PreSessScript}.lwidentity.orig
        fi

        if grep '/usr/bin/X11/sessreg -a -w /var/log/wtmp -u none -l $DISPLAY $USER' $PreSessScript > /dev/null 2>&1; then

	    if cat $PreSessScript | sed 's/\/usr\/bin\/X11\/sessreg\ -a\ -w\ \/var\/log\/wtmp\ -u\ none\ -l\ \$DISPLAY\ \$USER/\/usr\/bin\/X11\/sessreg\ -a\ -w\ \/var\/log\/wtmp\ -u\ none\ -l\ \$DISPLAY\ \"\$USER\"/' > $PreSessScript.new; then
		mv $PreSessScript.new $PreSessScript
	    fi
        fi
    fi
}

WaitForNetworkManager()
{
    type nm-tool 2>&1 >/dev/null
    if [ $? -ne 0 ]; then
	echo "Could not find nm-tool" >&2
	return 1
    fi

    for count in `seq 15`; do
	echo "Waiting..." >&2
	sleep 1
	output=`nm-tool`
	if [ $? -ne 0 ]; then
	    echo "Could not query NetworkManager" >&2
	    return 1
	fi
	status=`echo "$output" | grep "State: connected"`
        echo "status=$status" >&2
	if [ "$status" = "State: connected" ]; then
	    break;
	fi
	if [ $count -eq 15 ]; then
	    echo "Timed out waiting for NetworkManager" >&2
	    return 1
	fi
    done
    echo "NetworkManager successfully started" >&2

    return 0
}

GracefulDbusRestart()
{
    if [ -x /etc/init.d/dbus ]; then
        # Ubuntu, SuSE
        /etc/init.d/dbus restart
	result=$?
	WaitForNetworkManager
        return $result
    elif [ -x /etc/init.d/messagebus ]; then
        # Fedora
        /etc/init.d/messagebus restart
	result=$?
	WaitForNetworkManager
        return $result
    fi
    return 0
}

GracefulCronRestart()
{
   sh ${SCRIPT_DIR}/gpcron.sh restart 
   return $?
}

GracefulNscdRestart()
{
    if [ -x /etc/init.d/nscd ]; then
        /etc/init.d/nscd status
        nscdStatus=$?
        if [ $nscdStatus -eq 0 ]; then
            /etc/init.d/nscd stop
            /etc/init.d/nscd start
        fi
    fi
    return 0
}

GracefulXDMRestart()
{
    # checks to see if someone is on the graphical console. If so,
    # gracefully bounces the kdm|gdm process. Otherwise,
    # does a less graceful restart

    #Default: check for ":0" in the 2nd column of the who output
    awkExpression='{ print $2 }'

    osType=`GetOsType`
    if [ "SunOS" = "$osType" ]; then
        #if SunOS, check for ":0" in the 6th column of the who output
        awkExpression='{ print $6 }'
    elif [ "Linux" = "$osType" ]; then
        distroType=`GetDistroType`
        if [ "$distroType" = "fedora" ]; then
            if [ `GetDistroVersion` != "3" ]; then
                #if Fedora Linux other than 3, check for ":0" in the 5th column of the who output
                awkExpression='{ print $5 }'
            fi
        fi
        if [ "$distroType" = "centos" -o "$distroType" = "rhel" ]; then
            if [ `GetDistroVersion` = "5" ]; then
                #if Centos 5 Linux, check for ":0" in the 5th column of the who output
                awkExpression='{ print $5 }'
            fi
        fi
    fi

    # On some systems, like Solaris, the X console is identified with :1,
    # so here we actually try a more general search for ":"
    if who | awk "$awkExpression" | grep ":" 1>/dev/null; then
        userViaX=1
    else
        userViaX=0
    fi

    if [ -f "/var/run/gdm.pid" ]; then
	# try to hack around RedHat systems
	# Try to schedule a safe restart first
	if [ -x "/opt/gnome/sbin/gdm-safe-restart" ]; then
	    /opt/gnome/sbin/gdm-safe-restart
	elif [ -x "/usr/sbin/gdm-safe-restart" ]; then
	    /usr/sbin/gdm-safe-restart
        elif [ -x "/etc/init.d/gdm" ]; then
            /etc/init.d/gdm reload
	elif [ $userViaX -eq 1 ]; then
	    # no user logged in via X -- bounce the puppy
	    if [ -x "/opt/gnome/sbin/gdm-restart" ]; then
		/opt/gnome/sbin/gdm-restart
	    elif [ -x "/usr/sbin/gdm-restart" ]; then
		/usr/sbin/gdm-restart
            elif [ -x "/etc/init.d/gdm" ]; then
                /etc/init.d/gdm restart
	    else
		echo "Could not find gdm-restart. Please restart your X server manually"
		echo "Continuing with domain join."
		return 0
	    fi
	else
		echo "Could not find gdm-safe-restart. Please restart your X server manually"
		echo "Continuing with domain join."
		return 0
	fi
    elif [ -x "/etc/init.d/xdm" ]; then
	# handle SuSE systems
	/etc/init.d/xdm status
	xdmStatus=$?
	xdmRestart=0

	if [ $xdmStatus -eq 0 ]; then
	    # xdm is running
	    if [ $userViaX -eq 1 ]; then
		skip=0
                # xdm reload on SuSE 9.0, SuSE 9.1, and SLES 9 has problems
                # (at least when using kdm), so skip in those cases.
                distroType=`GetDistroType`
                if [ "$distroType" = "sles" ]; then
                    distroVersion=`GetDistroVersion`
                    if [ "$distroVersion" = "9" ]; then
                        skip=1
                    fi
                elif [ "$distroType" = "suse" ]; then
                    distroVersion=`GetDistroVersion`
                    if [ "$distroVersion" = "9.0" ]; then
                        skip=1
                    elif [ "$distroVersion" = "9.1" ]; then
                        skip=1
                    fi
                fi
		if [ "0" != "$skip" ]; then
		    echo "Skipping \"/etc/init.d/xdm reload\" as it can cause logout on this OS."
		    echo "Please restart it manually using \"/etc/init.d/xdm reload\""
		    xdmRestart=0
		else
		    /etc/init.d/xdm reload
		    xdmRestart=$?
		fi
	    else
		/etc/init.d/xdm try-restart
		xdmRestart=$?
	    fi

	    if [ "0" != $xdmRestart ]; then
		echo "Could not restart XDM. Please restart it manually using \"/etc/init.d/xdm restart\""
		echo "Continuing with domain join."
		return 0
	    fi

	    return $xdmRestart
	fi
    fi
}

ConfigurePamForADLogin()
{
	if [ 'enable' = "$1" ]; then
		isEnable=1
		options=--enable
	elif [ 'disable' = "$1" ]; then
		isEnable=0
		options=--disable
	else
		echo "ConfigurePamForADLogin <enable|disable>";
		return 1;
	fi

	${SCRIPT_DIR}/domainjoin-cli --log . configure pam $options || return $?

	${SCRIPT_DIR}/domainjoin-cli --log . configure ssh $options || return $?

	GracefulSSHDRestart
	ConfigurePamForADLogin_rc=$?
	if [ "0" != $ConfigurePamForADLogin_rc ]; then
	    echo "Could not restart sshd"
	    if [ "1" = "$isEnable" ]; then
		return $ConfigurePamForADLogin_rc
	    else
		ConfigurePamForADLogin_rc=0
	    fi
	fi

	GracefulXDMRestart
	ConfigurePamForADLogin_rc=$?
	if [ "0" != $ConfigurePamForADLogin_rc ]; then
	    echo "Could not restart XDM"
	    if [ "1" = "$isEnable" ]; then
		return $ConfigurePamForADLogin_rc
	    else
		ConfigurePamForADLogin_rc=0
	    fi
	fi

	GracefulCronRestart
	ConfigurePamForADLogin_rc=$?
	if [ "0" != $ConfigurePamForADLogin_rc ]; then
	    echo "Could not restart Cron"
	    if [ "1" = "$isEnable" ]; then
		return $ConfigurePamForADLogin_rc
	    else
		ConfigurePamForADLogin_rc=0
	    fi
	fi
	
	GracefulDbusRestart
	ConfigurePamForADLogin_rc=$?
	if [ "0" != $ConfigurePamForADLogin_rc ]; then
            echo "Could not restart system message bus (dbus)"
	    if [ "1" = "$isEnable" ]; then
		return $ConfigurePamForADLogin_rc
	    else
		ConfigurePamForADLogin_rc=0
	    fi
	fi

	GracefulNscdRestart
	ConfigurePamForADLogin_rc=$?
	if [ "0" != $ConfigurePamForADLogin_rc ]; then
            echo "Could not restart name service cache daemon (nscd)"
	    if [ "1" = "$isEnable" ]; then
		return $ConfigurePamForADLogin_rc
	    else
		ConfigurePamForADLogin_rc=0
	    fi
	fi
	return $?
}

ConfigureShellPrompt()
{
    if [ 'enable' = "$1" ]; then    
	perl ${SCRIPT_DIR}/ConfigureShellPrompt.pl || return $?
    else
	return 0
    fi
}

ConfigureAppArmor()
{
    if [ 'enable' = "$1" ]; then
	if [ -f /etc/apparmor.d/abstractions/nameservice ]; then
	    if grep "centeris" /etc/apparmor.d/abstractions/nameservice 2>&1 >/dev/null; then
		echo "AppArmor profile already patched" >&2
	    else
		echo "Applying AppArmor profile patch" >&2
		if grep "mr," /etc/apparmor.d/abstractions/nameservice 2>&1 >/dev/null; then
		    cat >>/etc/apparmor.d/abstractions/nameservice <<__AAEOF__
# centeris
/usr/centeris/lib/*.so*     mr,
/usr/centeris/lib64/*.so*   mr,
/tmp/.lwidentity/pipe       rw,
# end centeris
__AAEOF__
		else
		    cat >>/etc/apparmor.d/abstractions/nameservice <<__AAEOF2__
# centeris
/usr/centeris/lib/*.so*     r,
/usr/centeris/lib64/*.so*   r,
/tmp/.lwidentity/pipe       rw,
# end centeris
__AAEOF2__
		fi
		rcapparmor restart || return $?
	    fi
	fi
    fi
    return 0
}

#
# main
#

if [ 0 = "$#" ]; then
    # if the script has no args, show help
    Help
    exit 0
elif [ 'enable' = "$1" ]; then
    /bin/true
elif [ 'disable' = "$1" ]; then
    /bin/true
else
    Help
    exit 1
fi

## now for the real work 
global_rc=0
ConfigurePamForADLogin "$@" || global_rc=$?

## fix restrictions on nscd and other daemones on SuSE systems
ConfigureAppArmor "$@" || global_rc=$?

## fix the bash command prompt on Redhat Systems
ConfigureShellPrompt "$@" || global_rc=$?
FixGdmLoginWithSpaces || global_rc=$?

if [ $global_rc -ne 0 ]; then
    echo "FAILED"
else
    # echo "SUCCESS"
    /bin/true
fi

exit $global_rc
