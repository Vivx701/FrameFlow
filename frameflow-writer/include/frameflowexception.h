#ifndef FRAMEFLOWEXCEPTION_H
#define FRAMEFLOWEXCEPTION_H

#include <QException>
#include <QString>

#define ERR_EMPTY_OUTPUT_PATH \
      QObject::tr("Output file path is empty.")

#define ERR_FILE_OPEN_FAILED \
      QObject::tr("Failed to open file for writing.")

#define ERR_NO_FRAMES \
      QObject::tr("No frames available to write.")

#define ERR_FRAME_WRITE_FAILED \
      QObject::tr("Failed to write frame.")

#define ERR_UNKNOWN_SAVE \
      QObject::tr("An unknown error occurred while saving the file.")

#define MSG_SAVE_SUCCESS \
      QObject::tr("File saved successfully.")

#define ERR_WRITER_INIT_FAILED \
    QObject::tr("Failed to initialize writer.")

#define ERR_INVALID_FRAMES \
    QObject::tr("Invalid frames.")

#define ERR_INVALID_FILE_TYPE \
    QObject::tr("Invalid file type.")

/* =========================
 *  Codec / FFmpeg Errors
 * ========================= */

#define ERR_CODEC_H264_NOT_FOUND \
QObject::tr("H.264 codec not found.")

#define ERR_STREAM_ALLOC_FAILED \
    QObject::tr("Could not allocate stream.")

#define ERR_CODEC_CONTEXT_ALLOC_FAILED \
    QObject::tr("Could not allocate video codec context.")

#define ERR_CODEC_OPEN_FAILED \
    QObject::tr("Could not open codec.")

class FrameFlowException : public QException
{
public:
    explicit FrameFlowException(QString message)
        : m_message(std::move(message)) {}

    void raise() const override { throw *this; }
    FrameFlowException *clone() const override
    {
        return new FrameFlowException(*this);
    }

    QString message() const { return m_message; }

private:
    QString m_message;
};

#endif // FRAMEFLOWEXCEPTION_H
