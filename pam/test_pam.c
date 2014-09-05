/*
  This program was contributed by Shane Watts
  [modifications by AGM]
  [more modifications by Kragen Sitaker]

  You need to add the following (or equivalent) to the /etc/pam.conf file.
  # check authorization
  check_user   auth       required     /usr/lib/security/pam_path2.so
  check_user   account    required     /usr/lib/security/pam_path2.so
 */
#include "config.h"

#ifdef HAVE_SECURITY_PAM_APPL_H
#include <security/pam_appl.h>
#endif

#ifdef HAVE_SECURITY_PAM_MISC_H
#include <security/pam_misc.h>
#endif

#ifdef HAVE_SECURITY_OPENPAM_H
#include <security/openpam.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#if defined (DARWIN_106) || (DARWIN_107) || (DARWIN_108) || (DARWIN_109)
static struct pam_conv conv = {
    openpam_ttyconv,
    NULL
};
#else
static struct pam_conv conv = {
    misc_conv,
    NULL
};
#endif

int main(int argc, char *argv[])
{
    pam_handle_t *pamh=NULL;
    int retval;
    const char *user="nobody";

    if(argc == 2) {
        user = argv[1];
    }

    if(argc > 2) {
        fprintf(stderr, "Usage: check_user [accountId]\n");
        exit(1);
    }

    retval = pam_start("check_user", user, &conv, &pamh);
        
    if (retval == PAM_SUCCESS)
        retval = pam_authenticate(pamh, 0);    /* is user really user? */

    if (retval == PAM_SUCCESS)
        retval = pam_acct_mgmt(pamh, 0);       /* permitted access? */

    /* This is where we have been authorized or not. */

    if (retval == PAM_SUCCESS) {
        fprintf(stdout, "Authenticated\n");
    } else {
        fprintf(stdout, "Not Authenticated: %s\n", pam_strerror(pamh, retval));
    }

    if (pam_end(pamh,retval) != PAM_SUCCESS) {     /* close Linux-PAM */
        pamh = NULL;
        fprintf(stderr, "check_user: failed to release authenticator\n");
        exit(1);
    }

    return ( retval == PAM_SUCCESS ? 0:1 );       /* indicate success */
}
