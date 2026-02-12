#ifndef FRAMEFLOWWRITER_H
#define FRAMEFLOWWRITER_H


#include "frameflow-writer_global.h"
#include <QObject>
#include <imagesprite.h>
#include <videofile.h>
#include <giffile.h>

/**
 * @brief A factory class for creating output file objects.
 */
class FRAMEFLOW_WRITER_EXPORT FrameFlowWriter: public QObject
{
public:
    /**
     * @brief Constructor for the OutputFileFactory class.
     * @param parent The parent object.
     */
    explicit FrameFlowWriter(QObject *parent = nullptr);

    /**
     * @brief Create a unique pointer to an output file object based on the specified file type.
     * @param fileType The type of output file to create.
     * @return A unique pointer to the created output file object.
     */
    static std::unique_ptr<IOutputFile> createOutputFile(const OutputType& fileType);
};

#endif // FRAMEFLOWWRITER_H
