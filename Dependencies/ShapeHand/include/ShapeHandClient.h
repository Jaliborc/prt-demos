#ifndef _ShapeHandClient_H_
#define _ShapeHandClient_H_

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

struct ShapeHandFrame;

class ShapeHandClient
{
public:
    ShapeHandClient(unsigned int clientPort, unsigned int serverPort = 3100);
    virtual ~ShapeHandClient();

    /** Assigns the client port. This is the port on which data is received. */
    void setClientPort(unsigned int port);

    /** Returns the client port. */
    unsigned int getClientPort() const;

    /** Assign the server port. This is the port on the remote ShapeRecorder server. */
     void setServerPort(unsigned int port);

    /** Returns the server port. */
    unsigned int getServerPort() const;

    /** Connect to the server, setup sockets for incoming data. */
    bool connect();

    /** Shutdown all socket connections. */
    void disconnect();

    /** Start streaming data. Disconnect will stop data. */
    bool requestDataStream();

    /** Read a single ShapeHand frame from the data stream. */
    bool getFrame(ShapeHandFrame& frame);

private:
    explicit ShapeHandClient() { }

    SOCKET mClientSocket;
    SOCKET mServerSocket;
    unsigned int mClientPort;
    unsigned int mServerPort;
};

#endif  // _ShapeHandClient_H_
