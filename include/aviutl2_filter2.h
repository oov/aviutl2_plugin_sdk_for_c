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
// Optional function to get required host version number
//   uint32_t RequiredVersion() - Returns the required host application version number
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
//
// Optional cache initialization function (see aviutl2_cache2.h)
//   void InitializeCache(struct aviutl2_cache_handle *cache)

#include <stdbool.h>
#include <stdint.h>

struct ID3D11Texture2D;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct IDWriteFont;
struct aviutl2_edit_section;

/**
 * Object handle
 */
#ifndef AVIUTL2_OBJECT_HANDLE_DEFINED
#define AVIUTL2_OBJECT_HANDLE_DEFINED
typedef void *aviutl2_object_handle;
#endif

/**
 * Track bar filter item
 */
struct aviutl2_filter_item_track {
  /**
   * Setting type (L"track2")
   * Backward compatibility with previous type L"track" is supported
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
   * Note: Values of 0.0001 or less can also be specified, but will be adjusted according to the min/max range
   */
  double const step;

  /**
   * Zero value display name
   * String displayed on the track bar when the value is 0
   */
  wchar_t const *zero_display;

  /**
   * Operation scale factor
   * Scale factor of track bar operation range relative to the setting value range
   */
  double const slider_ratio;
};

/**
 * Track bar group filter item
 * Track group supports only 2 or 3 track items
 */
struct aviutl2_filter_item_track_group {
  /**
   * Setting type (L"trackgroup")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;

  /**
   * Track item group
   * Null-terminated list of pointers to aviutl2_filter_item_track
   */
  struct aviutl2_filter_item_track **tracks;
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
 * Check box filter item (per section)
 */
struct aviutl2_filter_item_check_section {
  /**
   * Setting type (L"checksection2")
   * Backward compatibility with previous type L"checksection" is supported
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

  /**
   * Initial value of per-section setting
   */
  bool const multi_section;
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
   * Setting values of each setting item are updated when this callback is invoked
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

/**
 * Separator filter item
 * Example: aviutl2_filter_item_separator separator = { L"separator", L"Center Coordinates" };
 */
struct aviutl2_filter_item_separator {
  /**
   * Setting type (L"separator")
   */
  wchar_t const *type;

