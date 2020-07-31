# ntfs-recover

A toolkit for recovering ntfs partitions and its files.

## What we have until now

- Unix IO API
- NTFS Boot Sector consistency check

## TODO

- $MTF consistency check
- Search for $MTF
- Force brute detect files and/or recover them to a specified path
- Dump partition information to file
- Build a CLI program
- Backup correct boot sector and $MTF

## Building

Clone and build:
```
$ git clone https://github.com/vSLG/ntfs-recover.git
$ cd ntfs-recover
$ make build -j$(nproc)
```

The binaries will be located at `bin` directory.

## Running

Currently there is no final binary. The generated binary is for testing purposes only. Use at your own risk.

## License
[LGPL-3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html)