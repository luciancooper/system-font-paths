#include <fontconfig/fontconfig.h>
#include <string>
#include "collection.h"

StringVector *getSystemFonts() {
    StringVector *res = new StringVector();
    // initialize the fontconfig library
    FcInit();
    // create an empty fontconfig pattern
    FcPattern *pattern = FcPatternCreate();
    // specify the font properties to retrieve
    FcObjectSet *objectSet = FcObjectSetBuild(FC_FILE, nullptr);
    // retrieve the font files
    FcFontSet *fontSet = FcFontList(nullptr, pattern, objectSet);
    if (fontSet) {
        for (int i = 0; i < fontSet->nfont; i++) {
            FcChar8 *fontFile;
            if (FcPatternGetString(fontSet->fonts[i], FC_FILE, 0, &fontFile) == FcResultMatch) {
                // convert the font file path from FcChar8 to std:string and add it to result set
                res->push_back(std::string(reinterpret_cast<char *>(fontFile)));
            }
        }
        // cleanup the font set
        FcFontSetDestroy(fontSet);
    }
    // cleanup
    FcObjectSetDestroy(objectSet);
    FcPatternDestroy(pattern);
    // finalize the fontconfig library
    FcFini();
    return res;
}