  /**
   * Setting name
   */
  wchar_t const *name;
};

//--------------------------------

/**
 * Vertex data structure (color)
 */
struct aviutl2_vertex_color {
  float x, y, z;    /**< Vertex coordinates */
  float r, g, b, a; /**< Vertex color (premultiplied alpha in the range 0.0 to 1.0) */
};

/**
 * Vertex data structure (color, normal)
 */
struct aviutl2_vertex_color_norm {
  float x, y, z;    /**< Vertex coordinates */
  float r, g, b, a; /**< Vertex color (premultiplied alpha in the range 0.0 to 1.0) */
  float vx, vy, vz; /**< Normal vector */
};

/**
 * Vertex data structure (texture)
 */
struct aviutl2_vertex_texture {
  float x, y, z; /**< Vertex coordinates */
  float u, v;    /**< Texture coordinates (normalized coordinates in the range 0.0 to 1.0) */
  float a;       /**< Vertex alpha value */
};

/**
 * Vertex data structure (texture, normal)
 */
struct aviutl2_vertex_texture_norm {
  float x, y, z;    /**< Vertex coordinates */
  float u, v;       /**< Texture coordinates (normalized coordinates in the range 0.0 to 1.0) */
  float a;          /**< Vertex alpha value */
  float vx, vy, vz; /**< Normal vector */
};

/**
 * Vertex list type
 */
enum aviutl2_vertex_type {
  aviutl2_vertex_type_triangle_color = 1,        /**< List of aviutl2_vertex_color triangles (vertex count must be a multiple of 3) */
  aviutl2_vertex_type_triangle_color_norm = 2,   /**< List of aviutl2_vertex_color_norm triangles (vertex count must be a multiple of 3) */
  aviutl2_vertex_type_triangle_texture = 3,      /**< List of aviutl2_vertex_texture triangles (vertex count must be a multiple of 3) */
  aviutl2_vertex_type_triangle_texture_norm = 4, /**< List of aviutl2_vertex_texture_norm triangles (vertex count must be a multiple of 3) */
  aviutl2_vertex_type_quad_color = 5,            /**< List of aviutl2_vertex_color quads (vertex count must be a multiple of 4) */
  aviutl2_vertex_type_quad_color_norm = 6,       /**< List of aviutl2_vertex_color_norm quads (vertex count must be a multiple of 4) */
  aviutl2_vertex_type_quad_texture = 7,          /**< List of aviutl2_vertex_texture quads (vertex count must be a multiple of 4) */
  aviutl2_vertex_type_quad_texture_norm = 8,     /**< List of aviutl2_vertex_texture_norm quads (vertex count must be a multiple of 4) */
};

/**
 * Sampler type
 */
enum aviutl2_sampler_mode {
  aviutl2_sampler_mode_clip = 0,   /**< Transparent color outside the region */
  aviutl2_sampler_mode_clamp = 1,  /**< Outermost color outside the region */
  aviutl2_sampler_mode_loop = 2,   /**< Loop outside the region */
  aviutl2_sampler_mode_mirror = 3, /**< Loop while mirroring the region outside the bounds */
  aviutl2_sampler_mode_dot = 4,    /**< No scaling interpolation (transparent color outside the region) */
};

/**
 * Output blend mode type (BlendState)
 */
enum aviutl2_blend_state_mode {
  aviutl2_blend_state_mode_copy = 0, /**< Copy output as-is */
  aviutl2_blend_state_mode_mask = 1, /**< Multiply alpha channel only (RGB values are not used) */
  aviutl2_blend_state_mode_draw = 2, /**< Alpha blend output */
  aviutl2_blend_state_mode_add = 3,  /**< Additive blend output */
};

/**
 * Input image pixel format type
 */
enum aviutl2_input_pixel_format {
  aviutl2_input_pixel_format_rgba = 28, /**< DXGI_FORMAT_R8G8B8A8_UNORM (PIXEL_RGBA) */
  aviutl2_input_pixel_format_bgra = 87, /**< DXGI_FORMAT_B8G8R8A8_UNORM */
  aviutl2_input_pixel_format_bgr = 88,  /**< DXGI_FORMAT_B8G8R8X8_UNORM */
  aviutl2_input_pixel_format_pa64 = 11, /**< DXGI_FORMAT_R16G16B16A16_UNORM */
  aviutl2_input_pixel_format_hf64 = 10, /**< DXGI_FORMAT_R16G16B16A16_FLOAT */
  aviutl2_input_pixel_format_yuy2 = 107, /**< DXGI_FORMAT_YUY2 */
  aviutl2_input_pixel_format_yc48 = 13, /**< DXGI_FORMAT_R16G16B16A16_SNORM (compatibility) */
};

/**
 * Output image pixel format type
 */
enum aviutl2_output_pixel_format {
  aviutl2_output_pixel_format_rgba = 28, /**< DXGI_FORMAT_R8G8B8A8_UNORM (PIXEL_RGBA) */
  aviutl2_output_pixel_format_pa64 = 11, /**< DXGI_FORMAT_R16G16B16A16_UNORM */
  aviutl2_output_pixel_format_hf64 = 10, /**< DXGI_FORMAT_R16G16B16A16_FLOAT */
};

/**
 * Blend mode type
 */
enum aviutl2_blend_mode {
  aviutl2_blend_mode_none = 0,        /**< Normal */
  aviutl2_blend_mode_add = 1,         /**< Add */
  aviutl2_blend_mode_sub = 2,         /**< Subtract */
  aviutl2_blend_mode_mul = 3,         /**< Multiply */
  aviutl2_blend_mode_screen = 4,      /**< Screen */
  aviutl2_blend_mode_overlay = 5,     /**< Overlay */
  aviutl2_blend_mode_light = 6,       /**< Comparison (lighter) */
  aviutl2_blend_mode_dark = 7,        /**< Comparison (darker) */
  aviutl2_blend_mode_brightness = 8,  /**< Brightness */
  aviutl2_blend_mode_chroma = 9,      /**< Chroma */
  aviutl2_blend_mode_shadow = 10,     /**< Shadow */
  aviutl2_blend_mode_light_dark = 11, /**< Light and dark */
  aviutl2_blend_mode_diff = 12,       /**< Difference */
};

/**
 * Billboard type
 */
enum aviutl2_billboard_mode {
  aviutl2_billboard_mode_none = 0,      /**< Standard orientation (do nothing) */
  aviutl2_billboard_mode_side = 1,      /**< Face the camera only horizontally */
  aviutl2_billboard_mode_direction = 2, /**< Face the camera only vertically and horizontally */
  aviutl2_billboard_mode_camera = 3,    /**< Face the camera */
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

