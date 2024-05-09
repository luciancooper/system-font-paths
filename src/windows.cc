#define WINVER 0x0600
#include <dwrite.h>
#include <Windows.h>
#include <string>
#include "collection.h"

std::string wcharToString(const wchar_t *wstr) {
    // Calculate the length of the converted string
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        // Error handling if conversion fails
        return "";
    }
    // Reserve space for the string (excluding null terminator)
    std::string str(len - 1, 0);
    // Perform the conversion
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], len, NULL, NULL);
    // Return converted string
    return str;
}

// gets a localized string for a font
std::string getPostscriptName(IDWriteFont *font) {
    std::string psName;
    IDWriteLocalizedStrings *strings = NULL;
    BOOL exists = false;
    HRESULT hr = font->GetInformationalStrings(DWRITE_INFORMATIONAL_STRING_POSTSCRIPT_NAME, &strings, &exists);
    if (FAILED(hr) || !exists) {
        return psName;
    }
    unsigned int localeIndex = 0;
    wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
    // Get the default locale for this user.
    int success = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
    // If the default locale is returned, find that locale name, otherwise use "en-us".
    if (success) {
        hr = strings->FindLocaleName(localeName, &localeIndex, &exists);
        if (FAILED(hr)) {
            // failed to find locale name
            strings->Release();
            return psName;
        }
    }
    if (!exists) {
        hr = strings->FindLocaleName(L"en-us", &localeIndex, &exists);
        if (FAILED(hr)) {
            // failed to find locale name 'en-us'
            strings->Release();
            return psName;
        }
    }
    if (!exists) {
        localeIndex = 0;
    }
    unsigned int len = 0;
    hr = strings->GetStringLength(localeIndex, &len);
    if (FAILED(hr)) {
        // failed to get the length of the postscript name
        strings->Release();
        return psName;
    }
    wchar_t *str = new wchar_t[len + 1];
    hr = strings->GetString(localeIndex, str, len + 1);
    if (FAILED(hr)) {
        // failed to get the postscript name as a wide character string
        strings->Release();
        delete str;
        return psName;
    }
    // convert from utf-16 to utf8
    psName = wcharToString(str);
    // cleanup
    delete str;
    strings->Release();
    // return postscript name
    return psName;
}

std::string getFilePath(IDWriteFontFile *file, IDWriteLocalFontFileLoader *pLocalFileLoader) {
    std::string filePath;
    // get the reference key for this file
    const void *referenceKey = NULL;
    unsigned int referenceKeySize = 0;
    HRESULT hr = file->GetReferenceKey(&referenceKey, &referenceKeySize);
    if (FAILED(hr)) {
        // failed to get reference key
        return filePath;
    }
    // get the length of the font file path
    unsigned int nameLength = 0;
    hr = pLocalFileLoader->GetFilePathLengthFromKey(referenceKey, referenceKeySize, &nameLength);
    if (FAILED(hr)) {
        // failed to get the length of the font file path
        return filePath;
    }
    // get the font file path as a wide character string
    wchar_t *filePathWide = new wchar_t[nameLength + 1];
    hr = pLocalFileLoader->GetFilePathFromKey(referenceKey, referenceKeySize, filePathWide, nameLength + 1);
    if (SUCCEEDED(hr)) {
        filePath = wcharToString(filePathWide);
    }
    return filePath;
}

StringVector *getSystemFonts() {
    StringVector *res = new StringVector();
    int count = 0;

    IDWriteFactory *pFactory = NULL;
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                     reinterpret_cast<IUnknown **>(&pFactory));
    if (FAILED(hr)) {
        // Failed to create DirectWrite factory
        return res;
    }

    // Get the system font collection
    IDWriteFontCollection *pFontCollection = NULL;
    hr = pFactory->GetSystemFontCollection(&pFontCollection, FALSE);
    if (FAILED(hr)) {
        // Failed to get system font collection
        pFactory->Release();
        return res;
    }

    // Get the number of font families in the collection
    int familyCount = pFontCollection->GetFontFamilyCount();

    // track postscript names we've already added using a set so we don't get any duplicates
    StringVector *psNames = new StringVector();

    for (int i = 0; i < familyCount; i++) {
        // Get the font family.
        IDWriteFontFamily *pFontFamily = NULL;
        hr = pFontCollection->GetFontFamily(i, &pFontFamily);
        if (FAILED(hr)) {
            // failed to get the font family
            continue;
        }
        // get the number of fonts
        int fontCount = pFontFamily->GetFontCount();
        // loop through each font in this family
        for (int j = 0; j < fontCount; j++) {
            // get the font
            IDWriteFont *pFont = NULL;
            hr = pFontFamily->GetFont(j, &pFont);
            if (FAILED(hr)) {
                // Failed to get font
                continue;
            }
            // get the font face
            IDWriteFontFace *pFace = NULL;
            hr = pFont->CreateFontFace(&pFace);
            if (FAILED(hr)) {
                // Failed to create font face
                pFont->Release();
                continue;
            }
            // get the font files from this font face
            // https://learn.microsoft.com/en-us/windows/win32/api/dwrite/nf-dwrite-idwritefontface-getfiles
            unsigned int numFiles = 0;
            hr = pFace->GetFiles(&numFiles, NULL);
            if (FAILED(hr) || numFiles == 0) {
                // Failed to get the number of font files
                pFace->Release();
                pFont->Release();
                continue;
            }
            IDWriteFontFile *pFiles = NULL;
            hr = pFace->GetFiles(&numFiles, &pFiles);
            if (FAILED(hr)) {
                // Failed to get the font file list
                pFace->Release();
                pFont->Release();
                continue;
            }
            // get the font file loader from the first font file
            IDWriteFontFileLoader *pFileLoader = NULL;
            hr = pFiles[0].GetLoader(&pFileLoader);
            if (FAILED(hr)) {
                // failed to get the font file loader
                pFiles->Release();
                pFace->Release();
                pFont->Release();
                continue;
            }
            // check if this is a local font file
            IDWriteLocalFontFileLoader *pLocalFileLoader = NULL;
            hr = pFileLoader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void **)&pLocalFileLoader);
            if (SUCCEEDED(hr)) {
                // get the postscript name for this font
                std::string postscriptName = getPostscriptName(pFont);
                if (!postscriptName.empty() && !psNames->contains(postscriptName)) {
                    // add this postscript name to the list
                    psNames->push_back(postscriptName);
                    // get the font file path
                    std::string filePath = getFilePath(&pFiles[0], pLocalFileLoader);
                    if (!filePath.empty()) {
                        // add the font file path to the results list
                        res->push_back(filePath);
                    }
                }
                // cleanup - release the local file loader
                pLocalFileLoader->Release();
            }
            // cleanup - release the file loader
            pFileLoader->Release();
            // cleanup
            pFiles->Release();
            pFace->Release();
            pFont->Release();
        }
        // cleanup the font family
        pFontFamily->Release();
    }
    // cleanup the font collection & write factory
    pFontCollection->Release();
    pFactory->Release();

    return res;
}
