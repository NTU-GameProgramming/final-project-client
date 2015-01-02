#include "json_socket.h"
#include <iostream>

JsonSocket::JsonSocket(string &ip, string &port) {
	this->ip = ip;
	this->port = port;
	this->ConnectSocket = INVALID_SOCKET;
	this->received_data = "";
	this->json_length = 0;
}

JsonSocket::~JsonSocket() {
	int iResult;
	    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
    }



    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}


// Setting callback function
void JsonSocket::setReceiveCallback(Callback *cb) {
	this->cb_on_receive = cb;
}

void JsonSocket::setErrorCallback(Callback *cb) {
	this->cb_on_error = cb;
}

int JsonSocket::beginConnect() {
	WSADATA wsaData;
	int iResult;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(this->ip.c_str(), this->port.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }


    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }  else { // Check port
			srand(time(NULL));
			for(int port = 2000 + rand() % 100;; ++port) {
				cout << "Check port: " << port << endl;
				sockaddr_in localAddr;
				localAddr.sin_family = AF_INET;
				localAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
				localAddr.sin_port = htons(port);
				if(bind(ConnectSocket,(SOCKADDR *) &localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
					cout << "The port is already in use: " << port << endl;
					//system("pause");
				} else {
					cout << "The port is valid: " << port << endl;
					//system("pause");
					break;
				}
			}
		}
		//u_long mode = 1;
		//ioctlsocket( ConnectSocket, FIONBIO, &mode);



        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
	u_long mode = 1;
	ioctlsocket( ConnectSocket, FIONBIO, &mode);
	return 0;
}

void JsonSocket::sendJsonMessage(Json::Value &json) {
	string formatted_json_string = this->formatJsonString(json);
	int iResult;
	// Send an initial buffer
    iResult = send( ConnectSocket, formatted_json_string.c_str(), formatted_json_string.length(), 0 );
    if (iResult == SOCKET_ERROR) {
		int err_code = WSAGetLastError();
		switch(err_code) {
			case WSAEWOULDBLOCK:
				break;
			default:
				break;
		}
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
       // return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
}

string JsonSocket::formatJsonString(Json::Value &json){
	string formatted_json_str;
	string json_str = this->json_writer.write(json);
	formatted_json_str = std::to_string(static_cast<long long>(json_str.length())) + "#" + json_str;

	return formatted_json_str;
}
void JsonSocket::receiveData() { 
	// Needed to be designed as non-blocking mode.
	int iResult;
	iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if ( iResult > 0 ) {
		//printf("Bytes received: %d\n", iResult);
		this->handleData(recvbuf, iResult, false);
	} else if ( iResult == 0 ) {
		printf("iResult equals zero. \n");
	} else {
		int err_code = WSAGetLastError();
		switch(err_code) {
			case WSAEWOULDBLOCK:
				break;
			default:
				cout << "send failed with error: " << err_code << endl;
				closesocket(ConnectSocket);
				break;
		}
		
	}
	//cout << "iResult = " << iResult << endl;
}

void JsonSocket::handleData(char const *buf, int datsize, bool is_recursive) {
	size_t pos = string::npos;
	if(!is_recursive) {
		//cout << "Buf: " << buf << endl;
		this->received_data.append(buf, datsize);
	}
	//cout << "after append: " << this->received_data << endl;
	if(this->json_length == 0) {  // data begins
		if((pos = this->received_data.find("#")) != string::npos) { // if "#" is found
			this->json_length = stoi(this->received_data.substr(0, pos));
			this->received_data = this->received_data.substr(pos+1);
		}
		//cout << "after begin: " << this->received_data << endl;
	}
	
	if(this->json_length != 0) {
		if(this->received_data.length() == this->json_length) {
			this->parseData(this->received_data);
			// Reset
			this->received_data = "";
			this->json_length = 0;
		} else if(this->received_data.length() >= this->json_length) {
			string partial = this->received_data.substr(0, this->json_length);
			//cout << "partial: " << partial << endl;
			// Trim
			this->received_data = this->received_data.substr(this->json_length);
			//cout << "after trim: " << this->received_data << endl;
			// Reset
			this->json_length = 0;
			this->parseData(partial);
			this->handleData(NULL, NULL, true);
		}
		//cout << "after parsed: " << this->received_data << endl;
	}

}

void JsonSocket::parseData(string &raw_data) {
	//cout << "First Parsed Data: " << raw_data;
	this->json_reader.parse(raw_data, this->json_object);
	//cout << "Second Parsed Data: " << raw_data;
	//cout << "Parse Data: " << this->json_writer.write(this->json_object);
	(this->cb_on_receive)->callback(this->json_object);
}
