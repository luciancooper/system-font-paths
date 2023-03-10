#include <CoreText/CoreText.h>
#include <Foundation/Foundation.h>
#include <string>
#include "collection.h"

StringVector *getSystemFonts() {
    // cache font collection for fast use in future calls
    static CTFontCollectionRef collection = NULL;
    if (collection == NULL) collection = CTFontCollectionCreateFromAvailableFonts(NULL);

    NSArray *matches = (NSArray *)CTFontCollectionCreateMatchingFontDescriptors(collection);
    StringVector *res = new StringVector();

    for (id m in matches) {
        CTFontDescriptorRef descriptor = (CTFontDescriptorRef)m;
        NSURL *url = (NSURL *)CTFontDescriptorCopyAttribute(descriptor, kCTFontURLAttribute);
        const char *fpath = [[url path] UTF8String];
        [url release];
        res->push_back(std::string(fpath));
    }
    [matches release];
    return res;
}
