# qtinwt, QtWebKit wrapped in Wt application
# Copyright (C) 2014-2015 Boris Nagaev
# See the LICENSE file for terms of use

Xvfb :99 -screen 0 1024x768x24 +extension RANDR +render > /dev/null 2>&1 &
XVFB_PID=$!
export DISPLAY=:99

./src/qtinwt --docroot /usr/share/Wt/ \
    --http-address 0.0.0.0 --http-port 30522

kill $XVFB_PID
