/**
 * MT4 struct SYMBOL_GROUP (Dateiformat "symgroups.raw")
 *
 * Die Größe der Datei ist fix und enthält Platz für exakt 32 Gruppen. Einzelne Gruppen können undefiniert sein.
 */
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
 * Gibt die Hintergrundfarbe einer SYMBOL_GROUP im "Market Watch"-Window zurück.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return uint - Farbe oder White, wenn für die Gruppe keine Hintergrundfarbe gesetzt ist
 *                (CLR_NONE wird vom Terminal als Black interpretiert)
 */
uint WINAPI sg_BackgroundColor(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg));
   if (sg->backgroundColor)
      return(sg->backgroundColor);
   return(White);
   #pragma EXPORT
}


/**
 * Gibt den Namen einer SYMBOL_GROUP innerhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sgs[] - Array
 * @param  int          index - Array-Index
 *
 * @return char* - Gruppenname
 */
const char* WINAPI sgs_Name(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sgs[index].name);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer SYMBOL_GROUP innerhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sgs[] - Array
 * @param  int          index - Array-Index
 *
 * @return char* - Gruppenbeschreibung
 */
const char* WINAPI sgs_Description(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(sgs[index].description);
   #pragma EXPORT
}


/**
 * Gibt die Hintergrundfarbe einer SYMBOL_GROUP innerhalb eines Arrays zurück.
 *
 * @param  SYMBOL_GROUP sgs[] - Array
 * @param  int          index - Array-Index
 *
 * @return uint - Farbe oder White, wenn für die Gruppe keine Hintergrundfarbe gesetzt ist
 *                (CLR_NONE wird vom Terminal als Black interpretiert)
 */
uint WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   if (sgs[index].backgroundColor)
      return(sgs[index].backgroundColor);
   return(White);
   #pragma EXPORT
}


/**
 * Setzt den Namen einer SYMBOL_GROUP.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         name
 *
 * @return const char* - der gesetzte Name
 */
const char* WINAPI sg_SetName(SYMBOL_GROUP* sg, const char* name) {
   if ((uint)sg   < MIN_VALID_POINTER)   return((char*)_NULL(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg)));
   if ((uint)name < MIN_VALID_POINTER)   return((char*)_NULL(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name)));
   int len = strlen(name);
   if (!len || len > sizeof(sg->name)-1) return((char*)_NULL(debug("ERROR:  illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(sg->name)-1)));
   return(strcpy(sg->name, name));
   #pragma EXPORT
}


/**
 * Setzt die Bechreibung einer SYMBOL_GROUP.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         description - eine vorhandene Beschreibung kann mit einem Leerstring gelöscht werden
 *
 * @return char* - die gesetzte Beschreibung
 */
const char* WINAPI sg_SetDescription(SYMBOL_GROUP* sg, const char* description) {
   if ((uint)sg          < MIN_VALID_POINTER)           return((char*)_NULL(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg)));
   if ((uint)description < MIN_VALID_POINTER)           return((char*)_NULL(debug("ERROR:  invalid parameter description = 0x%p (not a valid pointer)", description)));
   if (strlen(description) > sizeof(sg->description)-1) return((char*)_NULL(debug("ERROR:  illegal length of parameter description = \"%s\" (max %d characters)", description, sizeof(sg->description)-1)));
   return(strcpy(sg->description, description));
   #pragma EXPORT
}


/**
 * Setzt die Hintergrundfarbe einer SYMBOL_GROUP im "Market Watch"-Window.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  int           color - Farbe
 *
 * @return int - dieselbe Farbe oder CLR_NONE, falls ein Fehler auftrat
 */
int WINAPI sg_SetBackgroundColor(SYMBOL_GROUP* sg, uint color) {
   if ((uint)sg < MIN_VALID_POINTER) return(_CLR_NONE(debug("ERROR:  invalid parameter sg = 0x%p (not a valid pointer)", sg)));
   if (color & 0xFF000000) {
      if (color != CLR_NONE)         return(_CLR_NONE(debug("ERROR:  invalid parameter color = %p (not a valid color)", color)));
      color = White;                // CLR_NONE wird vom Terminal als Black interpretiert
   }
   return(sg->backgroundColor = color);
   #pragma EXPORT
}


/**
 * Setzt den Namen einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sgs[] - Array
 * @param  int          index - Array-Index
 * @param  char*        name  - Name
 *
 * @return char* - der gesetzte Name
 */
const char* WINAPI sgs_SetName(SYMBOL_GROUP sgs[], int index, const char* name) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)_NULL(debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs)));
   if (index     < 0)                 return((char*)_NULL(debug("ERROR:  invalid parameter index = %d (not a valid index)", index)));
   return(sg_SetName(&sgs[index], name));
   #pragma EXPORT
}


/**
 * Setzt die Beschreibung einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sgs[]       - Array
 * @param  int          index       - Array-Index
 * @param  char*        description - Beschreibung
 *
 * @return char* - die gesetzte Beschreibung
 */
const char* WINAPI sgs_SetDescription(SYMBOL_GROUP sgs[], int index, const char* description) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)_NULL(debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs)));
   if (index     < 0)                 return((char*)_NULL(debug("ERROR:  invalid parameter index = %d (not a valid index)", index)));
   return(sg_SetDescription(&sgs[index], description));
   #pragma EXPORT
}


/**
 * Setzt die Hintergrundfarbe einer SYMBOL_GROUP innerhalb eines Arrays.
 *
 * @param  SYMBOL_GROUP sgs[] - Array
 * @param  int          index - Array-Index
 * @param  int          color - Farbe
 *
 * @return int - dieselbe Farbe oder CLR_NONE, falls ein Fehler auftrat
 */
int WINAPI sgs_SetBackgroundColor(SYMBOL_GROUP sgs[], int index, uint color) {
   if ((uint)sgs < MIN_VALID_POINTER) return(_CLR_NONE(debug("ERROR:  invalid parameter sgs = 0x%p (not a valid pointer)", sgs)));
   if (index     < 0)                 return(_CLR_NONE(debug("ERROR:  invalid parameter index = %d (not a valid index)", index)));
   return(sg_SetBackgroundColor(&sgs[index], color));
   #pragma EXPORT
}
