# dyed
DYnamic Environment variable Deamon
## Warning 
Please note that dyed is written entirely by chatgpt (including this `readme.md`) and is used at your own risk!

<!--toc:start-->
- [dyed](#dyed)
  - [Warning](#warning)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Configuration File Format](#configuration-file-format)
  - [License](#license)
<!--toc:end-->

`dyed` is a simple tool that monitors a configuration directory for changes and outputs the contents of all regular files in the directory to a single output file. It is designed to be used in conjunction with the dye utility to dynamically update environment variables based on the contents of the configuration directory.
## Installation

To build dyed from source, simply clone the repository and run make:

```sh
$ git clone https://github.com/st0nie/dyed.git
$ cd dyed
$ make
```

This will produce a binary named `dyed` that can be installed anywhere in your system.
## Usage

`dyed` is typically started as a daemon in the background, so that it can continuously monitor the configuration directory for changes. To start the daemon, simply run:

```sh
$ dyed &
```

By default, `dyed` will monitor the `~/.config/dye` directory for changes and output the contents of all regular files in that directory to `/tmp/dyed.env`. These defaults can be overridden by specifying the directory and output file paths as command line arguments:

```sh
$ dyed /path/to/config/dir /path/to/output/file
```

## Configuration File Format

A configuration file is a plain text file containing environment variable assignments in the format `NAME=value`, one per line. Blank lines and lines beginning with `#` are ignored.

Example configuration file:

```sh
FOO=bar
BAZ=qux
```

## License

dyed is licensed under the MIT License. See the LICENSE file for details.
