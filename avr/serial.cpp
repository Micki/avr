#include "serial.h"

Serial::Serial(wxStatusBar *statusBar, wxTextCtrl *textCtrl, plotWindow* plotWindow)
	: active_(true), m_ioService(), m_timer(m_ioService), m_serialPort(m_ioService)
{
	_statusBar = statusBar;
	_textCtrl = textCtrl;
	_plotWindow = plotWindow;

}

void Serial::Connect()
{
	_statusBar->SetForegroundColour(wxColour(0, 100, 0));

	wxStaticText* text = new wxStaticText(_statusBar, wxID_ANY, wxT("Connected"), wxPoint(10, 5), wxDefaultSize, 0);


	try {
		//boost::asio::serial_port m_serialPort(m_ioService, "COM3");

		m_serialPort.open("COM3");
		m_serialPort.set_option(boost::asio::serial_port::baud_rate(250000));
		m_serialPort.set_option(boost::asio::serial_port::character_size(8U));
		m_serialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port_base::stop_bits::one));
		m_serialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port_base::parity::type::even));
		m_serialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port_base::flow_control::type::hardware));

		read_start();

		boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
	}
	catch (boost::system::system_error& e)
	{
		wxMessageBox(boost::diagnostic_information(e));

		_statusBar->ClearBackground();
		_statusBar->SetForegroundColour(wxColour(100, 0, 0));
		_statusBar->SetLabelText(wxT("Connection failed"));
	}

	text->Show(true);
}

void Serial::write(const char msg) // pass the write data to the do_write function via the io service in the other thread
{
	m_ioService.post(boost::bind(&Serial::do_write, this, msg));
}

void Serial::close() // call the do_close function via the io service in the other thread
{
	m_ioService.post(boost::bind(&Serial::do_close, this, boost::system::error_code()));
}

bool Serial::active() // return true if the socket is still active
{
	return active_;
}

void Serial::read_start(void)
{ // Start an asynchronous read and call read_complete when it completes or fails
	//m_serialPort.async_read_some(
	//	boost::asio::buffer(read_msg_, max_read_length),
	//	boost::bind(
	//		&Serial::read_complete,
	//		this,
	//		boost::asio::placeholders::error,
	//		boost::asio::placeholders::bytes_transferred
	//	)
	//);

	m_serialPort.async_read_some(
		boost::asio::buffer(read_msg_, max_read_length),
		boost::bind(&Serial::read_complete, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, _plotWindow)
		);


	m_timer.expires_from_now(boost::posix_time::milliseconds(2000));
	m_timer.async_wait(
		boost::bind(&Serial::wait_callback, this, boost::asio::placeholders::error)
		);

}

void Serial::wait_callback(const boost::system::error_code& error)
{
	if (error)
	{
		// Data was read and this timeout was canceled
		return;
	}

	_textCtrl->AppendText("timeout");
	m_serialPort.cancel();
	this->do_close(error);

	//m_serialPort.cancel();  // will cause read_callback to fire with an error
}

void Serial::read_complete(const boost::system::error_code& error, std::size_t bytes_transferred, plotWindow* plotWindow)
{ // the asynchronous read operation has now completed or failed and returned an error
	if (error || !bytes_transferred)
	{
		// No data was read!
		this->do_close(error);
		return;
	}

	m_timer.cancel();

	std::stringstream ss;
	ss << read_msg_;

	plotWindow->Plot();

	_textCtrl->AppendText(ss.str());

	read_start();
}

void Serial::do_write(const char msg)
{ // callback to handle write call from outside this class
	bool write_in_progress = !write_msgs_.empty(); // is there anything currently being written?
	write_msgs_.push_back(msg); // store in write buffer
	if (!write_in_progress) // if nothing is currently being written, then start
		write_start();
}

void Serial::write_start(void)
{ // Start an asynchronous write and call write_complete when it completes or fails
	boost::asio::async_write(m_serialPort,
		boost::asio::buffer(&write_msgs_.front(), 1),
		boost::bind(&Serial::write_complete,
		this,
		boost::asio::placeholders::error));
}

void Serial::write_complete(const boost::system::error_code& error)
{ // the asynchronous read operation has now completed or failed and returned an error
	if (!error)
	{ // write completed, so send next write data
		write_msgs_.pop_front(); // remove the completed data
		if (!write_msgs_.empty()) // if there is anthing left to be written
			write_start(); // then start sending the next item in the buffer
	}
	else
		do_close(error);
}

void Serial::do_close(const boost::system::error_code& error)
{ // something has gone wrong, so close the socket & make this object inactive
	std::stringstream ss;

	if (error == boost::asio::error::operation_aborted) // if this call is the result of a timer cancel()
		return; // ignore it because the connection cancelled the timer

	if (error)
		ss << "Error: " << error.message() << std::endl; // show the error message
	else
		ss << "Error: Connection did not succeed.\n";
	ss << "Press Enter to exit\n";

	wxMessageBox(ss.str());

	m_serialPort.close();
	active_ = false;
}