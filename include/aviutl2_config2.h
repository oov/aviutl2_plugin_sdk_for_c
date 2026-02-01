// https://github.com/oov/aviutl2_plugin_sdk_for_c
// The MIT License / Copyright (c) 2025 oov
#pragma once

// This file based on config2.h from AviUtl ExEdit2 Plugin SDK
// LICENSE: The MIT License / Copyright (c) 2025 Kenkun

// Any plugin can export the following function to enable config support:
//
// Config initialization function (called before InitializePlugin())
//   void InitializeConfig(struct aviutl2_config_handle *config)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

/**
 * Font information
 */
struct aviutl2_font_info {
  wchar_t const *name; /**< Font name */
  float size;          /**< Font size */
};

/**
 * Config handle interface for AviUtl2 plugins
 */
struct aviutl2_config_handle {
  /**
   * Application data folder path
   */
  wchar_t const *app_data_path;

  /**
   * Get text defined in the current language settings
   * The language settings section to reference is the filename of the plugin that defined InitializeConfig()
   * @param handle Config handle provided by the host
   * @param text Original text (key name in .aul2 file)
   * @return Pointer to defined text (returns the argument text pointer if undefined).
   *         Valid until language settings are updated
   */
  wchar_t const *(*translate)(struct aviutl2_config_handle *handle, wchar_t const *text);

  /**
   * Get text defined in the current language settings from any section
   * @param handle Config handle provided by the host
   * @param section Language settings section (section name in .aul2 file)
   * @param text Original text (key name in .aul2 file)
   * @return Pointer to defined text (returns the argument text pointer if undefined).
   *         Valid until language settings are updated
   */
  wchar_t const *(*get_language_text)(struct aviutl2_config_handle *handle,
                                      wchar_t const *section,
                                      wchar_t const *text);

  /**
   * Get font information defined in the config file
   * @param handle Config handle provided by the host
   * @param key Key name in [Font] section of config file (style.conf)
   * @return Pointer to font info structure (returns default font if cannot be obtained).
   *         Valid until this function is called again
   */
  struct aviutl2_font_info *(*get_font_info)(struct aviutl2_config_handle *handle, char const *key);

  /**
   * Get color code defined in the config file
   * For multiple colors, the first color is obtained
   * @param handle Config handle provided by the host
   * @param key Key name in [Color] section of config file (style.conf)
   * @return Defined color code value (returns 0 if cannot be obtained)
   */
  int (*get_color_code)(struct aviutl2_config_handle *handle, char const *key);

  /**
   * Get layout size defined in the config file
   * @param handle Config handle provided by the host
   * @param key Key name in [Layout] section of config file (style.conf)
   * @return Defined size (returns 0 if cannot be obtained)
   */
  int (*get_layout_size)(struct aviutl2_config_handle *handle, char const *key);

  /**
   * Get color code defined in the config file
   * @param handle Config handle provided by the host
   * @param key Key name in [Color] section of config file (style.conf)
   * @param index Index of the color to get (specify -1 to return the number of colors)
   * @return Defined color code value (returns 0 if cannot be obtained)
   */
  int (*get_color_code_index)(struct aviutl2_config_handle *handle, char const *key, int index);
};
