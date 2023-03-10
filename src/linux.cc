#include <fontconfig/fontconfig.h>

#include <string>

#include "collection.h"

StringVector *getSystemFonts() {
    StringVector *res = new StringVector();
    FcConfig *config = FcInitLoadConfigAndFonts();
    if (!config) {
        return res;
    }
    FcFontSet *fontSet = FcConfigGetFonts(config, FcSetSystem);
    if (!fontSet) {
        FcConfigDestroy(config);
        return res;
    }
    for (int i = 0; i < fontSet->nfont; i++) {
        FcChar8 *path;
        FcPattern *pattern = fontSet->fonts[i];
        if (FcPatternGetString(pattern, FC_FILE, 0, &path) == FcResultMatch) {
            res->push_back(std::string(reinterpret_cast<const char *>(path)));
        }
    }
    FcFontSetDestroy(fontSet);
    FcConfigDestroy(config);
    return res;
}
