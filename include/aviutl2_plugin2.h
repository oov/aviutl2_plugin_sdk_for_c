// https://github.com/oov/aviutl2_plugin_sdk_for_c
// The MIT License / Copyright (c) 2025 oov
#pragma once

// This file based on plugin2.h from AviUtl ExEdit2 Plugin SDK
// LICENSE: The MIT License / Copyright (c) 2025 Kenkun

// Plugins must export the following functions for the host to call:
//
// Optional function to return a pointer to the common plugin structure
//   struct aviutl2_common_plugin_table *GetCommonPluginTable(void)
//
// Required plugin registration function
//   void RegisterPlugin(struct aviutl2_host_app_table *host)
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

struct aviutl2_input_plugin_table;
struct aviutl2_output_plugin_table;
struct aviutl2_filter_plugin_table;
struct aviutl2_script_module_table;
struct aviutl2_edit_handle;
struct aviutl2_project_file;
struct aviutl2_pixel_rgba;
struct IDWriteFontCollection;
struct IDWriteFont;

/**
 * Common plugin structure
 */
struct aviutl2_common_plugin_table {
  wchar_t const *name;        /**< Plugin name */
  wchar_t const *information; /**< Plugin information */
};

//--------------------------------

/**
 * Object handle
 */
#ifndef AVIUTL2_OBJECT_HANDLE_DEFINED
#define AVIUTL2_OBJECT_HANDLE_DEFINED
typedef void *aviutl2_object_handle;
#endif

/**
 * Effect handle
 */
#ifndef AVIUTL2_EFFECT_HANDLE_DEFINED
#define AVIUTL2_EFFECT_HANDLE_DEFINED
typedef void *aviutl2_effect_handle;
#endif

/**
 * Object layer and frame information
 * In object frames, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_object_layer_frame {
  int layer; /**< Layer number */
  int start; /**< Start frame number */
  int end;   /**< End frame number */
};

/**
 * Media information
 */
struct aviutl2_media_info {
  int video_track_num; /**< Video track count (0 if no video) */
  int audio_track_num; /**< Audio track count (0 if no audio) */
  double total_time;   /**< Total time (0 for still images) */
  int width, height;   /**< Resolution */
};

/**
 * Module type constants
 */
enum {
  aviutl2_module_type_script_filter = 1, /**< Filter script */
  aviutl2_module_type_script_object = 2, /**< Object script */
  aviutl2_module_type_script_camera = 3, /**< Camera script */
  aviutl2_module_type_script_track = 4,  /**< Track bar script */
  aviutl2_module_type_script_module = 5, /**< Script module */
  aviutl2_module_type_plugin_input = 6,  /**< Input plugin */
  aviutl2_module_type_plugin_output = 7, /**< Output plugin */
  aviutl2_module_type_plugin_filter = 8, /**< Filter plugin */
  aviutl2_module_type_plugin_common = 9, /**< Common plugin */
};

/**
 * Module information
 */
struct aviutl2_module_info {
  int type;                    /**< Module type (aviutl2_module_type_*) */
  wchar_t const *name;         /**< Module name */
  wchar_t const *information;  /**< Module information */
};

/**
 * Track bar information
 */
struct aviutl2_track_info {
  wchar_t const *mode; /**< Name of track bar movement mode (NULL if no movement) */
  double *param;       /**< Pointer to array of track bar parameter values (NULL if no parameter values) */
  int param_num;       /**< Number of track bar parameter values */
  bool accelerate;     /**< Whether acceleration is enabled */
  bool decelerate;     /**< Whether deceleration is enabled */
  bool twopoint;       /**< Whether midpoint ignore is enabled */
  bool timecontrol;    /**< Whether time control is enabled */
  int group_num;       /**< Number of track bars in the belonging group (1 if not grouped) */
  int group_index;     /**< Index within the belonging group */
  wchar_t const *group_name; /**< Name of the belonging group (NULL if not grouped) */
};

/**
 * Number of palette colors
 */
enum {
  aviutl2_palette_info_palette_num = 64,
};

/**
 * Palette information
 */
struct aviutl2_palette_info {
  struct {
    uint8_t r, g, b, a; /**< Palette color (a is always 255) */
  } color[aviutl2_palette_info_palette_num];
};

/**
 * BPM information
 */
struct aviutl2_bpm_info {
  float tempo;  /**< Tempo */
  int beat;     /**< Beat */
  double start; /**< Start position (seconds) */
  float offset; /**< Beat offset (seconds) */
};

/**
 * Event type
 */
enum aviutl2_event_type {
  aviutl2_event_type_update_object = 1,       /**< Object information update */
  aviutl2_event_type_change_edit_frame = 2,   /**< Current edit frame moved */
  aviutl2_event_type_change_edit_scene = 3,   /**< Current edit scene changed (includes scene info update) */
  aviutl2_event_type_change_focus_object = 4, /**< Selected object changed */
};

//--------------------------------

/**
 * Edit information
 * In edit info, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_edit_info {
  int width, height;        /**< Scene resolution */
  int rate, scale;          /**< Scene frame rate */
  int sample_rate;          /**< Scene sampling rate */
  int frame;                /**< Current cursor frame number */
  int layer;                /**< Current selected layer number */
  int frame_max;            /**< Maximum frame number where objects exist */
  int layer_max;            /**< Maximum layer number where objects exist */
  int display_frame_start;  /**< Start frame number displayed in layer editor */
  int display_layer_start;  /**< Start layer number displayed in layer editor */
  int display_frame_num;    /**< Number of frames displayed in layer editor (not exact) */
  int display_layer_num;    /**< Number of layers displayed in layer editor (not exact) */
  int select_range_start;   /**< Start frame number of frame range selection (-1 if not selected) */
  int select_range_end;     /**< End frame number of frame range selection (-1 if not selected) */
  float grid_bpm_tempo;     /**< Grid(BPM) tempo (first BPM entry) */
  int grid_bpm_beat;        /**< Grid(BPM) beat (first BPM entry) */
  float grid_bpm_offset;    /**< Grid(BPM) beat offset (first BPM entry) */
  int scene_id;             /**< Scene ID */
};

