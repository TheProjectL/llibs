#ifndef LLIBS_FRAMEWORK_STR_H_
#define LLIBS_FRAMEWORK_STR_H_

/**************************************************************************//**
 * @file     str.h
 * @brief    Project L - Llibs, cross-platform framework / str.h
 * @version  V0.2.0
 * @date     5. Jan 2024
 * @author   Matvey Rybalkin
******************************************************************************/

/* Must be included in llibs.h */

/* Include libraries */
#include "types.h"

/** 
  * @brief UTF-8 string literal prefix
  */
#define X(str) (lc *)(str)

/** 
  * @brief UTF-8 one-byte character prefix
  */
#define Y(char) (lc)(char)

/** 
  * @brief UTF-8 4-byte character prefix
  */
#define Z(char) (lu64)(char)

/* Standard functions */

/** 
  * @brief Calculate lenght of certain string in UTF-8
  * 
  * @param[in] str string pointer
  * @return Lenght of the string
  * 
  */
extern lsz lslen(const lc* str);

/** 
  * @brief Calculate size in bytes of certain string in UTF-8
  * 
  * @param[in] str string pointer
  * @return Size in bytes
  * 
  */
extern lsz lssize(const lc* str);

/** 
  * @brief Calculate size in bytes based on number of characters
  * 
  * @param[in] str string pointer
  * @param[in] n number of symbols
  * @return Size in bytes
  * 
  */
extern lsz lssizen(const lc* str, lsz n);

/** 
  * @brief Compare two strings in UTF-8
  * 
  * @param[in] str1 string 1 pointer
  * @param[in] str2 string 2 pointer
  * @return true if the are equual, else false
  * 
  */
extern lbool lscmp(const lc* str1, const lc* str2);

/** 
  * @brief Copy one string to another
  * 
  * @param[in] dest destination string
  * @param[in] src source string
  */
extern void lscpy(lc* dest, const lc* src);

/** 
  * @brief Concatenate two strings
  * 
  * @param[in] dest destination string
  * @param[in] src source string
  */
extern void lscat(lc* dest, const lc* src);

#endif // LLIBS_FRAMEWORK_STR_H_