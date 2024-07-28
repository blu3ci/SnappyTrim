#include "videotrimmer.h"

bool trimVideo(const QString &inputPath, const QString &outputPath, double startMs, double endMs)
{
    const AVOutputFormat *outputFormat = NULL;
    AVFormatContext *inputContext = NULL;
    AVFormatContext *outputContext = NULL;
    AVPacket *pkt = NULL;
    int ret;
    int streamIndex = 0;
    int *streamMapping = NULL;
    int streamMappingSize = 0;
    int *streamRescaledStart = NULL;
    int *streamRescaledEnd = NULL;
    double startSeconds = startMs / 1000;
    double endSeconds = endMs / 1000;

    pkt = av_packet_alloc();

    if (!pkt)
        return false;

    if ((ret = avformat_open_input(&inputContext, inputPath.toLatin1().data(), 0, 0)) < 0)
    {
        goto end;
    }

    if ((ret = avformat_find_stream_info(inputContext, 0)) < 0)
    {
        goto end;
    }

    avformat_alloc_output_context2(&outputContext, NULL, NULL, outputPath.toLatin1().data());

    if (!outputContext)
    {
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    streamMappingSize = inputContext->nb_streams;
    streamMapping = (int *)av_calloc(streamMappingSize, sizeof(*streamMapping));
    streamRescaledStart = (int *)av_calloc(streamMappingSize, sizeof(*streamRescaledStart));
    streamRescaledEnd = (int *)av_calloc(streamMappingSize, sizeof(*streamRescaledEnd));

    if (!streamMapping)
    {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    outputFormat = outputContext->oformat;

    for (size_t i = 0; i < streamMappingSize; i++)
    {
        AVStream *inStream = inputContext->streams[i];
        AVStream *outStream;
        AVCodecParameters *inCodecpar = inStream->codecpar;

        streamRescaledStart[i] = av_rescale_q(startSeconds * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);
        streamRescaledEnd[i] = av_rescale_q(endSeconds * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);

        if (inCodecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            inCodecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            inCodecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            streamMapping[i] = -1;
            continue;
        }

        streamMapping[i] = streamIndex++;

        outStream = avformat_new_stream(outputContext, NULL);

        if (!outStream)
        {
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        if ((ret = avcodec_parameters_copy(outStream->codecpar, inCodecpar)) < 0)
            goto end;

        outStream->codecpar->codec_tag = 0;
    }

    if (!(outputFormat->flags & AVFMT_NOFILE))
    {
        if ((ret = avio_open(&outputContext->pb, outputPath.toLatin1().data(), AVIO_FLAG_WRITE)) < 0)
            goto end;
    }

    if ((ret = avformat_write_header(outputContext, NULL)) < 0)
        goto end;

    if ((ret = avformat_seek_file(inputContext, -1, INT64_MIN, startSeconds * AV_TIME_BASE, startSeconds * AV_TIME_BASE, 0)))
        goto end;

    while (true)
    {
        if ((ret = av_read_frame(inputContext, pkt)) < 0)
            break;

        if (pkt->stream_index >= streamMappingSize || streamMapping[pkt->stream_index] < 0 || pkt->pts > streamRescaledEnd[pkt->stream_index])
        {
            av_packet_unref(pkt);
            continue;
        }

        pkt->stream_index = streamMapping[pkt->stream_index];

        pkt->pts -= streamRescaledStart[pkt->stream_index];
        pkt->dts -= streamRescaledStart[pkt->stream_index];

        av_packet_rescale_ts(pkt, inputContext->streams[pkt->stream_index]->time_base, outputContext->streams[pkt->stream_index]->time_base);
        pkt->pos = -1;

        if ((ret = av_interleaved_write_frame(outputContext, pkt)) < 0)
            break;
    }

    av_write_trailer(outputContext);

end:
    av_packet_free(&pkt);

    avformat_close_input(&inputContext);

    if (outputContext && !(outputFormat->flags & AVFMT_NOFILE))
        avio_closep(&outputContext->pb);
    avformat_free_context(outputContext);

    av_freep(&streamMapping);
    av_freep(&streamRescaledStart);
    av_freep(&streamRescaledEnd);

    if (ret < 0 && ret != AVERROR_EOF)
    {
        return false;
    }

    return true;
}