  /**
   * Current layer number of the object
   * Note: This is the layer number of the drawing target object
   */
  int layer;

  /**
   * Current target index for multi-object processing
   * Note: For individual objects
   */
  int index;

  /**
   * Number of targets for multi-object processing (1 = single object / 0 = unknown)
   * Note: For individual objects
   */
  int num;

  /**
   * Start frame number of the object in global (scene) coordinates (0 based)
   */
  int frame_s;

  /**
   * End frame number of the object in global (scene) coordinates (0 based)
   */
  int frame_e;
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

/**
 * Object image parameter structure
 */
struct aviutl2_object_image_param {
  float x, y, z;    /**< Base coordinates */
  float rx, ry, rz; /**< Rotation angle (360.0 is one full rotation) */
  float sx, sy, sz; /**< Scale (1.0 = normal size) */
  float cx, cy, cz; /**< Center coordinates (relative to the base coordinates) */
  float alpha;      /**< Opacity (0.0 to 1.0 / 0.0 = transparent / 1.0 = opaque) */
};

/**
 * Object audio parameter structure
 */
struct aviutl2_object_audio_param {
  float vol_l, vol_r; /**< Volume multiplier (1.0 = normal) */
};

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
   * Get current object image data in PIXEL_RGBA format (from VRAM)
   * @param buffer Pointer to image data storage where the image data will be stored
   */
  void (*get_image_data)(struct aviutl2_pixel_rgba *buffer);

  /**
   * Set current object image data in PIXEL_RGBA format (write to VRAM)
   * @param buffer Pointer to image data to write to VRAM (if nullptr, image size is changed with uninitialized data)
   * @param width Image width
   * @param height Image height
   */
  void (*set_image_data)(struct aviutl2_pixel_rgba const *buffer, int width, int height);

  /**
   * Get pointer to current object's D3D image resource (ID3D11Texture2D)
   * @return Pointer to object's ID3D11Texture2D
   *         Valid until current image is changed (set_image_data) or filter processing ends
   */
  struct ID3D11Texture2D *(*get_image_texture2d)(void);

  /**
   * Get pointer to current framebuffer D3D image resource (ID3D11Texture2D)
   * @return Pointer to framebuffer ID3D11Texture2D
   *         Valid until filter processing ends
   */
  struct ID3D11Texture2D *(*get_framebuffer_texture2d)(void);

  /**
   * Edit section interface
   * Read-only functions are available during filter processing
   */
  struct aviutl2_edit_section *edit;

  /**
   * Current image parameter information of the object
   * Parameters can be modified directly
   * Note: These parameters are relative to the image output item parameters (same as script obj.ox and similar)
   */
  struct aviutl2_object_image_param *param;

  /**
   * Get image output item parameters of the specified object
   * @param object Target object handle (specify NULL to target the current object)
   *               Only objects in the scene targeted by current filter processing can be obtained
   * @param offset Time offset to retrieve in seconds (0 for current time)
   * @param param Pointer to parameter storage
   * @param param_size Size of parameter storage. Only that size is retrieved
   * @return false if the parameters cannot be obtained (for example, if a non-image object is specified)
   */
  bool (*get_output_image_param)(aviutl2_object_handle object,
                                 double offset,
                                 struct aviutl2_object_image_param *param,
                                 int param_size);

  /**
   * Get the image object on the specified layer
   * @param layer Target layer number
   * @param offset Time offset to retrieve in seconds (0 for current time)
   * @return Handle of the obtained object (returns NULL if it does not exist)
   */
  aviutl2_object_handle (*get_image_object)(int layer, double offset);

