#ifndef ICONTHEMEMANAGER_H
#define ICONTHEMEMANAGER_H

#include <QObject>
#include <QMap>
#include <QIcon>



enum class ICONTYPE {
    NEW_BUTTON,
    BROWSE_BUTTON,
    CBOARD_BUTTON,
    EXPORT_BUTTON,
    SETTINGS_BUTTON,
    ABOUT_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    MOVEBACK_BUTTON,
    MOVEFRONT_BUTTON,
    DELETE_BUTTON
};

//Theme names
#define MATERIAL  "Material"
#define DEFAULTTHEME "Default"


class IconThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit IconThemeManager(QObject *parent = nullptr);

    QIcon getIcon(ICONTYPE iconType);
    void setTheme(const QString &themeName);

signals:
    void themeChanged();

private:
    QString m_currentTheme;
    QMap<QString, QMap<ICONTYPE, QString>> m_themeIcons;
};

#endif // ICONTHEMEMANAGER_H
