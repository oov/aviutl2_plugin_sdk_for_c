// https://github.com/oov/aviutl2_plugin_sdk_for_c
// The MIT License / Copyright (c) 2025 oov
#pragma once

// This file based on filter2.h from AviUtl ExEdit2 Plugin SDK
// LICENSE: The MIT License / Copyright (c) 2025 Kenkun

// Filter plugins must export the following functions for the host to call:
//
// Required entry point returning the filter definition
//   struct aviutl2_filter_plugin_table *GetFilterPluginTable(void)
//
// Optional plugin DLL initialization function (version is the host application version)
//   bool InitializePlugin(uint32_t version)
//
// Optional plugin DLL uninitialization function
//   void UninitializePlugin(void)
//
// Optional logger initialization function (see aviutl2_logger2.h). Called before InitializePlugin()
//   void InitializeLogger(struct aviutl2_log_handle *logger)
//
// Optional config initialization function (see aviutl2_config2.h). Called before InitializePlugin()
//   void InitializeConfig(struct aviutl2_config_handle *config)

#include <stdbool.h>
#include <stdint.h>

struct ID3D11Texture2D;
struct aviutl2_edit_section;

/**
 * Track bar filter item
 */
struct aviutl2_filter_item_track {
  /**
   * Setting type (L"track")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value when filter function is called
   */
  double value;

  /**
   * Minimum value
   */
  double const s;

  /**
   * Maximum value
   */
  double const e;

  /**
   * Setting step (1.0 / 0.1 / 0.01 / 0.001 )
   */
  double const step;
};

/**
 * Check box filter item
 */
struct aviutl2_filter_item_check {
  /**
   * Setting type (L"check")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value when filter function is called
   */
  bool value;
};

/**
 * Color picker filter item
 */
struct aviutl2_filter_item_color {
  /**
   * Setting type (L"color")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Color value
   * Accessible as uint32_t code or as separate components
   */
  union {
    /**
     * Color value as 32-bit integer
     */
    uint32_t code;

    /**
     * Color value as individual components
     */
    struct {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t x;
    };
  } value;
};

/**
 * Select list filter item
 */
struct aviutl2_filter_item_select {
  /**
   * Setting type (L"select")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value when filter function is called
   */
  int value;

  /**
   * Selection list item
   */
  struct aviutl2_filter_item_select_item {
    /**
     * Selection name
     */
    wchar_t const *name;

    /**
     * Selection value
     */
    int value;
  } *list;
};

/**
 * File path filter item
 */
struct aviutl2_filter_item_file {
  /**
   * Setting type (L"file")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value when filter function is called
   */
  wchar_t const *value;

  /**
   * File filter
   */
  wchar_t const *filefilter;
};

/**
 * Generic data filter item (non-UI item)
 * Allows the filter procedure to share arbitrary data blocks
 */
struct aviutl2_filter_item_data {
  /**
   * Setting type (L"data")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Pointer to data block, updated to current value during filter processing
   */
  void *value;

  /**
   * Size of data block in bytes (1024 bytes or less)
   */
  int size;
};

/**
 * Settings group filter item
 * Groups subsequent setting items
 * Set name to empty to define the end of a group
 */
struct aviutl2_filter_item_group {
  /**
   * Setting type (L"group")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Default visibility state
   */
  bool const default_visible;
};

/**
 * Button filter item
 * Note: Uses the same callback signature as edit callbacks in plugin2.h
 */
struct aviutl2_filter_item_button {
  /**
   * Setting type (L"button")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Callback function when button is pressed
   */
  void (*callback)(struct aviutl2_edit_section *edit);
};

/**
 * String filter item (single line string)
 */
struct aviutl2_filter_item_string {
  /**
   * Setting type (L"string")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value pointer when filter function is called
   */
  wchar_t const *value;
};

/**
 * Text filter item (multi-line string)
 */
struct aviutl2_filter_item_text {
  /**
   * Setting type (L"text")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value pointer when filter function is called
   */
  wchar_t const *value;
};

/**
 * Folder selection filter item
 * Example: aviutl2_filter_item_folder folder = { L"folder", L"Folder", L"" };
 */
struct aviutl2_filter_item_folder {
  /**
   * Setting type (L"folder")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Setting value
   * Updated to current value pointer when filter function is called
   */
  wchar_t const *value;
};

//--------------------------------

/**
 * RGBA 32-bit structure
 */
struct aviutl2_pixel_rgba {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

/**
 * Scene information
 */
struct aviutl2_scene_info {
  /**
   * Scene width, height
   */
  int width, height;

  /**
   * Scene frame rate
   */
  int rate, scale;

  /**
   * Scene sampling rate
   */
  int sample_rate;
};

/**
 * Object information
 */
struct aviutl2_object_info {
  /**
   * Object ID (unique ID within the application)
   * Note: This is the unique ID of the drawing target object
   */
  int64_t id;

