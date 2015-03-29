# QtWebKit wrapped in Wt application with PNG web snapshots

[![Build Status](https://travis-ci.org/starius/qtinwt.png?branch=master)](https://travis-ci.org/starius/qtinwt)
[![License](http://img.shields.io/badge/License-GPL2-brightgreen.png)](LICENSE)

Build and install (Debian):

```bash
$ sudo apt-get install make cmake cmake-curses-gui g++ \
    libwt-dev libwthttp-dev libqt4-dev libqtwebkit-dev xvfb
$ cmake .
$ make
$ ./run.sh
```

Open http://127.0.0.1:30522/ in Web browser with JavaScript on.
