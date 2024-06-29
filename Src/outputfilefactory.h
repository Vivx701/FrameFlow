#ifndef OUTPUTFILEFACTORY_H
#define OUTPUTFILEFACTORY_H

#include <QObject>
#include <pdffile.h>
#include <imagesprite.h>
#include <videofile.h>
#include <giffile.h>
#include <htmlgallery.h>

/**
 * @brief Enum representing the different types of output files.
 */
enum OutputType{PDF, VIDEO, GIF, SPRITE, HTMLGALLERY};

/**
 * @brief A factory class for creating output file objects.
 */
class OutputFileFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for the OutputFileFactory class.
     * @param parent The parent object.
     */
    explicit OutputFileFactory(QObject *parent = nullptr);

    /**
     * @brief Create a unique pointer to an output file object based on the specified file type.
     * @param fileType The type of output file to create.
     * @return A unique pointer to the created output file object.
     */
    static std::unique_ptr<IOutputFile> createOutputFile(const OutputType& fileType);

signals:

};

#endif // OUTPUTFILEFACTORY_H
