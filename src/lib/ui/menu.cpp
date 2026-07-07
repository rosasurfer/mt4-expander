#include "expander.h"
#include "lib/string.h"
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
      if (mi.fType == MFT_STRING && mi.wID >= ID_CHART_TEMPLATES_USER1 && mi.wID < ID_CHART_TEMPLATES_USER1 + 100) {
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

   struct local {
      static wstring MenuItemToStr(MENUITEMINFOW mi) {
         wstring sType = L"";
         if      (mi.fType & MFT_BITMAP)    sType.append(L"BITMAP");
         else if (mi.fType & MFT_SEPARATOR) sType.append(L"SEPARATOR");
         else              /*MFT_STRING*/   sType.append(L"\"").append(mi.dwTypeData).append(L"\"");

         wstring sProps = L" (";

         if (mi.wID) {
            wchar* s = asformat(L"id=%u, ", mi.wID);
            sProps.append(s);
            free(s);
         }
         if (mi.dwItemData) {
            wchar* s = asformat(L"itemData=%p, ", mi.dwItemData);
            sProps.append(s);
            free(s);
         }
         if (mi.hSubMenu) {
            wchar* s = asformat(L"hSubMenu=%p, ", mi.hSubMenu);
            sProps.append(s);
            free(s);
         }
         
         wstring flags = L"";
         uint fType = mi.fType & ~(MFT_BITMAP | MFT_SEPARATOR);
         if (fType & MFT_RADIOCHECK)   { flags.append(L"|MFT_RADIOCHECK");   fType &= ~MFT_RADIOCHECK;   }
         if (fType & MFT_MENUBREAK)    { flags.append(L"|MFT_MENUBREAK");    fType &= ~MFT_MENUBREAK;    }
         if (fType & MFT_MENUBARBREAK) { flags.append(L"|MFT_MENUBARBREAK"); fType &= ~MFT_MENUBARBREAK; }
         if (fType & MFT_RIGHTORDER)   { flags.append(L"|MFT_RIGHTORDER");   fType &= ~MFT_RIGHTORDER;   }
         if (fType & MFT_RIGHTJUSTIFY) { flags.append(L"|MFT_RIGHTJUSTIFY"); fType &= ~MFT_RIGHTJUSTIFY; }
         if (fType & MFT_OWNERDRAW)    { flags.append(L"|MFT_OWNERDRAW");    fType &= ~MFT_OWNERDRAW;    }
         if (fType)                    { flags.append(L"|").append(to_wstring(fType));                   }
         if (flags.length()) {
            sProps.append(flags, 1, flags.length()-1).append(L", ");
         }
         
         if (sProps.length() > 2) {
            sProps.erase(sProps.length() - 2).append(L")");
         }
         else {
            sProps = L"";
         }
         return sType.append(sProps);
      }
   };

   debug("HMENU=%p", hMenu);
   
   for (int i=0; i < size; i++) {
      wchar text[256] = {};
      
      MENUITEMINFOW mi = {};
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_STRING | MIIM_DATA;
      mi.dwTypeData = text;
      mi.cch = countof(text);
      if (!GetMenuItemInfoW(hMenu, i, TRUE, &mi)) return !error(ERR_WIN32_ERROR + GetLastError(), "GetMenuItemInfoW(%p, i=%d)", hMenu, i);
      
      debug_raw("  %S", local::MenuItemToStr(mi).c_str());
   }
   return TRUE;
}
