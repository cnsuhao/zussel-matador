/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MYSQL_SEQUENCER_HPP
#define MYSQL_SEQUENCER_HPP

#include "database/database_sequencer.hpp"
#include "database/mysql/mysql_statement.hpp"

namespace oos {

namespace mysql {

class mysql_database;

class mysql_sequencer : public database_sequencer
{
public:
  mysql_sequencer(mysql_database *db);
  virtual ~mysql_sequencer();

  virtual void create();
  virtual void begin();
  virtual void commit();
  virtual void rollback();
  virtual void drop();
  virtual void destroy();

private:
  mysql_database *db_;
  long backup_;
  mysql_statement update_;
};

}

}

#endif /* MYSQL_SEQUENCER_HPP */