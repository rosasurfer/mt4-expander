#include "expander.h"
#include "lib/ui/menu.h"


/**
 * Whether an activated menu is the "Chart->Templates" menu of MT4. Called on WM_INITMENUPOPUP.
 *
 * @param  HMENU hMenu - menu for which WM_INITMENUPOPUP was sent
 *
 * @return BOOL
 */
BOOL WINAPI IsChartTemplatesMenu(HMENU hMenu) {
   int size = GetMenuItemCount(hMenu);
   if (size < 0) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemCount(hMenu=%p)", hMenu);
   if (size < 4) return FALSE;

   // check the first 4 menu items (always fix, compatible with i18n)
   for (int i=0; i < 4; i++) {
      MENUITEMINFOW mi = {};
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU;

      if (!GetMenuItemInfoW(hMenu, i, TRUE, &mi)) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemInfoW(%p, %d)", hMenu, i);

      switch (i) {
         case 0:
            if (mi.wID == ID_CHART_TEMPLATES_SAVE) continue;   // MENUITEM "&Save Template...", 33220
            break;
         case 1:
            if (mi.wID == ID_CHART_TEMPLATES_LOAD) continue;   // MENUITEM "&Load Template...", 35511
            break;
         case 2:
            if (mi.hSubMenu) continue;                         // POPUP "Remove Template", has submenu
            break;
         case 3:
            if (mi.fType & MFT_SEPARATOR) continue;            // MENUITEM SEPARATOR
            break;
      }
      return FALSE;
   }
   return TRUE;
}


/**
 * Modify the "Chart->Templates" menu of MT4. Called on WM_INITMENUPOPUP.
 *
 * @param  HMENU hMenu - "Chart->Templates" menu
 *
 * @return BOOL - success status
 */
BOOL WINAPI RebuildChartTemplatesMenu(HMENU hMenu) {
   int size = GetMenuItemCount(hMenu);
   if (size < 0) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemCount(hMenu=%p)", hMenu);

   for (int i=size-1; i >= 0; i--) {
      wchar text[MAX_FNAME] = {};
      MENUITEMINFOW mi = {};
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
      mi.dwTypeData = text;
      mi.cch = countof(text);

      if (!GetMenuItemInfoW(hMenu, i, TRUE, &mi)) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemInfoW(%p, %d)", hMenu, i);

      // if an actual chart template item
      if (mi.fType == MFT_STRING && mi.wID >= ID_CHART_TEMPLATES_USER1) {
         wstring label(text);
         BOOL modified = ModifyChartTemplateLabel(label);

         if (IsChartTemplateSeparatorName(label)) {            // replace by a real separator
            DeleteMenu(hMenu, i, MF_BYPOSITION);
            InsertMenuW(hMenu, i, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
         }
         else if (modified) {                                  // or update the item's text
            mi.fMask = MIIM_STRING;
            mi.dwTypeData = const_cast<wchar*>(label.c_str());
            SetMenuItemInfoW(hMenu, i, TRUE, &mi);
         }
      }
   }
   return TRUE;
}


/**
 * Modify the label of a chart template item. Removes a prefix of "two digits followed by at least two spaces" (if found).
 * The function modifies the string.
 *
 * @param  _InOut_ wstring &label - menu item text
 *
 * @return BOOL - whether a matching prefix was found and removed
 */
BOOL WINAPI ModifyChartTemplateLabel(wstring &label) {
   size_t i = 0;
   while (i < label.length() && iswdigit(label[i])) i++;       // strip numeric prefix: "01 Default" -> " Default"

   if (i == 2) {
      while (i < label.length() && iswspace(label[i])) i++;    // strip following spaces: " Default" -> "Default"
   }
   if (i >= 4 && i < label.length()) {
      label.erase(0, i);                                       // ensure a non-empty label
      return TRUE;
   }
   return FALSE;
}


/**
 * Whether the label of a chart template item is a pseudonym for a menu separator.
 *
 * @param  wstring &label - menu item text
 *
 * @return BOOL
 */
BOOL WINAPI IsChartTemplateSeparatorName(const wstring &label) {
   for (size_t i=0; i < label.length(); i++) {
      if (label[i] != L'-' && !iswspace(label[i])) {
         return FALSE;
      }
   }
   return TRUE;
}


/**
 * Log a text representation of a menu to the system debugger.
 *
 * @param  HMENU hMenu
 *
 * @return BOOL - success status
 */
BOOL WINAPI DumpMenu(HMENU hMenu) {
   int size = GetMenuItemCount(hMenu);
   if (size < 0) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemCount(hMenu=%p)", hMenu);

   for (int i=0; i < size; i++) {
      wchar text[256] = {};

      MENUITEMINFOW mi = {};
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_STRING | MIIM_DATA;
      mi.dwTypeData = text;
      mi.cch = countof(text);
      if (!GetMenuItemInfoW(hMenu, i, TRUE, &mi)) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemInfoW(%p, i=%d)", hMenu, i);

      debug("%p  %d  \"%S\" (type=%d, item=%p, id=%d, hSubMenu=%p)", hMenu, i, text, mi.fType, mi.dwItemData, mi.wID, mi.hSubMenu);
   }
   return TRUE;
}
