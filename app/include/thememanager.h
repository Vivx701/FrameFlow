#ifndef ICONTHEMEMANAGER_H
#define ICONTHEMEMANAGER_H

#include <QObject>
#include <QMap>
#include <QIcon>
#include <QStringList>

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

//Icon Theme names
#define MATERIALICON  "MaterialIcon"
#define DEFAULTTHEMEICON "DefaultIcon"

//Style Theme names
#define DEFAULTSTYLE   "DefaultStyle"
#define DARKSTYLE  "DarkStyle"
#define LIGHTSTYLE  "LightStyle"

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);
    void loadStyles();
    void loadIcons();

    QIcon getIcon(ICONTYPE iconType);
    void setIconTheme(const QString &themeName);
    void setStyleTheme(const QString &themeName);
    QString getCurrentStyleSheet();
    void reload();

    QString currentStyle() const;
    QString currentIconTheme() const;

    QStringList getIconThemeList();
    QStringList getStyleThemeList();

signals:
    void iconThemeChanged();
    void styleThemeChanged();

private:
    QString m_currentIconTheme;
    QString m_currentStyle;
    QMap<QString, QMap<ICONTYPE, QString>> m_themeIcons;
    QMap<QString, QString> m_themeStyles;
};

#endif // ICONTHEMEMANAGER_H
