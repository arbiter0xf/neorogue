#ifndef MESSAGE_FILE_TRANSFER_CONTROL_HPP_DEFINED
#define MESSAGE_FILE_TRANSFER_CONTROL_HPP_DEFINED

#include <string>

#include "Message.hpp"

/*
 * File transfer control message structure:
 * [ message type : size  : payload : padding ]
 * [ 1byte        : 1byte : xbytes  : ybytes  ]
 * total size = 32bytes
 * message type identifies this message as a file transfer control (FTC) message
 * size tells the size of payload
 * xbytes = at maximum 'handshake size' - 'size' - 'message type'
 * ybytes = 'handshake size' - 'message type' - 'size' - 'payload'
 */

#define MESSAGE_FTC_SIZE 32
#define MESSAGE_FTC_TYPE_SIZE 1
#define MESSAGE_FTC_SIZE_SIZE 1
#define MESSAGE_FTC_PAYLOAD_MAX_SIZE \
    MESSAGE_FTC_SIZE \
    - MESSAGE_FTC_TYPE_SIZE \
    - MESSAGE_FTC_SIZE_SIZE

enum class FtcRequest {
    unknown,
    getMaps,
    getMapAssets,
};

class MessageFileTransferControl
{
public:
    MessageFileTransferControl() = default;
    MessageFileTransferControl(const unsigned char payload[MESSAGE_FTC_PAYLOAD_MAX_SIZE], size_t payloadSize);
    MessageFileTransferControl(const unsigned char message[MESSAGE_FTC_SIZE]);

    static void getPayloadGetMaps(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE]);
    static void getPayloadGetMapAssets(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE]);
    static std::string getPayloadGetMapsStr(void);
    static std::string getPayloadGetMapAssetsStr(void);

    unsigned char getType(void);
    unsigned char getSize(void);
    void getRawPayload(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE]);
    void getRawMessage(unsigned char dest[MESSAGE_FTC_SIZE]);
    std::string getMessageStr(void);
    std::string getPayloadStr(void);

    FtcRequest matchToRequest(void);

    static const int indexType;
    static const int indexSize;
    static const int indexPayload;

private:
    static const unsigned char payloadGetMaps[MESSAGE_FTC_PAYLOAD_MAX_SIZE];
    static const unsigned char payloadGetMapAssets[MESSAGE_FTC_PAYLOAD_MAX_SIZE];

    unsigned char rawMessage[MESSAGE_FTC_SIZE];
};

#endif // MESSAGE_FILE_TRANSFER_CONTROL_HPP_DEFINED