/**
 * Edit section
 * In edit section, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_edit_section {
  /**
   * Edit information (not available with call_read_section)
   */
  struct aviutl2_edit_info *info;

  /**
   * Create object alias at specified position (not available with call_read_section)
   * @param alias Pointer to object alias data (UTF-8). Same format as object alias file (.object)
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If frame information exists in alias data, length is set from that frame information.
   *               If 0 is specified for frame count, length or add position are auto-adjusted
   * @return Handle of created object (returns NULL on failure). Fails if overlapping with existing object or if alias
   * data is invalid. For multi-object alias data, returns handle of the first object. All objects are created.
   */
  aviutl2_object_handle (*create_object_from_alias)(char const *alias, int layer, int frame, int length);

  /**
   * Find object at specified frame number or later
   * If called from a filter plugin, searches objects in the target scene being processed
   * @param layer Target layer number
   * @param frame Frame number to start search from
   * @return Handle of found object (returns NULL if not found)
   */
  aviutl2_object_handle (*find_object)(int layer, int frame);

  /**
   * Get number of target effects in object
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @return Number of target effects (0 if not found)
   */
  int (*count_object_effect)(aviutl2_object_handle object, wchar_t const *effect);

  /**
   * Get object layer and frame information
   * @param object Object handle
   * @return Object layer and frame information
   */
  struct aviutl2_object_layer_frame (*get_object_layer_frame)(aviutl2_object_handle object);

  /**
   * Get object alias data
   * @param object Object handle
   * @return Pointer to object alias data (UTF-8) (returns NULL if cannot be obtained).
   *         Same format as object alias file.
   *         Returned string is valid until the next call to a function returning a string in the same thread,
   *         or until callback processing ends
   */
  char const *(*get_object_alias)(aviutl2_object_handle object);

  /**
   * Get object configuration item value as string
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @param item Target configuration item name (key name in alias file)
   * @return Pointer to obtained configuration value (UTF-8) (returns NULL if cannot be obtained).
   *         Same format as configuration value in alias file.
   *         Returned string is valid until the next call to a function returning a string in the same thread,
   *         or until callback processing ends
   */
  char const *(*get_object_item_value)(aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item);

  /**
   * Set object configuration item value as string (not available with call_read_section)
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @param item Target configuration item name (key name in alias file)
   * @param value Configuration value (UTF-8). Same format as configuration value in alias file
   * @return true if configuration succeeded (fails if target does not exist)
   */
  bool (*set_object_item_value)(aviutl2_object_handle object,
                                wchar_t const *effect,
                                wchar_t const *item,
                                char const *value);

  /**
   * Move object (not available with call_read_section)
   * @param object Object handle
   * @param layer Destination layer number
   * @param frame Destination frame number
   * @return true if move succeeded (fails if object exists at destination)
   */
  bool (*move_object)(aviutl2_object_handle object, int layer, int frame);

  /**
   * Delete object (not available with call_read_section)
   * Do not delete objects created within the same edit section
   * @param object Object handle
   */
  void (*delete_object)(aviutl2_object_handle object);

  /**
   * Get handle of currently selected object in object configuration window
   * @return Object handle (returns NULL if no selection)
   */
  aviutl2_object_handle (*get_focus_object)(void);

  /**
   * Set object to be selected in object configuration window (not available with call_read_section)
   * Set at end of callback processing
   * @param object Object handle (specifying NULL clears the selection)
   */
  void (*set_focus_object)(aviutl2_object_handle object);

  /**
   * Get pointer to project file (not available with call_read_section)
   * @param edit Edit handle
   * @return Pointer to project file structure. Valid until callback processing ends
   */
  struct aviutl2_project_file *(*get_project_file)(struct aviutl2_edit_handle *edit);

  /**
   * Get handle of selected object in layer editor by index
   * @param index Selection index (0 based)
   * @return Object handle (NULL if index is out of range)
   */
  aviutl2_object_handle (*get_selected_object)(int index);

  /**
   * Get number of selected objects in layer editor
   * @return Number of selected objects
   */
  int (*get_selected_object_num)(void);

  /**
   * Get layer and frame position from mouse coordinates (not available with call_read_section)
   * Calculates from coordinates of the last mouse move window message
   * When obtained inside the callback function during file D&D, this becomes the drop position
   * @param layer Pointer to store layer number
   * @param frame Pointer to store frame number
   * @return true if mouse coordinates are on layer editor
   */
  bool (*get_mouse_layer_frame)(int *layer, int *frame);

  /**
   * Get layer and frame position from specified screen coordinates (not available with call_read_section)
   * @param x Screen X coordinate
   * @param y Screen Y coordinate
   * @param layer Pointer to store layer number
   * @param frame Pointer to store frame number
   * @return true if screen coordinates are on layer editor
   */
  bool (*pos_to_layer_frame)(int x, int y, int *layer, int *frame);

  /**
   * Check if specified media file is supported
   * @param file Media file path
   * @param strict If true, check if file can actually be loaded. If false, check if extension is supported
   * @return true if supported
   */
  bool (*is_support_media_file)(wchar_t const *file, bool strict);

  /**
   * Get information of specified media file
   * Cannot get info for files other than video, audio, and image files
   * @param file Media file path
   * @param info Pointer to media info storage
   * @param info_size Size of media info storage (only size bytes are retrieved if different from MEDIA_INFO)
   * @return true if info was obtained
   */
  bool (*get_media_info)(wchar_t const *file, struct aviutl2_media_info *info, int info_size);

  /**
   * Create object from media file at specified position (not available with call_read_section)
   * @param file Media file path
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If 0 is specified, length or position are auto-adjusted
   * @return Handle of created object (returns NULL on failure)
   *         Fails if overlapping with existing object or if media file is not supported
   */
  aviutl2_object_handle (*create_object_from_media_file)(wchar_t const *file, int layer, int frame, int length);

  /**
   * Create object at specified position (not available with call_read_section)
   * @param effect Effect name (effect.name value in alias file)
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If 0 is specified, length or position are auto-adjusted
   * @return Handle of created object (returns NULL on failure)
   *         Fails if overlapping with existing object or if specified effect is not supported
   */
  aviutl2_object_handle (*create_object)(wchar_t const *effect, int layer, int frame, int length);

  /**
   * Set current layer/frame position (not available with call_read_section)
   * Adjusted to available range
   * @param layer Layer number
   * @param frame Frame number
   */
  void (*set_cursor_layer_frame)(int layer, int frame);

  /**
   * Set display start position of layer/frame in layer editor (not available with call_read_section)
   * Adjusted to available range
   * @param layer Display start layer number
   * @param frame Display start frame number
   */
  void (*set_display_layer_frame)(int layer, int frame);

  /**
   * Set frame range selection (not available with call_read_section)
   * Adjusted to available range
   * @param start Start frame number
   * @param end End frame number. Specifying -1 for both start and end clears the selection
   */
  void (*set_select_range)(int start, int end);

  /**
   * Set grid(BPM) (not available with call_read_section)
   * @param tempo Tempo
   * @param beat Beat
   * @param offset Base time
   */
  void (*set_grid_bpm)(float tempo, int beat, float offset);

  /**
   * Get object name
   * @param object Object handle
   * @return Pointer to object name (returns NULL if using standard name).
   *         Valid until object is edited or callback processing ends
   */
  wchar_t const *(*get_object_name)(aviutl2_object_handle object);

  /**
   * Set object name (not available with call_read_section)
   * @param object Object handle
   * @param name Object name (specifying NULL or empty string sets to standard name)
   */
  void (*set_object_name)(aviutl2_object_handle object, wchar_t const *name);

  /**
   * Get layer name
   * @param layer Layer number
   * @return Pointer to layer name (returns NULL if using standard name).
   *         Valid until layer is edited or callback processing ends
   */
  wchar_t const *(*get_layer_name)(int layer);

  /**
   * Set layer name (not available with call_read_section)
   * @param layer Layer number
   * @param name Layer name (specifying NULL or empty string sets to standard name)
   */
  void (*set_layer_name)(int layer, wchar_t const *name);

  /**
   * Get scene name
   * @return Pointer to scene name.
   *         Valid until scene is edited or callback processing ends
   */
  wchar_t const *(*get_scene_name)(void);

  /**
   * Set scene name (not available with call_read_section)
   * Scene operations currently do not support Undo
   * @param name Scene name.
   *             Scene name is required (does not change if NULL or empty string)
   */
  void (*set_scene_name)(wchar_t const *name);

  /**
   * Set scene resolution (not available with call_read_section)
   * Scene operations currently do not support Undo
   * @param width Width
   * @param height Height
   */
  void (*set_scene_size)(int width, int height);

  /**
   * Set scene frame rate (not available with call_read_section)
   * Scene operations currently do not support Undo
   * @param rate Frame rate
   * @param scale Frame rate scale
   */
  void (*set_scene_frame_rate)(int rate, int scale);

  /**
   * Set scene sampling rate (not available with call_read_section)
   * Scene operations currently do not support Undo
   * @param sample_rate Sampling rate
   */
  void (*set_scene_sample_rate)(int sample_rate);

  /**
   * Get layer visible/hidden state
   * @param layer Layer number
   * @return true if the layer is visible
   */
  bool (*get_layer_enable)(int layer);

  /**
   * Set layer visible/hidden state (not available with call_read_section)
   * @param layer Layer number
   * @param enable Layer visibility state to set
   */
  void (*set_layer_enable)(int layer, bool enable);

  /**
   * Get layer lock state
   * @param layer Layer number
   * @return true if the layer is locked
   */
  bool (*get_layer_lock)(int layer);

  /**
   * Set layer lock state (not available with call_read_section)
   * @param layer Layer number
   * @param lock Layer lock state to set
   */
  void (*set_layer_lock)(int layer, bool lock);

  /**
   * Get the number of sections in an object
   * @param object Object handle
   * @return Number of sections
   */
  int (*get_object_section_num)(aviutl2_object_handle object);

  /**
   * Get the position of the currently selected object section
   * @return Section number (-1 if nothing is selected)
   */
  int (*get_focus_object_section)(void);

  /**
   * Get the start frame number of an object section
   * @param object Object handle
   * @param section Section number
   * @return Start frame number of section (-1 if not available)
   */
  int (*get_object_section_frame)(aviutl2_object_handle object, int section);

  /**
   * Get the value of an object's track bar item at the specified frame position
   * If called from a filter plugin, only objects in the target scene being processed can be obtained
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   *               If there are multiple effects with the same name, you can specify an index with ":n" suffix
   *               (n is a zero-based index)
   * @param item Target track bar item name (key name in alias file)
   * @param frame Target frame number to retrieve (fractional part can specify an in-between frame position)
   * @param value Pointer to storage for the track bar item value
   * @return true if value was obtained (fails if target is not found)
   */
  bool (*get_object_track_value)(
      aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item, double frame, double *value);

  /**
   * Get the value of an object's check box item (including per-section check boxes) at the specified frame position
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   *               If there are multiple effects with the same name, you can specify an index with ":n" suffix
   *               (n is a zero-based index)
   * @param item Target check box item name (key name in alias file)
   * @param frame Target frame number to retrieve (used for per-section check boxes)
   * @param value Pointer to storage for the check box item value
   * @return true if value was obtained (fails if target is not found)
   */
  bool (*get_object_check_value)(
      aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item, int frame, bool *value);

  /**
   * Get information about an object's track bar item
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   *               If there are multiple effects with the same name, you can specify an index with ":n" suffix
   *               (n is a zero-based index)
   * @param item Target track bar item name (key name in alias file)
   * @param info Pointer to storage for track bar information
   * @param info_size Size of track bar information storage (if different from aviutl2_track_info, only info_size
   *                  bytes are obtained)
   * @return true if info was obtained (fails if target is not found)
   */
  bool (*get_object_track_info)(aviutl2_object_handle object,
                                wchar_t const *effect,
                                wchar_t const *item,
                                struct aviutl2_track_info *info,
                                int info_size);

  /**
   * Get current palette name
   * Format is [label_name.palette_name] when labeled
   * @return Current palette name. Valid until callback processing ends
   */
  wchar_t const *(*get_palette_name)(void);

  /**
   * Get information of specified palette
   * @param name Palette name
   * @param info Pointer to palette info storage
   * @param info_size Size of palette info storage (only size bytes are retrieved if different from PALETTE_INFO)
   * @return true if info was obtained (fails if target is not found)
   */
  bool (*get_palette_info)(wchar_t const *name, struct aviutl2_palette_info *info, int info_size);

  /**
   * Get a pointer to the DirectWrite font of a registered font (IDWriteFont)
   * @param font Font name (registered name in the application)
   * @return Pointer to IDWriteFont (returns NULL if the specified font does not exist)
   */
  struct IDWriteFont *(*get_font)(wchar_t const *font);

  /**
   * Get names of items belonging to an object's track bar group
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   *               If there are multiple effects with the same name, you can specify an index with ":n" suffix
   *               (n is a zero-based index)
   * @param group_name Target track bar group item name (key name in alias file)
   * @param item_names Pointer to storage for belonging item names
   * @param item_num Number of item names storage entries
   * @return Number of belonging item names that were obtained (returns 0 if the specified group does not exist)
   *         If item_names is NULL, returns the number of belonging items
   */
  int (*get_object_track_group_names)(
      aviutl2_object_handle object, wchar_t const *effect, wchar_t const *group_name, wchar_t const **item_names, int item_num);

  /**
   * Deprecated because it has been replaced by a new function
   */
  int (*deprecated_get_grid_bpm_list)(struct aviutl2_bpm_info *bpm_list, int bpm_num);

  /**
   * Deprecated because it has been replaced by a new function
   */
  void (*deprecated_set_grid_bpm_list)(struct aviutl2_bpm_info *bpm_list, int bpm_num);

  /**
   * Find effect from object
   * @param object Object handle to search
   * @param effect Effect name to search (effect.name value in alias file)
   *               If there are multiple effects with the same name, you can specify an index with ":n" suffix
   *               (n is a zero-based index)
   *               If NULL is specified, the first effect is returned
   * @return Handle of found effect (returns NULL if not found)
   *         Effect handle is valid until effect is destroyed or callback processing ends
   */
  aviutl2_effect_handle (*find_effect)(aviutl2_object_handle object, wchar_t const *effect);

  /**
   * Get list of effects from object
   * @param object Object handle
   * @param effect_list Pointer to storage for effect handle list
   * @param effect_num Number of effect handles that can be stored
   * @return Number of effect handles obtained (returns 0 if unavailable)
   *         If effect_list is NULL, returns the number of effects owned by the object
   *         Effect handles are valid until effect is destroyed or callback processing ends
   */
  int (*get_effect_list)(aviutl2_object_handle object, aviutl2_effect_handle *effect_list, int effect_num);

  /**
   * Get effect name
   * @param effect Effect handle
   * @return Pointer to effect name (returns NULL if unavailable)
   */
  wchar_t const *(*get_effect_name)(aviutl2_effect_handle effect);

  /**
   * Get effect enabled state
   * @param effect Effect handle
   * @return true if the effect is enabled
   */
  bool (*get_effect_enable)(aviutl2_effect_handle effect);

  /**
   * Set effect enabled state (not available with call_read_section)
   * @param effect Effect handle
   * @param enable Effect enabled state to set
   *               If the effect is an output item (standard drawing, etc.), it cannot be changed
   *               (always enabled)
   */
  void (*set_effect_enable)(aviutl2_effect_handle effect, bool enable);

  /**
   * Get effect lock state
   * @param effect Effect handle
   * @return true if the effect is locked
   */
  bool (*get_effect_lock)(aviutl2_effect_handle effect);

  /**
   * Set effect lock state (not available with call_read_section)
   * @param effect Effect handle
   * @param lock Effect lock state to set
   *             If the effect is audio, it cannot be changed
   *             If the effect is an output item (standard drawing, etc.), it cannot be changed
   *             (synchronized with input item)
   */
  void (*set_effect_lock)(aviutl2_effect_handle effect, bool lock);

  /**
   * Get effect setting item value as string
   * @param effect Effect handle
   * @param item Target setting item name (key name in alias file)
   * @return Pointer to obtained setting value (UTF-8) (returns NULL if unavailable)
   *         Same format as setting value in alias file
   *         Returned string is valid until the next call to a function returning a string in the same thread,
   *         or until callback processing ends
   */
  char const *(*get_effect_item_value)(aviutl2_effect_handle effect, wchar_t const *item);

  /**
   * Set effect setting item value as string (not available with call_read_section)
   * @param effect Effect handle
   * @param item Target setting item name (key name in alias file)
   * @param value Setting value (UTF-8), same format as setting value in alias file
   * @return true if setting succeeded (fails if target is not found)
   */
  bool (*set_effect_item_value)(aviutl2_effect_handle effect, wchar_t const *item, char const *value);

  /**
   * Get value of an effect's track bar item at specified frame position
   * If called from a filter plugin, only objects in the target scene being processed can be retrieved
   * @param effect Effect handle
   * @param item Target track bar item name (key name in alias file)
   * @param frame Target frame number to retrieve (fractional part can specify in-between frame position)
   * @param value Pointer to storage for track bar item value
   * @return true if value was obtained (fails if target is not found)
   */
  bool (*get_effect_track_value)(aviutl2_effect_handle effect, wchar_t const *item, double frame, double *value);

  /**
   * Get value of an effect's check box item (including per-section check boxes) at specified frame position
   * @param effect Effect handle
   * @param item Target check box item name (key name in alias file)
   * @param frame Target frame number to retrieve (used for per-section check boxes)
   * @param value Pointer to storage for check box item value
   * @return true if value was obtained (fails if target is not found)
   */
  bool (*get_effect_check_value)(aviutl2_effect_handle effect, wchar_t const *item, int frame, bool *value);

  /**
   * Get information about an effect's track bar item
   * @param effect Effect handle
   * @param item Target track bar item name (key name in alias file)
   * @param info Pointer to storage for track bar information
   * @param info_size Size of track bar information storage
   *                  (if different from aviutl2_track_info, only info_size bytes are obtained)
   * @return true if info was obtained (fails if target is not found)
   */
  bool (*get_effect_track_info)(aviutl2_effect_handle effect,
                                wchar_t const *item,
                                struct aviutl2_track_info *info,
                                int info_size);

  /**
   * Get list of BPM entries used by Grid(BPM)
   * @param bpm_list Pointer to storage for BPM list
   * @param bpm_num Number of BPM entries that can be stored
   * @param bpm_size Size of BPM information structure
   *                 (if different from aviutl2_bpm_info, only bpm_size bytes are obtained)
   * @return Number of BPM entries obtained
   *         If bpm_list is NULL, returns the number of BPM entries configured in Grid(BPM)
   */
  int (*get_grid_bpm_list)(struct aviutl2_bpm_info *bpm_list, int bpm_num, int bpm_size);

  /**
   * Set list of BPM entries used by Grid(BPM) (not available with call_read_section)
   * @param bpm_list Pointer to BPM list to set
   * @param bpm_num Number of BPM entries to set
   * @param bpm_size Size of BPM information structure
   *                 (if different from aviutl2_bpm_info, only bpm_size bytes are set)
   */
  void (*set_grid_bpm_list)(struct aviutl2_bpm_info *bpm_list, int bpm_num, int bpm_size);

  /**
   * Add an effect to an object (not available with call_read_section)
   * @param object Object handle to add the effect to
   * @param effect Effect name to add (effect.name value in alias file)
   *         If the effect is an input/output item (such as Figure or Standard Drawing), it is replaced
   * @return Handle of the added effect (returns NULL if it cannot be added)
   *         The effect handle is valid until the effect is destroyed or callback processing ends
   */
  aviutl2_effect_handle (*create_effect)(aviutl2_object_handle object, wchar_t const *effect);

  /**
   * Delete an effect from an object (not available with call_read_section)
   * @param object Object handle to delete the effect from
   * @param effect Effect handle to delete
   * @return true if deletion succeeded
   */
  bool (*delete_effect)(aviutl2_object_handle object, aviutl2_effect_handle effect);

  /**
   * Add an intermediate point (section) to an object (not available with call_read_section)
   * @param object Object handle to add the intermediate point to
   * @param frame Frame number at which to add the intermediate point
   * @return true if addition succeeded
   */
  bool (*create_object_section)(aviutl2_object_handle object, int frame);

  /**
   * Delete an intermediate point (section) from an object (not available with call_read_section)
   * @param object Object handle to delete the intermediate point from
   * @param section Section number of the intermediate point to delete (section number at the start position)
   * @return true if deletion succeeded
   */
  bool (*delete_object_section)(aviutl2_object_handle object, int section);

  /**
   * Move the start position of an object's section (not available with call_read_section)
   * Note: If the section number to move (section) is the number of sections (last section + 1), the end point is moved
   * @param object Object handle whose section is to be moved
   * @param section Section number of the section to move (0 to the number of sections)
   * @param frame Destination frame number. Moving across sections is not possible
   * @return true if movement succeeded
   */
  bool (*move_object_section)(aviutl2_object_handle object, int section, int frame);

  /**
   * Move the order of an effect (not available with call_read_section)
   * Note: The order can be moved when the effect type is a filter effect
   * @param object Object handle whose effect order is to be moved
   * @param effect Effect handle whose order is to be moved
   * @param index Destination order index
   * @return Order index after movement (returns -1 if the target is not found)
   */
  int (*move_effect)(aviutl2_object_handle object, aviutl2_effect_handle effect, int index);

  /**
   * Get the value of an effect's generic data item
   * @param effect Effect handle
   * @param item Target setting item name (key name in alias file)
   * @param data Pointer to storage for generic data
   * @param size Size of generic data storage. If different from the actual size, only size bytes are obtained
   * @return Size of generic data obtained (returns 0 if unavailable)
   *         If data is NULL, returns the size of the generic data
   */
  int (*get_effect_data_value)(aviutl2_effect_handle effect,
                               wchar_t const *item,
                               void *data,
                               int size);

  /**
   * Set the value of an effect's generic data item (not available with call_read_section)
   * @param effect Effect handle
   * @param item Target setting item name (key name in alias file)
   * @param data Pointer to generic data to set
   * @param size Size of generic data to set
   * @return true if setting succeeded (fails if the target is not found)
   */
  bool (*set_effect_data_value)(aviutl2_effect_handle effect,
                                wchar_t const *item,
                                void *data,
                                int size);

  /**
   * Set edit data to the edited state
   * Note: Normally set automatically
   */
  void (*set_edited_state)(void);

  /**
   * Get the list of marked frames
   * @param frame_list Pointer to storage for the list of frame numbers
   * @param frame_num Number of entries in the frame number list storage
   * @return Number of frame numbers obtained
   *         If frame_list is NULL, returns the number of marked frames
   */
  int (*get_mark_frame_list)(int *frame_list, int frame_num);

  /**
   * Get the memo of the mark at the specified frame
   * @param frame Frame number whose mark memo is to be retrieved
   * @return Pointer to the mark memo (returns NULL if unavailable)
   *         Valid until the mark is edited or callback processing ends
   */
  wchar_t const *(*get_mark_frame_memo)(int frame);

  /**
   * Mark the specified frame (not available with call_read_section)
   * If the frame is already marked, its memo is updated
   * @param frame Frame number to mark
   * @param memo Mark memo (specifying NULL sets an empty memo)
   */
  void (*set_mark_frame)(int frame, wchar_t const *memo);

  /**
   * Clear the mark at the specified frame (not available with call_read_section)
   * @param frame Frame number whose mark is to be cleared
   */
  void (*clear_mark_frame)(int frame);

  /**
   * Move the mark at the specified frame (not available with call_read_section)
   * @param frame Frame number whose mark is to be moved
   * @param frame_to Destination frame number for the mark
   * @return true if movement succeeded (fails if the source is not marked or the destination is already marked)
   */
  bool (*move_mark_frame)(int frame, int frame_to);

  /**
   * Set the information of the specified palette (not available with call_read_section)
   * Note: Saves and reloads the palette file
   * @param name Palette name
   * @param info Pointer to palette information
   * @param info_size Size of palette information (size of PALETTE_INFO)
   * @return true if setting succeeded (fails if the target is not found)
   */
  bool (*set_palette_info)(wchar_t const *name,
                           struct aviutl2_palette_info *info,
                           int info_size);
};

