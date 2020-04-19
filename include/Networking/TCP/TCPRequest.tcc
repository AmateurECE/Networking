///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/18/2020
////

#include <Networking/TCP/TCPRequest.h>

#include <unistd.h>

// TODO: Make this class a struct and put it in TCPListener
// TODO: Rename file banners in tcc files.
// TODO: Test with valgrind
// TODO: Don't initialize m_socket twice
template<class HostType>
Networking::TCP::TCPRequest<HostType>
::TCPRequest(int socket, HostType connectingAddress,
             std::function<void(unsigned int,const HostType&)>& userHandler,
             std::function<void(const std::string&)> logStream)
  : m_socket{0}, m_connectingAddress{connectingAddress},
    m_userHandler{userHandler}, m_logStream{logStream}
{
  m_socket = std::shared_ptr<int>{new int, [](int* pInt){
      ::close(*pInt);
      delete pInt;
    }};
  *m_socket = socket;
}

template<class HostType>
void Networking::TCP::TCPRequest<HostType>::handle()
{
  m_userHandler(*m_socket, m_connectingAddress);
}

///////////////////////////////////////////////////////////////////////////////
