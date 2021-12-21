#ifndef ICARUS_IMAGE_FILE_H_
#define ICARUS_IMAGE_FILE_H_

#include <iostream>

#include <taglib/audioproperties.h>
#include <taglib/tag.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>
#include <taglib/tfile.h>



namespace icarus_lib::utility {

class image_file : public TagLib::File {
public:
    image_file(char const *file);
    ~image_file() = default;

    TagLib::ByteVector data();

protected:
    virtual TagLib::Tag *tag() const { return 0; }
    virtual TagLib::AudioProperties *audioProperties() const { return 0; }
    virtual bool save() { return false; }
};


}

#endif