/**
 * Edit handle
 * Cannot be used from within RegisterPlugin processing except for get_host_app_window()
 */
struct aviutl2_edit_handle {
  /**
   * Call callback function (func_proc_edit) to edit project data
   * Edits within callback function are made under update lock state for exclusive control
   * Objects edited in callback function are automatically registered to Undo
   * Callback function is called from the main thread
   * @param func_proc_edit Callback function for editing
   * @return true on success. Fails if edit is not available (during output)
   */
  bool (*call_edit_section)(void (*func_proc_edit)(struct aviutl2_edit_section *edit));

  /**
   * Call callback function (func_proc_edit) to edit project data with parameter
   * @param param Pointer to arbitrary user data
   * @param func_proc_edit Callback function for editing
   * @return true on success. Fails if edit is not available (during output)
   */
  bool (*call_edit_section_param)(void *param, void (*func_proc_edit)(void *param, struct aviutl2_edit_section *edit));

  /**
   * Get edit information
   * Acquires read lock for exclusive control of edit information.
   * If already locked in the same thread, retrieves without additional locking.
   * @param info Pointer to edit info storage
   * @param info_size Size of edit info storage (only size bytes are retrieved if different from EDIT_INFO)
   */
  void (*get_edit_info)(struct aviutl2_edit_info *info, int info_size);

