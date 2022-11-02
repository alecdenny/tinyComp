/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   ArialRoundedBold_ttf;
    const int            ArialRoundedBold_ttfSize = 49296;

    extern const char*   knobsvg_svg;
    const int            knobsvg_svgSize = 14044;

    extern const char*   knobIndicator_svg;
    const int            knobIndicator_svgSize = 210;

    extern const char*   bg_svg;
    const int            bg_svgSize = 122549;

    extern const char*   screenOverlay_svg;
    const int            screenOverlay_svgSize = 879;

    extern const char*   screenSVG_svg;
    const int            screenSVG_svgSize = 54950;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
