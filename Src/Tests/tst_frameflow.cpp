#include <QtTest>
#include <QCoreApplication>
#include <FrameFlowSplashScreen.h>
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTest>

class TestFrameFlow : public QObject
{
    Q_OBJECT

public:
    TestFrameFlow();
    ~TestFrameFlow();

private slots:
    void test_splashscreen();
    void test_languageLoading();
    void test_mainWindow();
    void test_writeFile();

private:
    MainWindow* mainWindow;
};

TestFrameFlow::TestFrameFlow()
{
}

TestFrameFlow::~TestFrameFlow()
{
}

void TestFrameFlow::test_splashscreen()
{
    QSize size(640, 360);
    QPixmap backgroundPixmap(":/Images/Resources/Images/splashscreen_bg.png");
    FrameFlowSplashScreen splash(backgroundPixmap);
    splash.setFixedSize(size);
    splash.show();

    // Verify that the splash screen is visible
    QVERIFY(splash.isVisible());

    // Verify the size of the splash screen
    QCOMPARE(splash.size(), size);
    // Process events to ensure the splash screen is rendered
    QTest::qWait(100);

    // Optional: Test hiding the splash screen
    QTimer::singleShot(500, &splash, &QSplashScreen::close);
    QTest::qWait(600);
    QVERIFY(!splash.isVisible());
}

void TestFrameFlow::test_languageLoading()
{

    //test english
    QString langFile = langMap[QLocale::English].first;
    QTranslator  translator;
    // Load the translation file
    QVERIFY(translator.load(langFile));
    qApp->installTranslator(&translator);
    QCOMPARE(QObject::tr(SOFTWARENAME), SOFTWARENAME);


    //test malayalam
    langFile = langMap[QLocale::Malayalam].first;
    // Load the translation file
    QVERIFY(translator.load(langFile));
    qApp->installTranslator(&translator);
    QCOMPARE(QObject::tr(SOFTWARENAME), "ഫ്രെയിംഫ്ലോ");


}

void TestFrameFlow::test_mainWindow()
{
   QSettings settings;
   QString lang = settings.value(LANGUAGE, "en_US").toString();
   QLocale locale(lang);
   QString langFile = langMap[locale.language()].first;
   QTranslator  translator;
   // Load the translation file
   if (translator.load(langFile))
   {
       qApp->setProperty(LANGUAGE.toLocal8Bit().data(), lang);
       qApp->installTranslator(&translator);
    }
    mainWindow =  new MainWindow();
    QVERIFY(mainWindow != nullptr);
    mainWindow->applySettings();
    mainWindow->setWindowTitle(QObject::tr(SOFTWARENAME));
    mainWindow->showMaximized();
    QVERIFY(mainWindow->isMaximized());

    QVERIFY(mainWindow->ui->SideBar != nullptr);
    QVERIFY(mainWindow->ui->imageListView != nullptr);
    QVERIFY(mainWindow->ui->graphicsView != nullptr);
    QVERIFY(mainWindow->ui->propertiesLayout != nullptr);

    //verify buttons
    QVERIFY(mainWindow->newButton != nullptr);
    QVERIFY(mainWindow->fileBrowseButton != nullptr);
    QVERIFY(mainWindow->addFromClipboardButton != nullptr);
    QVERIFY(mainWindow->exportButton != nullptr);
    QVERIFY(mainWindow->settingsButton != nullptr);
    QVERIFY(mainWindow->aboutButton != nullptr);

    QVERIFY(mainWindow->ui->leftButton != nullptr);
    QVERIFY(mainWindow->ui->rightButton != nullptr);
    QVERIFY(mainWindow->ui->moveBackButton != nullptr);
    QVERIFY(mainWindow->ui->moveFrontButton != nullptr);
    QVERIFY(mainWindow->ui->deleteButton != nullptr);

}

void TestFrameFlow::test_writeFile()
{
    PDFAttributes testAttrib;
    testAttrib.specificSettings["Fill"] = "Fill";
    mainWindow->m_model.addImage("://Tests/Files/Red.jpg");
    mainWindow->m_model.addImage("://Tests/Files/Green.jpg");
    mainWindow->m_model.addImage("://Tests/Files/Blue.jpg");
    mainWindow->m_model.addImage("://Tests/Files/Multicolor.jpg");
    QCOMPARE(mainWindow->m_model.rowCount(), 4);

    testAttrib.filePath = "test_output.pdf";
    mainWindow->writeFile(testAttrib, OutputType::PDF);
    // Verify that the file was created
    QVERIFY(QFile::exists("test_output.pdf"));
}


QTEST_MAIN(TestFrameFlow)

#include "tst_frameflow.moc"
