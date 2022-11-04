#include "MessageFileTransferControl.hpp"

#include <stdexcept>
#include "Log.hpp"

const unsigned char MessageFileTransferControl::payloadGetMaps[MESSAGE_FTC_PAYLOAD_MAX_SIZE] = "get maps";
const unsigned char MessageFileTransferControl::payloadGetMapAssets[MESSAGE_FTC_PAYLOAD_MAX_SIZE] = "get map assets";

const int MessageFileTransferControl::indexType = 0;
const int MessageFileTransferControl::indexSize = 1;
const int MessageFileTransferControl::indexPayload = 2;

MessageFileTransferControl::MessageFileTransferControl(
        const unsigned char payload[MESSAGE_FTC_PAYLOAD_MAX_SIZE],
        size_t payloadSize)
{
    if (payloadSize > MESSAGE_FTC_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct ftc message from payload with too large size");
    }

    rawMessage[MessageFileTransferControl::indexType] = Message::typeFileTransferControl;
    rawMessage[MessageFileTransferControl::indexSize] = (unsigned char) payloadSize;
    std::memcpy(
            rawMessage + MessageFileTransferControl::indexPayload,
            payload,
            payloadSize);

    const int payloadEnd = MessageFileTransferControl::indexPayload + (unsigned int) payloadSize;
    if (!(MESSAGE_FTC_SIZE == payloadEnd + MessageFileTransferControl::indexPayload)) {
        // Add padding if payload does not reach the end of message.
        for (unsigned int i = payloadEnd; i < MESSAGE_FTC_SIZE; ++i) {
            rawMessage[i] = 0;
        }
    }
}

MessageFileTransferControl::MessageFileTransferControl(const unsigned char message[MESSAGE_FTC_SIZE])
{
    unsigned char payloadSize = 0;
    unsigned char messageType = 0;

    messageType = message[MessageFileTransferControl::indexType];
    if (messageType != Message::typeFileTransferControl) {
        std::string msg = "Trying to construct ftc message with incorrect type: ";
        msg += std::to_string((int) messageType);
        throw std::runtime_error(msg);
    }

    payloadSize = message[MessageFileTransferControl::indexSize];
    if (payloadSize > MESSAGE_FTC_PAYLOAD_MAX_SIZE) {
        throw std::runtime_error("Trying to construct ftc message from message with too large payload size");
    }

    std::memcpy(rawMessage, message, sizeof(rawMessage));
}

void MessageFileTransferControl::getPayloadGetMaps(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE])
{
    std::memcpy(dest, payloadGetMaps, sizeof(payloadGetMaps));
}

void MessageFileTransferControl::getPayloadGetMapAssets(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE])
{
    std::memcpy(dest, payloadGetMapAssets, sizeof(payloadGetMapAssets));
}

std::string MessageFileTransferControl::getPayloadGetMapsStr(void)
{
    std::string str((const char*) payloadGetMaps, sizeof(payloadGetMaps));
    return str;
}

std::string MessageFileTransferControl::getPayloadGetMapAssetsStr(void)
{
    std::string str((const char*) payloadGetMapAssets, sizeof(payloadGetMapAssets));
    return str;
}

unsigned char MessageFileTransferControl::getType(void)
{
    return rawMessage[MessageFileTransferControl::indexType];
}

unsigned char MessageFileTransferControl::getSize(void)
{
    return rawMessage[MessageFileTransferControl::indexSize];
}

void MessageFileTransferControl::getRawMessage(unsigned char dest[MESSAGE_FTC_SIZE])
{
    std::memcpy(dest, rawMessage, sizeof(rawMessage));
}

void MessageFileTransferControl::getRawPayload(unsigned char dest[MESSAGE_FTC_PAYLOAD_MAX_SIZE])
{
    size_t payloadSize = (size_t) getSize();

    std::memcpy(
            dest,
            rawMessage + MessageFileTransferControl::indexPayload,
            payloadSize);
}

std::string MessageFileTransferControl::getMessageStr(void)
{
    std::string message((const char*) rawMessage, sizeof(rawMessage));
    return message;
}

std::string MessageFileTransferControl::getPayloadStr(void)
{
    unsigned char rawPayload[MESSAGE_FTC_PAYLOAD_MAX_SIZE] = {0};

    getRawPayload(rawPayload);
    std::string payload((const char*) rawPayload, sizeof(rawPayload));

    return payload;
}

FtcRequest MessageFileTransferControl::matchToRequest(void)
{
    std::string payload = getPayloadStr();

    if (0 == payload.compare(MessageFileTransferControl::getPayloadGetMapsStr())) {
        return FtcRequest::getMaps;
    }

    if (0 == payload.compare(MessageFileTransferControl::getPayloadGetMapAssetsStr())) {
        return FtcRequest::getMapAssets;
    }

    return FtcRequest::unknown;
}
