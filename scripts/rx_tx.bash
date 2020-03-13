cmd() { printf '%s\r' "$@" > /dev/ttyUSB0 && read -rd $'\n' rx < /dev/ttyUSB0 && printf '>%s\n' "$rx"; }
# cmd() { printf '%s\r' "$@" > /dev/ttyUSB0 && hexdump -C < /dev/ttyUSB0; }