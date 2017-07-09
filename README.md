# OpenCV Video Stream

Open camera and then send each frame through UDP socket.
Client : Read camera -> convert to jpg -> encode to base64 -> send base64 data
Server : Event -> get data -> decode base64 -> show frame data
# Requirements
  - OpenCV 2.4
  - Libevent

# Compiling
```sh
$ cd opencvstream
$ make clean; make
```

# Running server
```sh
$ ./server
```

Make sure udp port 8088 is not used

# Running client
```sh
$ ./client <serverhost> <camera>
```

```sh
$ ./client 10.122.1.99 0
```

Make sure UDP port 8089 is not used

# License

DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
Version 0, July 2017

 Copyright (C) 2017 Arif Nur Khoirudin <hello@arifnurkhoirudin.com>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
0. You just DO WHAT THE FUCK YOU WANT TO.
