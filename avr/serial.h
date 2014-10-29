#define WIN32_LEAN_AND_MEAN
#include <deque>
#include <iostream>
#include <cmath>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/exception/diagnostic_information.hpp> 
#include <wx/colour.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/msgdlg.h>

#include "plotWindow.h"

class Serial
{
public:
	Serial::Serial(wxStatusBar *statusBar, wxTextCtrl *textCtrl, plotWindow* plotWindow);
	void Serial::Connect();

	void Serial::Plot(std::int16_t test, plotWindow* plotWindow);

	void write(const char msg);
	void close();
	bool active();
private:
	boost::asio::io_service m_ioService;
	boost::asio::serial_port m_serialPort;
	boost::asio::deadline_timer m_timer;
	boost::system::error_code m_error;

	wxStatusBar* _statusBar;
	wxTextCtrl* _textCtrl;

	plotWindow* _plotWindow;

	static const int max_read_length = 512; // maximum amount of data to read in one operation

	bool active_; // remains true while this object is still operating
	char read_msg_[max_read_length]; // data read from the socket
	std::deque<char> write_msgs_; // buffered write data

	boost::thread t;

	void Serial::wait_callback(const boost::system::error_code& error);

	void Serial::read_start(void);
	void Serial::read_complete(const boost::system::error_code& error, std::size_t bytes_transferred, plotWindow* plotWindow);

	void Serial::do_write(const char msg);
	void Serial::write_start(void);
	void Serial::write_complete(const boost::system::error_code& error);

	void Serial::do_close(const boost::system::error_code& error);
};