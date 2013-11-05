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

import os, sys, re
from device import Device
from avr_reader import AVRDeviceReader
import glob
# add python module logger to path
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'logger'))
from logger import Logger

if __name__ == "__main__":
	"""
	Some test code
	"""
	level = 'warn'
	logger = Logger(level)
	devices = []
	peri_name = "all"
	bitfield_pattern = ""
	
	for arg in sys.argv[1:]:
		if arg in ['error', 'warn', 'info', 'debug', 'disabled']:
			level = arg
			logger.setLogLevel(level)
			continue
		
		if "ATtiny" in arg or "ATmega" in arg or 'AT90' in arg:
			xml_path = os.path.join(os.path.dirname(__file__), '..', '..', '..', 'AVR_devices', (arg + '*'))
			files = glob.glob(xml_path)
			for file in files:
				# deal with this here, rather than rewrite half the name merging
				if os.path.basename(file) != "ATtiny28.xml":
					part = AVRDeviceReader(file, logger)
					devices.append(Device(part, logger))
			continue
		
		if any(arg.startswith(per) for per in ["EXT", "TWI", "USART", "SPI", "AD_CON", "USB", "CAN", "DA_CON", "USI", "TIMER"]):
			peri_name = arg
			continue
		
		bitfield_pattern = arg
	
	logger.setLogLevel('debug')
	
	peripherals = []
	for dev in devices:
		attributes = dev.getAttributes('peripherals')
		for attribute in attributes:
			for peripheral in [p for p in attribute['value'] if p.name.startswith(peri_name)]:
				peripherals.append({'ids': [dev.id], 'peripheral': peripheral})
	
	registers = []
	for peri in peripherals:
		for reg in peri['peripheral'].registers:
			registers.append({'ids': list(peri['ids']), 'register': reg})
	
	registers.sort(key=lambda k : k['register'].name)
	merged = []
	
	while len(registers) > 0:
		current = registers[0]
		registers.remove(current)
		
		matches = []
		
		for peri in registers:
			if current['register'] == peri['register']:
				matches.append(peri)
		
		for match in matches:
			current['ids'].extend(match['ids'])
			registers.remove(match)
		
		if len(matches) == 0:
			logger.warn("No match for register: " + current['register'].name + " of " + str([id.string for id in current['ids']]))
		
		merged.append(current)
	
	filtered_devices = []
	filtered_registers = []
	all_names = []
	
	for dev in merged:
		reg = dev['register']
		dev['ids'].sort(key=lambda k : (int(k.name or 0), k.type))
		all_names.extend([id.string for id in dev['ids']])
		if bitfield_pattern == "":
			filtered_registers.append(dev['register'].name)
			s = "Devices:\n"
			ii = 0
			for id in dev['ids']:
				s += id.string.replace("at","") + "  \t"
				ii += 1
				if ii > 7:
					ii = 0
					s += "\n"
			logger.debug(s)
			logger.info(str(reg) + "\n")
		
		if reg.getFieldsWithPattern(bitfield_pattern) != None:
			filtered_registers.append(dev['register'].name)
			filtered_devices.append(dev)
	
	all_filtered_names = []
	if bitfield_pattern != "":
		logger.info("Registers containing BitField pattern '" + bitfield_pattern + "':")
		for dev in filtered_devices:
			all_filtered_names.extend([id.string for id in dev['ids']])
			s = "Devices:\n"
			ii = 0
			for id in dev['ids']:
				s += id.string.replace("at","") + "  \t"
				ii += 1
				if ii > 7:
					ii = 0
					s += "\n"
			logger.debug(s)
			logger.info(str(dev['register']) + "\n")
	
	filtered_registers = list(set(filtered_registers))
	filtered_registers.sort()
	
	logger.info("Summary registers:")
	for name in filtered_registers:
		logger.debug(name)
	logger.info("Remaining devices:")
	all_names = set(all_names) - set(all_filtered_names)
	all_names = list(all_names)
	all_names.sort()
	s = "\n"
	ii = 0
	for id in all_names:
		s += id.replace("at","") + "  \t"
		ii += 1
		if ii > 7:
			ii = 0
			s += "\n"
	logger.debug(s)
