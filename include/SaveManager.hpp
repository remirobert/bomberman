#ifndef _SAVEMANAGER_H_
# define _SAVEMANAGER_H_

# include <sys/stat.h>
# include <sys/types.h>
# include <fstream>
# include <cerrno>

# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>

# include "Game.hpp"

class SaveManager
{
public:
  static void save(Game *game)
  {
    std::cout << "Saving..." << std::endl;
    if (mkdir("./save", 0755) < 0 && errno != EEXIST)
      return ;
    std::ofstream ofs(std::string("./save/save_" + getDate()).c_str());

    if (ofs.is_open())
      {
	boost::archive::text_oarchive oa(ofs);
	oa << *game;
      }
  }

  static void load(const std::string& filename, Game *game)
  {
    std::ifstream ifs(std::string("./save/" + filename).c_str());

    if (ifs.is_open())
      {
	boost::archive::text_iarchive ia(ifs);
	ia >> *game;
      }
  }
};

#endif /* _SAVEMANAGER_H_ */
