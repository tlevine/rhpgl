#!/usr/bin/env python2
import sys, subprocess
from tempfile import mktemp

import chiplotle

filename = sys.argv[1]

commands = chiplotle.io.import_hpgl_file(filename)
png_filename = chiplotle.io.export(commands, mktemp(), fmt = 'png')
subprocess.Popen(['feh', png_filename])

# plotter = chiplotle.instantiate_virtual_plotter()
