#include "icarus_lib/utility/image_file.hpp"

#include <taglib/attachedpictureframe.h>
#include <taglib/tfilestream.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/audioproperties.h>
#include <taglib/tag.h>
#include <taglib/tfile.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>

using TagLib::File;
using TagLib::ByteVector;

namespace icarus_lib::utility
{

image_file::image_file(char const *file) : TagLib::File(file)
{
}

TagLib::ByteVector image_file::data()
{
    return readBlock(length());
}


}
