/**
 * MQL-Interface zum Zugriff auf ein struct SYMBOL_GROUP (Dateiformat "symgroups.raw").
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt den Namen einer SYMBOL_GROUP zurück.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return char* - Gruppenname
 */
const char* WINAPI sg_Name(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   return(sg->name);
   #pragma EXPORT
}


/**
 * Gibt den Namen einer SYMBOL_GROUP inherhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sg[]  - Array
 * @param  uint         index - Array-Index
 *
 * @return char* - Gruppenname
 */
const char* WINAPI sgs_Name(const SYMBOL_GROUP sg[], uint index) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index     < 0)                return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sg[index].name);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer SYMBOL_GROUP zurück.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return char* - Gruppenbeschreibung
 */
const char* WINAPI sg_Description(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   return(sg->description);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer SYMBOL_GROUP innerhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sg[]  - Array
 * @param  uint         index - Array-Index
 *
 * @return char* - Gruppenbeschreibung
 */
const char* WINAPI sgs_Description(const SYMBOL_GROUP sg[], uint index) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index    < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sg[index].description);
   #pragma EXPORT
}


/**
 * Gibt die Hintergrundfarbe einer SYMBOL_GROUP im "Market Watch"-Window zurück.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return uint - Farbe oder CLR_NONE, wenn für die Gruppe keine Hintergrundfarbe gesetzt ist
 */
uint WINAPI sg_BackgroundColor(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (sg->backgroundColor)
      return(sg->backgroundColor);
   return(CLR_NONE);
   #pragma EXPORT
}


/**
 * Gibt die Hintergrundfarbe einer SYMBOL_GROUP innerhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sg[]  - Array
 * @param  uint         index - Array-Index
 *
 * @return uint - Farbe oder CLR_NONE, wenn für die Gruppe keine Hintergrundfarbe gesetzt ist
 */
uint WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sg[], uint index) {
   if ((uint)sg < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index    < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   if (sg[index].backgroundColor)
      return(sg[index].backgroundColor);
   return(CLR_NONE);
   #pragma EXPORT
}


/**
 * Setzt den Namen einer SYMBOL_GROUP.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         name
 *
 * @return char* - derselbe Name oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI sg_setName(SYMBOL_GROUP* sg, const char* name) {
   if ((uint)sg   < MIN_VALID_POINTER)    return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if ((uint)name < MIN_VALID_POINTER)    return((char*)debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if (strlen(name) > sizeof(sg->name)-1) return((char*)debug("ERROR:  invalid parameter name = \"%s\" (max %d characters)", name, sizeof(sg->name)-1));
   return(strcpy(sg->name, name));
   #pragma EXPORT
}


/**
 * Setzt den Namen einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sg[]  - Array
 * @param  uint         index - Array-Index
 * @param  char*        name  - Name
 *
 * @return char* - derselbe Name oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI sgs_setName(SYMBOL_GROUP sg[], uint index, const char* name) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index    < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sg_setName(&sg[index], name));
   #pragma EXPORT
}


/**
 * Setzt die Bechreibung einer SYMBOL_GROUP.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         description
 *
 * @return char* - dieselbe Beschreibung oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI sg_setDescription(SYMBOL_GROUP* sg, const char* description) {
   if ((uint)sg          < MIN_VALID_POINTER)           return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if ((uint)description < MIN_VALID_POINTER)           return((char*)debug("ERROR:  invalid parameter description = 0x%p (not a valid pointer)", description));
   if (strlen(description) > sizeof(sg->description)-1) return((char*)debug("ERROR:  invalid parameter description = \"%s\" (max %d characters)", description, sizeof(sg->description)-1));
   return(strcpy(sg->description, description));
   #pragma EXPORT
}


/**
 * Setzt die Beschreibung einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sg[]        - Array
 * @param  uint         index       - Array-Index
 * @param  char*        description - Beschreibung
 *
 * @return char* - dieselbe Beschreibung oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI sgs_setDescription(SYMBOL_GROUP sg[], uint index, const char* description) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index    < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sg_setDescription(&sg[index], description));
   #pragma EXPORT
}


/**
 * Setzt die Hintergrundfarbe einer SYMBOL_GROUP im "Market Watch"-Window.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  uint          color - White=0x00FFFFFF; CLR_NONE=0xFFFFFFFF
 *
 * @return uint - dieselbe Farbe oder EMPTY_COLOR = (uint)-2, falls ein Fehler auftrat
 */
uint WINAPI sg_setBackgroundColor(SYMBOL_GROUP* sg, uint color) {
   if ((uint)sg < MIN_VALID_POINTER) {
      debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg);
      return(EMPTY_COLOR);
   }
   if (color!=0xFFFFFFFF && color & 0xFF000000) {  // das höchste Byte muß auf 0 stehen
      debug("ERROR:  invalid parameter color = 0x%p (not a valid color)", color);
      return(EMPTY_COLOR);
   }
   sg->backgroundColor = color;
   return(color);
   #pragma EXPORT
}


/**
 * Setzt die Hintergrundfarbe einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sg[]  - Array
 * @param  uint         index - Array-Index
 * @param  uint         color - White=0x00FFFFFF; CLR_NONE=0xFFFFFFFF
 *
 * @return int - dieselbe Farbe
 */
uint WINAPI sgs_setBackgroundColor(SYMBOL_GROUP sg[], uint index, uint color) {
   if ((uint)sg < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (index    < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sg_setBackgroundColor(&sg[index], color));
   #pragma EXPORT
}