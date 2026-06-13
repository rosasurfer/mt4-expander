#include "expander.h"
#include "lib/ui/menu.h"


/**
 * Whether an activated popup menu is the "Chart->Templates" menu of MT4. Called on WM_INITMENUPOPUP.
 *
 * @param  HMENU hPopup - menu for which WM_INITMENUPOPUP was sent
 *
 * @return BOOL
 */
BOOL WINAPI IsChartTemplatesMenu(HMENU hPopup) {
   int size = GetMenuItemCount(hPopup);
   if (size < 0) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemCount(hPopup=%p)", hPopup);
   if (size < 4) return FALSE;

   // check the first 4 menu items (always fix, i18n compatible)
   for (int i=0; i < 4; i++) {
      MENUITEMINFOW mi = {};
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU;

      if (!GetMenuItemInfoW(hPopup, i, TRUE, &mi)) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemInfoW(hPopup=%p)", hPopup);

      switch (i) {
         case 0:        // MENUITEM "&Save Template...", 33220
            if (mi.wID == ID_CHART_TEMPLATES_SAVE) continue;
            break;
         case 1:        // MENUITEM "&Load Template...", 35511
            if (mi.wID == ID_CHART_TEMPLATES_LOAD) continue;
            break;
         case 2:        // POPUP "Remove Template"
            if (mi.hSubMenu) continue;
            break;
         case 3:        // MENUITEM SEPARATOR
            if (mi.fType & MFT_SEPARATOR) continue;
            break;
      }
      return FALSE;
   }

   debug("WM_INITMENUPOPUP  %p  \"Charts->Templates\" detected", hPopup);
   return TRUE;
}


/**
 *
 */
BOOL WINAPI RebuildChartTemplatesMenu(HMENU hPopup) {
   return TRUE;
}
