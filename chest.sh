#!/usr/bin/env sh

if not type cmake 2>/dev/null; then
    echo "Can't find cmake!"
    exit 0;
fi

(cmake -S. -Bbuild)
(make -C build)

