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

#include "write-xyz.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgimp/gimp.h>
#include <zlib.h>
#include <glib/gstdio.h>

int write_xyz(const gchar *filename, gint image_id, gint drawable_id)
{
    GimpDrawable *drawable;
    GimpPixelRgn rgn;
    long int uncompressed_size;
    long int compressed_size;
    void *uncompressed_data,
         * compressed_data;
    int width, height;

    guchar *palette;
    gint c_palette;

    FILE *file;

    // Get the drawable
    drawable = gimp_drawable_get(drawable_id);

    // Get the width and height
    width = drawable->width;
    height = drawable->height;

    // Get a pixel region from the layer
    gimp_pixel_rgn_init(&rgn,
                        drawable,
                        0, 0,
                        width, height,
                        FALSE, FALSE);

    // Determine the size of the array of image data to get
    // and allocate it.
    uncompressed_size = 256*3 + width * height;
    uncompressed_data = malloc(uncompressed_size);

    // Get the palette data
    palette = gimp_image_get_colormap(image_id, &c_palette);

    // Copy over the colors and set the rest to 0
    memcpy(uncompressed_data, palette, c_palette * 3);
    memset(uncompressed_data + c_palette * 3, 0, (256 - c_palette) * 3);

    // Get the image data
    gimp_pixel_rgn_get_rect(&rgn,
                            (guchar *)(uncompressed_data + 256*3),
                            0, 0,
                            width, height);

    // We have the image data, now compress it
    compressed_size = compressBound(uncompressed_size);
    compressed_data = malloc(compressed_size);
    compress((Bytef *)compressed_data, &compressed_size, (Bytef *)uncompressed_data, 256*3 + width * height);

    // Free the uncompressed data
    free(uncompressed_data);

    // Detach the drawable
    gimp_drawable_detach(drawable);

    // Open the file
    file = g_fopen(filename, "wb");
    if (!file) {
        free(compressed_data);
        return 0;
    }

    // Write the data and be done with it.
    static char xyz_header[4] = {'X', 'Y', 'Z', '1'};
    fwrite(xyz_header, 4, 1, file);
    fwrite(&width, 2, 1, file);
    fwrite(&height, 2, 1, file);
    fwrite(compressed_data, compressed_size, 1, file);
    free(compressed_data);
    fclose(file);

    return 1;
}
