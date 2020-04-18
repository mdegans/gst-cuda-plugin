#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once

#ifdef HAVE_CONFIG_MESON_H
#include "config_meson.h"
#elif HAVE_CONFIG_CMAKE_H
#include "config_cmake.h"
#else
#include "config_dev.h"
#endif

#endif // __CONFIG_H__