  /**
   * Draw the specified image resource to the framebuffer
   * @param resource Image resource name
   *                 "object" = current object (NULL also selects current object)
   *                 "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                 "tempbuffer" = virtual buffer
   *                 "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                 "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   * @param x Base X coordinate
   * @param y Base Y coordinate
   * @param z Base Z coordinate
   * @param rx Rotation angle around X (360.0 is one full rotation)
   * @param ry Rotation angle around Y (360.0 is one full rotation)
   * @param rz Rotation angle around Z (360.0 is one full rotation)
   * @param sx Scale on X (1.0 = normal size)
   * @param sy Scale on Y (1.0 = normal size)
   * @param sz Scale on Z (1.0 = normal size)
   * @param alpha Opacity (0.0 to 1.0 / 0.0 = transparent / 1.0 = opaque)
   * @return false on failure (for example, if the image resource name is invalid)
   */
  bool (*draw_image)(wchar_t const *resource,
                     float x,
                     float y,
                     float z,
                     float rx,
                     float ry,
                     float rz,
                     float sx,
                     float sy,
                     float sz,
                     float alpha);

  /**
   * Draw a polygon with the specified vertex list to the framebuffer
   * @param vertex_type Vertex list type
   * @param vertex_list Pointer to the vertex data list (pointer to a buffer of the specified vertex type)
   * @param vertex_num Number of vertices in the list
   * @param resource Texture image resource name (used only when textured)
   *                 "object" = current object (NULL also selects current object)
   *                 "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                 "tempbuffer" = virtual buffer
   *                 "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                 "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   * @return false on failure (for example, if the vertex count is invalid)
   */
  bool (*draw_poly)(enum aviutl2_vertex_type vertex_type, void const *vertex_list, int vertex_num, wchar_t const *resource);

  /**
   * Set the default anchor frame
   * Automatically set when func_proc_video() returns true
   * Use this when all object drawing is handled manually with draw_image() and related functions
   * @param width Object width (specify 0 to use a fixed-size anchor frame)
   * @param height Object height (specify 0 to use a fixed-size anchor frame)
   */
  void (*set_default_anchor)(int width, int height);

  /**
   * Set the blend mode for drawing
   * Drawing becomes heavier when a blend mode is used
   * @param blend Blend mode
   */
  void (*set_blend_mode)(enum aviutl2_blend_mode blend);

  /**
   * Set the shininess for drawing
   * Used when the light source settings of camera control are enabled
   * @param shine Shininess (0.0 to 1.0)
   */
  void (*set_material_shine)(float shine);

  /**
   * Set the sampler for drawing
   * @param sampler Sampler type
   */
  void (*set_sampler_mode)(enum aviutl2_sampler_mode sampler);

  /**
   * Set whether back faces are hidden during drawing
   * @param culling true to hide back faces
   */
  void (*set_culling_state)(bool culling);

  /**
   * Set whether the object faces the camera during drawing
   * @param billboard Billboard type
   */
  void (*set_billboard_mode)(enum aviutl2_billboard_mode billboard);

  /**
   * Create an image resource (writes data to VRAM)
   * Image resources can be used for draw_image() and similar drawing
   * If an existing image resource name is specified, the resource is updated
   * @param resource Image resource name to create
   *                 "object" = current object (NULL also selects current object)
   *                 "resource:xxxx" = standard resource (xxxx is an arbitrary name). Released after filter processing
   *                 "tempbuffer" = virtual buffer (internally the same implementation as cache buffer)
   *                 "cache:xxxx" = cache buffer (xxxx is an arbitrary name). Shared with rendering process
   * @param buffer Pointer to image data (PIXEL_RGBA format). If NULL, the resource is created without initial data
   * @param width Image width
   * @param height Image height
   */
  void (*create_image_resource)(wchar_t const *resource, struct aviutl2_pixel_rgba const *buffer, int width, int height);

