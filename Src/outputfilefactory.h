#ifndef OUTPUTFILEFACTORY_H
#define OUTPUTFILEFACTORY_H

#include <QObject>
#include <pdffile.h>

enum OutputType{PDF, VIDEO, GIF};
class OutputFileFactory : public QObject
{
    Q_OBJECT
public:
    explicit OutputFileFactory(QObject *parent = nullptr);
    static  std::unique_ptr<IOutputFile> createOutputFile(const OutputType& fileType);

signals:

};

#endif // OUTPUTFILEFACTORY_H
