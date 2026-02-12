#include "thememanager.h"

ThemeManager::ThemeManager(QObject *parent) : QObject(parent),
    m_currentIconTheme(DEFAULTTHEMEICON),
    m_currentStyle(DEFAULTSTYLE)
{
    loadStyles();
    loadIcons();
}

void ThemeManager::loadStyles()
{
    m_themeStyles[DEFAULTSTYLE] = ":/Theme/Resources/Theme/Default.qss";
    m_themeStyles[DARKSTYLE] = ":/Theme/Resources/Theme/Dark.qss";
    m_themeStyles[LIGHTSTYLE] = ":/Theme/Resources/Theme/Light.qss";
}

void ThemeManager::loadIcons()
{
    // Initialize with default themes and icons
    m_themeIcons[MATERIALICON][ICONTYPE::NEW_BUTTON]       = ":/Icons/Material/Resources/Icons/Material/new.png";
    m_themeIcons[MATERIALICON][ICONTYPE::BROWSE_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/add_photo.png";
    m_themeIcons[MATERIALICON][ICONTYPE::CBOARD_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/paste.png";
    m_themeIcons[MATERIALICON][ICONTYPE::EXPORT_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/save.png";
    m_themeIcons[MATERIALICON][ICONTYPE::SETTINGS_BUTTON]  = ":/Icons/Material/Resources/Icons/Material/settings.png";
    m_themeIcons[MATERIALICON][ICONTYPE::ABOUT_BUTTON]     = ":/Icons/Material/Resources/Icons/Material/info.png";
    m_themeIcons[MATERIALICON][ICONTYPE::LEFT_BUTTON]      = ":/Icons/Material/Resources/Icons/Material/left.png";
    m_themeIcons[MATERIALICON][ICONTYPE::RIGHT_BUTTON]     = ":/Icons/Material/Resources/Icons/Material/right.png";
    m_themeIcons[MATERIALICON][ICONTYPE::MOVEBACK_BUTTON]  = ":/Icons/Material/Resources/Icons/Material/image-move-back.png";
    m_themeIcons[MATERIALICON][ICONTYPE::MOVEFRONT_BUTTON] = ":/Icons/Material/Resources/Icons/Material/image-move.png";
    m_themeIcons[MATERIALICON][ICONTYPE::DELETE_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/delete.png";

    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::NEW_BUTTON]      = ":/Icons/Default/Resources/Icons/Default/new-box.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::BROWSE_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/add.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::CBOARD_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/paste.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::EXPORT_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/export.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::SETTINGS_BUTTON] = ":/Icons/Default/Resources/Icons/Default/cog.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::ABOUT_BUTTON]    = ":/Icons/Default/Resources/Icons/Default/information-outline.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::LEFT_BUTTON]     = ":/Icons/Default/Resources/Icons/Default/arrow-left-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::RIGHT_BUTTON]    = ":/Icons/Default/Resources/Icons/Default/arrow-right-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::MOVEBACK_BUTTON] = ":/Icons/Default/Resources/Icons/Default/image-move-back.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::MOVEFRONT_BUTTON]= ":/Icons/Default/Resources/Icons/Default/image-move.png";
    m_themeIcons[DEFAULTTHEMEICON][ICONTYPE::DELETE_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/image-remove-outline.png";
}

QIcon ThemeManager::getIcon(ICONTYPE iconType)
{
    if (!m_themeIcons.contains(m_currentIconTheme))
    {
        return QIcon();
    }
    const auto &themeMap = m_themeIcons.value(m_currentIconTheme);
    if (!themeMap.contains(iconType))
    {
        return QIcon();
    }
    return QIcon(themeMap.value(iconType));
}

void ThemeManager::setIconTheme(const QString &themeName)
{
    if (m_currentIconTheme != themeName && m_themeIcons.contains(themeName)) {
        m_currentIconTheme = themeName;
        emit iconThemeChanged();
    }
}

void ThemeManager::setStyleTheme(const QString &themeName)
{
    if (m_currentStyle != themeName && m_themeStyles.contains(themeName)) {
        m_currentStyle = themeName;
        emit styleThemeChanged();
    }
}

QString ThemeManager::getCurrentStyleSheet()
{
    return m_themeStyles[m_currentStyle];
}

void ThemeManager::reload()
{
    emit iconThemeChanged();
    emit styleThemeChanged();
}

QString ThemeManager::currentStyle() const
{
    return m_currentStyle;
}

QString ThemeManager::currentIconTheme() const
{
    return m_currentIconTheme;
}

QStringList ThemeManager::getIconThemeList()
{
    return m_themeIcons.keys();
}

QStringList ThemeManager::getStyleThemeList()
{
    return m_themeStyles.keys();
}