  /**
   * Get pointer to D3D image resource for the specified image resource (ID3D11Texture2D)
   * @param resource Image resource name
   *                 "object" = current object (NULL also selects current object)
   *                 "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                 "tempbuffer" = virtual buffer
   *                 "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                 "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   *                 "random" = random buffer (256x256 random values in range 0.0 to 1.0, DXGI_FORMAT_R32_FLOAT/r only)
   * @return Pointer to ID3D11Texture2D for the image resource (NULL if resource is not found)
   *         Valid until the image resource changes or filter processing ends
   */
  struct ID3D11Texture2D *(*get_image_resource_texture2d)(wchar_t const *resource);

  /**
   * Copy image resource
   * @param dst_resource Destination image resource name
   *                     If an existing image resource name is specified, the resource is updated
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name). Released after filter processing
   *                     "tempbuffer" = virtual buffer (internally the same implementation as cache buffer)
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name). Shared with rendering process
   * @param src_resource Source image resource name
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                     "framebuffer" = framebuffer
   *                     "tempbuffer" = virtual buffer
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                     "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   *                     "random" = random buffer (256x256 random values in range 0.0 to 1.0, DXGI_FORMAT_R32_FLOAT/r only)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*copy_image_resource)(wchar_t const *dst_resource, wchar_t const *src_resource);

  /**
   * Clear image resource
   * @param resource Image resource name to clear
   *                 "object" = current object (NULL also selects current object)
   *                 "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                 "tempbuffer" = virtual buffer
   *                 "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   * @param color Clear color
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*clear_image_resource)(wchar_t const *resource, struct aviutl2_pixel_rgba color);

  /**
   * Draw specified image resource to destination image resource
   * @param dst_resource Destination image resource name
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                     "tempbuffer" = virtual buffer
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   * @param src_resource Source image resource name
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                     "tempbuffer" = virtual buffer
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                     "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   * @param x Base X coordinate
   * @param y Base Y coordinate
   * @param z Base Z coordinate
   * @param rx Rotation angle around X (360.0 is one full rotation)
   * @param ry Rotation angle around Y (360.0 is one full rotation)
   * @param rz Rotation angle around Z (360.0 is one full rotation)
   * @param sx Scale on X (1.0 = normal size)
   * @param sy Scale on Y (1.0 = normal size)
   * @param sz Scale on Z (1.0 = normal size)
   * @param alpha Opacity (0.0 to 1.0 / 0.0 = transparent / 1.0 = opaque)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*draw_image_to_resource)(wchar_t const *dst_resource,
                                 wchar_t const *src_resource,
                                 float x,
                                 float y,
                                 float z,
                                 float rx,
                                 float ry,
                                 float rz,
                                 float sx,
                                 float sy,
                                 float sz,
                                 float alpha);

  /**
   * Draw polygon with specified vertex list to destination image resource
   * @param dst_resource Destination image resource name
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                     "tempbuffer" = virtual buffer
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   * @param vertex_type Vertex list type
   * @param vertex_list Pointer to the vertex data list (pointer to a buffer of the specified vertex type)
   * @param vertex_num Number of vertices in the list
   * @param src_resource Texture image resource name (used only when textured)
   *                     "object" = current object (NULL also selects current object)
   *                     "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *                     "tempbuffer" = virtual buffer
   *                     "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   *                     "image:xxxx" = image file (xxxx is an image file path). The image is cached in VRAM
   * @return false on failure (for example, if vertex count is invalid)
   */
  bool (*draw_poly_to_resource)(wchar_t const *dst_resource,
                                enum aviutl2_vertex_type vertex_type,
                                void const *vertex_list,
                                int vertex_num,
                                wchar_t const *src_resource);

