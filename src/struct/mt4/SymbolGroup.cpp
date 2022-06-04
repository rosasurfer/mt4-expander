/**
 * MT4 struct SYMBOL_GROUP (file format of "symgroups.raw")
 *
 * The file size is fix, a file always contains 32 groups. Unused group entries are empty (zeroed).
 */
#include "expander.h"
#include "struct/mt4/SymbolGroup.h"


/**
 * Return a SYMBOL_GROUP's name.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return char* - name
 */
const char* WINAPI sg_Name(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg));
   return(sg->name);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a SYMBOL_GROUP's description.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return char* - description
 */
const char* WINAPI sg_Description(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg));
   return(sg->description);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a SYMBOL_GROUP's color in the "Market Watch" window. Custom field, originally the color is stored only in the SYMBOL itself.
 *
 * @param  SYMBOL_GROUP* sg
 *
 * @return uint - color value or White if no color is set for the group/symbol (the terminal interpretes CLR_NONE as Black)
 */
uint WINAPI sg_BackgroundColor(const SYMBOL_GROUP* sg) {
   if ((uint)sg < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg)));
   if (sg->backgroundColor)
      return(sg->backgroundColor);
   return(White);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a SYMBOL_GROUP's name.
 *
 * @param  SYMBOL_GROUP sgs[] - array
 * @param  int          index - index
 *
 * @return char* - name
 */
const char* WINAPI sgs_Name(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(sgs[index].name);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a SYMBOL_GROUP's description.
 *
 * @param  SYMBOL_GROUP sgs[] - array
 * @param  int          index - index
 *
 * @return char* - description
 */
const char* WINAPI sgs_Description(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(sgs[index].description);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a SYMBOL_GROUP's color in the "Market Watch" window. Custom field, originally the color is stored only in the SYMBOL itself.
 *
 * @param  SYMBOL_GROUP sgs[] - array
 * @param  int          index - index
 *
 * @return uint - color value or White if no color is set for the group/symbol (the terminal interpretes CLR_NONE as Black)
 */
uint WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sgs[], int index) {
   if ((uint)sgs < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs)));
   if (index     < 0)                 return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index)));
   if (sgs[index].backgroundColor)
      return(sgs[index].backgroundColor);
   return(White);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a SYMBOL_GROUP's name.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         name
 *
 * @return const char* - the same name
 */
const char* WINAPI sg_SetName(SYMBOL_GROUP* sg, const char* name) {
   if ((uint)sg   < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg));
   if ((uint)name < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(sg->name)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(sg->name)-1));

   if (!strcpy(sg->name, name))
      return(NULL);
   return(name);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a SYMBOL_GROUP's description.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  char*         description - an empty string deletes the description
 *
 * @return char* - the same description
 */
const char* WINAPI sg_SetDescription(SYMBOL_GROUP* sg, const char* description) {
   if ((uint)sg          < MIN_VALID_POINTER)           return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg));
   if ((uint)description < MIN_VALID_POINTER)           return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter description: 0x%p (not a valid pointer)", description));
   if (strlen(description) > sizeof(sg->description)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter description: \"%s\" (max %d characters)", description, sizeof(sg->description)-1));

   if (!strcpy(sg->description, description))
      return(NULL);
   return(description);
   #pragma EXPANDER_EXPORT
}


/**
 * Set SYMBOL_GROUP's color in the "Market Watch" window.
 *
 * @param  SYMBOL_GROUP* sg
 * @param  uint          color
 *
 * @return uint - the same color or CLR_NONE in case of errors
 */
uint WINAPI sg_SetBackgroundColor(SYMBOL_GROUP* sg, uint color) {
   if ((uint)sg < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter sg: 0x%p (not a valid pointer)", sg)));
   if (color & 0xFF000000) {
      if (color != CLR_NONE)         return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter color: %p (not a valid color)", color)));
      color = White;                 // the terminal interpretes CLR_NONE as Black
   }
   return(sg->backgroundColor = color);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a SYMBOL_GROUP's name
 *
 * @param  SYMBOL_GROUP sgs[] - array
 * @param  int          index - index
 * @param  char*        name  - name
 *
 * @return char* - the same name
 */
const char* WINAPI sgs_SetName(SYMBOL_GROUP sgs[], int index, const char* name) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(sg_SetName(&sgs[index], name));
   #pragma EXPANDER_EXPORT
}


/**
 * Set a SYMBOL_GROUP's description
 *
 * @param  SYMBOL_GROUP sgs[]       - array
 * @param  int          index       - index
 * @param  char*        description - description
 *
 * @return char* - the same description
 */
const char* WINAPI sgs_SetDescription(SYMBOL_GROUP sgs[], int index, const char* description) {
   if ((uint)sgs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(sg_SetDescription(&sgs[index], description));
   #pragma EXPANDER_EXPORT
}


/**
 * Set SYMBOL_GROUP's color in the "Market Watch" window.
 *
 * @param  SYMBOL_GROUP sgs[] - array
 * @param  int          index - index
 * @param  uint         color - color
 *
 * @return uint - the same color or CLR_NONE in case of errors
 */
uint WINAPI sgs_SetBackgroundColor(SYMBOL_GROUP sgs[], int index, uint color) {
   if ((uint)sgs < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter sgs: 0x%p (not a valid pointer)", sgs)));
   if (index     < 0)                 return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index)));
   return(sg_SetBackgroundColor(&sgs[index], color));
   #pragma EXPANDER_EXPORT
}
