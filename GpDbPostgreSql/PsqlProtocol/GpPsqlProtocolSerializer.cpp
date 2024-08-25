#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolSerializer.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

namespace GPlatform::PSQL {

size_t  ProtocolSerializer::SSerialize
(
    const PSQL::StartupMessageDescRQ&   aMsgDesc,
    GpBytesArray&                       aBufferOut
)
{
    GpByteWriterStorageByteArray    writerStorage{aBufferOut};
    GpByteWriter                    writer{writerStorage};

    // length
    auto lengthRef = writer.Ref<u_int_32>();

    // protocol_version
    writer.UI32(aMsgDesc.protocol_version);

    // attributes
    for (const auto&[key, val]: aMsgDesc.attributes)
    {
        writer.Bytes(key);
        writer.UI8(0);
        writer.Bytes(val);
        writer.UI8(0);
    }

    writer.UI8(0);

    // update length
    const size_t totalSize = writer.TotalWrite();
    lengthRef.Write(NumOps::SConvert<u_int_32>(totalSize));

    return totalSize;
}

size_t  ProtocolSerializer::SSerialize
(
    const SASLInitialResponseDescRQ&    aMsgDesc,
    GpBytesArray&                       aBufferOut
)
{
    GpByteWriterStorageByteArray    writerStorage{aBufferOut};
    GpByteWriter                    writer{writerStorage};

    // message_id
    writer.UI8(static_cast<u_int_8>(aMsgDesc.message_id));

    // length
    auto lengthRef = writer.Ref<u_int_32>();

    // name
    writer.NullTerminatedString(aMsgDesc.name);

    // client_first_message
    writer.UI32(NumOps::SConvert<u_int_32>(std::size(aMsgDesc.client_first_message)));
    writer.Bytes(aMsgDesc.client_first_message);

    // update length
    const size_t totalSize = writer.TotalWrite();
    lengthRef.Write(NumOps::SConvert<u_int_32>(totalSize - 1/*message_id*/));

    return totalSize;
}

size_t  ProtocolSerializer::SSerialize
(
    const SASLResponseDescRQ&   aMsgDesc,
    GpBytesArray&               aBufferOut
)
{
    GpByteWriterStorageByteArray    writerStorage{aBufferOut};
    GpByteWriter                    writer{writerStorage};

    // message_id
    writer.UI8(static_cast<u_int_8>(aMsgDesc.message_id));

    // length
    auto lengthRef = writer.Ref<u_int_32>();

    // client_final_message
    writer.Bytes(aMsgDesc.client_final_message);

    // update length
    const size_t totalSize = writer.TotalWrite();
    lengthRef.Write(NumOps::SConvert<u_int_32>(totalSize - 1/*message_id*/));

    return totalSize;
}

}// namespace GPlatform::PSQL