  /**
   * Restart the host application
   */
  void (*restart_host_app)(void);

  /**
   * Enumerate effect names via callback function (func_proc_enum_effect)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_effect Callback function for effect name enumeration
   */
  void (*enum_effect_name)(void *param,
                           void (*func_proc_enum_effect)(void *param, wchar_t const *name, int type, int flag));

  /**
   * Enumerate module information via callback function (func_proc_enum_module)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_module Callback function for module information enumeration
   */
  void (*enum_module_info)(void *param,
                           void (*func_proc_enum_module)(void *param, struct aviutl2_module_info *info));

  /**
   * Get the main window handle of the host application
   * @return Main window handle
   */
  HWND (*get_host_app_window)(void);

  /**
   * Get the edit state
   * @return Edit state (aviutl2_edit_state_*)
   */
  int (*get_edit_state)(void);

  /**
   * Call callback function (func_proc_read_section) to read project data
   * During read, data is in read-lock state so that data is not updated, and processing is done within the callback function
   * Update functions of EDIT_SECTION etc. cannot be used. See notes on each item of EDIT_SECTION
   * Callback function is called from the same thread as the caller
   * @param func_proc_read_section Callback function
   * @return true on success. Fails if read is not available (during output, etc.)
   */
  bool (*call_read_section)(void (*func_proc_read_section)(struct aviutl2_edit_section *edit));

