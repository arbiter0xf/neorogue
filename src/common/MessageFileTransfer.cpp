#include "MessageFileTransfer.hpp"

#include <stdexcept>

const int MessageFileTransfer::indexType = 0;
const int MessageFileTransfer::indexSize =
    MessageFileTransfer::indexType +
    MESSAGE_FILE_TRANSFER_TYPE_SIZE;
const int MessageFileTransfer::indexPayload =
    MessageFileTransfer::indexSize +
    MESSAGE_FILE_TRANSFER_SIZE_SIZE;

MessageFileTransfer::MessageFileTransfer(
        const unsigned char payload[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE],
        size_t payloadSize)
{
    if (payloadSize > MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct file transfer message from payload with too large size");
    }

    rawMessage[MessageFileTransfer::indexType]          = Message::typeFileTransfer;
    rawMessage[MessageFileTransfer::indexSize]          = (unsigned char) (((unsigned short) payloadSize) >> (1 * 8));
    rawMessage[MessageFileTransfer::indexSize + 1]      = (unsigned char) (((unsigned short) payloadSize) >> (0 * 8));
    std::memcpy(
            rawMessage + MessageFileTransfer::indexPayload,
            payload,
            payloadSize);

    const unsigned int payloadEnd = MessageFileTransfer::indexPayload + (unsigned short) payloadSize;
    if (!(MESSAGE_FILE_TRANSFER_SIZE == payloadEnd + MessageFileTransfer::indexPayload)) {
        // Add padding if payload does not reach the end of message.
        for (unsigned int i = payloadEnd; i< MESSAGE_FILE_TRANSFER_SIZE; ++i) {
            rawMessage[i] = 0;
        }
    }
}

MessageFileTransfer::MessageFileTransfer(const unsigned char message[MESSAGE_FILE_TRANSFER_SIZE])
{
    unsigned char payloadSize = 0;
    unsigned char messageType = 0;

    messageType = message[MessageFileTransfer::indexType];
    if (Message::typeFileTransfer != messageType) {
        std::string msg = "Trying to construct file transfer message with incorrect type: ";
        msg += std::to_string((int) messageType);
        throw std::runtime_error(msg);
    }

    payloadSize = message[MessageFileTransfer::indexSize];
    if (payloadSize > MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct file transfer message from message with too large payload size");
    }

    std::memcpy(rawMessage, message, sizeof(rawMessage));
}

unsigned char MessageFileTransfer::getType(void)
{
    return rawMessage[MessageFileTransfer::indexType];
}

// TODO automated test
unsigned short MessageFileTransfer::getSize(void)
{
    unsigned short size = 0;

    size = ((unsigned short) rawMessage[MessageFileTransfer::indexSize])        << (1 * 8);
    size = ((unsigned short) rawMessage[MessageFileTransfer::indexSize + 1])    << (0 * 8);

    return size;
}

void MessageFileTransfer::getPayload(unsigned char dest[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE])
{
    size_t payloadSize = (size_t) getSize();

    std::memcpy(
            dest,
            rawMessage + MessageFileTransfer::indexPayload,
            payloadSize);
}

std::string MessageFileTransfer::getPayloadStr(void)
{
    unsigned char rawPayload[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE] = {0};

    getPayload(rawPayload);
    std::string payload((const char*) rawPayload, getSize());

    return payload;
}

std::string MessageFileTransfer::getPayloadStrFull(void)
{
    unsigned char rawPayload[MESSAGE_FILE_TRANSFER_PAYLOAD_MAX_SIZE] = {0};

    getPayload(rawPayload);
    std::string payload((const char*) rawPayload, sizeof(rawPayload));

    return payload;
}

void MessageFileTransfer::getMessage(unsigned char dest[MESSAGE_FILE_TRANSFER_SIZE])
{
    std::memcpy(dest, rawMessage, sizeof(rawMessage));
}
