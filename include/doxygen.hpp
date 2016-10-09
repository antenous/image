/**
 * @mainpage
 *
 * @section introdunction_sec Introduction
 *
 * <b>image</b> is a project for running conversions between <i>bitmap</i> (BMP)
 * and <i>direct draw surface</i> (DDS) file formats.
 *
 * @section running_sec Running
 *
 * Running the conversions is simply done by giving the name of the file to
 * convert to the project executable. The file type is detected automatically
 * and if the file has no problems the resulting file will be stored on disk
 * as <i>out.<filetype></i>. When converting from a bitmap file the filetype
 * will be <i>dds</i> for direct draw surface image, when running the conversion
 * the other way around the file type will be <i>bmp</i>.
 *
 * @section limitations_sec Limitations
 *
 * For now the project only supports uncompressed bitmap files with 24bit color
 * depth. The image height and width will have to be a multiple of four. The
 * direct draw surface image supports only DXT1 (BC1) compression.
 *
 * @namespace image
 *
 * @brief Top level namespace
 *
 * All the code in this project resides in the <code>image</code> namespace.
 * */