  /**
   * call_read_section() with param argument
   * @param param Pointer to arbitrary user data
   * @param func_proc_read_section Callback function
   * @return true on success. Fails if read is not available (during output, etc.)
   */
  bool (*call_read_section_param)(void *param,
                                  void (*func_proc_read_section)(void *param, struct aviutl2_edit_section *edit));

  /**
   * Enumerate effect setting items via callback function (func_proc_enum_effect_item)
   * @param effect Target effect name (effect.name value in alias file)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_effect_item Callback function for effect setting item enumeration
   * @return true if obtained. Fails if target is not found
   */
  bool (*enum_effect_item)(wchar_t const *effect,
                           void *param,
                           void (*func_proc_enum_effect_item)(void *param, wchar_t const *name, int type));

  /**
   * Render the video of the current scene
   * This function only enqueues a rendering task and returns immediately
   * The callback function is called from the event notification thread after rendering is completed
   * @param frame Frame number to render
   * @param param Pointer to arbitrary user data
   * @param func_proc_rendering_video Callback function called on completion
   *        buffer: Pointer to rendered image data (PIXEL_RGBA format)
   *        width,height: Rendered image size
   *        pitch: Number of bytes per row in rendered image data
   * @return true if rendering request succeeds (fails during output and similar states)
   */
  bool (*rendering_scene_video)(int frame,
                                void *param,
                                void (*func_proc_rendering_video)(void *param,
                                                                   int frame,
                                                                   void const *buffer,
                                                                   int width,
                                                                   int height,
                                                                   int pitch));

