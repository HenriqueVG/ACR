#ifndef _SONGPRINTFILE_H
#define _SONGPRINTFILE_H

#include <songprint.h>

sp_rv 
sp_identify_file(const gchar * filename, sp_id_recv_t ***matches,
                 gint *num_matches, gchar ** url);

#endif /*_SONGPRINTFILE_H*/
