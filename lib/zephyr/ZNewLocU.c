/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZNewLocateUser function.
 *
 *	Created by:	Robert French
 *
 *	$Source$
 *	$Author$
 *
 *	Copyright (c) 1987,1988,1991 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
/* $Header$ */

#include <internal.h>

#ifndef lint
static const char rcsid_ZNewLocateUser_c[] =
    "$Id$";
#endif

Code_t ZLocateUser(realm, user, nlocs, auth)
    char *realm;
    char *user;
    int *nlocs;
    Z_AuthProc auth;
{
    Code_t retval;
    ZNotice_t notice;
    ZAsyncLocateData_t zald;

    (void) ZFlushLocations();	/* ZFlushLocations never fails (the library
				   is allowed to know this). */

    if ((retval = ZRequestLocations(realm, user, &zald, UNACKED,
				    auth)) != ZERR_NONE)
	return(retval);

    retval = Z_WaitForNotice (&notice, ZCompareALDPred, &zald, SRV_TIMEOUT);
    if (retval == ZERR_NONOTICE)
	return ETIMEDOUT;
    if (retval != ZERR_NONE)
	return retval;

    if ((retval = ZParseLocations(&notice, &zald, nlocs, NULL)) != ZERR_NONE) {
	ZFreeNotice(&notice);
	return(retval);
    }

    ZFreeNotice(&notice);
    ZFreeALD(&zald);
    return(ZERR_NONE);
}
