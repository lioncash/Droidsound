/*
 *                 sc68 - IO plugin initialization
 *             Copyright (C) 2001-2009 Benjamin Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* $Id: io68.c 126 2009-07-15 08:58:51Z benjihan $ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "io68.h"
#include "sc68/msg68.h"

static const struct {
  const char * name;
  int  (* init)    (int *, char **);
  void (* shutdown)(void);
} func[] = {
  { "paula",     paulaio_init,   paulaio_shutdown   },
  { "ym-2149",   ymio_init,      ymio_shutdown      },
  { "microwire", mwio_init,      mwio_shutdown      },
  { "mfp-68901", mfpio_init,     mfpio_shutdown     },
  { "shifter",   shifterio_init, shifterio_shutdown },
};

const int max = sizeof(func) / sizeof(*func);

int io68_init(int * argc, char ** argv)
{
  int i,err;
  for ( err = i = 0; i < max; ++i ) {
    if (func[i].init) {
      err = func[i].init(argc, argv);
      if (err) {
        msg68_error("io68: failed to initialize *%s* IO plugin\n",func[i].name);
        break;
      }
    }
  }
  return err;
}

void io68_shutdown(void)
{
  int i;
  for ( i=0; i < max; ++i ) {
    if (func[i].shutdown)
      func[i].shutdown();
  }
}

void io68_destroy(io68_t * const io)
{
  if (io && io->destroy) {
    io->destroy(io);
  }
}
