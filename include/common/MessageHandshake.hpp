#ifndef MESSAGE_HANDSHAKE_HPP_DEFINED
#define MESSAGE_HANDSHAKE_HPP_DEFINED

#include "Message.hpp"

/*
 * Handshake message structure:
 * [ message type : size  : payload : padding ]
 * [ 1byte        : 1byte : xbytes  : ybytes  ]
 * handshake size = 32bytes
 * message type identifies this message as a handshake message
 * size tells the size of payload
 * xbytes = at maximum 'handshake size' - 'size' - 'message type'
 * ybytes = 'handshake size' - 'message type' - 'size' - 'payload'
 */

#define MESSAGE_HANDSHAKE_SIZE 32
#define MESSAGE_HANDSHAKE_TYPE_SIZE 1
#define MESSAGE_HANDSHAKE_SIZE_SIZE 1
#define MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE \
    MESSAGE_HANDSHAKE_SIZE \
    - MESSAGE_HANDSHAKE_TYPE_SIZE \
    - MESSAGE_HANDSHAKE_SIZE_SIZE

#define MESSAGE_HANDSHAKE_VERSION "0.0.1"

class MessageHandshake {
public:
    MessageHandshake(
            const char payload[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE],
            size_t payloadSize);

    char getSize(void);
    char getType(void);
    void getPayload(char dest[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE]);
    void getMessage(char dest[MESSAGE_HANDSHAKE_SIZE]);

    static const int indexType;
    static const int indexSize;
    static const int indexPayload;

    inline static const char version[] = MESSAGE_HANDSHAKE_VERSION;

    static const char payloadVersion[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE];

private:
    char rawMessage[MESSAGE_HANDSHAKE_SIZE];
};

#endif // MESSAGE_HANDSHAKE_HPP_DEFINED
