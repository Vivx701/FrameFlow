#include "iconthememanager.h"

IconThemeManager::IconThemeManager(QObject *parent) : QObject(parent)
{
    // Initialize with default themes and icons
    m_themeIcons[LIGHTTHEME][ICONTYPE::NEW_BUTTON]       = ":/Dark/icons/Resources/Icons/dark/new-box.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::BROWSE_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/plus-box.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::CBOARD_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/paste.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::EXPORT_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/export.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::SETTINGS_BUTTON]  = ":/Dark/icons/Resources/Icons/dark/cog.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::ABOUT_BUTTON]     = ":/Dark/icons/Resources/Icons/dark/information-outline.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::LEFT_BUTTON]      = ":/Dark/icons/Resources/Icons/dark/arrow-left-bold-box-outline.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::RIGHT_BUTTON]     = ":/Dark/icons/Resources/Icons/dark/arrow-right-bold-box-outline.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::MOVEBACK_BUTTON]  = ":/Dark/icons/Resources/Icons/dark/image-move-back.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::MOVEFRONT_BUTTON] = ":/Dark/icons/Resources/Icons/dark/image-move.png";
    m_themeIcons[LIGHTTHEME][ICONTYPE::DELETE_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/image-remove-outline.png";


    m_themeIcons[DARKTHEME][ICONTYPE::NEW_BUTTON]       = ":/Dark/icons/Resources/Icons/dark/new-box.png";
    m_themeIcons[DARKTHEME][ICONTYPE::BROWSE_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/plus-box.png";
    m_themeIcons[DARKTHEME][ICONTYPE::CBOARD_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/paste.png";
    m_themeIcons[DARKTHEME][ICONTYPE::EXPORT_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/export.png";
    m_themeIcons[DARKTHEME][ICONTYPE::SETTINGS_BUTTON]  = ":/Dark/icons/Resources/Icons/dark/cog.png";
    m_themeIcons[DARKTHEME][ICONTYPE::ABOUT_BUTTON]     = ":/Dark/icons/Resources/Icons/dark/information-outline.png";
    m_themeIcons[DARKTHEME][ICONTYPE::LEFT_BUTTON]      = ":/Dark/icons/Resources/Icons/dark/arrow-left-bold-box-outline.png";
    m_themeIcons[DARKTHEME][ICONTYPE::RIGHT_BUTTON]     = ":/Dark/icons/Resources/Icons/dark/arrow-right-bold-box-outline.png";
    m_themeIcons[DARKTHEME][ICONTYPE::MOVEBACK_BUTTON]  = ":/Dark/icons/Resources/Icons/dark/image-move-back.png";
    m_themeIcons[DARKTHEME][ICONTYPE::MOVEFRONT_BUTTON] = ":/Dark/icons/Resources/Icons/dark/image-move.png";
    m_themeIcons[DARKTHEME][ICONTYPE::DELETE_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/image-remove-outline.png";



    m_themeIcons[DEFAULTTHEME][ICONTYPE::NEW_BUTTON]      = ":/Dark/icons/Resources/Icons/dark/new-box.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::BROWSE_BUTTON]   = ":/Dark/icons/Resources/Icons/dark/plus-box.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::CBOARD_BUTTON]   = ":/Dark/icons/Resources/Icons/dark/paste.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::EXPORT_BUTTON]   = ":/Dark/icons/Resources/Icons/dark/export.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::SETTINGS_BUTTON] = ":/Dark/icons/Resources/Icons/dark/cog.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::ABOUT_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/information-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::LEFT_BUTTON]     = ":/Dark/icons/Resources/Icons/dark/arrow-left-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::RIGHT_BUTTON]    = ":/Dark/icons/Resources/Icons/dark/arrow-right-bold-box-outline.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::MOVEBACK_BUTTON] = ":/Dark/icons/Resources/Icons/dark/image-move-back.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::MOVEFRONT_BUTTON]= ":/Dark/icons/Resources/Icons/dark/image-move.png";
    m_themeIcons[DEFAULTTHEME][ICONTYPE::DELETE_BUTTON]   = ":/Dark/icons/Resources/Icons/dark/image-remove-outline.png";

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
