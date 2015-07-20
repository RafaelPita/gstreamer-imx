#ifndef GST_IMX_COMMON_CANVAS_H
#define GST_IMX_COMMON_CANVAS_H

#include <gst/gst.h>
#include <gst/video/video.h>

#include "region.h"


G_BEGIN_DECLS


typedef struct _GstImxCanvas GstImxCanvas;


typedef enum
{
	GST_IMX_CANVAS_EMPTY_REGION_INDEX_TOP = 0,
	GST_IMX_CANVAS_EMPTY_REGION_INDEX_BOTTOM = 1,
	GST_IMX_CANVAS_EMPTY_REGION_INDEX_LEFT = 2,
	GST_IMX_CANVAS_EMPTY_REGION_INDEX_RIGHT = 3
}
GstImxCanvasEmptyRegionIndices;


/**
 * GstImxCanvasVisibilityFlags:
 *
 * Flags identifying a visible region within a canvas. Used for the visibility
 * bitmask in the canvas to check if a region is visible.
 *
 * The empty regions are guaranteed to start at bit 0. It is therefore valid
 * to go over all empty regions simply by using (1 << i) in a loop, where i starts
 * at 0, and ends at 3.
 */
typedef enum
{
	GST_IMX_CANVAS_VISIBILITY_FLAG_REGION_EMPTY_TOP    = (1 << GST_IMX_CANVAS_EMPTY_REGION_INDEX_TOP),
	GST_IMX_CANVAS_VISIBILITY_FLAG_REGION_EMPTY_BOTTOM = (1 << GST_IMX_CANVAS_EMPTY_REGION_INDEX_BOTTOM),
	GST_IMX_CANVAS_VISIBILITY_FLAG_REGION_EMPTY_LEFT   = (1 << GST_IMX_CANVAS_EMPTY_REGION_INDEX_LEFT),
	GST_IMX_CANVAS_VISIBILITY_FLAG_REGION_EMPTY_RIGHT  = (1 << GST_IMX_CANVAS_EMPTY_REGION_INDEX_RIGHT),
	GST_IMX_CANVAS_VISIBILITY_FLAG_REGION_INNER = (1 << 4)
}
GstImxCanvasVisibilityFlags;


typedef enum
{
	GST_IMX_CANVAS_INNER_ROTATION_NONE,
	GST_IMX_CANVAS_INNER_ROTATION_90_DEGREES,
	GST_IMX_CANVAS_INNER_ROTATION_180_DEGREES,
	GST_IMX_CANVAS_INNER_ROTATION_270_DEGREES,
	GST_IMX_CANVAS_INNER_ROTATION_HFLIP,
	GST_IMX_CANVAS_INNER_ROTATION_VFLIP
}
GstImxCanvasInnerRotation;


/**
 * GstImxCanvas:
 *
 * Rectangular space containing multiple regions.
 * The outer region contains all the other ones fully. Any pixels that
 * lie in the outer but not the inner region is in one of the empty
 * regions. Blitters are supposed to paint the empty regions with the
 * fill_color, which is a 32-bit RGBA tuple, in format: 0xRRGGBBAA.
 * The inner region contains the actual video frame.
 * The visibility mask describes what regions are visible.
 * The margin values determine margin sizes in pixels between inner and
 * outer region. The margin is applied prior to the computation of the
 * inner region.
 */
struct _GstImxCanvas
{
	GstImxRegion outer_region;
	guint32 fill_color;
	guint margin_left, margin_top, margin_right, margin_bottom;
	gboolean keep_aspect_ratio;
	GstImxCanvasInnerRotation inner_rotation;

	/* these are computed by calculate_inner_region() */
	GstImxRegion inner_region;

	/* these are computed by clip() */
	GstImxRegion clipped_outer_region;
	GstImxRegion clipped_inner_region;
	GstImxRegion empty_regions[4];
	guint8 visibility_mask;
};


GType gst_imx_canvas_inner_rotation_get_type(void);


gboolean gst_imx_canvas_does_rotation_transpose(GstImxCanvasInnerRotation rotation);
void gst_imx_canvas_calculate_inner_region(GstImxCanvas *canvas, GstVideoInfo const *info);
void gst_imx_canvas_clip(GstImxCanvas *canvas, GstImxRegion const *screen_region, GstVideoInfo const *info, GstImxRegion *source_subset);



G_END_DECLS


#endif