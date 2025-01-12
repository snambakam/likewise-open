#!/bin/bash -x

autoreconf -vif .. && \
../configure \
    CFLAGS="-Wall -Werror -D_FORTIFY_SOURCE=2 -O2" \
    LDFLAGS="-ldl -pie -fPIE" \
    --prefix=/usr \
    --enable-debug=yes \
    --libdir=/usr/lib64 \
    --localstatedir=/var/lib/lightwave