  /**
   * Execute pixel shader
   * @param cso_file Compiled pixel shader binary file name (file name only)
   *                 Loaded from the same folder as the plugin and cached
   *                 Pixel Shader 5.0 compiled binaries are supported
   * @param target Output target image resource name
   *               Set as Direct3D render target
   *               "object" = current object (NULL also selects current object)
   *               "resource:xxxx" = standard resource (xxxx is an arbitrary name)
   *               "framebuffer" = framebuffer
   *               "tempbuffer" = virtual buffer
   *               "cache:xxxx" = cache buffer (xxxx is an arbitrary name)
   * @param resource_list Pointer to list of referenced image resource names (no binding when NULL)
   *                      Set to Direct3D shader resources (t0-)
   *                      Resource names that can be referenced:
   *                      "object", "resource:xxxx", "tempbuffer", "cache:xxxx", "image:xxxx", "random"
   *                      Cannot use the same resource as render target
   * @param resource_num Number of referenced image resources
   * @param constant Pointer to constant buffer data (no constant buffer when NULL)
   * @param constant_size Constant buffer size
   * @param blend_state Direct3D BlendState (NULL copies output as-is)
   * @param sampler_state Direct3D SamplerState(s0) (no sampler state when NULL)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*exec_pixelshader_file)(wchar_t const *cso_file,
                                wchar_t const *target,
                                wchar_t const **resource_list,
                                int resource_num,
                                void *constant,
                                int constant_size,
                                struct ID3D11BlendState *blend_state,
                                struct ID3D11SamplerState *sampler_state);

  /**
   * Execute compute shader
   * @param cso_file Compiled compute shader binary file name (file name only)
   *                 Loaded from the same folder as the plugin and cached
   *                 Compute Shader 5.0 compiled binaries are supported
   * @param target_list Pointer to list of read/write image resource names (Direct3D unordered access resources u0-)
   *                    Resource names that can be specified:
   *                    "object", "resource:xxxx", "framebuffer", "tempbuffer", "cache:xxxx"
   * @param target_num Number of read/write image resources
   * @param resource_list Pointer to list of referenced image resource names (no binding when NULL)
   *                      Set to Direct3D shader resources (t0-)
   *                      Resource names that can be referenced:
   *                      "object", "resource:xxxx", "tempbuffer", "cache:xxxx", "image:xxxx", "random"
   *                      Cannot use the same resource as unordered access target
   * @param resource_num Number of referenced image resources
   * @param constant Pointer to constant buffer data (no constant buffer when NULL)
   * @param constant_size Constant buffer size
   * @param count_x Thread group count on X axis
   * @param count_y Thread group count on Y axis
   * @param count_z Thread group count on Z axis
   * @param sampler_state Direct3D SamplerState(s0) (no sampler state when NULL)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*exec_computeshader_file)(wchar_t const *cso_file,
                                  wchar_t const **target_list,
                                  int target_num,
                                  wchar_t const **resource_list,
                                  int resource_num,
                                  void *constant,
                                  int constant_size,
                                  int count_x,
                                  int count_y,
                                  int count_z,
                                  struct ID3D11SamplerState *sampler_state);

  /**
   * Get predefined D3D output blend resource pointer (ID3D11BlendState)
   * @param blend Output blend mode type
   * @return Pointer to ID3D11BlendState (NULL if unsupported type)
   */
  struct ID3D11BlendState *(*get_blend_state)(enum aviutl2_blend_state_mode blend);

  /**
   * Get predefined D3D sampler resource pointer (ID3D11SamplerState)
   * @param sampler Sampler type
   * @return Pointer to ID3D11SamplerState (NULL if unsupported type)
   */
  struct ID3D11SamplerState *(*get_sampler_state)(enum aviutl2_sampler_mode sampler);

