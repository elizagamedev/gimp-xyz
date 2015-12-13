/*=======================================================================
              XYZ load / save plugin for the GIMP
                 Copyright 2013 - Mathew Velasquez
          (Adapted from the WebP plugin by Nathan Osman)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======================================================================*/

#include "read-xyz.h"

#include <string.h>
#include <stdlib.h>
#include <libgimp/gimp.h>
#include <zlib.h>
#include <glib/gstdio.h>

int read_xyz(const gchar *filename)
{
    FILE *file;
    void *compressed_data,
         * uncompressed_data;
    long int size_dest, size_src;
    int width, height;
    gint32 new_image_id,
           new_layer_id;
    GimpDrawable *drawable;
    GimpPixelRgn rgn;


    // Try to open the file
    file = g_fopen(filename, "rb");
    if (!file)
        goto error;

    // Read the header
    char header[4];
    if (fread(header, 4, 1, file) != 1)
        goto error;

    // Compare the header
    static char xyz_header[4] = {'X', 'Y', 'Z', '1'};
    if (memcmp(xyz_header, header, 4))
        goto error;

    // Read the width and height
    width = 0;
    height = 0;
    if (fread(&width, 2, 1, file) != 1)
        goto error;
    if (fread(&height, 2, 1, file) != 1)
        goto error;

    // Get the file size
    fseek(file, 0, SEEK_END);
    size_src = ftell(file) - 8;
    fseek(file, 8, SEEK_SET);

    // Now prepare a buffer of that size
    // and read the data.
    compressed_data = malloc(size_src);
    if (fread(compressed_data, size_src, 1, file) != 1)
        goto error;

    // Close the file
    fclose(file);
    file = NULL;

    // Uncompress the data
    size_dest = 256*3 + width * height;
    uncompressed_data = malloc(size_dest);
    if (uncompress((Bytef *)uncompressed_data, (uLongf *)&size_dest, (Bytef *)compressed_data, (uLongf)size_src) != Z_OK)
        goto error;

    // Now create the new indexed image.
    new_image_id = gimp_image_new(width, height, GIMP_INDEXED);

    // Add the palette
    gimp_image_set_colormap(new_image_id, uncompressed_data, 256);

    // Create the new layer
    new_layer_id = gimp_layer_new(new_image_id,
                                  "Background",
                                  width, height,
                                  GIMP_INDEXED_IMAGE,
                                  100,
                                  GIMP_NORMAL_MODE);

    // Get the drawable for the layer
    drawable = gimp_drawable_get(new_layer_id);

    // Get a pixel region from the layer
    gimp_pixel_rgn_init(&rgn,
                        drawable,
                        0, 0,
                        width, height,
                        TRUE, FALSE);

    // Now FINALLY set the pixel data
    gimp_pixel_rgn_set_rect(&rgn,
                            uncompressed_data + 256*3,
                            0, 0,
                            width, height);

    // We're done with the drawable
    gimp_drawable_flush(drawable);
    gimp_drawable_detach(drawable);

    // Free the image data
    free(compressed_data);
    free(uncompressed_data);

    // Add the layer to the image
    gimp_image_add_layer(new_image_id, new_layer_id, 0);

    // Set the filename
    gimp_image_set_filename(new_image_id, filename);

    return new_image_id;

error:
    if (file)
        fclose(file);
    return -1;
}
