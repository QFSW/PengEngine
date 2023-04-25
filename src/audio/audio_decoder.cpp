#include "audio_decoder.h"

#include <fstream>
#include <filesystem>

#include <core/logger.h>

#include "profiling/scoped_event.h"

using namespace audio;

// DECODE_ERROR begin
#define DECODE_ERROR(fmt, ...)          \
    Logger::error(fmt, __VA_ARGS__);    \
    return RawAudioData::corrupt_data() \
// DECODE_ERROR end

// DECODE_CHECK begin
#define DECODE_CHECK(expr, fmt, ...)    \
    if (!(expr))                        \
    {                                   \
        DECODE_ERROR(fmt, __VA_ARGS__); \
    } do {} while (0)
// DECODE_CHECK end

RawAudioData AudioDecoder::load_file(const std::string& path)
{
    SCOPED_EVENT("AudioDecoder - loading audio file", path.c_str());
    Logger::log("Loading audio file '%s'", path.c_str());

    namespace fs = std::filesystem;
    const fs::path ext = fs::path(path).extension();

    if (ext == ".wav" || ext == ".wave")
    {
        return load_wave(path);
    }

    DECODE_ERROR(
        "Cannot decode audio file '%s': unsupported extension %s",
        path.c_str(), ext.string().c_str()
    );
}

RawAudioData AudioDecoder::load_wave(const std::string& path)
{
    SCOPED_EVENT("AudioDecoder - loading WAVE file");

    std::ifstream file(path, std::ios::binary);
    DECODE_CHECK(file.is_open(), "Cannot open file %s", path.c_str());

    // Encoding for WAV headers and chunks taken from here
    // http://tiny.systems/software/soundProgrammer/WavFormatDocs.pdf

    // TRY_READ begin
#define TRY_READ(dst, size)                                                 \
    do                                                                      \
    {                                                                       \
        file.read(reinterpret_cast<char*>(dst), size);                      \
        DECODE_CHECK(                                                       \
            !file.eof(),                                                    \
            "Invalid WAV file - EOF reached at %d bytes during decoding",   \
            std::streamoff(file.tellg())                                    \
        );                                                                  \
    } while (0)
    // TRY_READ end

    RawAudioData audio_data;

    struct ChunkDescriptor
    {
        uint32_t chunk_id;
        uint32_t chunk_size;
        uint32_t format;
    } chunk_descriptor;

    TRY_READ(&chunk_descriptor, sizeof(chunk_descriptor));
    chunk_descriptor.chunk_id = std::byteswap(chunk_descriptor.chunk_id);
    chunk_descriptor.format = std::byteswap(chunk_descriptor.format);

    DECODE_CHECK(
        chunk_descriptor.chunk_id == 0x52494646,
        "Invalid WAV file - ChunkId == 0x%x instead of 0x52494646 \"RIFF\"",
        chunk_descriptor.chunk_id
    );

    // TODO: check chunk_size

    DECODE_CHECK(
        chunk_descriptor.format == 0x57415645,
        "Invalid WAV file - Format == 0x%x instead of 0x57415645 \"WAVE\"",
        chunk_descriptor.format
    );

    struct FmtSubChunk
    {
        uint32_t sub_chunk_id;
        uint32_t sub_chunk_size;
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_range;
        uint16_t block_align;
        uint16_t bits_per_sample;

        // Unused fields due to PCM encoding assumption
        // ExtraParamSize
        // ExtraParams
    } fmt_sub_chunk;

    TRY_READ(&fmt_sub_chunk, sizeof(fmt_sub_chunk));
    fmt_sub_chunk.sub_chunk_id = std::byteswap(fmt_sub_chunk.sub_chunk_id);

    DECODE_CHECK(
        fmt_sub_chunk.sub_chunk_id == 0x666d7420,
        "Invalid WAV file - Subchunk1ID == 0x%x instead of 0x666d7420 \"fmt \"",
        fmt_sub_chunk.sub_chunk_id
    );

    // TODO: check sub_chunk_size

    DECODE_CHECK(
        fmt_sub_chunk.audio_format == 1,
        "Unsupported WAV encoding 0x%x - must be 0x01 (PCM)",
        fmt_sub_chunk.audio_format
    );

    audio_data.num_channels = fmt_sub_chunk.num_channels;
    DECODE_CHECK(
        fmt_sub_chunk.num_channels > 0,
        "Invalid WAV file - num channels cannot be %d",
        fmt_sub_chunk.num_channels
    );

    audio_data.sample_rate = fmt_sub_chunk.sample_rate;
    DECODE_CHECK(
        fmt_sub_chunk.sample_rate > 0,
        "Invalid WAV file - sample rate cannot be %dHz",
        fmt_sub_chunk.sample_rate
    );

    // TODO: check ByteRate and BlockAlign

    audio_data.bits_per_sample = fmt_sub_chunk.bits_per_sample;
    DECODE_CHECK(
        fmt_sub_chunk.bits_per_sample > 0 && fmt_sub_chunk.bits_per_sample % 8 == 0,
        "Invalid WAV file - invalid bits per sample of %d",
        fmt_sub_chunk.bits_per_sample
    );

    struct DataSubChunk
    {
        uint32_t sub_chunk_id;
        uint32_t sub_chunk_size;
    } data_sub_chunk;

    TRY_READ(&data_sub_chunk, sizeof(data_sub_chunk));
    data_sub_chunk.sub_chunk_id = std::byteswap(data_sub_chunk.sub_chunk_id);

    DECODE_CHECK(
        data_sub_chunk.sub_chunk_id == 0x64617461,
        "Invalid WAV file - Subchunk2ID == 0x%x instead of 0x64617461 \"data\"",
        data_sub_chunk.sub_chunk_id
    );

    audio_data.samples.resize(data_sub_chunk.sub_chunk_size);
    TRY_READ(audio_data.samples.data(), data_sub_chunk.sub_chunk_size);

    return audio_data;

#undef TRY_READ
}
