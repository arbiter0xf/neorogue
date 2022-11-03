#ifndef MESSAGE_HANDSHAKE_HPP_DEFINED
#define MESSAGE_HANDSHAKE_HPP_DEFINED

#include <string>

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
#define MESSAGE_HANDSHAKE_VERSION_PREFIX "version"

#define MESSAGE_HANDSHAKE_SWITCH_MESSAGE_PREFIX "switch_to_message"

#define MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER ":"

class MessageHandshake {
public:
    MessageHandshake() = default;
    MessageHandshake(
            const char payload[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE],
            size_t payloadSize);
    MessageHandshake(
            char message[MESSAGE_HANDSHAKE_SIZE]);

    char getSize(void);
    char getType(void);
    void getRawPayload(char dest[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE]);
    void getRawMessage(char dest[MESSAGE_HANDSHAKE_SIZE]);
    std::string getMessage(void);
    std::string getPayload(void);
    std::string getVersion(void);
    char getMessageSwitch(void);

    bool canSwitchToMessage(char messageType);

    static std::string getPayloadVersion(void);
    static std::string getPayloadSwitchToFileTransferControl(void);

    static const int indexType;
    static const int indexSize;
    static const int indexPayload;

private:
    static const char payloadVersion[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE];
    static const char payloadSwitchToFileTransferControl[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE];

    char rawMessage[MESSAGE_HANDSHAKE_SIZE];
};

#endif // MESSAGE_HANDSHAKE_HPP_DEFINED