  /**
   * Render the audio of the current scene
   * This function only enqueues a rendering task and returns immediately
   * The callback function is called from the event notification thread after rendering is completed
   * @param frame Frame number to render
   * @param param Pointer to arbitrary user data
   * @param func_proc_rendering_audio Callback function called on completion
   *        buffer0: Pointer to rendered audio data (left channel, PCM(float) 32-bit format)
   *        buffer1: Pointer to rendered audio data (right channel, PCM(float) 32-bit format)
   *        sample_num: Number of rendered audio samples
   * @return true if rendering request succeeds (fails during output and similar states)
   */
  bool (*rendering_scene_audio)(int frame,
                                void *param,
                                void (*func_proc_rendering_audio)(void *param,
                                                                   int frame,
                                                                   float const *buffer0,
                                                                   float const *buffer1,
                                                                   int sample_num));

  /**
   * Wait until all rendering tasks in progress are completed
   * Note: Calling this while holding a read lock or edit lock may cause deadlock
   */
  void (*wait_rendering_task)(void);

  /**
   * Enumerate font names via callback function (func_proc_enum_font)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_font Callback function for font name enumeration
   */
  void (*enum_font_name)(void *param, void (*func_proc_enum_font)(void *param, wchar_t const *name));

  /**
   * Enumerate palette names via callback function (func_proc_enum_palette)
   * Acquires read lock for exclusive control of palette information.
   * If already locked in the same thread, retrieves without additional locking.
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_palette Callback function for palette name enumeration
   */
  void (*enum_palette_name)(void *param, void (*func_proc_enum_palette)(void *param, wchar_t const *name));

  /**
   * Render the video of the specified object
   * This function only enqueues a rendering task and returns immediately
   * The callback function is called from the event notification thread after rendering is completed
   * @param object Object handle to render
   * @param frame Frame number to render
   * @param apply_effect Whether to apply additional filter effects. Group control additional effects are not applied
   * @param param Pointer to arbitrary user data
   * @param func_proc_rendering_video Callback function called on completion
   *        buffer: Pointer to rendered image data (PIXEL_RGBA format)
   *        width,height: Rendered image size
   *        pitch: Number of bytes per row in rendered image data
   * @return true if rendering request succeeds (fails for objects outside the target or during output)
   */
  bool (*rendering_object_video)(
      aviutl2_object_handle object,
      int frame,
      bool apply_effect,
      void *param,
      void (*func_proc_rendering_video)(void *param,
                                        int frame,
                                        void const *buffer,
                                        int width,
                                        int height,
                                        int pitch));

  /**
   * Render the audio of the specified object
   * This function only enqueues a rendering task and returns immediately
   * The callback function is called from the event notification thread after rendering is completed
   * @param object Object handle to render
   * @param frame Frame number to render
   * @param apply_effect Whether to apply additional filter effects. Group control additional effects are not applied
   * @param param Pointer to arbitrary user data
   * @param func_proc_rendering_audio Callback function called on completion
   *        buffer0: Pointer to rendered audio data (left channel, PCM(float) 32-bit format)
   *        buffer1: Pointer to rendered audio data (right channel, PCM(float) 32-bit format)
   *        sample_num: Number of rendered audio samples
   * @return true if rendering request succeeds (fails during output and similar states)
   */
  bool (*rendering_object_audio)(
      aviutl2_object_handle object,
      int frame,
      bool apply_effect,
      void *param,
      void (*func_proc_rendering_audio)(void *param,
                                        int frame,
                                        float const *buffer0,
                                        float const *buffer1,
                                        int sample_num));

