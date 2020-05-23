/*
 * GStreamer
 * Copyright (C) 2006 Stefan Kost <ensonic@users.sf.net>
 * Copyright (C) 2020 Niels De Graef <niels.degraef@gmail.com>
 * Copyright (C) 2020 Michael de Gans <michael.john.degans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#ifndef GST_CUDAFILTER_H__
#define GST_CUDAFILTER_H__

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

#include <BaseCudaFilter.hpp>

G_BEGIN_DECLS

typedef BaseFilter BaseFilter;

#define GST_TYPE_CUDAFILTER (gst_cudafilter_get_type())
G_DECLARE_FINAL_TYPE(GstCudaFilter,
                     gst_cudafilter,
                     GST,
                     CUDAFILTER,
                     GstBaseTransform)

struct _GstCudaFilter {
  GstBaseTransform element;

  // Base CUDA filter
  BaseFilter* filter;

  // properties:
  gboolean silent;
};

G_END_DECLS

#endif /* GST_CUDAFILTER_H__ */
