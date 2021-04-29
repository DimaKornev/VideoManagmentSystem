#ifndef IPCAMERA_H
#define IPCAMERA_H

#include <QtWidgets>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/avcodec.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>

#include "devicemanagement.h"
#include "mediamanagement.h"

using namespace ONVIF;

typedef struct _tagYUVFrame
{
    unsigned char*  yuvData;
    int         width;
    int         height;
    qint64      frameTime;
}YUVFrame;

typedef void    (fnReceiveFrameProc)(int sender, YUVFrame yuvFrame);

class IpCamera : public QObject
{
    Q_OBJECT
public:
    explicit IpCamera(QObject *parent = 0);
    ~IpCamera();

    static void    init();

    bool    open(QString streamUri);

    void    close();

    void    setReconnect(int reconnect);

    int     isRunning();
    void    setRunning(int);

    void    frameProc();
    void    connectProc();
    YUVFrame    readFrame();
    void    getCurFrame(QByteArray& frameData, int& width, int& height, qint64& imageTime);

    void    setReceiveFrameProc(int sendAddr, fnReceiveFrameProc* proc);

signals:
    void    requestReconnect();
    void    frameChanged();
    void    logOut(QString);

public slots:
    void    reconnect();

protected:
    AVFormatContext *m_fmt_ctx;
    AVCodecContext *m_dec_ctx;
    int m_video_stream_index;

    AVPacket m_packet;
    AVFrame *m_frame;

    HANDLE				m_frameThread;
    HANDLE              m_connectThread;
    QMutex              m_mutex;
    int                 m_running;
    int                 m_connecting;

    unsigned char*      m_frameData[3];
    int                 m_index;

    MediaManagement*    m_mdediaManagement;
    QString             m_ipAddress;
    QString             m_userName;
    QString             m_password;

    QString             m_streamUrl;

    fnReceiveFrameProc* m_receiveFrameProc;
    int                 m_sender;
    int                 m_receiveIFrame;

    int                 m_reconnect;

    YUVFrame            m_curFrame;
    QMutex              m_frameMutex;
    QWaitCondition      m_frameCond;

};

#endif // IPCAMERA_H