  /**
   * Get names of items belonging to the group containing the specified setting item
   * @param effect Target effect name (effect.name value in alias file)
   * @param item Target setting item name (key name in alias file)
   * @param item_names Pointer to storage for names of items belonging to the group
   * @param item_num Number of entries in the item names storage
   * @param item_index Pointer to storage for the index of the target setting item within the group (not stored if NULL)
   * @return Number of belonging item names obtained (returns 0 if the item does not belong to a group)
   *         If item_names is NULL, returns the number of items in the group
   */
  int (*get_effect_item_group_names)(wchar_t const *effect,
                                     wchar_t const *item,
                                     wchar_t const **item_names,
                                     int item_num,
                                     int *item_index);
};

/**
 * Edit state constants
 */
enum {
  aviutl2_edit_state_edit = 0, /**< Editing */
  aviutl2_edit_state_play = 1, /**< Preview playing */
  aviutl2_edit_state_save = 2, /**< File output in progress */
};

/**
 * Effect type constants (may be extended in the future)
 */
enum {
  aviutl2_effect_type_filter = 1,     /**< Filter effect */
  aviutl2_effect_type_input = 2,      /**< Media input */
  aviutl2_effect_type_transition = 3, /**< Scene change */
  aviutl2_effect_type_control = 4,    /**< Object control */
  aviutl2_effect_type_output = 5,     /**< Media output */
};

/**
 * Effect flag constants (may be extended in the future)
 */
enum {
  aviutl2_effect_flag_video = 1,  /**< Supports video */
  aviutl2_effect_flag_audio = 2,  /**< Supports audio */
  aviutl2_effect_flag_filter = 4, /**< Supports filter object */
  aviutl2_effect_flag_camera = 8, /**< Supports camera effects */
};

/**
 * Effect item type constants (may be extended in the future)
 */
enum {
  aviutl2_effect_item_type_integer = 1, /**< Integer */
  aviutl2_effect_item_type_number = 2,  /**< Number */
  aviutl2_effect_item_type_check = 3,   /**< Check box */
  aviutl2_effect_item_type_text = 4,    /**< Text */
  aviutl2_effect_item_type_string = 5,  /**< String */
  aviutl2_effect_item_type_file = 6,    /**< File */
  aviutl2_effect_item_type_color = 7,   /**< Color */
  aviutl2_effect_item_type_select = 8,  /**< List selection */
  aviutl2_effect_item_type_scene = 9,   /**< Scene */
  aviutl2_effect_item_type_range = 10,  /**< Layer range */
  aviutl2_effect_item_type_combo = 11,  /**< Combination of list and text */
  aviutl2_effect_item_type_mask = 12,   /**< Mask */
  aviutl2_effect_item_type_font = 13,   /**< Font */
  aviutl2_effect_item_type_figure = 14, /**< Figure */
  aviutl2_effect_item_type_data = 15,   /**< Data */
  aviutl2_effect_item_type_folder = 16, /**< Folder */
};

//--------------------------------

/**
 * Project file
 * Used in callbacks for loading and saving project files, and in edit callback functions
 * Project save data is part of plugin data
 */
struct aviutl2_project_file {
  /**
   * Get string (UTF-8) saved in project
   * @param key Key name (UTF-8)
   * @return Pointer to obtained string (NULL if not set).
   *         Valid until callback processing ends
   */
  char const *(*get_param_string)(char const *key);

  /**
   * Save string (UTF-8) to project
   * @param key Key name (UTF-8)
   * @param value String to save (UTF-8)
   */
  void (*set_param_string)(char const *key, char const *value);

  /**
   * Get binary data saved in project
   * @param key Key name (UTF-8)
   * @param data Pointer to storage for obtained data
   * @param size Size of data to obtain (fails if different from saved size)
   * @return true if data obtained correctly
   */
  bool (*get_param_binary)(char const *key, void *data, int size);

  /**
   * Save binary data to project
   * @param key Key name (UTF-8)
   * @param data Pointer to data to save
   * @param size Size of data to save (4096 bytes or less)
   */
  void (*set_param_binary)(char const *key, void *data, int size);

  /**
   * Delete all data saved in project
   */
  void (*clear_params)(void);

  /**
   * Get project file path
   * @return Pointer to project file path (may not be set)
   *         Valid until callback processing ends
   */
  wchar_t const *(*get_project_file_path)(void);
};

//--------------------------------

/**
 * Host application table
 */
struct aviutl2_host_app_table {
  /**
   * Set plugin information
   * @param information Plugin information
   * @note GetCommonPluginTable() is now the recommended method
   */
  void (*set_plugin_information)(wchar_t const *information);

  /**
   * Register input plugin
   * @param input_plugin_table Input plugin table
   */
  void (*register_input_plugin)(struct aviutl2_input_plugin_table *input_plugin_table);

  /**
   * Register output plugin
   * @param output_plugin_table Output plugin table
   */
  void (*register_output_plugin)(struct aviutl2_output_plugin_table *output_plugin_table);

  /**
   * Register filter plugin
   * @param filter_plugin_table Filter plugin table
   * @note When FLAG_USERDATA is used, UninitializePlugin() is called after edit resources have been destroyed
   */
  void (*register_filter_plugin)(struct aviutl2_filter_plugin_table *filter_plugin_table);

  /**
   * Register script module
   * @param script_module_table Script module table
   */
  void (*register_script_module)(struct aviutl2_script_module_table *script_module_table);

  /**
   * Register import menu
   * @param name Import menu name
   * @param func_proc_import Callback function for import menu selection
   */
  void (*register_import_menu)(wchar_t const *name, void (*func_proc_import)(struct aviutl2_edit_section *edit));

  /**
   * Register export menu
   * @param name Export menu name
   * @param func_proc_export Callback function for export menu selection
   */
  void (*register_export_menu)(wchar_t const *name, void (*func_proc_export)(struct aviutl2_edit_section *edit));

  /**
   * Register window client
   * @param name Window name
   * @param hwnd Window handle. WS_CHILD will be added to window and parent window will be set (WS_POPUP will be
   * removed)
   */
  void (*register_window_client)(wchar_t const *name, HWND hwnd);

  /**
   * Get handle for editing project data
   * @return Edit handle
   */
  struct aviutl2_edit_handle *(*create_edit_handle)(void);

  /**
   * Register function to be called when project file is loaded (called during project initialization)
   * @param func_project_load Callback function for project file load
   */
  void (*register_project_load_handler)(void (*func_project_load)(struct aviutl2_project_file *project));

  /**
   * Register function to be called just before project file is saved
   * @param func_project_save Callback function for project file save
   */
  void (*register_project_save_handler)(void (*func_project_save)(struct aviutl2_project_file *project));

