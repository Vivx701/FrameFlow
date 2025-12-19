#ifndef GIFFILE_H
#define GIFFILE_H

#include <QObject>
#include <QImage>
#include "IOutputFile.h"


/**
 * @class GifFile
 * @brief A class for creating and saving GIF files.
 *
 * This class provides functionality to create GIF files, including animated GIFs,
 * using the gif.h library.
 */
class GifFile : public IOutputFile
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a GifFile object.
     * @param parent The parent QObject.
     */
    explicit GifFile(QObject *parent = nullptr);

    /**
     * @brief Saves the GIF file.
     *
     * This method handles the entire process of creating a GIF file,
     * including initialization, adding frames, and finalization.
     */
    void save() override;
private:
    int8_t m_depth=8;
};

#endif // GIFFILE_H
