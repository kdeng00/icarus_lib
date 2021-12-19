#ifndef IMAGEFILE_H_
#define IMAGEFILE_H_

#include <iostream>

#include <taglib/attachedpictureframe.h>
#include <taglib/tag.h>
#include <taglib/tfile.h>
#include <taglib/tfilestream.h>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>


namespace icarus_lib::utility {

class image_file : public TagLib::File {
public:
    image_file(const char *file) : TagLib::File(file)
    { }

    TagLib::ByteVector data()
    {
        return readBlock(length());
    }

private:
    virtual TagLib::Tag *tag() const { return 0; }
    virtual TagLib::AudioProperties *audio_properties() const { return 0; }
    virtual bool save() { return false; }
};

}

#endif