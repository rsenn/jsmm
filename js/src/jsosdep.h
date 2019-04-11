/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef jsosdep_h___
#define jsosdep_h___
/*
 * OS (and machine, and compiler XXX) dependent information.
 */

#if defined(XP_WIN) || defined(XP_OS2)

#if defined(_WIN32) || defined (XP_OS2)
#define JS_HAVE_LONG_LONG 1
#else
#define JS_HAVE_LONG_LONG 0
#endif
#endif /* XP_WIN || XP_OS2 */

#ifdef XP_BEOS
#define JS_HAVE_LONG_LONG 1
#endif


#ifdef XP_UNIX

/*
 * Get OS specific header information.
 */
#if defined(AIXV3) || defined(AIX)
#define JS_HAVE_LONG_LONG 1

#elif defined(BSDI)
#define JS_HAVE_LONG_LONG 1

#elif defined(HPUX)
#define JS_HAVE_LONG_LONG 1

#elif defined(IRIX)
#define JS_HAVE_LONG_LONG 1

#elif defined(linux)
#define JS_HAVE_LONG_LONG 1

#elif defined(OSF1)
#define JS_HAVE_LONG_LONG 1

#elif defined(_SCO_DS)
#define JS_HAVE_LONG_LONG 0

#elif defined(SOLARIS)
#define JS_HAVE_LONG_LONG 1

#elif defined(FREEBSD)
#define JS_HAVE_LONG_LONG 1

#elif defined(SUNOS4)
#define JS_HAVE_LONG_LONG 0

/*
** Missing function prototypes
*/

extern void *sbrk(int);

#elif defined(UNIXWARE)
#define JS_HAVE_LONG_LONG 0

#elif defined(VMS) && defined(__ALPHA)
#define JS_HAVE_LONG_LONG 1

#endif

#endif /* XP_UNIX */

#endif /* jsosdep_h___ */