  /**
   * Execute pixel shader
   * @param data Pointer to compiled pixel shader data (for example, data embedded from a header file)
   * @param data_size Size of compiled pixel shader data
   * @param target Output target image resource name
   * @param resource_list Pointer to list of referenced image resource names (no binding when NULL)
   * @param resource_num Number of referenced image resources
   * @param constant Pointer to constant buffer data (no constant buffer when NULL)
   * @param constant_size Constant buffer size
   * @param blend_state Direct3D BlendState (NULL copies output as-is)
   * @param sampler_state Direct3D SamplerState(s0) (no sampler state when NULL)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*exec_pixelshader_data)(uint8_t const *data,
                                int data_size,
                                wchar_t const *target,
                                wchar_t const **resource_list,
                                int resource_num,
                                void *constant,
                                int constant_size,
                                struct ID3D11BlendState *blend_state,
                                struct ID3D11SamplerState *sampler_state);

  /**
   * Execute compute shader
   * @param data Pointer to compiled compute shader data (for example, data embedded from a header file)
   * @param data_size Size of compiled compute shader data
   * @param target_list Pointer to list of read/write image resource names (Direct3D unordered access resources u0-)
   * @param target_num Number of read/write image resources
   * @param resource_list Pointer to list of referenced image resource names (no binding when NULL)
   * @param resource_num Number of referenced image resources
   * @param constant Pointer to constant buffer data (no constant buffer when NULL)
   * @param constant_size Constant buffer size
   * @param count_x Thread group count on X axis
   * @param count_y Thread group count on Y axis
   * @param count_z Thread group count on Z axis
   * @param sampler_state Direct3D SamplerState(s0) (no sampler state when NULL)
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*exec_computeshader_data)(uint8_t const *data,
                                  int data_size,
                                  wchar_t const **target_list,
                                  int target_num,
                                  wchar_t const **resource_list,
                                  int resource_num,
                                  void *constant,
                                  int constant_size,
                                  int count_x,
                                  int count_y,
                                  int count_z,
                                  struct ID3D11SamplerState *sampler_state);

  /**
   * Get the size of the specified image resource
   * @param resource Image resource name
   * @param width Pointer to storage for image width
   * @param height Pointer to storage for image height
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*get_image_resource_size)(wchar_t const *resource, int *width, int *height);

  /**
   * Get image data in the specified format from an image resource (from VRAM)
   * @param resource Image resource name
   * @param buffer Pointer to image data destination
   * @param width Destination image width
   * @param height Destination image height
   * @param pitch Number of bytes per row in destination image data
   * @param format Pixel format for output image data
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*get_image_resource_data)(wchar_t const *resource,
                                  void *buffer,
                                  int width,
                                  int height,
                                  int pitch,
                                  enum aviutl2_output_pixel_format format);

  /**
   * Set image data in the specified format to an image resource (write to VRAM)
   * If a non-existing image resource name is specified, a new resource is created
   * @param resource Image resource name to create/update
   * @param buffer Pointer to image data
   * @param width Image width
   * @param height Image height
   * @param pitch Number of bytes per row in image data
   * @param format Pixel format of input image data
   * @return false on failure (for example, if an image resource name is invalid)
   */
  bool (*set_image_resource_data)(wchar_t const *resource,
                                  void const *buffer,
                                  int width,
                                  int height,
                                  int pitch,
                                  enum aviutl2_input_pixel_format format);

  /**
   * Deprecated because this is redundant
   * Moved to aviutl2_edit_section
   */
  struct IDWriteFont *(*deprecated_get_font)(wchar_t const *font);
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
  void (*set_sample_data)(float const *buffer, int channel);

  /**
   * Edit section interface
   * Read-only functions are available during filter processing
   */
  struct aviutl2_edit_section *edit;

  /**
   * Current audio parameter information of the object
   * Parameters can be modified directly
   * Note: These parameters are relative to the audio output item parameters
   */
  struct aviutl2_object_audio_param *param;

  /**
   * Get audio output item parameters of the specified object
   * @param object Target object handle (specify NULL to target the current object)
   *               Only objects in the scene targeted by current filter processing can be obtained
   * @param offset Time offset to retrieve in seconds (0 for current time)
   * @param param Pointer to parameter storage
   * @param param_size Size of parameter storage. Only that size is retrieved
   * @return false if the parameters cannot be obtained (for example, if a non-audio object is specified)
   */
  bool (*get_output_audio_param)(aviutl2_object_handle object,
                                 double offset,
                                 struct aviutl2_object_audio_param *param,
                                 int param_size);

  /**
   * Get the audio object at the specified layer position
   * @param layer Target layer number
   * @param offset Time offset to retrieve in seconds (0 for current time)
   * @return Handle of the obtained object (returns NULL if it does not exist)
   */
  aviutl2_object_handle (*get_audio_object)(int layer, double offset);
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
   * @return Returning false aborts subsequent filter and output processing
   */
  bool (*func_proc_video)(struct aviutl2_filter_proc_video *video);

  /**
   * Audio filter function pointer
   * Called only if FLAG_AUDIO is set
   * @param audio Pointer to audio filter processing parameters
   * @return Returning false aborts subsequent filter and output processing
   */
  bool (*func_proc_audio)(struct aviutl2_filter_proc_audio *audio);
};
