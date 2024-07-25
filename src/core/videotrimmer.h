#include <QString>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/timestamp.h>
}

class VideoTrimmer
{
public:
    VideoTrimmer();

    void openFile();
    void saveFile(QString location);
};