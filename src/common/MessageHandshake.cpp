#include "MessageHandshake.hpp"

#include <cstring>
#include <stdexcept>

#include "Log.hpp"

const int MessageHandshake::indexType = 0;
const int MessageHandshake::indexSize = 1;
const int MessageHandshake::indexPayload = 2;

const char MessageHandshake::payloadVersion[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE] =
    MESSAGE_HANDSHAKE_VERSION_PREFIX
    MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER
    MESSAGE_HANDSHAKE_VERSION;
const char MessageHandshake::payloadSwitchToFileTransferControl[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE] =
    MESSAGE_HANDSHAKE_SWITCH_MESSAGE_PREFIX
    MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER;

// TODO tests for MessageHandshake

/*
 * Create a zero-padded handshake message from a given payload.
 */
MessageHandshake::MessageHandshake(
        const char payload[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE],
        size_t payloadSize)
{
    if (payloadSize > MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct handshake message from payload with too large size");
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

MessageHandshake::MessageHandshake(char message[MESSAGE_HANDSHAKE_SIZE])
{
    char payloadSize = 0;

    payloadSize = message[MessageHandshake::indexSize];

    if (payloadSize > MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct handshake message from message with too large payload size");
    }

    std::memcpy(rawMessage, message, sizeof(rawMessage));
}

std::string MessageHandshake::getPayloadVersion(void)
{
    std::string str(MessageHandshake::payloadVersion);
    return str;
}

std::string MessageHandshake::getPayloadSwitchToFileTransferControl(void)
{
    std::string str(MessageHandshake::payloadSwitchToFileTransferControl);
    str += Message::typeFileTransferControl;
    return str;
}

char MessageHandshake::getType(void)
{
    return rawMessage[MessageHandshake::indexType];
}

char MessageHandshake::getSize(void)
{
    return rawMessage[MessageHandshake::indexSize];
}

void MessageHandshake::getRawPayload(char dest[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE])
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

void MessageHandshake::getRawMessage(char dest[MESSAGE_HANDSHAKE_SIZE])
{
    std::string msg = "Copying rawMessage: ";
    msg += rawMessage;
    Log::d(msg);

    msg = "sizeof(rawMessage) is: ";
    msg += std::to_string(sizeof(rawMessage));
    Log::d(msg);

    std::memcpy(dest, rawMessage, sizeof(rawMessage));
}

std::string MessageHandshake::getMessage(void)
{
    std::string message = std::string(rawMessage);

    return message;
}

std::string MessageHandshake::getPayload(void)
{
    char rawPayload[MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE] = {0};

    getRawPayload(rawPayload);
    std::string payload = std::string(rawPayload);

    return payload;
}

std::string MessageHandshake::getVersion(void)
{
    std::string payload = getPayload();
    std::string delimiter = std::string(MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER);
    std::string version;
    const int findStart = 0;
    std::size_t versionStart = 0;
    std::size_t versionLen = 0;

    versionStart = payload.find(delimiter, findStart) + 1;
    if (std::string::npos == versionStart) {
        throw std::runtime_error("Failed to find start of version value from payload");
    }

    versionLen =
        MESSAGE_HANDSHAKE_PAYLOAD_MAX_SIZE -
        std::strlen(MESSAGE_HANDSHAKE_VERSION_PREFIX) -
        std::strlen(MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER);

    version = payload.substr(versionStart, versionLen);

    return version;
}

bool MessageHandshake::canSwitchToMessage(char messageType)
{
    return messageType == Message::typeFileTransferControl;
}

char MessageHandshake::getMessageSwitch(void)
{
    std::string msg;
    std::string payload = getPayload();
    std::string delimiter = std::string(MESSAGE_HANDSHAKE_PAYLOAD_DELIMITER);
    const int findStart = 0;
    std::size_t messageSwitchStart = 0;
    char messageSwitch = 0;

    messageSwitchStart = payload.find(delimiter, findStart) + 1;
    if (std::string::npos == messageSwitchStart) {
        throw std::runtime_error("Failed to find start of message switch value from payload");
    }

    messageSwitch = payload[messageSwitchStart];

    if (!canSwitchToMessage(messageSwitch)) {
        msg = "Unsupported message switch. Trying to switch from handshake to: ";
        msg += messageSwitch;
        throw std::runtime_error(msg);
    }

    return messageSwitch;
}