  /**
   * Register layer context menu item (layer editor, no selection state)
   * @param name Menu item name. Use '\' in name to create hierarchical display
   * @param func_proc_layer_menu Callback invoked on menu selection
   */
  void (*register_layer_menu)(wchar_t const *name, void (*func_proc_layer_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register object context menu item (layer editor, object selected state)
   * @param name Menu item name. Use '\' in name to create hierarchical display
   * @param func_proc_object_menu Callback invoked on menu selection
   */
  void (*register_object_menu)(wchar_t const *name, void (*func_proc_object_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register config menu
   * After registering config menu, registering window client will add "Settings" to system menu
   * @param name Config menu name
   * @param func_config Callback function for config menu selection
   */
  void (*register_config_menu)(wchar_t const *name, void (*func_config)(HWND hwnd, HINSTANCE dll_hinst));

  /**
   * Register edit menu
   * @param name Edit menu name. Use '\' in name to create hierarchical display
   * @param func_proc_edit_menu Callback function for edit menu selection
   */
  void (*register_edit_menu)(wchar_t const *name, void (*func_proc_edit_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register function to be called when cache clear operation is performed
   * @param func_proc_clear_cache Callback function for cache clear
   */
  void (*register_clear_cache_handler)(void (*func_proc_clear_cache)(struct aviutl2_edit_section *edit));

  /**
   * Register function to be called right after scene is changed
   * Also called when scene settings are updated
   * @param func_proc_change_scene Callback function for scene change
   */
  void (*register_change_scene_handler)(void (*func_proc_change_scene)(struct aviutl2_edit_section *edit));

  /**
   * Register import menu (added to File menu in window menu)
   * Calls callback with param argument without using edit section
   * @param name Import menu name
   * @param param Pointer to arbitrary user data
   * @param func_proc_import Callback function for import menu selection
   */
  void (*register_import_menu_param)(wchar_t const *name, void *param, void (*func_proc_import)(void *param));

  /**
   * Register export menu (added to File menu in window menu)
   * Calls callback with param argument without using edit section
   * @param name Export menu name
   * @param param Pointer to arbitrary user data
   * @param func_proc_export Callback function for export menu selection
   */
  void (*register_export_menu_param)(wchar_t const *name, void *param, void (*func_proc_export)(void *param));

  /**
   * Register layer context menu item (layer editor, no object selected, right-click menu)
   * Calls callback with param argument without using edit section
   * @param name Layer menu name. Use '\' in name to create hierarchical display
   * @param param Pointer to arbitrary user data
   * @param func_proc_layer_menu Callback function for layer menu selection
   */
  void (*register_layer_menu_param)(wchar_t const *name, void *param, void (*func_proc_layer_menu)(void *param));

  /**
   * Register object context menu item (layer editor, object selected, right-click menu)
   * Calls callback with param argument without using edit section
   * @param name Object menu name. Use '\' in name to create hierarchical display
   * @param param Pointer to arbitrary user data
   * @param func_proc_object_menu Callback function for object menu selection
   */
  void (*register_object_menu_param)(wchar_t const *name, void *param, void (*func_proc_object_menu)(void *param));

  /**
   * Register edit menu
   * Calls callback with param argument without using edit section
   * @param name Edit menu name. Use '\' in name to create hierarchical display
   * @param param Pointer to arbitrary user data
   * @param func_proc_edit_menu Callback function for edit menu selection
   */
  void (*register_edit_menu_param)(wchar_t const *name, void *param, void (*func_proc_edit_menu)(void *param));

  /**
   * Register a callback function for file drag & drop
   * @param name Name displayed in tooltip during drag and input plugin settings
   * @param filefilter File filter for supported drag & drop files
   * @param func_proc_file_drop Callback function when file is dropped
   */
  void (*register_file_drop_handler)(wchar_t const *name, wchar_t const *filefilter, void (*func_proc_file_drop)(struct aviutl2_edit_section *edit, wchar_t const *file));

  /**
   * Register a callback function for file drag & drop
   * Calls callback with param argument without using edit section
   * @param name Name displayed in tooltip during drag and input plugin settings
   * @param filefilter File filter for supported drag & drop files
   * @param param Pointer to arbitrary user data
   * @param func_proc_file_drop Callback function when file is dropped
   */
  void (*register_file_drop_param_handler)(wchar_t const *name, wchar_t const *filefilter, void *param, void (*func_proc_file_drop)(void *param, wchar_t const *file));

  /**
   * Register an object-edit item menu (added to the object edit right-click menu)
   * @param name Item menu name. Use '\' in name to create hierarchical display
   * @param allow_effect_only Whether to allow effect-only selection. If true, callbacks may be invoked with item as
   *                          NULL
   * @param func_proc_item_menu Callback function when item menu is selected
   * @note Callback arguments use the same format as get_object_item_value()
   */
  void (*register_object_item_menu)(wchar_t const *name,
                                    bool allow_effect_only,
                                    void (*func_proc_item_menu)(struct aviutl2_edit_section *edit, aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item));

  /**
   * Register an object-edit item menu (added to the object edit right-click menu)
   * Calls callback with param argument without using edit section
   * @param name Item menu name. Use '\' in name to create hierarchical display
   * @param allow_effect_only Whether to allow effect-only selection. If true, callbacks may be invoked with item as
   *                          NULL
   * @param param Pointer to arbitrary user data
   * @param func_proc_item_menu Callback function when item menu is selected
   * @note Callback arguments use the same format as get_object_item_value()
   */
  void (*register_object_item_menu_param)(wchar_t const *name,
                                          bool allow_effect_only,
                                          void *param,
                                          void (*func_proc_item_menu)(void *param, aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item));

  /**
   * Register a script module by specifying the module name
   * @param script_module_table Script module table
   * @param module_name Module name
   */
  void (*register_script_module_name)(struct aviutl2_script_module_table *script_module_table, wchar_t const *module_name);

  /**
   * Register a font collection
   * @param collection Font collection (pointer to IDWriteFontCollection)
   *                   A collection created from DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED) is likely usable
   */
  void (*register_font_collection)(struct IDWriteFontCollection *collection);

  /**
   * Register callback function for specified event
   * Callback function is called from the event notification thread
   * call_edit_section() cannot be used from event processing
   * @param type Event type
   * @param param Pointer to arbitrary user data
   * @param func_proc_event Callback function for event processing
   */
  void (*register_event_listener)(enum aviutl2_event_type type,
                                  void *param,
                                  void (*func_proc_event)(void *param));
};
