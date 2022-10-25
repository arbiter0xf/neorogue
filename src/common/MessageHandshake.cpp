#include "MessageHandshake.hpp"

#include <cstring>
#include <stdexcept>

#include "Log.hpp"

const int MessageHandshake::indexType = 0;
const int MessageHandshake::indexSize = 1;
const int MessageHandshake::indexPayload = 2;

const char MessageHandshake::payloadVersion[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE]
    = "version:" MESSAGE_HANDSHAKE_VERSION;

// TODO tests for MessageHandshake

/*
 * Create a zero-padded handshake message from a given payload.
 */
MessageHandshake::MessageHandshake(
        const char payload[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE],
        size_t payloadSize)
{
    if (payloadSize > MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct handshake message with too large payload");
    }

    rawMessage[MessageHandshake::indexType] = Message::typeHandshake;
    rawMessage[MessageHandshake::indexSize] = (char) payloadSize;
    std::memcpy(
            rawMessage + MessageHandshake::indexPayload,
            payload,
            payloadSize);

    const int payloadEnd = MessageHandshake::indexPayload + (int) payloadSize;
    if (!(MESSAGE_HANDSHAKE_SIZE == payloadEnd + MessageHandshake::indexPayload)) {
        // Add padding if payload does not reach the end of message.
        for (int i = payloadEnd; i < MESSAGE_HANDSHAKE_SIZE; ++i) {
            rawMessage[i] = 0;
        }
    }
}

char MessageHandshake::getType(void)
{
    return rawMessage[MessageHandshake::indexType];
}

char MessageHandshake::getSize(void)
{
    return rawMessage[MessageHandshake::indexSize];
}

void MessageHandshake::getPayload(char dest[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE])
{
    size_t payloadSize = (size_t) getSize();

    std::string msg = "getPayload(), payloadSize is: ";
    msg += std::to_string(payloadSize);
    Log::d(msg);

    std::memcpy(
            dest,
            rawMessage + MessageHandshake::indexPayload,
            payloadSize);
}

void MessageHandshake::getMessage(char dest[MESSAGE_HANDSHAKE_SIZE])
{
    std::string msg = "Copying rawMessage: ";
    msg += rawMessage;
    Log::d(msg);

    msg = "sizeof(rawMessage) is: ";
    msg += std::to_string(sizeof(rawMessage));
    Log::d(msg);

    std::memcpy(dest, rawMessage, sizeof(rawMessage));
}
