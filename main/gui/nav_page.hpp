#ifndef M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_
#define M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_

/**
 * @brief If the nav page is drawn once, this function will not draw it again.
 *        Instead, it will just show the GUI objects by removing LV_OBJ_FLAG_HIDDEN.
 */
void drawNavPage();

void hideNavPage();

#endif // M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_