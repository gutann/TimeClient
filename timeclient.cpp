#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
using namespace std;

class TimeClient {
public:
	boost::array<char, 8> buf;

	TimeClient() {};
	~TimeClient() {};
	void write_handler(boost::shared_ptr<std::string> pstr, error_code ec,
                size_t bytes_transferred)
        {
                if(ec)
                std::cout<< "發送失敗!" << std::endl;
                else
                std::cout<< *pstr << " 已發送2 " << std::endl;
        }

	void read_handler(const boost::system::error_code& error)
        {	
		ofstream myfile;
  		myfile.open("example.txt");
		myfile.close();
                if(!error)
                {
                        std::cout << "Message: " << buf.data() << std::endl << std::flush;
                }
                else
                {
                        std::cout << "Error occurred." << std::endl << std::flush;
                }
        }


    void start(boost::asio::io_service &io_service, char* server_ip)
    {
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(server_ip, "1000");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);

    	// The connection is open. All we need to do now is read the response from the daytime service.
    	int nFail = 0;
    	for (;;)
    	{
      		// We use a boost::array to hold the received data.
      		//boost::array<char, 128> buf;
      		boost::system::error_code error;

      		// The boost::asio::buffer() function automatically determines
      		// the size of the array to help prevent buffer overruns.
      		boost::shared_ptr<std::string> pstr(new std::string("time from client"));
      		socket.send(boost::asio::buffer(*pstr));
      		cout << "msg send" << endl << std::flush;
      		sleep(2);

#if 0
	//	socket.async_read_some(boost::asio::buffer(buf), boost::bind(&TimeClient::read_handler, this, boost::asio::placeholders::error));
		boost::asio::async_read(socket, 
		boost::asio::buffer(buf),
		boost::asio::transfer_at_least(1),
		boost::bind(&TimeClient::read_handler, this, boost::asio::placeholders::error));
		//std::cout << std::endl;
#endif

#if 1
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
      // When the server closes the connection,
      // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error,
      // which is how we know to exit the loop.
      //if (error == boost::asio::error::eof)
      {
        if (len > 0)
        {
            std::cout.write(buf.data(), len);
            std::cout << std::endl << std::flush;
        }
        else
        {
            nFail++;
            cout << "nothing is read" << endl << std::flush;
        }
        //break; // Connection closed cleanly by peer.
        if (nFail > 2)
           break;
      }
      if (error)
      {
        throw boost::system::system_error(error); // Some other error.
        cout << "Error" << endl << std::flush;
        break;
      }
      sleep(1);
#endif
	}
    }
};

int main(int argc, char* argv[])
{
  try
  {
    // the user should specify the server - the 2nd argument
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    // Any program that uses asio need to have at least one io_service object
    boost::asio::io_service io_service;

    // Convert the server name that was specified as a parameter to the application, to a TCP endpoint. 
    // To do this, we use an ip::tcp::resolver object.
    tcp::resolver resolver(io_service);

    TimeClient client;
    client.start(io_service, argv[1]);
    return 0;
    // A resolver takes a query object and turns it into a list of endpoints. 
    // We construct a query using the name of the server, specified in argv[1], 
    // and the name of the service, in this case "daytime".
    //tcp::resolver::query query(argv[1], "daytime");
    tcp::resolver::query query(argv[1], "1000");

    // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator. 
    // A default constructed ip::tcp::resolver::iterator object can be used as an end iterator.
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Now we create and connect the socket.
    // The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints, 
    // so we need to try each of them until we find one that works. 
    // This keeps the client program independent of a specific IP version. 
    // The boost::asio::connect() function does this for us automatically.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // The connection is open. All we need to do now is read the response from the daytime service.
    int nFail = 0;
    for (;;)
    {
      // We use a boost::array to hold the received data. 
      boost::array<char, 128> buf;
      boost::system::error_code error;

      // The boost::asio::buffer() function automatically determines 
      // the size of the array to help prevent buffer overruns.
      boost::shared_ptr<std::string> pstr(new std::string("AAAACK"));
      socket.send(boost::asio::buffer(*pstr));
      cout << "msg send" << endl;	
      sleep(2);
 
      //boost::asio::streambuf sb;
      //boost::asio::async_read(socket, boost::asio::buffer(buf),boost::bind(&TimeClient::read_handler, this, boost::asio::placeholders::error));
      
#if 0  //sync_read part
 
      size_t len = socket.read_some(boost::asio::buffer(buf), error);
      // When the server closes the connection, 
      // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error, 
      // which is how we know to exit the loop.
      //if (error == boost::asio::error::eof)
      {
	if (len > 0)
	{
	    std::cout.write(buf.data(), len);
            std::cout << std::endl << std::flush;    
	}
	else
	{
	    nFail++;		
            cout << "nothing is read" << endl << std::flush; 
	}
        //break; // Connection closed cleanly by peer.
	if (nFail > 2)
	   break;
      }
      if (error)
      {
        throw boost::system::system_error(error); // Some other error.
     	cout << "Error" << endl << std::flush;
	break;
      }
      sleep(1);
#endif
      //std::cout.write(buf.data(), len);
      std::cout << std::endl;
    }
  }
  // handle any exceptions that may have been thrown.
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