  /**
   * Current frame number of the object
   */
  int frame;

  /**
   * Total number of frames of the object
   */
  int frame_total;

  /**
   * Current time of the object (seconds)
   */
  double time;

  /**
   * Total time of the object (seconds)
   */
  double time_total;

  /**
   * Current image size of the object (for video filters)
   */
  int width, height;

  /**
   * Current audio sample position of the object (for audio filters)
   */
  int64_t sample_index;

  /**
   * Total number of audio samples of the object (for audio filters)
   */
  int64_t sample_total;

  /**
   * Current number of audio samples of the object (for audio filters)
   */
  int sample_num;

  /**
   * Number of audio channels of the object (for audio filters), typically 2
   */
  int channel_num;

  /**
   * Effect identifier inside the object (unique during application lifetime)
   */
  int64_t effect_id;

  /**
   * Flags
   */
  int flag;
};

enum {
  /**
   * Is filter object
   */
  aviutl2_object_info_flag_filter_object = 1,
};

/**
 * Check if object is a filter object
 * @param info Object information
 * @return true if filter object
 */
static inline bool aviutl2_object_info_is_filter_object(struct aviutl2_object_info const *info) {
  return info->flag & aviutl2_object_info_flag_filter_object;
}

//--------------------------------

/**
 * Video filter processing structure
 */
struct aviutl2_filter_proc_video {
  /**
   * Scene information
   */
  struct aviutl2_scene_info const *scene;

  /**
   * Object information
   */
  struct aviutl2_object_info const *object;

  /**
   * Get current object image data (from VRAM)
   * @param buffer Pointer to image data storage where the image data will be stored
   */
  void (*get_image_data)(struct aviutl2_pixel_rgba *buffer);

  /**
   * Set current object image data (write to VRAM)
   * @param buffer Pointer to image data to write to VRAM (if nullptr, image size is changed with uninitialized data)
   * @param width Image width
   * @param height Image height
   */
  void (*set_image_data)(struct aviutl2_pixel_rgba *buffer, int width, int height);

  /**
   * Get pointer to current object image data (ID3D11Texture2D)
   * @return Pointer to object image data
   *         Valid until current image is changed (set_image_data) or filter processing ends
   */
  struct ID3D11Texture2D *(*get_image_texture2d)(void);

  /**
   * Get pointer to current framebuffer image data (ID3D11Texture2D)
   * @return Pointer to framebuffer image data
   *         Valid until filter processing ends
   */
  struct ID3D11Texture2D *(*get_framebuffer_texture2d)(void);
};

/**
 * Audio filter processing structure
 */
struct aviutl2_filter_proc_audio {
  /**
   * Scene information
   */
  struct aviutl2_scene_info const *scene;

  /**
   * Object information
   */
  struct aviutl2_object_info const *object;

  /**
   * Get current object audio sample data
   * @param buffer Pointer to audio data storage where the audio data will be stored (audio data is PCM float 32-bit)
   * @param channel Audio channel number (0 = left/mono, 1 = right)
   */
  void (*get_sample_data)(float *buffer, int channel);

  /**
   * Set current object audio sample data
   * @param buffer Pointer to audio data to write (audio data is PCM float 32-bit)
   * @param channel Audio channel number (0 = left/mono, 1 = right)
   */
  void (*set_sample_data)(float *buffer, int channel);
};

//--------------------------------

/**
 * Filter plugin table
 */
struct aviutl2_filter_plugin_table {
  enum {
    /**
     * Support video filter
     */
    aviutl2_filter_plugin_table_flag_video = 1,

    /**
     * Support audio filter
     */
    aviutl2_filter_plugin_table_flag_audio = 2,

    /**
     * Support media object input (when making a media object)
     */
    aviutl2_filter_plugin_table_flag_input = 4,

    /**
     * Support filter object (when supporting filter objects)
     * For filter objects, image size cannot be changed
     */
    aviutl2_filter_plugin_table_flag_filter = 8,
  };

  /**
   * Flags
   */
  int flag;

  /**
   * Plugin name
   */
  wchar_t const *name;

  /**
   * Label display value (NULL for default label)
   */
  wchar_t const *label;

  /**
   * Plugin information
   */
  wchar_t const *information;

  /**
   * Configuration items (null-terminated array of pointers to aviutl2_filter_item_* structures)
   */
  void **items;

  /**
   * Video filter function pointer
   * Called only if FLAG_VIDEO is set
   * @param video Pointer to video filter processing parameters
   * @return true if processing succeeded, false otherwise
   */
  bool (*func_proc_video)(struct aviutl2_filter_proc_video *video);

  /**
   * Audio filter function pointer
   * Called only if FLAG_AUDIO is set
   * @param audio Pointer to audio filter processing parameters
   * @return true if processing succeeded, false otherwise
   */
  bool (*func_proc_audio)(struct aviutl2_filter_proc_audio *audio);
};
