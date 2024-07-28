#include <QString>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/timestamp.h>
}

bool trimVideo(const QString &inputPath, const QString &outputPath, double startMs, double endMs);