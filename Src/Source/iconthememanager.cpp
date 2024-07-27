#include "iconthememanager.h"

IconThemeManager::IconThemeManager(QObject *parent) : QObject(parent)
{
    // Initialize with default themes and icons

    m_themeIcons[MATERIAL][ICONTYPE::NEW_BUTTON]       = ":/Icons/Material/Resources/Icons/Material/new.png";
    m_themeIcons[MATERIAL][ICONTYPE::BROWSE_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/add_photo.png";
    m_themeIcons[MATERIAL][ICONTYPE::CBOARD_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/paste.png";
    m_themeIcons[MATERIAL][ICONTYPE::EXPORT_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/save.png";
    m_themeIcons[MATERIAL][ICONTYPE::SETTINGS_BUTTON]  = ":/Icons/Material/Resources/Icons/Material/settings.png";
    m_themeIcons[MATERIAL][ICONTYPE::ABOUT_BUTTON]     = ":/Icons/Material/Resources/Icons/Material/info.png";
    m_themeIcons[MATERIAL][ICONTYPE::LEFT_BUTTON]      = ":/Icons/Material/Resources/Icons/Material/left.png";
    m_themeIcons[MATERIAL][ICONTYPE::RIGHT_BUTTON]     = ":/Icons/Material/Resources/Icons/Material/right.png";
    m_themeIcons[MATERIAL][ICONTYPE::MOVEBACK_BUTTON]  = ":/Icons/Material/Resources/Icons/Material/image-move-back.png";
    m_themeIcons[MATERIAL][ICONTYPE::MOVEFRONT_BUTTON] = ":/Icons/Material/Resources/Icons/Material/image-move.png";
    m_themeIcons[MATERIAL][ICONTYPE::DELETE_BUTTON]    = ":/Icons/Material/Resources/Icons/Material/delete.png";



    m_themeIcons[DEFAULTTHEME][ICONTYPE::NEW_BUTTON]      = ":/Icons/Default/Resources/Icons/Default/new-box.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::BROWSE_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/add.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::CBOARD_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/paste.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::EXPORT_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/export.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::SETTINGS_BUTTON] = ":/Icons/Default/Resources/Icons/Default/cog.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::ABOUT_BUTTON]    = ":/Icons/Default/Resources/Icons/Default/information-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::LEFT_BUTTON]     = ":/Icons/Default/Resources/Icons/Default/arrow-left-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::RIGHT_BUTTON]    = ":/Icons/Default/Resources/Icons/Default/arrow-right-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::MOVEBACK_BUTTON] = ":/Icons/Default/Resources/Icons/Default/image-move-back.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::MOVEFRONT_BUTTON]= ":/Icons/Default/Resources/Icons/Default/image-move.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::DELETE_BUTTON]   = ":/Icons/Default/Resources/Icons/Default/image-remove-outline.png";

    m_currentTheme = DEFAULTTHEME; // Set default theme
}

QIcon IconThemeManager::getIcon(ICONTYPE iconType)
{
    QString iconPath = m_themeIcons[m_currentTheme][iconType];
    return QIcon(iconPath);
}

void IconThemeManager::setTheme(const QString &themeName)
{
    if (m_currentTheme != themeName && m_themeIcons.contains(themeName)) {
        m_currentTheme = themeName;
        emit themeChanged();
    }
}
