#ifndef MESSAGE_FILE_TRANSFER_HPP_DEFINED
#define MESSAGE_FILE_TRANSFER_HPP_DEFINED

#include <string>

#include "Message.hpp"

/*
 * File transfer control message structure:
 * [ message type : size   : payload : padding ]
 * [ 1byte        : 2bytes : xbytes  : ybytes  ]
 * total size = 4096bytes
 * message type identifies this message as a file transfer (FT) message
 * size tells the size of payload
 * xbytes = at maximum 'handshake size' - 'size' - 'message type'
 * ybytes = 'handshake size' - 'message type' - 'size' - 'payload'
 */

#define MESSAGE_FILE_TRANSFER_SIZE 4096
#define MESSAGE_FILE_TRANSFER_TYPE_SIZE 1
#define MESSAGE_FILE_TRANSFER_SIZE_SIZE 2
#define MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE \
    MESSAGE_FILE_TRANSFER_SIZE \
    - MESSAGE_FILE_TRANSFER_TYPE_SIZE \
    - MESSAGE_FILE_TRANSFER_SIZE_SIZE

class MessageFileTransfer
{
public:
    MessageFileTransfer() = default;
    MessageFileTransfer(const unsigned char payload[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE], size_t payloadSize);
    MessageFileTransfer(const unsigned char message[MESSAGE_FILE_TRANSFER_SIZE]);

    unsigned char getType(void);
    unsigned short getSize(void);

    void getPayload(unsigned char dest[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE]);
    std::string getPayloadStr(void);
    std::string getPayloadStrFull(void);

    void getMessage(unsigned char dest[MESSAGE_FILE_TRANSFER_SIZE]);

    static const int indexType;
    static const int indexSize;
    static const int indexPayload;

private:
    unsigned char rawMessage[MESSAGE_FILE_TRANSFER_SIZE];
};

#endif // MESSAGE_FILE_TRANSFER_HPP_DEFINED
