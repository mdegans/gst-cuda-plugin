/*
 * GStreamer
 * Copyright (C) 2006 Stefan Kost <ensonic@users.sf.net>
 * Copyright (C) 2020 Michael de Gans <michael.john.degans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-cudafilter
 *
 * Cudafilter is an example CUDA filter for gstreamer.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m nvarguscamerasrc ! cudafilter ! autovideosink
 * ]|
 * </refsect2>
 */

#include "config.h"

#include "gstcudafilter.h"

// gstreamer
#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/controller/controller.h>
#include <gst/video/video-format.h>

GST_DEBUG_CATEGORY_STATIC (gst_cudafilter_debug);
#define GST_CAT_DEFAULT gst_cudafilter_debug

static const char ELEMENT_NAME[] = "cudafilter";
static const char ELEMENT_TYPE[] = "Filter";
static const char ELEMENT_DESCRIPTION[] = "Simple CUDA Filter Element";
static const char ELEMENT_AUTHOR_AND_EMAIL[] = PACKAGE_AUTHOR " " PACKAGE_EMAIL;

/* Filter signals and args */
enum
{
	/* FILL ME */
	LAST_SIGNAL
};

enum
{
	PROP_0,
	PROP_SILENT,
};

/* the capabilities of the inputs and outputs.
 *
 * static src and sink are both: video/x-raw(memory:NVMM) {NV12, RGBA}
 */
static GstStaticPadTemplate sink_template =
GST_STATIC_PAD_TEMPLATE (
	"sink",
	GST_PAD_SINK,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS(
		GST_VIDEO_CAPS_MAKE_WITH_FEATURES("memory:NVMM", "{ NV12, RGBA }"))
);

static GstStaticPadTemplate src_template =
GST_STATIC_PAD_TEMPLATE (
	"src",
	GST_PAD_SRC,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS(
		GST_VIDEO_CAPS_MAKE_WITH_FEATURES("memory:NVMM", "{ NV12, RGBA }"))
);

#define gst_cudafilter_parent_class parent_class
G_DEFINE_TYPE (GstCudaFilter, gst_cudafilter, GST_TYPE_BASE_TRANSFORM);

static void gst_cudafilter_set_property (GObject * object, guint prop_id,
	const GValue * value, GParamSpec * pspec);
static void gst_cudafilter_get_property (GObject * object, guint prop_id,
	GValue * value, GParamSpec * pspec);

static GstFlowReturn gst_cudafilter_transform_ip (GstBaseTransform * base,
	GstBuffer * outbuf);
static gboolean gst_cudafilter_start (GstBaseTransform * base);
static gboolean gst_cudafilter_stop (GstBaseTransform * base);

/* GObject vmethod implementations */

/* initialize the cudafilter's class */
static void
gst_cudafilter_class_init (GstCudaFilterClass * klass)
{
	GObjectClass *gobject_class;
	GstElementClass *gstelement_class;

	gobject_class = (GObjectClass *) klass;
	gstelement_class = (GstElementClass *) klass;

	gobject_class->set_property = gst_cudafilter_set_property;
	gobject_class->get_property = gst_cudafilter_get_property;

	g_object_class_install_property (gobject_class, PROP_SILENT,
		g_param_spec_boolean (
			"silent", "Silent", "Produce verbose output ?",
			FALSE, GParamFlags(G_PARAM_READWRITE | GST_PARAM_CONTROLLABLE)));

	gst_element_class_set_details_simple (gstelement_class,
		ELEMENT_NAME,
		ELEMENT_TYPE,
		ELEMENT_DESCRIPTION,
		ELEMENT_AUTHOR_AND_EMAIL);

	gst_element_class_add_pad_template (gstelement_class,
			gst_static_pad_template_get (&src_template));
	gst_element_class_add_pad_template (gstelement_class,
			gst_static_pad_template_get (&sink_template));

	/* register vmethods
	 */
	GST_BASE_TRANSFORM_CLASS (klass)->transform_ip =
		GST_DEBUG_FUNCPTR (gst_cudafilter_transform_ip);
	GST_BASE_TRANSFORM_CLASS (klass)->start = 
		GST_DEBUG_FUNCPTR (gst_cudafilter_start);
	GST_BASE_TRANSFORM_CLASS (klass)->stop = 
		GST_DEBUG_FUNCPTR (gst_cudafilter_stop);

	/* debug category for fltering log messages
	 */
	GST_DEBUG_CATEGORY_INIT (
		gst_cudafilter_debug, ELEMENT_NAME, 0, ELEMENT_DESCRIPTION);
}


/* initialize the new element
 * initialize instance structure
 */
static void
gst_cudafilter_init (GstCudaFilter *filter)
{
	filter->silent = FALSE;
	filter->filter = NULL;
}


/* start the element and create external resources
 *
 * https://gstreamer.freedesktop.org/documentation/base/gstbasetransform.html?gi-language=c#GstBaseTransformClass::start
 */

static gboolean
gst_cudafilter_start (GstBaseTransform * base)
{
	GstCudaFilter * filter = GST_CUDAFILTER (base);

	/* create a CUDA filter for this instance
	 */
	filter->filter = new TestCudaFilter();

	return true;
}

/* stop the element and free external resources
 *
 * https://gstreamer.freedesktop.org/documentation/base/gstbasetransform.html?gi-language=c#GstBaseTransformClass::stop
 */

static gboolean
gst_cudafilter_stop (GstBaseTransform * base)
{
	GstCudaFilter * filter = GST_CUDAFILTER (base);

	/* destroy the CUDA filter
	 */
	delete filter->filter;

	return true;
}

/* do in-place work on the buffer (override the 'transform' method for copy)
 */
static GstFlowReturn
gst_cudafilter_transform_ip (GstBaseTransform * base, GstBuffer * outbuf)
{
	GstCudaFilter * filter = GST_CUDAFILTER (base);

	if (filter->silent == FALSE)
		GST_LOG("%s got buffer.", GST_ELEMENT_NAME(filter));
	
	if (GST_CLOCK_TIME_IS_VALID (GST_BUFFER_TIMESTAMP (outbuf)))
		gst_object_sync_values (GST_OBJECT (filter), GST_BUFFER_TIMESTAMP (outbuf));

	return filter->filter->on_buffer(outbuf);

	return GST_FLOW_OK;
}

/* __setattr__
 */
static void
gst_cudafilter_set_property (GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec)
{
	GstCudaFilter *filter = GST_CUDAFILTER (object);

	switch (prop_id) {
		case PROP_SILENT:
			filter->silent = g_value_get_boolean (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}


/* __getattr__
 */
static void
gst_cudafilter_get_property (GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec)
{
	GstCudaFilter *filter = GST_CUDAFILTER (object);

	switch (prop_id) {
		case PROP_SILENT:
			g_value_set_boolean (value, filter->silent);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}


