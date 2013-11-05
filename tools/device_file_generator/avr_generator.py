#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright (c) 2013, Roboterclub Aachen e.V.
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of the Roboterclub Aachen e.V. nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# -----------------------------------------------------------------------------

import os, sys
from device import Device
from avr_reader import AVRDeviceReader
from avr_writer import AVRDeviceWriter
from merger import DeviceMerger
import glob
# add python module logger to path
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'logger'))
from logger import Logger

if __name__ == "__main__":
	"""
	Some test code
	"""
	level = 'info'
	logger = Logger(level)
	devices = []
	
	for arg in sys.argv[1:]:
		if arg in ['error', 'warn', 'info', 'debug', 'disabled']:
			level = arg
			logger.setLogLevel(level)
			continue
		xml_path = os.path.join(os.path.dirname(__file__), '..', '..', '..', 'AVR_devices', (arg + '*'))
		files = glob.glob(xml_path)
		for file in files:
			# deal with this here, rather than rewrite half the name merging
			if os.path.basename(file) != "ATtiny28.xml":
				part = AVRDeviceReader(file, logger)
				devices.append(Device(part, logger))
	
	merger = DeviceMerger(devices, logger)
	merger.mergedByType()
	merger.mergedByName()
	
	folder = os.path.join(os.path.dirname(__file__), '..', '..', 'src', 'xpcc', 'architecture', 'platform', 'xml', 'avr')
	
	for dev in merger.mergedDevices:
		writer = AVRDeviceWriter(dev, logger)
		writer.write(folder)

