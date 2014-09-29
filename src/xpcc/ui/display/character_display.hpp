// coding: utf-8
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_CHARACTER_DISPLAY_HPP
#define XPCC_CHARACTER_DISPLAY_HPP

#include <stdint.h>
#include <xpcc/io/iostream.hpp>

namespace xpcc
{

/**
 * Base class for alpha-numeric LCDs (liquid crystal display)
 *
 * About this implementation:
 *
 * In contrast to other drivers which use static methods,
 * this class is directly derived from IODevice with all the virtual
 * methods because it will be most likely used to create a IOStream
 * which then handles the output to the display.
 *
 * Therefore creating this class with static methods and using a wrapper
 * class to create a IODevice seems unnecessary complicated.
 *
 * @author	Fabian Greif
 * @ingroup	lcd
 */
class CharacterDisplay : public IOStream
{
public:
	enum
	Command
	{
		ClearDisplay		= 0b00000001,	///< Clear the display content
		ResetCursor			= 0b00000010,	///< Set cursor to position (0,0)

		IncrementCursor		= 0b00000110,	///< Increments address upon write
		DecrementCursor		= 0b00000100,	///< Decrements address upon write

		DisableDisplay		= 0b00001000,	///< Disable display
		EnableDisplay		= 0b00001100,	///< Enable display
		DisableCursor		= 0b00001000,	///< Disable cursor
		EnableCursor		= 0b00001010,	///< Enable cursor
		DisableCursorBlink	= 0b00001000,	///< Disable blinking cursor
		EnableCursorBlink	= 0b00001001,	///< Enable blinking cursor

		ShiftCursorLeft		= 0b00010000,	///< Shift cursor left
		ShiftCursorRight	= 0b00010100,	///< Shift cursor right
	};

public:
	/// Constructor
	CharacterDisplay(uint8_t width, uint8_t height);

	/// Initialize the display
	void
	initialize();

	/**
	 * Write a character
	 *
	 * This method provides an automatic wrap-round if the output reaches
	 * the end of the current line or a newline character is detected.
	 *
	 * Use writeRaw() if this behavior is not wanted.
	 */
	void
	write(char c);

	/**
	 * Write a raw character at cursor position
	 *
	 * Unlike write() no further processing will occur.
	 *
	 * @see	write()
	 */
	virtual void
	writeRaw(char c) = 0;

	/**
	 * Excute a command.
	 *
	 * @see Command
	 * @param	command		Command to execute
	 */
	void
	execute(Command command);

	/**
	 * Set cursor to specified position
	 *
	 * @param	column	horizontal position
	 * @param	line	vertical position
	 */
	virtual void
	setCursor(uint8_t column, uint8_t line) = 0;

	/// clear the entire display and reset the cursor
	void
	clear();

protected:
	// Interface class for the IOStream
	class Writer : public IODevice
	{
	public:
		Writer(CharacterDisplay *parent) :
			parent(parent)
		{
		}

		/// Draw a single character
		virtual size_t
		write(char c);

		using IODevice::write;

		/// unused
		virtual void
		flush();

		/// unused, returns always \c false
		virtual int16_t
		read();

	private:
		CharacterDisplay *parent;
	};

protected:
	Writer writer;

	uint8_t lineWidth;
	uint8_t lineCount;

	uint8_t column;
	uint8_t line;
};

} // namespace xpcc

#endif // XPCC_CHARACTER_DISPLAY_HPP
