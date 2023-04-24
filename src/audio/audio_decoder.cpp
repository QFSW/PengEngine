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

    if (path == ".wav" || path == ".wave")
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

    std::ifstream file(path);
    if (!file.is_open())
    {
        DECODE_ERROR("Cannot open file %s", path.c_str());
    }

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

    struct FmtSubChunk
    {
        uint32_t sub_chunk1_id;
        uint32_t sub_chunk1_size;
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_range;
        uint16_t block_align;
        uint16_t bits_per_sample;

        // ExtraParamSize
        // ExtraParams
    } fmt_sub_chunk;

    TRY_READ(&chunk_descriptor, sizeof(ChunkDescriptor));
    TRY_READ(&fmt_sub_chunk, sizeof(FmtSubChunk));

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

    DECODE_CHECK(
        fmt_sub_chunk.sub_chunk1_id == 0x666d7420,
        "Invalid WAV file - Subchunk1ID == 0x%x instead of 0x666d7420 \"fmt \"",
        fmt_sub_chunk.sub_chunk1_id
    );

    // TODO: check the rest of the fmt sub chunk

    return audio_data;

#undef TRY_READ
}
