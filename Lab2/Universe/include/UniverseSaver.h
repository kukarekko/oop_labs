#ifndef UNIVERSESAVER_H
#define UNIVERSESAVER_H

#include "Universe.h"
#include "../Utils/FileWriter.h"


class UniverseSaver {
public:
    static bool saveToFile(const Universe& universe,
                          const std::string& fileName);

private:
    static void writeUniverse(const Universe& universe,
                            FileWriter& writer);
};

#endif //UNIVERSESAVER_H
