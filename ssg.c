/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2023 Beaver (GEGL SSG (stroke, shadow, glow) )
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shape2)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE2,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE2,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND2, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShape2)

property_enum   (grow_shape, _("Grow shape"),
                 GeglstrokeGrowShape2, gegl_stroke_grow_shape2,
                 GEGL_stroke_GROW_SHAPE_CIRCLE2)
  description   (_("The shape to expand or contract the stroke in"))



property_color (color, _("Color"), "#000000")
    ui_meta     ("role", "output-extent")

property_color (colorssg, _("Color Overlay"), "#ffffff")



property_double (x, _("Horizontal position of SSG"), 0.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-40, 40)
  ui_steps      (-40, 40)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Vertical position of SSG"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-40, 40)
  ui_steps      (-40, 40)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")



property_double (blurstroke, _("Blur to create Shadow or Glow"), 0.5)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 100.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (stroke, _("Grow radius of SSG"), 9.0)
  value_range   (0, 300.0)
  ui_range      (0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the stroke/shadow before blurring; a negative value will contract the shadow instead"))


property_double (opacity, _("Opacity"), 2)
  value_range   (0.0, 4.0)
  ui_steps      (0.0, 4.0)
ui_meta     ("role", "output-extent")

property_double  (alpha_percentile, _("Alpha percentile"), 0)
  value_range (0, 100)
  description (_("Neighborhood alpha percentile"))
    ui_meta     ("role", "output-extent")

property_int  (radius, _("Make the SSG go inward or outward"), 1)
  value_range (-250, 10)
  ui_range    (0, 2)
  description (_("Hidden Settings are meant for Graphs and removing edge pixels with the erase or split blend mode"))


property_double (blur, _("Smooth SSG"), 0.0)
   description  (_("A light blur to smooth the rough edges"))
  value_range (0, 1.5)
  ui_range (0, 1.5)
  ui_gamma (3.5)
    ui_meta     ("role", "output-extent")


property_file_path(image, _("Image file overlay"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))



property_double (blur2, _("Blur Image file Overlay"), 0.0)
   description  (_("A light blur to smooth the rough edges"))
  value_range (0, 40.0)
  ui_range (0, 40.0)
  ui_gamma (1.5)

property_double (opacityssg, _("Opacity for everything"), 2)
  value_range   (0.0, 4.0)
  ui_steps      (0.0, 4.0)

property_double (hue, _("Hue Rotation for everything - will over ride original color"),  0.0)
   description  (_("Color rotation that adjust all GEGL nodes below it'"))
   value_range  (-180.0, 180.0)



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     ssg
#define GEGL_OP_C_SOURCE ssg.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *median, *blur, *id1, *ssg, *xor, *color, *atop, *image, *blur2, *opacity, *hue;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  image   = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);


  color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  atop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 
   median   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

 ssg    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

 blur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

 blur2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  id1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  hue    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

xor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 60, NULL);

  gegl_operation_meta_redirect (operation, "colorssg", color, "value");
  gegl_operation_meta_redirect (operation, "color", ssg, "color");
  gegl_operation_meta_redirect (operation, "opacity", ssg, "value");
  gegl_operation_meta_redirect (operation, "opacityssg", opacity, "value");
  gegl_operation_meta_redirect (operation, "stroke", ssg, "grow-radius");
  gegl_operation_meta_redirect (operation, "blurstroke", ssg, "radius");
  gegl_operation_meta_redirect (operation, "x", ssg, "x");
  gegl_operation_meta_redirect (operation, "y", ssg, "y");
  gegl_operation_meta_redirect (operation, "grow_shape", ssg, "grow-shape");
  gegl_operation_meta_redirect (operation, "alpha_percentile", median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "radius", median, "radius");
  gegl_operation_meta_redirect (operation, "image", image, "src");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "blur", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blur", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "blur2", blur2, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blur2", blur2, "std-dev-y");




  gegl_node_link_many (input, median, blur, id1, ssg, xor, color, atop, opacity, hue, output, NULL);
  gegl_node_link_many (image, blur2, NULL);
  gegl_node_connect_from (xor, "aux", id1, "output");
  gegl_node_connect_from (atop, "aux", blur2, "output");





}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:ssg",
    "title",       _("Add a Stroke, Shadow or Glow"),
    "categories",  "Artistic",
    "reference-hash", "3ado316vg22a00x03vv5sb2ac",
    "description", _("Make a duplicate layer of image you want to apply SSG to, then apply SSG to the top or bottom duplicate layer. SSG is an enhanced fork of the drop shadow filter that starts as a outline, knocks out the original image and allows image file overlays.  "
                     ""),
    NULL);
}

#endif
